/*
  Part of: CCSemver
  Contents: tests for comparator parsers


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
test_new (cce_destination_t upper_L, char const * const expected, char const * const input_str, bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_cmp_t *	cmp;

    if (0) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, input.len, input.str); }

    cmp = ccsemver_cmp_new_guarded(L, cmp_H, &input);

    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&input));
      cctests_assert(L, true  == ccsemver_input_at_end(&input));
    }

    {
      int	slen = ccsemver_cmp_write(cmp, buffer, 1024);

      if (0) { fprintf(stderr, "%s: serialised: len=%d, cmp='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_body_handlers(L);
  }
}

void
test_init (cce_destination_t upper_L, char const * const expected, char const * const input_str, bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_cmp_t	cmp[1];

    if (0) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, input.len, input.str); }

    ccsemver_cmp_init_guarded(L, cmp_H, cmp, &input);

    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&input));
      cctests_assert(L, true  == ccsemver_input_at_end(&input));
    }

    {
      int	slen = ccsemver_cmp_write(cmp, buffer, 1024);

      if (0) { fprintf(stderr, "%s: serialised: len=%d, cmp='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_body_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_cmp_and (cce_destination_t upper_L,
	      char const * const expected,
	      char const * const base_str,
	      char const * const more_str,
	      bool parse_to_the_end)
{
  cce_location_t	L[1];
  cce_clean_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	base_input = ccsemver_input_new(base_str, strlen(base_str), 0);
    ccsemver_input_t	more_input = ccsemver_input_new(more_str, strlen(more_str), 0);
    char		buffer[1024];
    ccsemver_cmp_t *	cmp;

    if (0) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, base_input.len, base_input.str); }
    if (0) { fprintf(stderr, "%s: more:  len=%lu, str='%s'\n", __func__, more_input.len, more_input.str); }

    cmp = ccsemver_cmp_new_guarded(L, cmp_H, &base_input);
    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&base_input));
      cctests_assert(L, true  == ccsemver_input_at_end(&base_input));
    }

    ccsemver_cmp_and(L, cmp, &more_input);
    /* Have we parsed the whole input? */
    if (parse_to_the_end) {
      cctests_assert(L, false == ccsemver_input_more(&more_input));
      cctests_assert(L, true  == ccsemver_input_at_end(&more_input));
    }

    {
      int	slen = ccsemver_cmp_write(cmp, buffer, 1024);

      if (0) { fprintf(stderr, "%s: serialised: len=%d, cmp='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Simple comparators: successful parsing.
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

/* Straight semantic version. */
GOOD_INPUT_NEW(test_1_1_1, true,	"1.2.3",			"1.2.3")
GOOD_INPUT_NEW(test_1_1_2, true,	"1.2.3-alpha.1",		"1.2.3-alpha.1")
GOOD_INPUT_NEW(test_1_1_3, true,	"1.2.3+x86-64",		"1.2.3+x86-64")
GOOD_INPUT_NEW(test_1_1_4, true,	"1.2.3-alpha.1+x86-64",	"1.2.3-alpha.1+x86-64")

/* Star X-Range. */
GOOD_INPUT_NEW(test_1_2_1, true,	">=0.0.0",		"*")
GOOD_INPUT_NEW(test_1_2_2, true,	">=1.0.0 <2.0.0",	"1.*")
GOOD_INPUT_NEW(test_1_2_3, true,	">=1.2.0 <1.3.0",	"1.2.*")
GOOD_INPUT_NEW(test_1_2_4, true,	">=0.0.0",		"*.*")
GOOD_INPUT_NEW(test_1_2_5, true,	">=0.0.0",		"*.*.*")
GOOD_INPUT_NEW(test_1_2_6, true,	">=1.0.0 <2.0.0",	"1.*.*")

/* Small x X-Range. */
GOOD_INPUT_NEW(test_1_3_1, true,	">=0.0.0",		"x")
GOOD_INPUT_NEW(test_1_3_2, true,	">=1.0.0 <2.0.0",	"1.x")
GOOD_INPUT_NEW(test_1_3_3, true,	">=1.2.0 <1.3.0",	"1.2.x")
GOOD_INPUT_NEW(test_1_3_4, true,	">=0.0.0",		"x.x")
GOOD_INPUT_NEW(test_1_3_5, true,	">=0.0.0",		"x.x.x")
GOOD_INPUT_NEW(test_1_3_6, true,	">=1.0.0 <2.0.0",	"1.x.x")

/* Capital X X-Range. */
GOOD_INPUT_NEW(test_1_4_1, true,	">=0.0.0",		"X")
GOOD_INPUT_NEW(test_1_4_2, true,	">=1.0.0 <2.0.0",	"1.X")
GOOD_INPUT_NEW(test_1_4_3, true,	">=1.2.0 <1.3.0",	"1.2.X")
GOOD_INPUT_NEW(test_1_4_4, true,	">=0.0.0",		"X.X")
GOOD_INPUT_NEW(test_1_4_5, true,	">=0.0.0",		"X.X.X")
GOOD_INPUT_NEW(test_1_4_6, true,	">=1.0.0 <2.0.0",	"1.X.X")

/* Miscellaneous. */

/* Blanks after valid simple comparator. */
GOOD_INPUT_NEW(test_1_5_1, false,	">=0.0.0",		"* ")
GOOD_INPUT_NEW(test_1_5_2, false,	">=0.0.0",		"*    ")
GOOD_INPUT_NEW(test_1_5_3, false,	">=0.0.0",		"*  \t\r\n  ")

/* Vertical bar after valid simple comparator. */
GOOD_INPUT_NEW(test_1_5_4, false,	">=0.0.0",		"*  |")
GOOD_INPUT_NEW(test_1_5_5, false,	">=0.0.0",		"* || *")

/* ------------------------------------------------------------------ */

/* Straight semantic version. */
GOOD_INPUT_INIT(test_2_1_1, true,	"1.2.3",			"1.2.3")
GOOD_INPUT_INIT(test_2_1_2, true,	"1.2.3-alpha.1",		"1.2.3-alpha.1")
GOOD_INPUT_INIT(test_2_1_3, true,	"1.2.3+x86-64",		"1.2.3+x86-64")
GOOD_INPUT_INIT(test_2_1_4, true,	"1.2.3-alpha.1+x86-64",	"1.2.3-alpha.1+x86-64")

/* Star X-Range. */
GOOD_INPUT_INIT(test_2_2_1, true,	">=0.0.0",		"*")
GOOD_INPUT_INIT(test_2_2_2, true,	">=1.0.0 <2.0.0",	"1.*")
GOOD_INPUT_INIT(test_2_2_3, true,	">=1.2.0 <1.3.0",	"1.2.*")
GOOD_INPUT_INIT(test_2_2_4, true,	">=0.0.0",		"*.*")
GOOD_INPUT_INIT(test_2_2_5, true,	">=0.0.0",		"*.*.*")
GOOD_INPUT_INIT(test_2_2_6, true,	">=1.0.0 <2.0.0",	"1.*.*")

/* Small x X-Range. */
GOOD_INPUT_INIT(test_2_3_1, true,	">=0.0.0",		"x")
GOOD_INPUT_INIT(test_2_3_2, true,	">=1.0.0 <2.0.0",	"1.x")
GOOD_INPUT_INIT(test_2_3_3, true,	">=1.2.0 <1.3.0",	"1.2.x")
GOOD_INPUT_INIT(test_2_3_4, true,	">=0.0.0",		"x.x")
GOOD_INPUT_INIT(test_2_3_5, true,	">=0.0.0",		"x.x.x")
GOOD_INPUT_INIT(test_2_3_6, true,	">=1.0.0 <2.0.0",	"1.x.x")

/* Capital X X-Range. */
GOOD_INPUT_INIT(test_2_4_1, true,	">=0.0.0",		"X")
GOOD_INPUT_INIT(test_2_4_2, true,	">=1.0.0 <2.0.0",	"1.X")
GOOD_INPUT_INIT(test_2_4_3, true,	">=1.2.0 <1.3.0",	"1.2.X")
GOOD_INPUT_INIT(test_2_4_4, true,	">=0.0.0",		"X.X")
GOOD_INPUT_INIT(test_2_4_5, true,	">=0.0.0",		"X.X.X")
GOOD_INPUT_INIT(test_2_4_6, true,	">=1.0.0 <2.0.0",	"1.X.X")

/* Miscellaneous. */

/* Blanks after valid simple comparator. */
GOOD_INPUT_INIT(test_2_5_1, false,	">=0.0.0",		"* ")
GOOD_INPUT_INIT(test_2_5_2, false,	">=0.0.0",		"*    ")
GOOD_INPUT_INIT(test_2_5_3, false,	">=0.0.0",		"*  \t\r\n  ")

/* Vertical bar after valid simple comparator. */
GOOD_INPUT_INIT(test_2_5_4, false,	">=0.0.0",		"*  |")
GOOD_INPUT_INIT(test_2_5_5, false,	">=0.0.0",		"* || *")


/** --------------------------------------------------------------------
 ** Simple comparators: parsing errors.
 ** ----------------------------------------------------------------- */

#define BAD_INPUT_NEW(FUNCNAME, INPUT_STR, CONDITION_IS) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
									\
    if (cce_location(L)) {						\
      if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); } \
      if (CONDITION_IS(cce_condition(L))) {				\
        cce_run_catch_handlers_final(L);				\
      } else {								\
        cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      test_new(L, "", INPUT_STR, false);				\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

#define BAD_INPUT_INIT(FUNCNAME, INPUT_STR, CONDITION_IS) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    cce_location_t	L[1];						\
									\
    if (cce_location(L)) {						\
      if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); } \
      if (CONDITION_IS(cce_condition(L))) {				\
        cce_run_catch_handlers_final(L);				\
      } else {								\
        cce_run_catch_handlers_raise(L, upper_L);			\
      }									\
    } else {								\
      test_init(L, "", INPUT_STR, false);				\
      cctests_assert(L, false);						\
      cce_run_body_handlers(L);					\
    }									\
  }

