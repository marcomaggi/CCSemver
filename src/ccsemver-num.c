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


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

char
ccsemver_num_parse (long * nump, char const * input_str, size_t input_len, size_t * input_offp)
{
  *nump = 0;
  if (*input_offp >= input_len) {
    return 1;
  } else {
    switch (input_str[*input_offp]) {
    case 'x':
    case 'X':
    case '*':
      *nump = CCSEMVER_NUM_X;
      ++(*input_offp);
      break;

    default:
      if (isdigit(input_str[*input_offp])) {
	char *	endptr;

	*nump = strtol(input_str + *input_offp, &endptr, 10);
	*input_offp += endptr - input_str - *input_offp;
      } else {
	return 1;
      }
      break;
    }
    return 0;
  }
}


/** --------------------------------------------------------------------
 ** Comparison.
 ** ----------------------------------------------------------------- */

char
ccsemver_num_comp(long const self, long const other)
{
  if ((CCSEMVER_NUM_X == self) || (CCSEMVER_NUM_X == other)) {
    return 0;
  } else if (self > other) {
    return 1;
  } else if (self < other) {
    return -1;
  } else {
    return 0;
  }
}

/* end of file */
