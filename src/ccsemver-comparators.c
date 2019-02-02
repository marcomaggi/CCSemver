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

static void ccsemver_cmp_parse (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

static void ccsemver_xrevert (ccsemver_sv_t * sv);

static ccsemver_cmp_t * expand_straight_or_xrange_comp (cce_destination_t L, ccsemver_cmp_t * cmp)
  __attribute__((__nonnull__(1,2),__returns_nonnull__));

static void parse_hyphen_tail      (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

static void parse_caret            (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

static void parse_tilde            (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Constructor and destructor.
 ** ----------------------------------------------------------------- */

static void
ccsemver_cmp_delete_after_new (ccsemver_cmp_t * cmp)
{
  if (cmp->sv.delete) {
    ccsemver_sv_delete(&(cmp->sv));
  }
  if (cmp->next) {
    ccsemver_cmp_delete(cmp->next);
  }
  memset(cmp, 0, sizeof(ccsemver_cmp_t));
  free(cmp);
}

ccsemver_cmp_t *
ccsemver_cmp_new (cce_destination_t upper_L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(upper_L, input);
  {
    cce_location_t		L[1];
    ccsemver_cmp_t *		cmp;
    cce_error_handler_t		cmp_H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      cmp         = cce_sys_malloc_guarded(L, cmp_H, sizeof(ccsemver_cmp_t));
      cmp->delete = ccsemver_cmp_delete_after_new;
      ccsemver_cmp_parse(L, cmp, input);
      cce_run_body_handlers(L);
    }
    return cmp;
  }
}

static ccsemver_cmp_t *
ccsemver_cmp_new_with_sv (cce_destination_t L, ccsemver_op_t op, ccsemver_sv_t * sv)
{
  ccsemver_cmp_t *	cmp = (ccsemver_cmp_t *) cce_sys_malloc(L, sizeof(ccsemver_cmp_t));
  cmp->delete	= ccsemver_cmp_delete_after_new;
  cmp->op	= op;
  cmp->sv	= *sv;
  cmp->next	= NULL;
  return cmp;
}

/* ------------------------------------------------------------------ */

static void
ccsemver_cmp_delete_after_init (ccsemver_cmp_t * cmp)
{
  if (cmp->sv.delete) {
    ccsemver_sv_delete(&(cmp->sv));
  }
  if (cmp->next) {
    ccsemver_cmp_delete(cmp->next);
  }
  memset(cmp, 0, sizeof(ccsemver_cmp_t));
}


ccsemver_cmp_t *
ccsemver_cmp_init (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);
  cmp->delete = ccsemver_cmp_delete_after_init;
  ccsemver_cmp_parse(L, cmp, input);
  return cmp;
}

/* ------------------------------------------------------------------ */

void
ccsemver_cmp_delete (ccsemver_cmp_t * cmp)
{
  if (cmp->delete) {
    cmp->delete(cmp);
  }
}

void
ccsemver_cmp_reset (ccsemver_cmp_t * cmp)
/* Reset the struct  to an initial, empty state.  This  function must be
   used when we embed  a struct of this type into  another struct and we
   might or  might not apply a  constructor to it: this  function resets
   the struct to a safe, empty state. */
{
  cmp->delete	= NULL;
  cmp->next	= NULL;
  cmp->op	= CCSEMVER_OP_EQ;
  ccsemver_sv_reset(&(cmp->sv));
}


/** --------------------------------------------------------------------
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
static void
ccsemver_handler_cmp_function (cce_condition_t const * C CCE_UNUSED, cce_handler_t * H)
{
  ccsemver_cmp_delete(H->pointer);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsemver_clean_handler_cmp_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_cmp_t * cmp)
{
  H->handler.function	= ccsemver_handler_cmp_function;
  H->handler.pointer	= cmp;
  cce_register_clean_handler(L, H);
}

void
ccsemver_error_handler_cmp_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_cmp_t * cmp)
{
  H->handler.function	= ccsemver_handler_cmp_function;
  H->handler.pointer	= cmp;
  cce_register_error_handler(L, H);
}

/* ------------------------------------------------------------------ */

ccsemver_cmp_t *
ccsemver_cmp_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_cmp_t *	cmp = ccsemver_cmp_new(L, input);
  ccsemver_clean_handler_cmp_init(L, H, cmp);
  return cmp;
}

ccsemver_cmp_t *
ccsemver_cmp_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_cmp_t *	cmp = ccsemver_cmp_new(L, input);
  ccsemver_error_handler_cmp_init(L, H, cmp);
  return cmp;
}

