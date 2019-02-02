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
#include <cctests.h>
#include <string.h>


void
test_id_fwrite (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	id_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		input_str[] = "alpha.1-beta.2-gamma.3";
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    ccsemver_id_t *	id;

    id = ccsemver_id_new_guarded(L, id_H, &input);
    ccsemver_id_fwrite(L, id, stdout);
    printf("\n");
    fflush(stdout);
    cce_run_body_handlers(L);
  }
}


void
test_sv_fwrite (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	sv_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		input_str[] = "v1.2.3-alpha.1+x86-64";
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    ccsemver_sv_t *	sv;

    sv = ccsemver_sv_new_guarded(L, sv_H, &input);
    ccsemver_sv_fwrite(L, sv, stdout);
    printf("\n");
    fflush(stdout);
    cce_run_body_handlers(L);
  }
}


void
test_cmp_fwrite (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	cmp_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		input_str[] = "<=1.2.3";
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    ccsemver_cmp_t *	cmp;

    cmp = ccsemver_cmp_new_guarded(L, cmp_H, &input);
    ccsemver_cmp_fwrite(L, cmp, stdout);
    printf("\n");
    fflush(stdout);
    cce_run_body_handlers(L);
  }
}


void
test_range_fwrite (cce_destination_t upper_L)
{
  cce_location_t	L[1];
  cce_clean_handler_t	rn_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    char const		input_str[] = ">=1.2.3 || <4.0.0";
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    ccsemver_range_t *	rn;

    rn = ccsemver_range_new_guarded(L, rn_H, &input);
    ccsemver_range_fwrite(L, rn, stdout);
    printf("\n");
    fflush(stdout);
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  ccsemver_library_init();
  cctests_init("utilities");
  {
    cctests_begin_group("printing");
    {
      cctests_run(test_id_fwrite);
      cctests_run(test_sv_fwrite);
      cctests_run(test_cmp_fwrite);
      cctests_run(test_range_fwrite);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
