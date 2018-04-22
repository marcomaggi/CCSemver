/*
  Part of: CCSemver
  Contents: input functions, especially lexers
  Date: Apr  6, 2018

  Abstract

	Input struct functions.

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


/** --------------------------------------------------------------------
 ** Lookahead functions.
 ** ----------------------------------------------------------------- */

char
ccsemver_input_lookahead (ccsemver_input_t * input, size_t offset)
{
  return input->str[offset + input->off];
}

bool
ccsemver_input_looking_at_blanked_dash (ccsemver_input_t const * const that_input)
/* Return true if there are at least 3 more characters in the input: one
   or  more blanks,  one dash,  one  or more  blanks.  Otherwise  return
   false. */
{
  ccsemver_input_t	input = *that_input;

  return (ccsemver_input_parse_blanks (&input)	&&
	  ccsemver_input_parse_dash   (&input)	&&
	  ccsemver_input_parse_blanks (&input));
}


/** --------------------------------------------------------------------
 ** Parser functions.
 ** ----------------------------------------------------------------- */

bool
ccsemver_input_parse_v (ccsemver_input_t * input)
/* If the next character is a  "v": advance the parsing position over it
   and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('v' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_dot (ccsemver_input_t * input)
/* If the next character is a  dot: advance the parsing position over it
   and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('.' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_dash (ccsemver_input_t * input)
/* If the next character is a dash: advance the parsing position over it
   and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('-' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_plus (ccsemver_input_t * input)
/* If the next character is a plus: advance the parsing position over it
   and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('+' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_bar (ccsemver_input_t * input)
/* If the next character is a  bar: advance the parsing position over it
   and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('|' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_equal (ccsemver_input_t * input)
/* If the next character is an  equal: advance the parsing position over
   it and return true; otherwise return false. */
{
  if (ccsemver_input_more(input) && ('=' == ccsemver_input_next(input))) {
    ++(input->off);
    return true;
  } else {
    return false;
  }
}


bool
ccsemver_input_parse_blanks (ccsemver_input_t * input)
/* Advance  the  parsing position  skipping  all  the blank  characters.
   Return  true  if at  least  one  blank  was found;  otherwise  return
   false. */
{
  bool	at_least_one_found = false;

  while (ccsemver_input_more(input) &&
	 ((' '  == ccsemver_input_next(input)) ||
	  ('\t' == ccsemver_input_next(input)) ||
	  ('\r' == ccsemver_input_next(input)) ||
	  ('\n' == ccsemver_input_next(input)))) {
    ++(input->off);
    at_least_one_found = true;
  }
  return at_least_one_found;
}

bool
ccsemver_input_parse_blanked_dash (ccsemver_input_t * that_input)
/* If the input is a sequence: at  least one blank, a dash, at least one
   blank parse it and return true; otherwise return false. */
{
  ccsemver_input_t	input = *that_input;

  if (ccsemver_input_parse_blanks (&input)	&&
      ccsemver_input_parse_dash   (&input)	&&
      ccsemver_input_parse_blanks (&input)) {
    that_input->off = input.off;
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_blanked_OR (ccsemver_input_t * that_input)
/* If the  input is a  sequence: at least one  blank, a vertical  bar, a
   vertical bar, at least one blank  parse it and return true; otherwise
   return false. */
{
  ccsemver_input_t	input = *that_input;

  if (ccsemver_input_parse_blanks (&input)	&&
      ccsemver_input_parse_bar    (&input)	&&
      ccsemver_input_parse_bar    (&input)	&&
      ccsemver_input_parse_blanks (&input)) {
    that_input->off = input.off;
    return true;
  } else {
    return false;
  }
}

bool
ccsemver_input_parse_comparator_separator (ccsemver_input_t * that_input)
/* Simple comparators  in compound comparators are  separated by blanks;
 * for example:
 *
 *	>=1.2.3 <4.5.6
 *
 * We  want  to  distinguish  this  case   from  the  case  of  a  range
 * speciication, in which  after the blank there is a  vertical bar; for
 * example:
 *
 *	>=1.2.3 || <4.5.6
 *
 * This function returns  true if the input is: at  least one blank, end
 * of input  or a  character different from  '|'.  Otherwise  it returns
 * false.
 */
{
  ccsemver_input_t	input = *that_input;

  if (ccsemver_input_parse_blanks(&input)) {
    if (ccsemver_input_more(&input)) {
      if ('|' != ccsemver_input_next(&input)) {
	that_input->off = input.off;
	return true;
      } else {
	return false;
      }
    } else {
      that_input->off = input.off;
      return true;
    }
  } else {
    return false;
  }
}


/** --------------------------------------------------------------------
 ** Assertions.
 ** ----------------------------------------------------------------- */

void
ccsemver_input_assert_more_input (cce_destination_t L, ccsemver_input_t * input)
{
  if (ccsemver_input_is_empty(input)) {
    cce_raise(L, ccsemver_condition_new_parser_empty_input());
  } else if (ccsemver_input_at_end(input)) {
    cce_raise(L, ccsemver_condition_new_parser_end_of_input());
  } else if (ccsemver_input_invalid_offset(input)) {
    cce_raise(L, ccsemver_condition_new_parser_invalid_input_offset());
  }
}

/* end of file */