/* ------------------------------------------------------------------ */

ccsemver_cmp_t *
ccsemver_cmp_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
{
  ccsemver_cmp_init(L, cmp, input);
  ccsemver_clean_handler_cmp_init(L, H, cmp);
  return cmp;
}

ccsemver_cmp_t *
ccsemver_cmp_init_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
{
  ccsemver_cmp_init(L, cmp, input);
  ccsemver_error_handler_cmp_init(L, H, cmp);
  return cmp;
}


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

void
ccsemver_cmp_parse (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
/* Parse a comparator, which is either standalone or part of a range.

   Compound  comparators  are  simple  comparators  separated  by  white
   spaces.  Here  we parse all  the simple comparators appending  to the
   linked list CMP.

   Comparators  in a  range are  separated  by "||"  operators: here  we
   handle  a  vertical bar  as  an  ending  separator, we  stop  parsing
   there. */
{
  if (ccsemver_input_is_empty(input)) {
    cmp->next	= NULL;
    cmp->op	= CCSEMVER_OP_GE;
  } else {
    ccsemver_input_assert_more_input(L, input);
    cmp->next	= NULL;
    cmp->op	= CCSEMVER_OP_EQ;

    switch (ccsemver_input_parse_next(input)) {

    case '^':
      /* Parse the caret comparator specification and expand it into two
	 appropriate comparators in the linked list CMP. */
      parse_caret(L, cmp, input);
      cmp = cmp->next;
      break;

    case '~':
      /* Parse the tilde comparator specification and expand it into two
	 appropriate comparators in the linked list CMP. */
      parse_tilde(L, cmp, input);
      cmp = cmp->next;
      break;

    case '>':
      /* Determine if it is a ">" or ">=" comparator. */
      if (ccsemver_input_parse_equal(input)) {
	cmp->op = CCSEMVER_OP_GE;
      } else {
	cmp->op = CCSEMVER_OP_GT;
      }
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	ccsemver_sv_init_range(L, &(cmp->sv), input);
	ccsemver_xrevert(&cmp->sv);
      }
      break;

    case '<':
      /* Determine if it is a "<" or "<=" comparator. */
      if (ccsemver_input_parse_equal(input)) {
	cmp->op = CCSEMVER_OP_LE;
      } else {
	cmp->op = CCSEMVER_OP_LT;
      }
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	ccsemver_sv_init_range(L, &(cmp->sv), input);
	ccsemver_xrevert(&cmp->sv);
      }
      break;

    case '=':
      cmp->op = CCSEMVER_OP_EQ;
      /* Parse the comparator specification and  expand it into a single
	 appropriate comparator in CMP. */
      {
	ccsemver_sv_init_range(L, &(cmp->sv), input);
	ccsemver_xrevert(&cmp->sv);
      }
      break;

    default:
      ccsemver_input_step_back(input);
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
      ccsemver_sv_init_range(L, &(cmp->sv), input);

      /* If input continues with at least 3 characters being: a space, a
	 dash, a space, then it is a hyphen range. */
      if (ccsemver_input_parse_blanked_dash(input)) {
	parse_hyphen_tail(L, cmp, input);
	cmp = cmp->next;
      } else {
	/* It is a straight partial version number or an X-range. */
	cmp = expand_straight_or_xrange_comp(L, cmp);
      }
      break;
    }

    if (0) { fprintf(stderr, "%s: done parsing simple comparator\n", __func__); }

    /* Either  we  are done  or  we  parse  the next  comparator.   Upon
       entering  this  function  CMP  is already  filled  with  a  valid
       comparator. */
    if (ccsemver_input_parse_comparator_separator(input)) {
      if (0) { fprintf(stderr, "%s: expecting the next comparator\n", __func__); }
      /* Parse the next comparator. */
      cmp->next = ccsemver_cmp_new(L, input);
    }
  }
}


