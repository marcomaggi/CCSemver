/*
  Part of: CCSemver
  Contents: parsing comparators and creating comparator objects


  This  is  free and  unencumbered  software  released into  the  public
  domain.

  Anyone  is free  to  copy,  modify, publish,  use,  compile, sell,  or
  distribute this software, either in source  code form or as a compiled
  binary,  for any  purpose, commercial  or non-commercial,  and by  any
  means.

  In jurisdictions that recognize copyright  laws, the author or authors
  of  this software  dedicate  any  and all  copyright  interest in  the
  software  to the  public  domain.   We make  this  dedication for  the
  benefit of the public  at large and to the detriment  of our heirs and
  successors.   We  intend  this  dedication  to  be  an  overt  act  of
  relinquishment in perpetuity of all  present and future rights to this
  software under copyright law.

  THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
  EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO  EVENT SHALL  THE AUTHORS  BE LIABLE FOR  ANY CLAIM,  DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION  WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org>
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccsemver-internals.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/** --------------------------------------------------------------------
 ** Constructor and destructor.
 ** ----------------------------------------------------------------- */

void
ccsemver_comp_ctor (ccsemver_comp_t *self)
{
  self->next	= NULL;
  self->op	= CCSEMVER_OP_EQ;
  ccsemver_ctor(&self->version);
}

void
ccsemver_comp_dtor (ccsemver_comp_t * self)
{
  if (self->next) {
    ccsemver_comp_dtor(self->next);
    free(self->next);
    self->next = NULL;
  }
}


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

static void ccsemver_xrevert (ccsemver_t * sv);
static ccsemver_comp_t * expand_straight_or_xrange_comp (ccsemver_comp_t * cmp);
static char parse_partial_semver   (ccsemver_t      * sv,  char const * input_str, size_t input_len, size_t * input_offp);
static char parse_hyphen_tail      (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp);
static char parse_caret            (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp);
static char parse_tilde            (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp);
static char comp_parse_next_if_any (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp);

char
ccsemver_comp_read (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp)
/* Parse a comparator, which is either standalone or part of a range.

   Compound  comparators  are  simple  comparators  separated  by  white
   spaces.  Here  we parse all  the simple comparators appending  to the
   linked list CMP.

   Comparators  in a  range are  separated  by "||"  operators: here  we
   handle  a  vertical bar  as  an  ending  separator, we  stop  parsing
   there. */
{
  ccsemver_comp_ctor(cmp);
  if (0 == input_len) {
    cmp->op = CCSEMVER_OP_GE;
    return 0;
  } else if (*input_offp < input_len) {
    switch (input_str[*input_offp]) {

    case '^':
      /* Skip the caret. */
      ++(*input_offp);
      /* Parse the caret comparator specification and expand it into two
	 appropriate comparators in the linked list CMP. */
      if (parse_caret(cmp, input_str, input_len, input_offp)) {
	return 1;
      }
      cmp = cmp->next;
      break;

    case '~':
      /* Skip the tilde. */
      ++(*input_offp);
      /* Parse the tilde comparator specification and expand it into two
	 appropriate comparators in the linked list CMP. */
      if (parse_tilde(cmp, input_str, input_len, input_offp)) {
	return 1;
      }
      cmp = cmp->next;
      break;

    case '>':
      /* Skip the greater-than. */
      ++(*input_offp);
      /* Determine if it is a ">" or ">=" comparator. */
      if ((*input_offp < input_len) && ('=' == input_str[*input_offp])) {
	/* Skip the equal. */
	++(*input_offp);
	cmp->op = CCSEMVER_OP_GE;
      } else {
	cmp->op = CCSEMVER_OP_GT;
      }
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
	  return 1;
	}
	ccsemver_xrevert(&cmp->version);
      }
      break;

    case '<':
      /* Skip the less-than. */
      ++(*input_offp);
      /* Determine if it is a "<" or "<=" comparator. */
      if ((*input_offp < input_len) && ('=' == input_str[*input_offp])) {
	/* Skip the equal. */
	++(*input_offp);
	cmp->op = CCSEMVER_OP_LE;
      } else {
	cmp->op = CCSEMVER_OP_LT;
      }
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
	  return 1;
	}
	ccsemver_xrevert(&cmp->version);
      }
      break;

    case '=':
      /* Skip the equal. */
      ++(*input_offp);
      cmp->op = CCSEMVER_OP_EQ;
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
	  return 1;
	}
	ccsemver_xrevert(&cmp->version);
      }
      break;

    default:
      /* Here  we assume  the comparator  is either  a straight  partial
       * version number, like:
       *
       *	'1.2.3'
       *	'1.2'
       *
       * or an X-range like:
       *
       *	'1.x'
       *	'1.2.*'
       *
       * or a hyphen range, like:
       *
       *	'1.2.3 - 2.3.4' := '>=1.2.3 <=2.3.4'
       *
       * so we start by parsing a partial semantic version specification.
       */
      if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
	return 1;
      }
      /* If input continues with at least 3 characters being: a space, a
	 dash, a space, then it is a hyphen range. */
      if ((*input_offp     < input_len) && (input_str[*input_offp]     == ' ') &&
	  (*input_offp + 1 < input_len) && (input_str[*input_offp + 1] == '-') &&
	  (*input_offp + 2 < input_len) && (input_str[*input_offp + 2] == ' ')) {
	/* Skip space+dash+space. */
	*input_offp += 3;
	if (parse_hyphen_tail(cmp, input_str, input_len, input_offp)) {
	  return 1;
	}
	cmp = cmp->next;
      } else {
	/* It is a straight partial version number or an X-range. */
	cmp = expand_straight_or_xrange_comp(cmp);
	if (NULL == cmp) {
	  return 1;
	}
      }
      break;
    }

    /* Upon  arriving   here  CMP  is   already  filled  with   a  valid
       comparator. */
    return comp_parse_next_if_any(cmp, input_str, input_len, input_offp);
  } else {
    return 1;
  }
}


