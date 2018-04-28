/*
  Part of: CCSemver
  Contents: tests for number parsers
  Date: Tue Apr 10, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This  is  free and  unencumbered  software  released into  the  public
  domain.

  Anyone  is free  to  copy,  modify, publish,  use,  compile, sell,  or
  distribute this software, either in source  code form or as a compiled
  binary,  for any  purpose, commercial  or non-commercial,  and by  any
  means.

  In jurisdictions that recognize copyright  laws, the author or authors
  of  this software  dedicate  any  and all  copyright  interest in  the
  software to the public domain. We make this dedication for the benefit
  of  the  public  at large  and  to  the  detriment  of our  heirs  and
  successors.  We  intend  this  dedication   to  be  an  overt  act  of
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Number parsers.
 ** ----------------------------------------------------------------- */

static void
test_number_successful_parsing (cce_destination_t upper_L, char const * input_str, long expected_num)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    long		num;

    num = ccsemver_parse_number(L, &input);
    cctests_assert(L, expected_num == num);

    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_1_1_1 (cce_destination_t upper_L)
/* Successfully parsing a number delimited by end-of-input. */
{
  test_number_successful_parsing(upper_L, "123", 123);
}

void
test_1_1_2 (cce_destination_t upper_L)
/* Successfully parsing a number delimited by a non-numeric character. */
{
  test_number_successful_parsing(upper_L, "12v3", 12);
}

void
test_1_1_3 (cce_destination_t upper_L)
/* Successfully parsing a number delimited by a dot. */
{
  test_number_successful_parsing(upper_L, "12.3", 12);
}

/* ------------------------------------------------------------------ */

void
test_1_2_1 (cce_destination_t upper_L)
/* Exception: empty input. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input    (cce_condition(L)) &&
	ccsemver_condition_is_parser_empty_input (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_1_2_2 (cce_destination_t upper_L)
/* Exception: end-of-input. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input     (cce_condition(L)) &&
	ccsemver_condition_is_parser_end_of_input (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


void
test_1_2_3 (cce_destination_t upper_L)
/* Exception: invalid input offset. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input             (cce_condition(L)) &&
	ccsemver_condition_is_parser_invalid_input_offset (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len + 3;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_1_3_1 (cce_destination_t upper_L)
/* Exception: expected number, found alphabetic string. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_number(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_1_3_2 (cce_destination_t upper_L)
/* Exception: expected number without sign, found negative sign. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_number(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "-123";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_1_3_3 (cce_destination_t upper_L)
/* Exception: expected number without sign, found positive sign. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_number(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "+123";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_1_3_4 (cce_destination_t upper_L)
/* Exception: expected number in positive "long" range, found out of range. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_number_out_of_range(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_number(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Numeric component parsers.
 ** ----------------------------------------------------------------- */

static void
test_numeric_component_successful_parsing (cce_destination_t upper_L, char const * input_str, long expected_num)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    long		num;

    num = ccsemver_parse_numeric_component(L, &input);
    cctests_assert(L, expected_num == num);

    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1_1 (cce_destination_t upper_L)
/* Successfully parsing a numeric component delimited by end-of-input. */
{
  test_numeric_component_successful_parsing(upper_L, "123", 123);
}

void
test_2_1_2 (cce_destination_t upper_L)
/* Successfully parsing a numeric component delimited by a non-numeric character. */
{
  test_numeric_component_successful_parsing(upper_L, "12v3", 12);
}

void
test_2_1_3 (cce_destination_t upper_L)
/* Successfully parsing a numeric component delimited by a dot. */
{
  test_numeric_component_successful_parsing(upper_L, "12.3", 12);
}

void
test_2_1_4 (cce_destination_t upper_L)
/* Successfully parsing a range numeric component. */
{
  test_numeric_component_successful_parsing(upper_L, "x", CCSEMVER_NUM_X);
}

void
test_2_1_5 (cce_destination_t upper_L)
/* Successfully parsing a range numeric component. */
{
  test_numeric_component_successful_parsing(upper_L, "X", CCSEMVER_NUM_X);
}

void
test_2_1_6 (cce_destination_t upper_L)
/* Successfully parsing a range numeric component. */
{
  test_numeric_component_successful_parsing(upper_L, "*", CCSEMVER_NUM_X);
}

/* ------------------------------------------------------------------ */

void
test_2_2_1 (cce_destination_t upper_L)
/* Exception: empty input. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input    (cce_condition(L)) &&
	ccsemver_condition_is_parser_empty_input (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_2_2_2 (cce_destination_t upper_L)
/* Exception: end-of-input. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input     (cce_condition(L)) &&
	ccsemver_condition_is_parser_end_of_input (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


void
test_2_2_3 (cce_destination_t upper_L)
/* Exception: invalid input offset. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_no_input             (cce_condition(L)) &&
	ccsemver_condition_is_parser_invalid_input_offset (cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len + 3;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_3_1 (cce_destination_t upper_L)
/* Exception: expected numeric component, found alphabetic string. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_numeric_component(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_2_3_2 (cce_destination_t upper_L)
/* Exception: expected numeric component without sign, found negative sign. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_numeric_component(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "-123";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_2_3_3 (cce_destination_t upper_L)
/* Exception: expected numeric component without sign, found positive sign. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_expected_numeric_component(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "+123";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

void
test_2_3_4 (cce_destination_t upper_L)
/* Exception: expected numeric component in positive "long" range, found out of range. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_number_out_of_range(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_error_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_parse_numeric_component(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Numeric component comparison.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t L)
{
  cctests_assert(L, 0 == ccsemver_num_comp(CCSEMVER_NUM_X, 123));
}

void
test_3_2 (cce_destination_t L)
{
  cctests_assert(L, 0 == ccsemver_num_comp(123, CCSEMVER_NUM_X));
}

void
test_3_3 (cce_destination_t L)
{
  cctests_assert(L, 0 == ccsemver_num_comp(CCSEMVER_NUM_X, CCSEMVER_NUM_X));
}

void
test_3_4 (cce_destination_t L)
{
  cctests_assert(L, -1 == ccsemver_num_comp(1, 2));
}

void
test_3_5 (cce_destination_t L)
{
  cctests_assert(L, +1 == ccsemver_num_comp(2, 1));
}

void
test_3_6 (cce_destination_t L)
{
  cctests_assert(L, 0  == ccsemver_num_comp(1, 1));
}


int
main (void)
{
  ccsemver_init();

  cctests_init("number and numeric component parsers");
  {
    cctests_begin_group("number parsers");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);

      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
      cctests_run(test_1_2_3);

      cctests_run(test_1_3_1);
      cctests_run(test_1_3_2);
      cctests_run(test_1_3_3);
      cctests_run(test_1_3_4);
    }
    cctests_end_group();

    cctests_begin_group("numeric component parsers");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_1_4);
      cctests_run(test_2_1_5);
      cctests_run(test_2_1_6);

      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);

      cctests_run(test_2_3_1);
      cctests_run(test_2_3_2);
      cctests_run(test_2_3_3);
      cctests_run(test_2_3_4);
    }
    cctests_end_group();

    cctests_begin_group("numeric component comparison");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
      cctests_run(test_3_3);
      cctests_run(test_3_4);
      cctests_run(test_3_5);
      cctests_run(test_3_6);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
