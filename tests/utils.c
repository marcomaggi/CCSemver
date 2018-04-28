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
#include <string.h>
#include <errno.h>


int
test_id_fwrite (void)
{
  char const		input_str[] = "1.2.3-alpha.1+x86-64";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 6);
  ccsemver_id_t		id;
  int			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    printf("test_id_fwrite: ");
    ccsemver_id_fwrite(&id, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  ccsemver_id_dtor(&id);
  return rv;
}


int
test_version_fwrite (void)
{
  char const		input_str[] = "1.2.3-alpha.1+x86-64";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 6);
  ccsemver_t		sv;
  int			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("test_version_fwrite: ");
    ccsemver_fwrite(&sv, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  ccsemver_dtor(&sv);
  return 0;
}


int
test_comparator_fwrite (void)
{
  char const		input_str[] = "<=1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 6);
  ccsemver_cmp_t	cmp;
  int			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("test_comparator_fwrite: ");
    ccsemver_cmp_fwrite(&cmp, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  ccsemver_cmp_dtor(&cmp);
  return rv;
}


int
test_range_fwrite (void)
{
  char const		input_str[] = ">=1.2.3 || <4.0.0";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 6);
  ccsemver_range_t	rn;
  int			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    printf("test_range_fwrite: ");
    ccsemver_range_fwrite(&rn, stdout);
    if (0 == errno) {
      printf("\n");
      rv = 0;
    }
  }
  ccsemver_range_dtor(&rn);
  return rv;
}


int
main (void)
{
  ccsemver_init();

  if (test_id_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_version_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_comparator_fwrite()) {
    return EXIT_FAILURE;
  }

  if (test_range_fwrite()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/* end of file */