static char
comp_parse_next_if_any (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp)
/* Either we  are done or we  parse the next comparator.   Upon entering
   this function CMP is already filled with a valid comparator. */
{
  /* If  input  continues  with  one  space followed  by  at  least  one
     character that is neither a space nor a vertical bar... */
  if ((*input_offp     < input_len) && input_str[*input_offp]     == ' ' &&
      (*input_offp + 1 < input_len) && input_str[*input_offp + 1] != ' ' && input_str[*input_offp + 1] != '|') {
    /* Skip the white space. */
    ++(*input_offp);
    /* Parse the next comparator. */
    cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (cmp->next) {
      return ccsemver_comp_read(cmp->next, input_str, input_len, input_offp);
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}


static void
ccsemver_xrevert (ccsemver_t * sv)
/* Normalise the version numbers in SV so that:
 *
 *    X.2.3	--> 0.0.0
 *    1.X.3	--> 1.0.0
 *    1.2.X	--> 1.2.0
 */
{
  if (CCSEMVER_NUM_X == sv->major) {
    sv->major = sv->minor = sv->patch = 0;
  } else if (CCSEMVER_NUM_X == sv->minor) {
    sv->minor = sv->patch = 0;
  } else if (CCSEMVER_NUM_X == sv->patch) {
    sv->patch = 0;
  }
}


static ccsemver_comp_t *
expand_straight_or_xrange_comp (ccsemver_comp_t * cmp)
/* Having already parsed a comparator into the struct referenced by CMP:
 * we handle it as a straight partial version number, like:
 *
 *	'1.2.3'
 *	'1.2'
 *
 * or an X-range like:
 *
 *	'1.x'
 *	'1.2.*'
 */
{
  /* If it is an X-range like: X.2.3 ... */
  if (CCSEMVER_NUM_X == cmp->version.major) {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->version);
    return cmp;
  }

  /* If it is an X-range like: 1.X.3 ... */
  else if (CCSEMVER_NUM_X == cmp->version.minor) {
    ccsemver_xrevert(&cmp->version);
    cmp->op = CCSEMVER_OP_GE;
    cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (cmp->next == NULL) {
      return NULL;
    }
    ccsemver_comp_ctor(cmp->next);
    cmp->next->op = CCSEMVER_OP_LT;
    cmp->next->version = cmp->version;
    ++cmp->next->version.major;
    return cmp->next;
  }

  /* If it is an X-range like: 1.2.X ... */
  else if (CCSEMVER_NUM_X == cmp->version.patch) {
    ccsemver_xrevert(&cmp->version);
    cmp->op = CCSEMVER_OP_GE;
    cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (cmp->next == NULL) {
      return NULL;
    }
    ccsemver_comp_ctor(cmp->next);
    cmp->next->op = CCSEMVER_OP_LT;
    cmp->next->version = cmp->version;
    ++cmp->next->version.minor;
    return cmp->next;
  }

  /* If it is a straight semantic version... */
  else {
    cmp->op = CCSEMVER_OP_EQ;
    return cmp;
  }
}


static char
parse_partial_semver (ccsemver_t * sv, char const * input_str, size_t input_len, size_t * input_offp)
/* Initialise the already allocated "ccsemver_t" struct referenced by SV
   with data from the input.

   It is  fine if there is  no input: in this  case SV is filled  with X
   specifications as numbers.

   It is fine  if the input is not a  full semantic version specifiction
   because of missing minor version or missing patch level: in this case
   SV  is filled  with  X  specifications as  minor  and/or patch  level
   numbers.

   If is fine if the input is not a semantic version: in this case SV is
   filled with X specifications as numbers.

   Whatever input is after the partial semantic version: leave it alone.
*/
{
  ccsemver_ctor(sv);

  /* If there is no input*/
  sv->major = sv->minor = sv->patch = CCSEMVER_NUM_X;

  if (*input_offp < input_len) {
    sv->raw = input_str + *input_offp;

    /* Parse the major number. */
    if (ccsemver_num_parse(&sv->major, input_str, input_len, input_offp)) {
      return 0;
    }

    /* If no more input or input is not a dot: return successfully. */
    if (*input_offp >= input_len || input_str[*input_offp] != '.') {
      return 0;
    }

    /* Skip the dot. */
    ++(*input_offp);

    /* Parse the minor number. */
    if (ccsemver_num_parse(&sv->minor, input_str, input_len, input_offp)) {
      return 1;
    }

    /* If no more input or input is not a dot: return successfully. */
    if (*input_offp >= input_len || input_str[*input_offp] != '.') {
      return 0;
    }

    /* Skip the dot. */
    ++(*input_offp);

    /* Parse the patch level number. */
    if  (ccsemver_num_parse(&sv->patch, input_str, input_len, input_offp)) {
      return 1;
    }

    /* If there  is a  prerelease tag:  parse it.  If  there is  a build
       metadata: parse it. */
    if ((input_str[*input_offp] == '-' && ccsemver_id_read(&sv->prerelease, input_str, input_len, (++*input_offp, input_offp))) ||
	(input_str[*input_offp] == '+' && ccsemver_id_read(&sv->build,      input_str, input_len, (++*input_offp, input_offp)))) {
      /* Error parsing the prerelease tag or the build metadata. */
      return 1;
    } else {
      /* Either ther is no prerelease tag or we have successfully parsed
	 a prerelease tag.  Either there is no build metadata or we have
	 successfully parsed a build metadata. */
      sv->len = input_str + *input_offp - sv->raw;
      return 0;
    }
  } else {
    return 0;
  }
}


static char
parse_caret (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp)
/* Caret ranges are such that:
 *
 *	'^1.2.3' := '>=1.2.3 <2.0.0'
 *	'^0.2.3' := '>=0.2.3 <0.3.0'
 *	'^0.0.3' := '>=0.0.3 <0.0.4'
 *
 * Upon arriving here we have already consumed the '^' from the input.
 *
 * A single caret range is expanded into two comparators: a greater than
 * or  equal to  (GE); a  less  than (LT).   We have  to initialise  the
 * comparator referenced  by CMP with  the GE specification; we  have to
 * build and  initialise a new  comparator referenced by  CMP->NEXT with
 * the LT specification.
 */
{
  /* Right  after the  caret character  we expect  a, possibly  partial,
     semantic version specification. */
  if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
    return 1;
  }

  /* Initialise  the GT  comparator.   If some  version  numbers are  X:
     convert  them  to  0  so  they are  a  right-side  limit  for  ">="
     operations. */
  {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->version);
  }

  /* Build the new LT comparator. */
  cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
  if (cmp->next) {
    ccsemver_comp_ctor(cmp->next);
    cmp->next->op = CCSEMVER_OP_LT;
    {
      ccsemver_t *	ltsv = &(cmp->next->version);

      *ltsv = cmp->version;
      /* Adjust   the  numbers   to   be  right-side   limits  for   ">"
	 operations. */
      if (ltsv->major) {
	++(ltsv->major);
	ltsv->minor = ltsv->patch = 0;
      } else if (ltsv->minor) {
	++(ltsv->minor);
	ltsv->patch = 0;
      } else {
	++(ltsv->patch);
      }
    }
    return 0;
  } else {
    return 1;
  }
}


