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

#include <ccsemver.h>
#include <stdlib.h>
#include <stdio.h>

#define STRNSIZE(s) (s), sizeof(s)-1

int test_match(char expected, const char *ccsemver_str, size_t ccsemver_len, const char *comp_str, size_t comp_len) {
  size_t offset = 0;
  char result;
  ccsemver_t ccsemver = {0};
  ccsemver_comp_t comp = {0};

  printf("test: `%.*s` ^ `%.*s`", (int) ccsemver_len, ccsemver_str, (int) comp_len, comp_str);
  if (ccsemver_read(&ccsemver, ccsemver_str, ccsemver_len, &offset)) {
    puts(" \tcouldn't parse ccsemver");
    return 1;
  }
  if (offset != ccsemver_len) {
    puts(" \tcouldn't parse fully base");
    return 1;
  }
  offset = 0;
  if (ccsemver_comp_read(&comp, comp_str, comp_len, &offset)) {
    puts(" \tcouldn't parse comp");
    return 1;
  }
  if (offset != comp_len) {
    puts(" \tcouldn't parse fully base");
    return 1;
  }
  result = ccsemver_match(&ccsemver, &comp);
  printf(" \t=> %d\t", result);
  if (result != expected) {
    printf(" != `%d`\n", expected);
    ccsemver_dtor(&ccsemver);
    ccsemver_comp_dtor(&comp);
    return 1;
  }
  printf(" == `%d`\n", expected);
  ccsemver_dtor(&ccsemver);
  ccsemver_comp_dtor(&comp);
  return 0;
}

int main(void) {
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
