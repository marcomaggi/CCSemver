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
#include <cctests.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void
test_match (cce_destination_t upper_L, bool expected, char const * const sv_str,  char const * const cmp_str, bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	sv_H[1];
  cce_clean_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
    ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
    ccsemver_sv_t *	sv;
    ccsemver_cmp_t *	cmp;

    if (0) { fprintf(stderr, "%s: sv: len=%lu, str='%s'\n", __func__, sv_input.len, sv_input.str); }
    if (0) { fprintf(stderr, "%s: cmp: len=%lu, str='%s'\n", __func__, cmp_input.len, cmp_input.str); }

    sv	= ccsemver_sv_new_guarded(L, sv_H, &sv_input);
    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&sv_input));
      cctests_assert(L, true  == ccsemver_input_at_end(&sv_input));
    }

    cmp	= ccsemver_cmp_new_guarded(L, cmp_H, &cmp_input);
    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&cmp_input));
      cctests_assert(L, true  == ccsemver_input_at_end(&cmp_input));
    }

    cctests_assert(L, expected == ccsemver_match(sv, cmp));

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Successfully matching ranges.
 ** ----------------------------------------------------------------- */

#define GOOD_INPUT(FUNCNAME, PARSE_TO_THE_END, EXPECTED, SV_STR, CMP_STR) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    test_match(upper_L, EXPECTED, SV_STR, CMP_STR, PARSE_TO_THE_END); \
  }

/* ------------------------------------------------------------------ */

GOOD_INPUT(test_1_1, true,	true, "v1.2.3", "1.2.3")
GOOD_INPUT(test_1_2, true,	true, "v1.2.3", "1.2.x")
GOOD_INPUT(test_1_3, true,	true, "v1.2.3", "1.x.x")
GOOD_INPUT(test_1_4, true,	true, "v1.2.3", "1.x")
GOOD_INPUT(test_1_5, true,	true, "v1.2.3", "1")
GOOD_INPUT(test_1_6, true,	true, "v1.2.3", "*")
GOOD_INPUT(test_1_7, true,	true, "v1.2.3", ">1")


int
main (void)
{
  ccsemver_library_init();
  cctests_init("matching ranges and semvers");

  cctests_begin_group("successful matching");
  {
    cctests_run(test_1_1);
    cctests_run(test_1_2);
    cctests_run(test_1_3);
    cctests_run(test_1_4);
    cctests_run(test_1_5);
    cctests_run(test_1_6);
    cctests_run(test_1_7);
  }
  cctests_end_group();

  cctests_final();
}

/* end of file */
