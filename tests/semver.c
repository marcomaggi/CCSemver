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
 * of  this software  dedicate any  and  all copyright  interest in  the
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
#include <string.h>

#define STRNSIZE(s) (s), sizeof(s)-1


int
test_read (char const * const expected, char const * const input_str, size_t const input_len)
{
  ccsemver_input_t	input = ccsemver_input_new(input_str, input_len, 0);
  unsigned		slen;
  char			buffer[1024];
  ccsemver_t		sv;

  printf("test: `%.*s`", (int) input.len, input.str);
  if (ccsemver_read(&sv, &input)) {
    puts(" \tcouldn't parse");
    return 1;
  }
  if (ccsemver_input_more(&input)) {
    puts(" \tcouldn't parse fully base");
    return 1;
  }
  slen = (unsigned) ccsemver_write(&sv, buffer, 1024);
  printf(" \t=> \t`%.*s`", slen, buffer);
  if (0 != memcmp(expected, buffer, (((size_t) slen > input.len) ? slen : input.len))) {
    printf(" != `%s`\n", expected);
    ccsemver_dtor(&sv);
    return 1;
  } else {
    printf(" == `%s`\n", expected);
    ccsemver_dtor(&sv);
    return 0;
  }
}


int
main (void)
{
  ccsemver_init();

  if (test_read("1.2.3", STRNSIZE("1.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-alpha", STRNSIZE("v1.2.3-alpha"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-alpha.2", STRNSIZE("1.2.3-alpha.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3+77", STRNSIZE("v1.2.3+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3+77.2", STRNSIZE("1.2.3+77.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-alpha.2+77", STRNSIZE("v1.2.3-alpha.2+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-alpha.2+77.2", STRNSIZE("1.2.3-alpha.2+77.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-al-pha.2+77", STRNSIZE("v1.2.3-al-pha.2+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3-al-pha.2+77.2", STRNSIZE("1.2.3-al-pha.2+77.2"))) {
    return EXIT_FAILURE;
  }

  /* Bad because empty string. */
  if (test_read("", STRNSIZE("")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because double starting "v". */
  if (test_read("", STRNSIZE("vv1.2.3")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because missing minor number and patch level number. */
  if (test_read("", STRNSIZE("v1")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because missing patch level number. */
  if (test_read("", STRNSIZE("v1.2")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because the patch level is not a number. */
  if (test_read("", STRNSIZE("v1.2.x")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because the minor number is not a number. */
  if (test_read("", STRNSIZE("v1.x.2")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because the major number is not a number. */
  if (test_read("", STRNSIZE("vx.1.2")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because missing identifier after the dash separator. */
  if (test_read("", STRNSIZE("v1.2.3-")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because an identifier component is numeric and starts with zero. */
  if (test_read("", STRNSIZE("v1.2.3-alpha.0123")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because missing build metadata after the plus separator. */
  if (test_read("", STRNSIZE("v1.2.3+")) == 0) {
    return EXIT_FAILURE;
  }
  /* Bad because missing build metadata after the plus separator. */
  if (test_read("", STRNSIZE("v1.2.3-alpha.7+")) == 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/* end of file */
