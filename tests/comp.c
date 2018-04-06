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
  ccsemver_comp_t	cmp = {0};

  printf("test: len=%lu, str='%s'", input_len, input_str);
  if (ccsemver_comp_read(&cmp, &input)) {
    puts(" \tcouldn't parse");
    return 1;
  }
  if (input.off != input.len) {
    puts(" \tcouldn't parse fully base");
    return 1;
  }
  slen = (unsigned) ccsemver_comp_write(&cmp, buffer, 1024);
  printf(" \t=> \t`%.*s`", slen, buffer);
  if (memcmp(expected, buffer, (size_t) slen > input.len ? slen : input.len) != 0) {
    printf(" != `%s`\n", expected);
    ccsemver_comp_dtor(&cmp);
    return 1;
  }
  printf(" == `%s`\n", expected);
  ccsemver_comp_dtor(&cmp);
  return 0;
}


int
test_comp_and (char const * const expected,
	       char const * const base_str, size_t base_len,
	       char const * const more_str, size_t more_len)
{
  ccsemver_input_t	base_input = ccsemver_input_new(base_str, base_len, 0);
  ccsemver_input_t	more_input = ccsemver_input_new(more_str, more_len, 0);
  size_t		slen;
  char			buffer[1024];
  ccsemver_comp_t	cmp;

  printf("test and variant: base_len=%lu, base_str='%s'", base_len, base_str);
  if (ccsemver_comp_read(&cmp, &base_input)) {
    puts(" \tcouldn't parse base");
    return 1;
  }
  if (base_input.off != base_input.len) {
    puts(" \tcouldn't parse fully base");
    return 1;
  }
  if (ccsemver_comp_and(&cmp, &more_input)) {
    puts(" \tand variant failed");
    return 1;
  }

  slen = (size_t) ccsemver_comp_write(&cmp, buffer, 1024);
  printf(" \t=> \tslen=%lu, buffer='%s'", slen, buffer);
  {
    size_t	total_len = base_input.len + more_input.len + 1;

    if (0 != memcmp(expected, buffer, ((slen > total_len) ? slen : total_len))) {
      printf(" != `%s`\n", expected);
      ccsemver_comp_dtor(&cmp);
      return 1;
    }
  }
  printf(" == `%s`\n", expected);
  ccsemver_comp_dtor(&cmp);
  return 0;
}


int
main (void)
{
  ccsemver_init();

  puts("failure:");
  if (test_read("", STRNSIZE("* ")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("*  ")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("*  |")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("* || *")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("abc")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE(">")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("<=")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("~")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("^")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("=")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("1.2.3 ")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("1.2.3 -")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("1.2.3 - ")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("a.2.3")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("1.a.3")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("", STRNSIZE("1.2.a")) == 0) {
    return EXIT_FAILURE;
  }
  if (test_read("1.2.3", STRNSIZE("1.2.3"))) {
    return EXIT_FAILURE;
  }

  puts("\nsome prerelease and build:");
  if (test_read(">=0.0.0 1.2.3-alpha", STRNSIZE("* 1.2.3-alpha"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3-alpha.2", STRNSIZE("* 1.2.3-alpha.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3+77", STRNSIZE("* 1.2.3+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3+77.2", STRNSIZE("* 1.2.3+77.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3-alpha.2+77", STRNSIZE("* 1.2.3-alpha.2+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3-alpha.2+77.2", STRNSIZE("* 1.2.3-alpha.2+77.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3-al-pha.2+77", STRNSIZE("* 1.2.3-al-pha.2+77"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 1.2.3-al-pha.2+77.2", STRNSIZE("* 1.2.3-al-pha.2+77.2"))) {
    return EXIT_FAILURE;
  }

  puts("\nx-range:");
  if (test_read(">=0.0.0", STRNSIZE("*"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.0.0 <2.0.0", STRNSIZE("1.x"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.0 <1.3.0", STRNSIZE("1.2.x"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0", STRNSIZE(""))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.0.0 <2.0.0", STRNSIZE("1"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.0 <1.3.0", STRNSIZE("1.2"))) {
    return EXIT_FAILURE;
  }

  puts("\nhyphen:");
  if (test_read(">=1.2.3 <=2.3.4", STRNSIZE("1.2.3 - 2.3.4"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.0 <=2.3.4", STRNSIZE("1.2 - 2.3.4"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.3 <2.4.0", STRNSIZE("1.2.3 - 2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.3 <3.0.0", STRNSIZE("1.2.3 - 2"))) {
    return EXIT_FAILURE;
  }

  puts("\ntidle:");
  if (test_read(">=1.2.3 <1.3.0", STRNSIZE("~1.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.2.0 <1.3.0", STRNSIZE("~1.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=1.0.0 <2.0.0", STRNSIZE("~1"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.2.3 <0.3.0", STRNSIZE("~0.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.2.0 <0.3.0", STRNSIZE("~0.2"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.0 <1.0.0", STRNSIZE("~0"))) {
    return EXIT_FAILURE;
  }

  puts("\ncaret:");
  if (test_read(">=1.2.3 <2.0.0", STRNSIZE("^1.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.2.3 <0.3.0", STRNSIZE("^0.2.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.0.3 <0.0.4", STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }

  puts("\nprimitive:");
  if (test_read(">=1.2.3 <2.0.0", STRNSIZE(">=1.2.3 <2.0"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.2.3 <0.3.0", STRNSIZE(">=0.2.3 <0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">=0.5.0 <0.0.4", STRNSIZE(">=0.5 <0.0.4"))) {
    return EXIT_FAILURE;
  }
  if (test_read(">0.0.3", STRNSIZE(">0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read("0.0.3", STRNSIZE("=0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_read("9.0.0", STRNSIZE("=9"))) {
    return EXIT_FAILURE;
  }

  puts("\nand:");
  if (test_comp_and(">=0.0.0 >=0.0.3 <0.0.4", STRNSIZE("*"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and(">=1.0.0 <2.0.0 >=0.0.3 <0.0.4", STRNSIZE("1.x"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and(">=1.2.0 <1.3.0 >=0.0.3 <0.0.4", STRNSIZE("1.2.x"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and(">=1.2.0 <1.3.0 >=1.2.0 <1.3.0 >=0.0.3 <0.0.4", STRNSIZE("1.2 1.2.x"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and(">=1.0.0 <2.0.0 >=0.0.3 <0.0.4", STRNSIZE("1"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and(">=1.2.0 <1.3.0 >=0.0.3 <0.0.4", STRNSIZE("1.2"), STRNSIZE("^0.0.3"))) {
    return EXIT_FAILURE;
  }
  if (test_comp_and("", STRNSIZE("1.2"), STRNSIZE("")) == 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/* end of file */
