/*
  Part of: CCSemver
  Contents: tests for identifier parsers
  Date: Apr 10, 2018

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
 ** Identifier parsers: new operation.
 ** ----------------------------------------------------------------- */

static void
test_identifier_successful_parsing_new (cce_destination_t upper_L, char const * input_str, char const * expected_id)
{
  cce_location_t	L[1];
  cce_clean_handler_t	id_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t *	id;
    size_t		serial_len	= 1024;
    char		serial_str[serial_len];

    id = ccsemver_id_new_guarded(L, id_H, &input);

    serial_len = ccsemver_id_write(id, serial_str, serial_len);
    if (0) {
      fprintf(stderr, "%s: id=", __func__);
      fwrite(serial_str, sizeof(char), serial_len, stderr);
      fprintf(stderr, "\n");

      fprintf(stderr, "%s: id components:", __func__);
      for (ccsemver_id_t * iter = id; iter; iter = iter->next) {
	fprintf(stderr, " '");
	fwrite(iter->raw, sizeof(char), iter->len, stderr);
	fprintf(stderr, "' (numeric?=%d)", iter->numeric);
      }
      fprintf(stderr, "\n");
    }

    if (0) {
      fprintf(stderr, "%s: serial_len=%lu, expected_len=%lu\n", __func__, serial_len, strlen(expected_id));
    }
    cctests_assert(L, serial_len == strlen(expected_id));
    cctests_assert_ascii(L, expected_id, serial_str, strlen(expected_id));

    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_1_1_1 (cce_destination_t upper_L)
/* Successfully parsing a numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "123", "123");
}

void
test_1_1_2 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha", "alpha");
}

void
test_1_1_3 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha.123", "alpha.123");
}

void
test_1_1_4 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha.123-beta.456-gamma.789", "alpha.123-beta.456-gamma.789");
}

void
test_1_1_5 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha-123.beta-456.gamma-789", "alpha-123.beta-456.gamma-789");
}

void
test_1_1_6 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha.123.beta.456.gamma.789", "alpha.123.beta.456.gamma.789");
}

void
test_1_1_7 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic and numeric identifier delimited by plus sign. */
{
  test_identifier_successful_parsing_new(upper_L, "alpha.123+", "alpha.123");
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_id_new(L, &input);
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_id_new(L, &input);
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len + 3;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_id_new(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_1_3_1 (cce_destination_t upper_L)
/* Exception: expected numeric component in positive "long" range, found out of range. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_number_out_of_range(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "a.99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);

    ccsemver_id_new(L, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Identifier parsers: init operation.
 ** ----------------------------------------------------------------- */

static void
test_identifier_successful_parsing_init (cce_destination_t upper_L, char const * input_str, char const * expected_id)
{
  cce_location_t	L[1];
  cce_clean_handler_t	id_H[1];

  if (cce_location(L)) {
    if (1) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t	id[1];
    size_t		serial_len	= 1024;
    char		serial_str[serial_len];

    ccsemver_id_init_guarded(L, id_H, id, &input);

    serial_len = ccsemver_id_write(id, serial_str, serial_len);
    if (0) {
      fprintf(stderr, "%s: id=", __func__);
      fwrite(serial_str, sizeof(char), serial_len, stderr);
      fprintf(stderr, "\n");

      fprintf(stderr, "%s: id components:", __func__);
      for (ccsemver_id_t * iter = id; iter; iter = iter->next) {
	fprintf(stderr, " '");
	fwrite(iter->raw, sizeof(char), iter->len, stderr);
	fprintf(stderr, "' (numeric?=%d)", iter->numeric);
      }
      fprintf(stderr, "\n");
    }

    if (0) {
      fprintf(stderr, "%s: serial_len=%lu, expected_len=%lu\n", __func__, serial_len, strlen(expected_id));
    }
    cctests_assert(L, serial_len == strlen(expected_id));
    cctests_assert_ascii(L, expected_id, serial_str, strlen(expected_id));

    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_1_1 (cce_destination_t upper_L)
/* Successfully parsing a numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "123", "123");
}

void
test_2_1_2 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha", "alpha");
}

void
test_2_1_3 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha.123", "alpha.123");
}

void
test_2_1_4 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha.123-beta.456-gamma.789", "alpha.123-beta.456-gamma.789");
}

void
test_2_1_5 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha-123.beta-456.gamma-789", "alpha-123.beta-456.gamma-789");
}

void
test_2_1_6 (cce_destination_t upper_L)
/* Successfully parsing a long alphabetic and numeric identifier delimited by end-of-input. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha.123.beta.456.gamma.789", "alpha.123.beta.456.gamma.789");
}

void
test_2_1_7 (cce_destination_t upper_L)
/* Successfully parsing an alphabetic and numeric identifier delimited by plus sign. */
{
  test_identifier_successful_parsing_init(upper_L, "alpha.123+", "alpha.123");
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t	id[1];

    ccsemver_id_init(L, id, &input);
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t	id[1];

    ccsemver_id_init(L, id, &input);
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
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "ciao";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= input_len + 3;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t	id[1];

    ccsemver_id_init(L, id, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}

/* ------------------------------------------------------------------ */

void
test_2_3_1 (cce_destination_t upper_L)
/* Exception: expected numeric component in positive "long" range, found out of range. */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    if (0) { fprintf(stderr, "%s: %s\n", __func__, cce_condition_static_message(cce_condition(L))); }
    if (ccsemver_condition_is_parser_number_out_of_range(cce_condition(L))) {
      cce_run_clean_handlers_final(L);
    } else {
      cce_run_catch_handlers_raise(L, upper_L);
    }
  } else {
    char const *	input_str	= "a.99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    size_t		input_len	= strlen(input_str);
    size_t		input_off	= 0;
    ccsemver_input_t	input		= ccsemver_input_new(input_str, input_len, input_off);
    ccsemver_id_t	id[1];

    ccsemver_id_init(L, id, &input);
    cce_raise(L, cctests_condition_new_failure());
    cce_run_clean_handlers(L);
  }
}


int
main (void)
{
  ccsemver_library_init();

  cctests_init("identifiers");
  {
    cctests_begin_group("identifier parsers, new operation");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_1_2);
      cctests_run(test_1_1_3);
      cctests_run(test_1_1_4);
      cctests_run(test_1_1_5);
      cctests_run(test_1_1_6);
      cctests_run(test_1_1_7);

      cctests_run(test_1_2_1);
      cctests_run(test_1_2_2);
      cctests_run(test_1_2_3);

      cctests_run(test_1_3_1);
    }
    cctests_end_group();

    cctests_begin_group("identifier parsers, init operation");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_1_4);
      cctests_run(test_2_1_5);
      cctests_run(test_2_1_6);
      cctests_run(test_2_1_7);

      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);

      cctests_run(test_2_3_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