static void
ccsemver_xrevert (ccsemver_sv_t * sv)
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


static ccsemver_cmp_t *
expand_straight_or_xrange_comp (cce_destination_t L, ccsemver_cmp_t * cmp)
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
 *
 * Return the new last node in the  linked list CMP, or NULL if an error
 * occurs.
 */
{
  /* If it is an X-range like: X.2.3 convert it into:
   *
   *	>0.0.0
   */
  if (CCSEMVER_NUM_X == cmp->sv.major) {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->sv);
    return cmp;
  }

  /* If it is an X-range like: 1.X.3 convert it into:
   *
   *	>=1.0.0 <2.0.0
   */
  else if (CCSEMVER_NUM_X == cmp->sv.minor) {
    ccsemver_xrevert(&cmp->sv);
    cmp->op	= CCSEMVER_OP_GE;
    cmp->next	= ccsemver_cmp_new_with_sv(L, CCSEMVER_OP_LT, &(cmp->sv));
    ++(cmp->next->sv.major);
    return cmp->next;
  }

  /* If it is an X-range like: 1.2.X convert it into.
   *
   *	>= 1.2.0 <1.3.0
   */
  else if (CCSEMVER_NUM_X == cmp->sv.patch) {
    ccsemver_xrevert(&cmp->sv);
    cmp->op	= CCSEMVER_OP_GE;
    cmp->next	= ccsemver_cmp_new_with_sv(L, CCSEMVER_OP_LT, &(cmp->sv));
    ++(cmp->next->sv.minor);
    return cmp->next;
  }

  /* If it is a straight semantic version like: 1.2.3 convert it into:
   *
   *	=1.2.3
   */
  else {
    cmp->op = CCSEMVER_OP_EQ;
    return cmp;
  }
}


static void
parse_caret (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
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
  ccsemver_sv_init_range(L, &(cmp->sv), input);

  /* Initialise  the GT  comparator.   If some  version  numbers are  X:
     convert  them  to  0  so  they are  a  right-side  limit  for  ">="
     operations. */
  {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->sv);
  }

  /* Build the new LT comparator. */
  cmp->next = ccsemver_cmp_new_with_sv(L, CCSEMVER_OP_LT, &(cmp->sv));

  /* Adjust the numbers to be right-side limits for "<" operations. */
  {
    ccsemver_sv_t *	ltsv = &(cmp->next->sv);

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
}


static void
parse_tilde (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
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
  ccsemver_sv_init_range(L, &(cmp->sv), input);

  /* Initialise  the GT  comparator.   If some  version  numbers are  X:
     convert  them  to  0  so  they are  a  right-side  limit  for  ">="
     operations. */
  {
    cmp->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&cmp->sv);
  }

  /* Build the new LT comparator. */
  cmp->next = ccsemver_cmp_new_with_sv(L, CCSEMVER_OP_LT, &(cmp->sv));

  /* Adjust the numbers to be right-side limits for "<" operations. */
  {
    ccsemver_sv_t *	ltsv = &(cmp->next->sv);

    if (ltsv->minor || ltsv->patch) {
      ++(ltsv->minor);
      ltsv->patch = 0;
    } else {
      ++(ltsv->major);
      ltsv->minor = ltsv->patch = 0;
    }
  }
}


