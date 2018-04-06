/*
 * This  is free  and  unencumbered software  released  into the  public
 * domain.
 *
 * Anyone  is free  to copy,  modify,  publish, use,  compile, sell,  or
 * distribute this software, either in source code form or as a compiled
 * binary, for  any purpose,  commercial or  non-commercial, and  by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software  dedicate any and all copyright interest  in the n *
 * software  to the  public  domain.  We make  this  dedication for  the
 * benefit of the public at large and  to the detriment of our heirs and
 * successors.  We  intend  this  dedication  to  be  an  overt  act  of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE  SOFTWARE IS  PROVIDED "AS  IS",  WITHOUT WARRANTY  OF ANY  KIND,
 * EXPRESS OR  IMPLIED, INCLUDING BUT  NOT LIMITED TO THE  WARRANTIES OF
 * MERCHANTABILITY,    FITNESS   FOR    A    PARTICULAR   PURPOSE    AND
 * NONINFRINGEMENT.  IN  NO EVENT  SHALL THE AUTHORS  BE LIABLE  FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR  OTHERWISE, ARISING FROM,  OUT OF  OR IN CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccsemver.h>
#include <stdlib.h>
#include <stdio.h>

#define STRNSIZE(s) (s), sizeof(s)-1


int
test_match (char expected,
	    char const * const sv_str, size_t const sv_len,
	    char const * const cmp_str,  size_t const cmp_len)
{
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  sv_len,  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, cmp_len, 0);
  char			result;
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;

  printf("test: `%.*s` ^ `%.*s`", (int) sv_len, sv_str, (int) cmp_len, cmp_str);
  if (ccsemver_read(&sv, &sv_input)) {
    puts(" \tcouldn't parse semantic version specification");
    return 1;
  }
  if (ccsemver_input_more(&sv_input)) {
    puts("\tunable to parse the whole semantic version specification");
    return 1;
  }

  if (ccsemver_comp_read(&cmp, &cmp_input)) {
    puts("\tcouldn't parse comparator specification");
    return 1;
  }
  if (ccsemver_input_more(&cmp_input)) {
    puts("\tunable to parse the whole comparator specification");
    return 1;
  }

  result = ccsemver_match(&sv, &cmp);
  printf(" \t=> %d\t", result);
  {
    int		rv;

    if (result != expected) {
      printf(" != `%d`\n", expected);
      rv = 1;
    } else {
      printf(" == `%d`\n", expected);
      rv = 0;
    }
    ccsemver_dtor(&sv);
    ccsemver_comp_dtor(&cmp);
    return rv;
  }
}


int
main (void)
{
  ccsemver_init();

  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("1.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("1.2.x"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("1.x.x"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("1.x"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("1"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE("*"))) {
    return EXIT_FAILURE;
  }
  if (test_match(1, STRNSIZE("v1.2.3"), STRNSIZE(">1"))) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/* end of file */