static char
parse_tilde (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp)
/* Tilde ranges are such that:
 *
 *	'~1.2.3' := '>=1.2.3 <1.3.0'
 *	'~1.2'   := '>=1.2.0 <1.3.0'
 *	'~1'     := '>=1.0.0 <2.0.0'
 *
 * Upon arriving here we have already consumed the '~' from the input.
 *
 * A single tilde range is expanded into two comparators: a greater than
 * or  equal to  (GE); a  less  than (LT).   We have  to initialise  the
 * comparator referenced  by CMP with  the GE specification; we  have to
 * build and  initialise a new  comparator referenced by  CMP->NEXT with
 * the LT specification.
 */
{
  /* Right  after the  tilde character  we expect  a, possibly  partial,
     semantic version specification. */
  if (parse_partial_semver(&cmp->version, input_str, input_len, input_offp)) {
    return 1;
  }

  /* Initialise  the GT  comparator.   If some  version  numbers are  X:
     convert  them  to  0  so  they are  a  right-side  limit  for  ">="
     operations. */
  {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->version);
  }

  /* Build the new LT comparator. */
  cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
  if (cmp->next) {
    ccsemver_comp_ctor(cmp->next);
    cmp->next->op = CCSEMVER_OP_LT;
    {
      ccsemver_t *	ltsv = &(cmp->next->version);

      *ltsv = cmp->version;
      /* Adjust   the  numbers   to   be  right-side   limits  for   ">"
	 operations. */
      if (ltsv->minor || ltsv->patch) {
	++(ltsv->minor);
	ltsv->patch = 0;
      } else {
	++(ltsv->major);
	ltsv->minor = ltsv->patch = 0;
      }
    }
    return 0;
  } else {
    return 1;
  }
}