static void
parse_hyphen_tail (cce_destination_t upper_L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
/* Hyphen ranges are such that:
 *
 *	'1.2.3 - 2.3.4' := '>=1.2.3 <=2.3.4'
 *
 * Upon arriving here we have already parsed the left limit and consumed
 * the '-' from the input.
 *
 * A single  hyphen range  is expanded into  two comparators:  a greater
 * than or  equal to (GE); either  a less than  (LT), or a less  than or
 * equal to (LE).
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
    ccsemver_xrevert(&cmp->sv);
  }

  /* Right after the blanks+dash+blanks characters we expect a, possibly
     partial, semantic version specification. */
  {
    cce_location_t	L[1];
    ccsemver_sv_t	partial_sv;
    cce_error_handler_t	partial_sv_H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      ccsemver_sv_init_range(L, &partial_sv, input);
      ccsemver_handler_sv_init(L, partial_sv_H, &partial_sv);

      /* Build the new LT comparator. */
      cmp->next = ccsemver_cmp_new_with_sv(L, CCSEMVER_OP_LT, &partial_sv);

      /* Adjust the numbers to be right-side limits for "<" operations. */
      {
	ccsemver_sv_t *	ltsv = &(cmp->next->sv);

	if (CCSEMVER_NUM_X == partial_sv.minor) {
	  ltsv->major = partial_sv.major + 1;
	  ltsv->minor = 0;
	  ltsv->patch = 0;
	} else if (CCSEMVER_NUM_X == partial_sv.patch) {
	  ltsv->major = partial_sv.major;
	  ltsv->minor = partial_sv.minor + 1;
	  ltsv->patch = 0;
	} else {
	  cmp->next->op = CCSEMVER_OP_LE;
	  *ltsv         = partial_sv;
	}
      }

      cce_run_body_handlers(L);
    }
  }
}


/** --------------------------------------------------------------------
 ** Composing comparators.
 ** ----------------------------------------------------------------- */

void
ccsemver_cmp_and (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
/* Given an already built and  initalised comparator, referenced by CMP:
   read a new comparator from the input and append it to the linked list
   CMP.  We accept empty input. */
{
  if (ccsemver_input_is_empty(input)) {
    return;
  } else if (ccsemver_input_at_end(input)) {
    cce_raise(L, ccsemver_condition_new_parser_end_of_input());
  } else if (ccsemver_input_invalid_offset(input)) {
    cce_raise(L, ccsemver_condition_new_parser_invalid_input_offset());
  } else {
    /* Read a comparator from the input string. */
    ccsemver_cmp_t *	new = ccsemver_cmp_new(L, input);

    /* Append the new comparator to the existing CMP. */
    {
      if (NULL == cmp->next) {
	cmp->next = new;
      } else {
	ccsemver_cmp_t * tail;

	/* Find the last comparator in the linked list CMP. */
	for (tail = cmp->next; tail->next; tail = tail->next);

	/* Append the new comparator to the linked list. */
	tail->next = new;
      }
    }
  }
}


/** --------------------------------------------------------------------
 ** Matching comparators and versions.
 ** ----------------------------------------------------------------- */

bool
ccsemver_match (ccsemver_sv_t const * sv, ccsemver_cmp_t const * cmp)
{
  int	result = ccsemver_sv_comp(sv, &(cmp->sv));

  /* If "SV < CMP->SV" and the operator is neither "<" nor "<=": no
     match. */
  if ((result < 0) && (cmp->op != CCSEMVER_OP_LT) && (cmp->op != CCSEMVER_OP_LE)) {
    return false;
  }

  /* If "SV > CMP->SV" and the operator is neither ">" nor ">=": no
     match. */
  if ((result > 0) && (cmp->op != CCSEMVER_OP_GT) && (cmp->op != CCSEMVER_OP_GE)) {
    return false;
  }

  /* If "SV  == CMP->SV" and the  operator is neither "="  nor "<="
     nor ">=": no match. */
  if ((result == 0) && (cmp->op != CCSEMVER_OP_EQ) && (cmp->op != CCSEMVER_OP_LE) && (cmp->op != CCSEMVER_OP_GE)) {
    return false;
  }

  /* If there  is a match between  SV and CMP->SV: go  on and match
     the next comparator (if any). */
  if (cmp->next) {
    return ccsemver_match(sv, cmp->next);
  } else {
    return true;
  }
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_cmp_write (ccsemver_cmp_t const * self, char * buffer, size_t len)
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
  ccsemver_sv_write(&(self->sv), semver, 256);
  if (self->next) {
    /* Remember that  "snprintf()" writes the  null byte, but  returns a
       number  of  characters  that  does not  include  the  terminating
       null! */
    return snprintf(buffer, len, "%s%s %.*s", op, semver, ccsemver_cmp_write(self->next, next, 1024), next);
  } else {
    /* Remember that  "snprintf()" writes the  null byte, but  returns a
       number  of  characters  that  does not  include  the  terminating
       null! */
    return snprintf(buffer, len, "%s%s", op, semver);
  }
}

/* end of file */
