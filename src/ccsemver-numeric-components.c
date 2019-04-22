/*
  Part of: CCSemver
  Contents: parser functions for numbers

  Abstract

	Parser functions for numbers.

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
#include <ctype.h>
#include <errno.h>


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

long
ccsemver_parse_numeric_component (cce_destination_t L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);

  switch (ccsemver_input_next(input)) {
  case 'x':
  case 'X':
  case '*':
    ++(input->off);
    return CCSEMVER_NUM_X;

  default:
    if (isdigit(ccsemver_input_next(input))) {
      char *	endptr;
      long	num;

      /* We have already determined that the first character is a digit,
	 so we  know that the return  value of "strtol()" is  a positive
	 number. */
      num = ccsemver_strtol(L, input->str + input->off, &endptr);
      input->off += endptr - input->str - input->off;
      return num;
    } else {
      cce_raise(L, ccsemver_condition_new_parser_expected_numeric_component(L));
    }
  }
}

long
ccsemver_parse_number (cce_destination_t L, ccsemver_input_t * input)
/* Parse an integer number. */
{
  ccsemver_input_assert_more_input(L, input);

  if (isdigit(ccsemver_input_next(input))) {
    char *	endptr;
    long	num;

    /* We have already  determined that the first character  is a digit,
       so we  know that  the return  value of  "strtol()" is  a positive
       number. */
    num = ccsemver_strtol(L, input->str + input->off, &endptr);
    input->off += endptr - input->str - input->off;
    return num;
  } else {
    cce_raise(L, ccsemver_condition_new_parser_expected_number(L));
  }
}


/** --------------------------------------------------------------------
 ** Comparison.
 ** ----------------------------------------------------------------- */

int
ccsemver_num_comp(long const num1, long const num2)
{
  if ((CCSEMVER_NUM_X == num1) || (CCSEMVER_NUM_X == num2)) {
    return 0;
  } else if (num1 > num2) {
    return 1;
  } else if (num1 < num2) {
    return -1;
  } else {
    return 0;
  }
}

/* end of file */