static char
parse_hyphen_tail (ccsemver_comp_t * cmp, char const * input_str, size_t input_len, size_t * input_offp)
/* Hyphen ranges are such that:
 *
 *	'1.2.3 - 2.3.4' := '>=1.2.3 <=2.3.4'
 *
 * Upon arriving here we have already parsed the left limit and consumed
 * the '-' from the input.
 *
 * A single  hyphen range  is expanded into  two comparators:  a greater
 * than or equal to (GE); a less than (LT).
 *
 * Upon entering this function: the left  limit is already stored in the
 * comparator  referenced by  CMP,  we  have to  normalise  it  as a  GE
 * comparator.
 *
 * We  have to  build  and  initialise a  new  comparator referenced  by
 * CMP->NEXT with the LT specification.
 */
{
  /* Convert CMP into  a GT comparator.  If some version  numbers are X:
     convert  them  to  0  so  they are  a  right-side  limit  for  ">="
     operations. */
  {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->version);
  }

  {
    ccsemver_t	partial_sv;

    /* Right after the space+dash+space  character we expect a, possibly
       partial, semantic version specification. */
    if (parse_partial_semver(&partial_sv, input_str, input_len, input_offp)) {
      return 1;
    }

    /* Build the new LT comparator. */
    cmp->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (cmp->next) {
      ccsemver_comp_ctor(cmp->next);
      cmp->next->op = CCSEMVER_OP_LT;

      /* Adjust the numbers to be right-side limits for ">" operations. */
      if (CCSEMVER_NUM_X == partial_sv.minor) {
	cmp->next->version.major = partial_sv.major + 1;
      } else if (CCSEMVER_NUM_X == partial_sv.patch) {
	cmp->next->version.major = partial_sv.major;
	cmp->next->version.minor = partial_sv.minor + 1;
      } else {
	cmp->next->op = CCSEMVER_OP_LE;
	cmp->next->version = partial_sv;
      }
      return 0;
    } else {
      return 1;
    }
  }
}


