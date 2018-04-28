/*
  Part of: CCSemver
  Contents: tests for semantic version parsers


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

#include <ccsemver.h>
#include <cctests.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void
test_new (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_clean_handler_t	sv_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_sv_t *	sv;

    if (0) { fprintf(stderr, "test: len=%lu, str='%s'\n", input.len, input.str); }

    sv = ccsemver_sv_new_guarded(L, sv_H, &input);

    /* Have we parsed the whole input? */
    cctests_assert(L, false == ccsemver_input_more(&input));
    cctests_assert(L, true  == ccsemver_input_at_end(&input));

    {
      int	slen = ccsemver_sv_write(sv, buffer, 1024);

      if (0) { fprintf(stderr, " \t=> \t`%.*s`\n", slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_clean_handlers(L);
  }
}

void
test_init (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_clean_handler_t	sv_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_sv_t	sv[1];

    if (0) { fprintf(stderr, "test: len=%lu, str='%s'\n", input.len, input.str); }

    ccsemver_sv_init_guarded(L, sv_H, sv, &input);

    /* Have we parsed the whole input? */
    cctests_assert(L, false == ccsemver_input_more(&input));
    cctests_assert(L, true  == ccsemver_input_at_end(&input));

    {
      int	slen = ccsemver_sv_write(sv, buffer, 1024);

      if (0) { fprintf(stderr, " \t=> \t`%.*s`\n", slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Good input strings.
 ** ----------------------------------------------------------------- */

#define GOOD_INPUT_NEW(FUNCNAME, EXPECTED, INPUT_STR)	\
  void							\
  FUNCNAME (cce_destination_t upper_L)			\
  {							\
    test_new(upper_L, EXPECTED, INPUT_STR);		\
  }

#define GOOD_INPUT_INIT(FUNCNAME, EXPECTED, INPUT_STR)	\
  void							\
  FUNCNAME (cce_destination_t upper_L)			\
  {							\
    test_init(upper_L, EXPECTED, INPUT_STR);		\
  }

GOOD_INPUT_NEW(test_1_1, "1.2.3",		"1.2.3")
GOOD_INPUT_NEW(test_1_2, "1.2.3-alpha",		"v1.2.3-alpha")
GOOD_INPUT_NEW(test_1_3, "1.2.3-alpha.2",	"1.2.3-alpha.2")
GOOD_INPUT_NEW(test_1_4, "1.2.3+77",		"v1.2.3+77")
GOOD_INPUT_NEW(test_1_5, "1.2.3+77.2",		"1.2.3+77.2")
GOOD_INPUT_NEW(test_1_6, "1.2.3-alpha.2+77",	"v1.2.3-alpha.2+77")
GOOD_INPUT_NEW(test_1_7, "1.2.3-alpha.2+77.2",	"1.2.3-alpha.2+77.2")
GOOD_INPUT_NEW(test_1_8, "1.2.3-al-pha.2+77",	"v1.2.3-al-pha.2+77")
GOOD_INPUT_NEW(test_1_9, "1.2.3-al-pha.2+77.2",	"1.2.3-al-pha.2+77.2")

GOOD_INPUT_INIT(test_2_1, "1.2.3",		"1.2.3")
GOOD_INPUT_INIT(test_2_2, "1.2.3-alpha",	"v1.2.3-alpha")
GOOD_INPUT_INIT(test_2_3, "1.2.3-alpha.2",	"1.2.3-alpha.2")
GOOD_INPUT_INIT(test_2_4, "1.2.3+77",		"v1.2.3+77")
GOOD_INPUT_INIT(test_2_5, "1.2.3+77.2",		"1.2.3+77.2")
GOOD_INPUT_INIT(test_2_6, "1.2.3-alpha.2+77",	"v1.2.3-alpha.2+77")
GOOD_INPUT_INIT(test_2_7, "1.2.3-alpha.2+77.2",	"1.2.3-alpha.2+77.2")
GOOD_INPUT_INIT(test_2_8, "1.2.3-al-pha.2+77",	"v1.2.3-al-pha.2+77")
GOOD_INPUT_INIT(test_2_9, "1.2.3-al-pha.2+77.2","1.2.3-al-pha.2+77.2")


/** --------------------------------------------------------------------
 ** Bad input strings.
 ** ----------------------------------------------------------------- */

#define BAD_INPUT_NEW(FUNCNAME, INPUT_STR, CONDITION_IS)	\
  void								\
  FUNCNAME (cce_destination_t upper_L)				\
  {								\
    cce_location_t	L[1];					\
								\
    if (cce_location(L)) {					\
      if (CONDITION_IS(cce_condition(L))) {			\
        cce_run_error_handlers_final(L);			\
      } else {							\
        cce_run_error_handlers_raise(L, upper_L);		\
      }								\
    } else {							\
      test_new(L, "", INPUT_STR);				\
      cctests_assert(L, false);					\
      cce_run_clean_handlers(L);				\
    }								\
  }

#define BAD_INPUT_INIT(FUNCNAME, INPUT_STR, CONDITION_IS)	\
  void								\
  FUNCNAME (cce_destination_t upper_L)				\
  {								\
    cce_location_t	L[1];					\
								\
    if (cce_location(L)) {					\
      if (CONDITION_IS(cce_condition(L))) {			\
        cce_run_error_handlers_final(L);			\
      } else {							\
        cce_run_error_handlers_raise(L, upper_L);		\
      }								\
    } else {							\
      test_init(L, "", INPUT_STR);				\
      cctests_assert(L, false);					\
      cce_run_clean_handlers(L);				\
    }								\
  }

/* ------------------------------------------------------------------ */

/* Bad because empty string. */
BAD_INPUT_NEW(test_3_1, "",			ccsemver_condition_is_parser_empty_input)

/* Bad because double starting "v". */
BAD_INPUT_NEW(test_3_2, "vv1.2.3",		ccsemver_condition_is_parser_expected_number)

/* Bad because missing minor number and patch level number. */
BAD_INPUT_NEW(test_3_3, "v1",			ccsemver_condition_is_parser_invalid_input)

/* Bad because missing patch level number. */
BAD_INPUT_NEW(test_3_4, "v1.2",			ccsemver_condition_is_parser_invalid_input)

/* Bad because the patch level is not a number. */
BAD_INPUT_NEW(test_3_5, "v1.2.x",		ccsemver_condition_is_parser_expected_number)

/* Bad because the minor number is not a number. */
BAD_INPUT_NEW(test_3_6, "v1.x.2",		ccsemver_condition_is_parser_expected_number)

/* Bad because the major number is not a number. */
BAD_INPUT_NEW(test_3_7, "vx.1.2",		ccsemver_condition_is_parser_expected_number)

/* Bad because missing identifier after the dash separator. */
BAD_INPUT_NEW(test_3_8, "v1.2.3-",		ccsemver_condition_is_parser_end_of_input)

/* Bad because an identifier component is numeric and starts with zero. */
BAD_INPUT_NEW(test_3_9, "v1.2.3-alpha.0123",	ccsemver_condition_is_parser_expected_identifier)

/* Bad because missing build metadata after the plus separator. */
BAD_INPUT_NEW(test_3_10, "v1.2.3+",		ccsemver_condition_is_parser_end_of_input)

/* Bad because missing build metadata after the plus separator. */
BAD_INPUT_NEW(test_3_11, "v1.2.3-alpha.7+",	ccsemver_condition_is_parser_end_of_input)

/* ------------------------------------------------------------------ */

/* Bad because empty string. */
BAD_INPUT_INIT(test_4_1, "",			ccsemver_condition_is_parser_empty_input)

/* Bad because double starting "v". */
BAD_INPUT_INIT(test_4_2, "vv1.2.3",		ccsemver_condition_is_parser_expected_number)

/* Bad because missing minor number and patch level number. */
BAD_INPUT_INIT(test_4_3, "v1",			ccsemver_condition_is_parser_invalid_input)

/* Bad because missing patch level number. */
BAD_INPUT_INIT(test_4_4, "v1.2",		ccsemver_condition_is_parser_invalid_input)

/* Bad because the patch level is not a number. */
BAD_INPUT_INIT(test_4_5, "v1.2.x",		ccsemver_condition_is_parser_expected_number)

/* Bad because the minor number is not a number. */
BAD_INPUT_INIT(test_4_6, "v1.x.2",		ccsemver_condition_is_parser_expected_number)

/* Bad because the major number is not a number. */
BAD_INPUT_INIT(test_4_7, "vx.1.2",		ccsemver_condition_is_parser_expected_number)

/* Bad because missing identifier after the dash separator. */
BAD_INPUT_INIT(test_4_8, "v1.2.3-",		ccsemver_condition_is_parser_end_of_input)

/* Bad because an identifier component is numeric and starts with zero. */
BAD_INPUT_INIT(test_4_9, "v1.2.3-alpha.0123",	ccsemver_condition_is_parser_expected_identifier)

/* Bad because missing build metadata after the plus separator. */
BAD_INPUT_INIT(test_4_10, "v1.2.3+",		ccsemver_condition_is_parser_end_of_input)

/* Bad because missing build metadata after the plus separator. */
BAD_INPUT_INIT(test_4_11, "v1.2.3-alpha.7+",	ccsemver_condition_is_parser_end_of_input)


int
main (void)
{
  ccsemver_init();
  cctests_init("semantic versions");
  {
    cctests_begin_group("correct parsing, new");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
      cctests_run(test_1_4);
      cctests_run(test_1_5);
      cctests_run(test_1_6);
      cctests_run(test_1_7);
      cctests_run(test_1_8);
      cctests_run(test_1_9);
    }
    cctests_end_group();

    cctests_begin_group("correct parsing, init");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_2_3);
      cctests_run(test_2_4);
      cctests_run(test_2_5);
      cctests_run(test_2_6);
      cctests_run(test_2_7);
      cctests_run(test_2_8);
      cctests_run(test_2_9);
    }
    cctests_end_group();

    cctests_begin_group("parsing errors, new");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
      cctests_run(test_3_3);
      cctests_run(test_3_4);
      cctests_run(test_3_5);
      cctests_run(test_3_6);
      cctests_run(test_3_7);
      cctests_run(test_3_8);
      cctests_run(test_3_9);
      cctests_run(test_3_10);
      cctests_run(test_3_11);
    }
    cctests_end_group();

    cctests_begin_group("parsing errors, init");
    {
      cctests_run(test_4_1);
      cctests_run(test_4_2);
      cctests_run(test_4_3);
      cctests_run(test_4_4);
      cctests_run(test_4_5);
      cctests_run(test_4_6);
      cctests_run(test_4_7);
      cctests_run(test_4_8);
      cctests_run(test_4_9);
      cctests_run(test_4_10);
      cctests_run(test_4_11);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
