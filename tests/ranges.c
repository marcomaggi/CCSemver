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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void
test_new (cce_destination_t upper_L, char const * const expected, char const * const input_str, bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_range_t *	range;

    if (0) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, input.len, input.str); }

    range = ccsemver_range_new_guarded(L, H, &input);

    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&input));
      cctests_assert(L, true  == ccsemver_input_at_end(&input));
    }

    {
      int	slen = ccsemver_range_write(range, buffer, 1024);

      if (0) { fprintf(stderr, "%s: serialised: len=%d, range='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_clean_handlers(L);
  }
}

void
test_init (cce_destination_t upper_L, char const * const expected, char const * const input_str, bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_range_t	range[1];

    if (0) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, input.len, input.str); }

    ccsemver_range_init_guarded(L, H, range, &input);

    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&input));
      cctests_assert(L, true  == ccsemver_input_at_end(&input));
    }

    {
      int	slen = ccsemver_range_write(range, buffer, 1024);

      if (0) { fprintf(stderr, "%s: serialised: len=%d, range='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Parsing X-Ranges.
 ** ----------------------------------------------------------------- */

#define GOOD_INPUT_NEW(FUNCNAME, PARSE_TO_THE_END, EXPECTED, INPUT_STR)	\
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    test_new(upper_L, EXPECTED, INPUT_STR, PARSE_TO_THE_END);		\
  }

#define GOOD_INPUT_INIT(FUNCNAME, PARSE_TO_THE_END, EXPECTED, INPUT_STR) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    test_init(upper_L, EXPECTED, INPUT_STR, PARSE_TO_THE_END);		\
  }

/* ------------------------------------------------------------------ */

GOOD_INPUT_NEW(test_1_1_1,  true,	">=0.0.0 || 1.2.3",			"* || 1.2.3")
GOOD_INPUT_NEW(test_1_1_2,  true,	">=1.0.0 <2.0.0 || >=2.0.0 <3.0.0",	"1.x || 2.x")
GOOD_INPUT_NEW(test_1_1_3,  true,	">=1.2.0 <1.3.0 || 3.0.0",		"1.2.x || 3.0.0")
GOOD_INPUT_NEW(test_1_1_4,  true,	">=1.0.0 <2.0.0 || >=2.0.0 <3.0.0 || >=3.0.0 <4.0.0", "1 || 2 || 3")
GOOD_INPUT_NEW(test_1_1_5,  true,	">=1.2.0 <1.3.0 || >=5.0.0",		"1.2 || >=5")

/* ------------------------------------------------------------------ */

GOOD_INPUT_INIT(test_1_2_1,  true,	">=0.0.0 || 1.2.3",			"* || 1.2.3")
GOOD_INPUT_INIT(test_1_2_2,  true,	">=1.0.0 <2.0.0 || >=2.0.0 <3.0.0",	"1.x || 2.x")
GOOD_INPUT_INIT(test_1_2_3,  true,	">=1.2.0 <1.3.0 || 3.0.0",		"1.2.x || 3.0.0")
GOOD_INPUT_INIT(test_1_2_4,  true,	">=1.0.0 <2.0.0 || >=2.0.0 <3.0.0 || >=3.0.0 <4.0.0", "1 || 2 || 3")
GOOD_INPUT_INIT(test_1_2_5,  true,	">=1.2.0 <1.3.0 || >=5.0.0",		"1.2 || >=5")


/** --------------------------------------------------------------------
 ** Parsing hypen ranges.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_2_1_1,  true,	">=1.2.3 <=2.3.4 || >=5.0.0",	"1.2.3 - 2.3.4 || >=5")
GOOD_INPUT_NEW(test_2_1_2,  true,	">=1.2.0 <=2.3.4 || >=5.0.0",	"1.2 - 2.3.4 || >=5")
GOOD_INPUT_NEW(test_2_1_3,  true,	">=1.2.3 <2.4.0 || >=5.0.0",	"1.2.3 - 2.3 || >=5")
GOOD_INPUT_NEW(test_2_1_4,  true,	">=1.2.3 <3.0.0 || >=5.0.0",	"1.2.3 - 2 || >=5")

/* ------------------------------------------------------------------ */

GOOD_INPUT_INIT(test_2_2_1,  true,	">=1.2.3 <=2.3.4 || >=5.0.0",	"1.2.3 - 2.3.4 || >=5")
GOOD_INPUT_INIT(test_2_2_2,  true,	">=1.2.0 <=2.3.4 || >=5.0.0",	"1.2 - 2.3.4 || >=5")
GOOD_INPUT_INIT(test_2_2_3,  true,	">=1.2.3 <2.4.0 || >=5.0.0",	"1.2.3 - 2.3 || >=5")
GOOD_INPUT_INIT(test_2_2_4,  true,	">=1.2.3 <3.0.0 || >=5.0.0",	"1.2.3 - 2 || >=5")


/** --------------------------------------------------------------------
 ** Parsing tilde ranges.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_3_1_1,  true,	">=1.2.3 <1.3.0 || >=5.0.0",	"~1.2.3 || >=5")
GOOD_INPUT_NEW(test_3_1_2,  true,	">=1.2.0 <1.3.0 || >=5.0.0",	"~1.2 || >=5")
GOOD_INPUT_NEW(test_3_1_3,  true,	">=1.0.0 <2.0.0 || >=5.0.0",	"~1 || >=5")
GOOD_INPUT_NEW(test_3_1_4,  true,	">=0.2.3 <0.3.0 || >=5.0.0",	"~0.2.3 || >=5")
GOOD_INPUT_NEW(test_3_1_5,  true,	">=0.2.0 <0.3.0 || >=5.0.0",	"~0.2 || >=5")
GOOD_INPUT_NEW(test_3_1_6,  true,	">=0.0.0 <1.0.0 || >=5.0.0",	"~0 || >=5")

/* ------------------------------------------------------------------ */

GOOD_INPUT_NEW(test_3_2_1,  true,	">=1.2.3 <1.3.0 || >=5.0.0",	"~1.2.3 || >=5")
GOOD_INPUT_NEW(test_3_2_2,  true,	">=1.2.0 <1.3.0 || >=5.0.0",	"~1.2 || >=5")
GOOD_INPUT_NEW(test_3_2_3,  true,	">=1.0.0 <2.0.0 || >=5.0.0",	"~1 || >=5")
GOOD_INPUT_NEW(test_3_2_4,  true,	">=0.2.3 <0.3.0 || >=5.0.0",	"~0.2.3 || >=5")
GOOD_INPUT_NEW(test_3_2_5,  true,	">=0.2.0 <0.3.0 || >=5.0.0",	"~0.2 || >=5")
GOOD_INPUT_NEW(test_3_2_6,  true,	">=0.0.0 <1.0.0 || >=5.0.0",	"~0 || >=5")


/** --------------------------------------------------------------------
 ** Parsing caret ranges.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_4_1_1,  true,	">=1.2.3 <2.0.0 || >=5.0.0",	"^1.2.3 || >=5")
GOOD_INPUT_NEW(test_4_1_2,  true,	">=0.2.3 <0.3.0 || >=5.0.0",	"^0.2.3 || >=5")
GOOD_INPUT_NEW(test_4_1_3,  true,	">=0.0.3 <0.0.4 || >=5.0.0",	"^0.0.3 || >=5")

/* ------------------------------------------------------------------ */

GOOD_INPUT_NEW(test_4_2_1,  true,	">=1.2.3 <2.0.0 || >=5.0.0",	"^1.2.3 || >=5")
GOOD_INPUT_NEW(test_4_2_2,  true,	">=0.2.3 <0.3.0 || >=5.0.0",	"^0.2.3 || >=5")
GOOD_INPUT_NEW(test_4_2_3,  true,	">=0.0.3 <0.0.4 || >=5.0.0",	"^0.0.3 || >=5")


int
main (void)
{
  ccsemver_library_init();
  cctests_init("parsing ranges");
  {
    cctests_begin_group("parsing x-ranges");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);
      cctests_run(test_1_1_4);
      cctests_run(test_1_1_5);

      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
      cctests_run(test_1_2_3);
      cctests_run(test_1_2_4);
      cctests_run(test_1_2_5);
    }
    cctests_end_group();

    cctests_begin_group("parsing hyphen ranges");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_1_4);

      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);
      cctests_run(test_2_2_4);
    }
    cctests_end_group();

    cctests_begin_group("parsing tilde ranges");
    {
      cctests_run(test_3_1_1);
      cctests_run(test_3_1_2);
      cctests_run(test_3_1_3);
      cctests_run(test_3_1_4);
      cctests_run(test_3_1_5);
      cctests_run(test_3_1_6);

      cctests_run(test_3_2_1);
      cctests_run(test_3_2_2);
      cctests_run(test_3_2_3);
      cctests_run(test_3_2_4);
      cctests_run(test_3_2_5);
      cctests_run(test_3_2_6);
    }
    cctests_end_group();

    cctests_begin_group("parsing caret ranges");
    {
      cctests_run(test_4_1_1);
      cctests_run(test_4_1_2);
      cctests_run(test_4_1_3);

      cctests_run(test_4_2_1);
      cctests_run(test_4_2_2);
      cctests_run(test_4_2_3);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