/** --------------------------------------------------------------------
 ** Composing comparators.
 ** ----------------------------------------------------------------- */

char
ccsemver_comp_and (ccsemver_comp_t * cmp, char const * str, size_t len, size_t * offset)
/* Given an already built and  initalised comparator, referenced by CMP:
   read a new comparator from the input and append it to the linked list
   CMP. */
{
  if (0 < len) {
    ccsemver_comp_t *	new;

    /* Read a comparator from the input string. */
    {
      new = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
      if (NULL == new) {
	return 1;
      } else {
	char rv = ccsemver_comp_read(new, str, len, offset);
	if (rv) {
	  ccsemver_comp_dtor(new);
	  free(new);
	  return rv;
	}
      }
    }

    /* Append the new comparator to the existing CMP. */
    {
      if (NULL == cmp->next) {
	cmp->next = new;
      } else {
	ccsemver_comp_t * tail;

	/* Find the last comparator in the linked list CMP. */
	for (tail = cmp->next; tail->next; tail = tail->next);

	/* Append the new comparator to the linked list. */
	tail->next = new;
      }
    }

    return 0;
  } else {
    return 1;
  }
}


/** --------------------------------------------------------------------
 ** Matching comparators and versions.
 ** ----------------------------------------------------------------- */

char
ccsemver_match (ccsemver_t const * sv, ccsemver_comp_t const * cmp)
{
  char result = ccsemver_comp(sv, &(cmp->version));

  /* If "SV < CMP->VERSION" and the operator is neither "<" nor "<=": no
     match. */
  if ((result < 0) && (cmp->op != CCSEMVER_OP_LT) && (cmp->op != CCSEMVER_OP_LE)) {
    return 0;
  }

  /* If "SV > CMP->VERSION" and the operator is neither ">" nor ">=": no
     match. */
  if ((result > 0) && (cmp->op != CCSEMVER_OP_GT) && (cmp->op != CCSEMVER_OP_GE)) {
    return 0;
  }

  /* If "SV  == CMP->VERSION" and the  operator is neither "="  nor "<="
     nor ">=": no match. */
  if ((result == 0) && (cmp->op != CCSEMVER_OP_EQ) && (cmp->op != CCSEMVER_OP_LE) && (cmp->op != CCSEMVER_OP_GE)) {
    return 0;
  }

  /* If there  is a match between  SV and CMP->VERSION: go  on and match
     the next comparator (if any). */
  if (cmp->next) {
    return ccsemver_match(sv, cmp->next);
  } else {
    return 1;
  }
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_comp_write (ccsemver_comp_t const * self, char * buffer, size_t len)
{
  char const *	op = "";
  char		semver[256];
  char		next[1024];

  switch (self->op) {
  case CCSEMVER_OP_EQ:
    break;
  case CCSEMVER_OP_LT:
    op = "<";
    break;
  case CCSEMVER_OP_LE:
    op = "<=";
    break;
  case CCSEMVER_OP_GT:
    op = ">";
    break;
  case CCSEMVER_OP_GE:
    op = ">=";
    break;
  }
  ccsemver_write(&(self->version), semver, 256);
  if (self->next) {
    return snprintf(buffer, len, "%s%s %.*s", op, semver, ccsemver_comp_write(self->next, next, 1024), next);
  } else {
    return snprintf(buffer, len, "%s%s", op, semver);
  }
}

/* end of file */