/* ------------------------------------------------------------------ */

/* Bad because: expected numeric component as major version number. */
BAD_INPUT_NEW(test_3_1,  "abc",		ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_2,  ">",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_3,  "<=",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_4,  "~",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_5,  "^",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_6, "=",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial sementic version expected after dash operator. */
BAD_INPUT_NEW(test_3_7, "1.2.3 -",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as major number. */
BAD_INPUT_NEW(test_3_8, "a.2.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as minor number. */
BAD_INPUT_NEW(test_3_9, "1.a.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as patch level. */
BAD_INPUT_NEW(test_3_10, "1.2.a",	ccsemver_condition_is_parser_expected_numeric_component)

/* ------------------------------------------------------------------ */

/* Bad because: expected numeric component as major version number. */
BAD_INPUT_INIT(test_4_1,  "abc",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_INIT(test_4_2,  ">",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_INIT(test_4_3,  "<=",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_INIT(test_4_4,  "~",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_INIT(test_4_5,  "^",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_INIT(test_4_6, "=",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial sementic version expected after dash operator. */
BAD_INPUT_INIT(test_4_7, "1.2.3 -",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as major number. */
BAD_INPUT_INIT(test_4_8, "a.2.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as minor number. */
BAD_INPUT_INIT(test_4_9, "1.a.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as patch level. */
BAD_INPUT_INIT(test_4_10, "1.2.a",	ccsemver_condition_is_parser_expected_numeric_component)


/** --------------------------------------------------------------------
 ** Prerelease tag and build metadata.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_5_1_1,  true,	"1.2.3-alpha",			"1.2.3-alpha")
GOOD_INPUT_NEW(test_5_1_2,  true,	"1.2.3-alpha.2",		"1.2.3-alpha.2")
GOOD_INPUT_NEW(test_5_1_3,  true,	"1.2.3+77",			"1.2.3+77")
GOOD_INPUT_NEW(test_5_1_4,  true,	"1.2.3+77.2",			"1.2.3+77.2")
GOOD_INPUT_NEW(test_5_1_5,  true,	"1.2.3-alpha.2+77",		"1.2.3-alpha.2+77")
GOOD_INPUT_NEW(test_5_1_6,  true,	"1.2.3-alpha.2+77.2",		"1.2.3-alpha.2+77.2")
GOOD_INPUT_NEW(test_5_1_7,  true,	"1.2.3-al-pha.2+77",		"1.2.3-al-pha.2+77")
GOOD_INPUT_NEW(test_5_1_8,  true,	"1.2.3-al-pha.2+77.2",		"1.2.3-al-pha.2+77.2")

GOOD_INPUT_NEW(test_5_2_1,  true,	">=0.0.0 1.2.3-alpha",		"* 1.2.3-alpha")
GOOD_INPUT_NEW(test_5_2_2,  true,	">=0.0.0 1.2.3-alpha.2",	"* 1.2.3-alpha.2")
GOOD_INPUT_NEW(test_5_2_3,  true,	">=0.0.0 1.2.3+77",		"* 1.2.3+77")
GOOD_INPUT_NEW(test_5_2_4,  true,	">=0.0.0 1.2.3+77.2",		"* 1.2.3+77.2")
GOOD_INPUT_NEW(test_5_2_5,  true,	">=0.0.0 1.2.3-alpha.2+77",	"* 1.2.3-alpha.2+77")
GOOD_INPUT_NEW(test_5_2_6,  true,	">=0.0.0 1.2.3-alpha.2+77.2",	"* 1.2.3-alpha.2+77.2")
GOOD_INPUT_NEW(test_5_2_7,  true,	">=0.0.0 1.2.3-al-pha.2+77",	"* 1.2.3-al-pha.2+77")
GOOD_INPUT_NEW(test_5_2_8,  true,	">=0.0.0 1.2.3-al-pha.2+77.2",	"* 1.2.3-al-pha.2+77.2")


/** --------------------------------------------------------------------
 ** X-Ranges.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_6_1_1,  true,	">=0.0.0",		"*")
GOOD_INPUT_NEW(test_6_1_2,  true,	">=1.0.0 <2.0.0",	"1.x")
GOOD_INPUT_NEW(test_6_1_3,  true,	">=1.2.0 <1.3.0",	"1.2.x")
GOOD_INPUT_NEW(test_6_1_4,  true,	">=1.0.0 <2.0.0",	"1")
GOOD_INPUT_NEW(test_6_1_5,  true,	">=1.2.0 <1.3.0",	"1.2")


/** --------------------------------------------------------------------
 ** Hyphen.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_7_1_1,  true,	">=1.2.3 <=2.3.4",	"1.2.3 - 2.3.4")
GOOD_INPUT_NEW(test_7_1_2,  true,	">=1.2.0 <=2.3.4",	"1.2   - 2.3.4")
GOOD_INPUT_NEW(test_7_1_3,  true,	">=1.2.3 <2.4.0",	"1.2.3 - 2.3")
GOOD_INPUT_NEW(test_7_1_4,  true,	">=1.2.3 <3.0.0",	"1.2.3 - 2")


/** --------------------------------------------------------------------
 ** Tilde.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_8_1_1,  true,	">=1.2.3 <1.3.0",	"~1.2.3")
GOOD_INPUT_NEW(test_8_1_2,  true,	">=1.2.0 <1.3.0",	"~1.2")
GOOD_INPUT_NEW(test_8_1_3,  true,	">=1.0.0 <2.0.0",	"~1")
GOOD_INPUT_NEW(test_8_1_4,  true,	">=0.2.3 <0.3.0",	"~0.2.3")
GOOD_INPUT_NEW(test_8_1_5,  true,	">=0.2.0 <0.3.0",	"~0.2")
GOOD_INPUT_NEW(test_8_1_6,  true,	">=0.0.0 <1.0.0",	"~0")


/** --------------------------------------------------------------------
 ** Caret.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_9_1_1,  true,	">=1.2.3 <2.0.0",	"^1.2.3")
GOOD_INPUT_NEW(test_9_1_2,  true,	">=0.2.3 <0.3.0",	"^0.2.3")
GOOD_INPUT_NEW(test_9_1_3,  true,	">=0.0.3 <0.0.4",	"^0.0.3")


/** --------------------------------------------------------------------
 ** Operators.
 ** ----------------------------------------------------------------- */

GOOD_INPUT_NEW(test_10_1_1,  true,	">=1.2.3 <2.0.0",	">=1.2.3 <2.0")
GOOD_INPUT_NEW(test_10_1_2,  true,	">=0.2.3 <0.3.0",	">=0.2.3 <0.3")
GOOD_INPUT_NEW(test_10_1_3,  true,	">=0.5.0 <0.0.4",	">=0.5 <0.0.4")
GOOD_INPUT_NEW(test_10_1_4,  true,	">0.0.3",		">0.0.3")
GOOD_INPUT_NEW(test_10_1_5,  true,	"0.0.3",		"=0.0.3")
GOOD_INPUT_NEW(test_10_1_6,  true,	"9.0.0",		"=9")


/** --------------------------------------------------------------------
 ** Appending comparators.
 ** ----------------------------------------------------------------- */

#define GOOD_APPEND(FUNCNAME, PARSE_TO_THE_END, EXPECTED, BASE_STR, MORE_STR) \
  void									\
  FUNCNAME (cce_destination_t upper_L)					\
  {									\
    test_cmp_and(upper_L, EXPECTED, BASE_STR, MORE_STR, PARSE_TO_THE_END); \
  }

GOOD_APPEND(test_11_1_1,  true,	">=0.0.0 >=0.0.3 <0.0.4",				"*",		"^0.0.3")
GOOD_APPEND(test_11_1_2,  true,	">=1.0.0 <2.0.0 >=0.0.3 <0.0.4",			"1.x",		"^0.0.3")
GOOD_APPEND(test_11_1_3,  true,	">=1.2.0 <1.3.0 >=0.0.3 <0.0.4",			"1.2.x",	"^0.0.3")
GOOD_APPEND(test_11_1_4,  true,	">=1.2.0 <1.3.0 >=1.2.0 <1.3.0 >=0.0.3 <0.0.4",		"1.2 1.2.x",	"^0.0.3")
GOOD_APPEND(test_11_1_5,  true,	">=1.0.0 <2.0.0 >=0.0.3 <0.0.4",			"1",	      "^0.0.3")
GOOD_APPEND(test_11_1_6,  true,	">=1.2.0 <1.3.0 >=0.0.3 <0.0.4",			"1.2",	      "^0.0.3")
GOOD_APPEND(test_11_1_7,  true,	">=1.2.0 <1.3.0",					"1.2",	      "")


int
main (void)
{
  ccsemver_library_init();
  cctests_init("parsing comparators");
  {
    cctests_begin_group("simple comparators: successful parsing, new function");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);
      cctests_run(test_1_1_4);

      /* Star wildcard. */
      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
      cctests_run(test_1_2_3);
      cctests_run(test_1_2_4);
      cctests_run(test_1_2_5);
      cctests_run(test_1_2_6);

      /* Small x wildcard. */
      cctests_run(test_1_3_1);
      cctests_run(test_1_3_2);
      cctests_run(test_1_3_3);
      cctests_run(test_1_3_4);
      cctests_run(test_1_3_5);
      cctests_run(test_1_3_6);

      /* Capital X wildcard. */
      cctests_run(test_1_4_1);
      cctests_run(test_1_4_2);
      cctests_run(test_1_4_3);
      cctests_run(test_1_4_4);
      cctests_run(test_1_4_5);
      cctests_run(test_1_4_6);

      cctests_run(test_1_5_1);
      cctests_run(test_1_5_2);
      cctests_run(test_1_5_3);
      cctests_run(test_1_5_4);
      cctests_run(test_1_5_5);
    }
    cctests_end_group();

    cctests_begin_group("simple comparators: successful parsing, init function");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_1_4);

      /* Star wildcard. */
      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);
      cctests_run(test_2_2_4);
      cctests_run(test_2_2_5);
      cctests_run(test_2_2_6);

      /* Small x wildcard. */
      cctests_run(test_2_3_1);
      cctests_run(test_2_3_2);
      cctests_run(test_2_3_3);
      cctests_run(test_2_3_4);
      cctests_run(test_2_3_5);
      cctests_run(test_2_3_6);

      /* Capital X wildcard. */
      cctests_run(test_2_4_1);
      cctests_run(test_2_4_2);
      cctests_run(test_2_4_3);
      cctests_run(test_2_4_4);
      cctests_run(test_2_4_5);
      cctests_run(test_2_4_6);

      cctests_run(test_2_5_1);
      cctests_run(test_2_5_2);
      cctests_run(test_2_5_3);
      cctests_run(test_2_5_4);
      cctests_run(test_2_5_5);
    }
    cctests_end_group();

    cctests_begin_group("parsing failures, new function");
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
    }
    cctests_end_group();

    cctests_begin_group("parsing failures, init function");
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
    }
    cctests_end_group();

    cctests_begin_group("prerelease tag and build metadata");
    {
      cctests_run(test_5_1_1);
      cctests_run(test_5_1_2);
      cctests_run(test_5_1_3);
      cctests_run(test_5_1_4);
      cctests_run(test_5_1_5);
      cctests_run(test_5_1_6);
      cctests_run(test_5_1_7);
      cctests_run(test_5_1_8);

      cctests_run(test_5_2_1);
      cctests_run(test_5_2_2);
      cctests_run(test_5_2_3);
      cctests_run(test_5_2_4);
      cctests_run(test_5_2_5);
      cctests_run(test_5_2_6);
      cctests_run(test_5_2_7);
      cctests_run(test_5_2_8);
    }
    cctests_end_group();

    cctests_begin_group("X-Ranges");
    {
      cctests_run(test_6_1_1);
      cctests_run(test_6_1_2);
      cctests_run(test_6_1_3);
      cctests_run(test_6_1_4);
      cctests_run(test_6_1_5);
    }
    cctests_end_group();

    cctests_begin_group("X-Ranges");
    {
      cctests_run(test_6_1_1);
      cctests_run(test_6_1_2);
      cctests_run(test_6_1_3);
      cctests_run(test_6_1_4);
      cctests_run(test_6_1_5);
    }
    cctests_end_group();

    cctests_begin_group("Hyphen");
    {
      cctests_run(test_7_1_1);
      cctests_run(test_7_1_2);
      cctests_run(test_7_1_3);
      cctests_run(test_7_1_4);
    }
    cctests_end_group();

    cctests_begin_group("Tilde");
    {
      cctests_run(test_8_1_1);
      cctests_run(test_8_1_2);
      cctests_run(test_8_1_3);
      cctests_run(test_8_1_4);
      cctests_run(test_8_1_5);
      cctests_run(test_8_1_6);
    }
    cctests_end_group();

    cctests_begin_group("Caret");
    {
      cctests_run(test_9_1_1);
      cctests_run(test_9_1_2);
      cctests_run(test_9_1_3);
    }
    cctests_end_group();

    cctests_begin_group("operators");
    {
      cctests_run(test_10_1_1);
      cctests_run(test_10_1_2);
      cctests_run(test_10_1_3);
      cctests_run(test_10_1_4);
      cctests_run(test_10_1_5);
      cctests_run(test_10_1_6);
    }
    cctests_end_group();

    cctests_begin_group("appending comparators");
    {
      cctests_run(test_11_1_1);
      cctests_run(test_11_1_2);
      cctests_run(test_11_1_3);
      cctests_run(test_11_1_4);
      cctests_run(test_11_1_5);
      cctests_run(test_11_1_6);
      cctests_run(test_11_1_7);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
