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
test_new (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_comp_t *	cmp;

    if (1) { fprintf(stderr, "%s: input: len=%lu, str='%s'\n", __func__, input.len, input.str); }

    cmp = ccsemver_comp_new_guarded(L, cmp_H, &input);

    /* Have we parsed the whole input? */
    cctests_assert(L, false == ccsemver_input_more(&input));
    cctests_assert(L, true  == ccsemver_input_at_end(&input));

    {
      int	slen = ccsemver_comp_write(cmp, buffer, 1024);

      if (1) { fprintf(stderr, "%s: serialised: len=%d, cmp='%s'\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_cleanup_handlers(L);
  }
}

void
test_init (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	cmp_H[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
    char		buffer[1024];
    ccsemver_comp_t	cmp[1];

    if (0) { fprintf(stderr, "test: len=%lu, str='%s'\n", input.len, input.str); }

    ccsemver_comp_init_guarded(L, cmp_H, cmp, &input);

    /* Have we parsed the whole input? */
    cctests_assert(L, false == ccsemver_input_more(&input));
    cctests_assert(L, true  == ccsemver_input_at_end(&input));

    {
      int	slen = ccsemver_comp_write(cmp, buffer, 1024);

      if (0) { fprintf(stderr, "%s: len=%d, cmp=%s\n", __func__, slen, buffer); }
      cctests_assert_equal_int(L, strlen(expected), slen);
      cctests_assert_ascii(L, expected, buffer, slen);
    }

    cce_run_cleanup_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Simple successful parsing.
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

/* ------------------------------------------------------------------ */

/* Straight semantic version. */
GOOD_INPUT_NEW(test_1_1_1, "1.2.3",			"1.2.3")
GOOD_INPUT_NEW(test_1_1_2, "1.2.3-alpha.1",		"1.2.3-alpha.1")
GOOD_INPUT_NEW(test_1_1_3, "1.2.3+x86-64",		"1.2.3+x86-64")
GOOD_INPUT_NEW(test_1_1_4, "1.2.3-alpha.1+x86-64",	"1.2.3-alpha.1+x86-64")

/* Star wildcard. */
GOOD_INPUT_NEW(test_1_2_1, ">=0.0.0",		"*")
GOOD_INPUT_NEW(test_1_2_2, ">=1.0.0 <2.0.0",	"1.*")
GOOD_INPUT_NEW(test_1_2_3, ">=1.2.0 <1.3.0",	"1.2.*")

/* Small x wildcard. */
GOOD_INPUT_NEW(test_1_3_1, ">=0.0.0",		"x")
GOOD_INPUT_NEW(test_1_3_2, ">=1.0.0 <2.0.0",	"1.x")
GOOD_INPUT_NEW(test_1_3_3, ">=1.2.0 <1.3.0",	"1.2.x")

/* Capital X wildcard. */
GOOD_INPUT_NEW(test_1_4_1, ">=0.0.0",		"X")
GOOD_INPUT_NEW(test_1_4_2, ">=1.0.0 <2.0.0",	"1.X")
GOOD_INPUT_NEW(test_1_4_3, ">=1.2.0 <1.3.0",	"1.2.X")

/* GOOD_INPUT_NEW(test_1_2, ">=0.0.0",		"X") */
/* GOOD_INPUT_NEW(test_1_3, ">=1.0.0 <2.0.0",	"1.X") */
/* GOOD_INPUT_NEW(test_1_4, ">=1.2.0 <1.3.0",	"1.2.X") */

/* ------------------------------------------------------------------ */

GOOD_INPUT_INIT(test_2_1, "1.2.3", "1.2.3")


/** --------------------------------------------------------------------
 ** Parsing errors.
 ** ----------------------------------------------------------------- */

#define BAD_INPUT_NEW(FUNCNAME, INPUT_STR, CONDITION_IS)	\
  void								\
  FUNCNAME (cce_destination_t upper_L)				\
  {								\
    cce_location_t	L[1];					\
								\
    if (cce_location(L)) {					\
      if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); } \
      if (CONDITION_IS(cce_condition(L))) {			\
        cce_run_error_handlers_final(L);			\
      } else {							\
        cce_run_error_handlers_raise(L, upper_L);		\
      }								\
    } else {							\
      test_new(L, "", INPUT_STR);				\
      cctests_assert(L, false);					\
      cce_run_cleanup_handlers(L);				\
    }								\
  }

#define BAD_INPUT_INIT(FUNCNAME, INPUT_STR, CONDITION_IS)	\
  void								\
  FUNCNAME (cce_destination_t upper_L)				\
  {								\
    cce_location_t	L[1];					\
								\
    if (cce_location(L)) {					\
      if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); } \
      if (CONDITION_IS(cce_condition(L))) {			\
        cce_run_error_handlers_final(L);			\
      } else {							\
        cce_run_error_handlers_raise(L, upper_L);		\
      }								\
    } else {							\
      test_init(L, "", INPUT_STR);				\
      cctests_assert(L, false);					\
      cce_run_cleanup_handlers(L);				\
    }								\
  }

/* ------------------------------------------------------------------ */

BAD_INPUT_NEW(test_3_1,  "* ",		ccsemver_condition_is_parser_invalid_input)
BAD_INPUT_NEW(test_3_2,  "*  ",		ccsemver_condition_is_parser_invalid_input)
BAD_INPUT_NEW(test_3_3,  "*  |",	ccsemver_condition_is_parser_invalid_input)

BAD_INPUT_NEW(test_3_4,  "* || *",	ccsemver_condition_is_parser_invalid_input)

/* Bad because: expected numeric component as major version number. */
BAD_INPUT_NEW(test_3_5,  "abc",		ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_6,  ">",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_7,  "<=",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_8,  "~",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_9,  "^",		ccsemver_condition_is_parser_end_of_input)

/* Bad because: partial semantic version expected after operator. */
BAD_INPUT_NEW(test_3_10, "=",		ccsemver_condition_is_parser_end_of_input)

BAD_INPUT_NEW(test_3_11, "1.2.3 ",	ccsemver_condition_is_parser_invalid_input)

/* Bad because: partial sementic version expected after dash operator. */
BAD_INPUT_NEW(test_3_12, "1.2.3 -",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as major number. */
BAD_INPUT_NEW(test_3_13, "a.2.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as minor number. */
BAD_INPUT_NEW(test_3_14, "1.a.3",	ccsemver_condition_is_parser_expected_numeric_component)

/* Bad because: numeric component expected as patch level. */
BAD_INPUT_NEW(test_3_15, "1.2.a",	ccsemver_condition_is_parser_expected_numeric_component)


#if 0
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
#endif


int
main (void)
{
  ccsemver_init();
  cctests_init("parsing comparators");
  {
    cctests_begin_group("simple successful parsing");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);
      cctests_run(test_1_1_4);

      /* Star wildcard. */
      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
      cctests_run(test_1_2_3);

      /* Small x wildcard. */
      cctests_run(test_1_3_1);
      cctests_run(test_1_3_2);
      cctests_run(test_1_3_3);

      /* Capital X wildcard. */
      cctests_run(test_1_4_1);
      cctests_run(test_1_4_2);
      cctests_run(test_1_4_3);

      cctests_run(test_2_1);
    }
    cctests_end_group();

    cctests_begin_group("parsing failures, new");
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
      cctests_run(test_3_12);
      cctests_run(test_3_13);
      cctests_run(test_3_14);
      cctests_run(test_3_15);
    }
    cctests_end_group();

#if 0
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
#endif

#if 0
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
#endif

  }
  cctests_final();
}

/* end of file */
