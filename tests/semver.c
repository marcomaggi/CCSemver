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

#define STRNSIZE(s) (s), sizeof(s)-1


void
test_new (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	sv_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
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

    cce_run_cleanup_handlers(L);
  }
}

void
test_init (cce_destination_t upper_L, char const * const expected, char const * const input_str)
{
  cce_location_t	L[1];
  cce_cleanup_handler_t	sv_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
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

    cce_run_cleanup_handlers(L);
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

    cctests_begin_group("parsing errors");
    {
#if (0)
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
#endif
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
