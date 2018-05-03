/*
  Part of: CCSemver
  Contents: CCExceptions condition objects definitions
  Date: Apr  1, 2018

  Abstract

	This  file defines  the CCExceptions  condition objects  used by
	CCSemver.  It also holds the library initialisation function.

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

#include "ccsemver-internals.h"


/** --------------------------------------------------------------------
 ** Condition objects: parser error.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_error;

static ccsemver_descriptor_parser_error_t ccsemver_descriptor_parser_error_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_error
};

ccsemver_descriptor_parser_error_t const * const ccsemver_descriptor_parser_error_ptr = &ccsemver_descriptor_parser_error_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_error_t const ccsemver_condition_parser_error_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_error_stru.descriptor)
};

ccsemver_condition_parser_error_t const * const ccsemver_condition_parser_error_ptr = &ccsemver_condition_parser_error_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_error (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "error parsing input string";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_error (ccsemver_condition_parser_error_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_error (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_error_ptr;
}

bool
ccsemver_condition_is_parser_error (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_error_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, no input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_no_input;

static ccsemver_descriptor_parser_no_input_t ccsemver_descriptor_parser_no_input_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_error_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_no_input
};

ccsemver_descriptor_parser_no_input_t const * const ccsemver_descriptor_parser_no_input_ptr =
  &ccsemver_descriptor_parser_no_input_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_no_input_t const ccsemver_condition_parser_no_input_stru = {
  .parser_error.runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_no_input_stru.descriptor)
};

ccsemver_condition_parser_no_input_t const * const ccsemver_condition_parser_no_input_ptr =
  &ccsemver_condition_parser_no_input_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_no_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: no input available";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_no_input (ccsemver_condition_parser_no_input_t * C)
{
  ccsemver_condition_init_parser_error(&(C->parser_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_no_input (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_no_input_ptr;
}

bool
ccsemver_condition_is_parser_no_input (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_no_input_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, input is empty.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_empty_input;

static ccsemver_descriptor_parser_empty_input_t ccsemver_descriptor_parser_empty_input_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_no_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_empty_input
};

ccsemver_descriptor_parser_empty_input_t const * const ccsemver_descriptor_parser_empty_input_ptr =
  &ccsemver_descriptor_parser_empty_input_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_empty_input_t const ccsemver_condition_parser_empty_input_stru = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_empty_input_stru.descriptor)
};

ccsemver_condition_parser_empty_input_t const * const ccsemver_condition_parser_empty_input_ptr =
  &ccsemver_condition_parser_empty_input_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_empty_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: input is empty";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_empty_input (ccsemver_condition_parser_empty_input_t * C)
{
  ccsemver_condition_init_parser_no_input(&(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_empty_input (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_empty_input_ptr;
}

bool
ccsemver_condition_is_parser_empty_input (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_empty_input_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, end of input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_end_of_input;

static ccsemver_descriptor_parser_end_of_input_t ccsemver_descriptor_parser_end_of_input_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_no_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_end_of_input
};

ccsemver_descriptor_parser_end_of_input_t const * const ccsemver_descriptor_parser_end_of_input_ptr =
  &ccsemver_descriptor_parser_end_of_input_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_end_of_input_t const ccsemver_condition_parser_end_of_input_stru = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_end_of_input_stru.descriptor)
};

ccsemver_condition_parser_end_of_input_t const * const ccsemver_condition_parser_end_of_input_ptr =
  &ccsemver_condition_parser_end_of_input_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_end_of_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: unexpected end of input";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_end_of_input (ccsemver_condition_parser_end_of_input_t * C)
{
  ccsemver_condition_init_parser_no_input(&(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_end_of_input (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_end_of_input_ptr;
}

bool
ccsemver_condition_is_parser_end_of_input (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_end_of_input_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, invalid input offset.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_invalid_input_offset;

static ccsemver_descriptor_parser_invalid_input_offset_t ccsemver_descriptor_parser_invalid_input_offset_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_no_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_invalid_input_offset
};

ccsemver_descriptor_parser_invalid_input_offset_t const * const ccsemver_descriptor_parser_invalid_input_offset_ptr =
  &ccsemver_descriptor_parser_invalid_input_offset_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_invalid_input_offset_t const ccsemver_condition_parser_invalid_input_offset_stru = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor =
    &(ccsemver_descriptor_parser_invalid_input_offset_stru.descriptor)
};

ccsemver_condition_parser_invalid_input_offset_t const * const ccsemver_condition_parser_invalid_input_offset_ptr =
  &ccsemver_condition_parser_invalid_input_offset_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_invalid_input_offset (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: invalid offset in input string";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_invalid_input_offset (ccsemver_condition_parser_invalid_input_offset_t * C)
{
  ccsemver_condition_init_parser_no_input(&(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_invalid_input_offset (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_invalid_input_offset_ptr;
}

bool
ccsemver_condition_is_parser_invalid_input_offset (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_invalid_input_offset_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, invalid input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_invalid_input;

static ccsemver_descriptor_parser_invalid_input_t ccsemver_descriptor_parser_invalid_input_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_error_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_invalid_input
};

ccsemver_descriptor_parser_invalid_input_t const * const ccsemver_descriptor_parser_invalid_input_ptr =
  &ccsemver_descriptor_parser_invalid_input_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_invalid_input_t const ccsemver_condition_parser_invalid_input_stru = {
  .parser_error.runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_invalid_input_stru.descriptor)
};

ccsemver_condition_parser_invalid_input_t const * const ccsemver_condition_parser_invalid_input_ptr =
  &ccsemver_condition_parser_invalid_input_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_invalid_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: invalid input";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_invalid_input (ccsemver_condition_parser_invalid_input_t * C)
{
  ccsemver_condition_init_parser_error(&(C->parser_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_invalid_input (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_invalid_input_ptr;
}

bool
ccsemver_condition_is_parser_invalid_input (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_invalid_input_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_number;

static ccsemver_descriptor_parser_expected_number_t ccsemver_descriptor_parser_expected_number_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_invalid_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_number
};

ccsemver_descriptor_parser_expected_number_t const * const ccsemver_descriptor_parser_expected_number_ptr =
  &ccsemver_descriptor_parser_expected_number_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_expected_number_t const ccsemver_condition_parser_expected_number_stru = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  &(ccsemver_descriptor_parser_expected_number_stru.descriptor)
};

ccsemver_condition_parser_expected_number_t const * const ccsemver_condition_parser_expected_number_ptr =
  &ccsemver_condition_parser_expected_number_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_number (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected number";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_number (ccsemver_condition_parser_expected_number_t * C)
{
  ccsemver_condition_init_parser_invalid_input(&(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_number (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_expected_number_ptr;
}

bool
ccsemver_condition_is_parser_expected_number (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_expected_number_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected numeric component.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_numeric_component;

static ccsemver_descriptor_parser_expected_numeric_component_t ccsemver_descriptor_parser_expected_numeric_component_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_invalid_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_numeric_component
};

ccsemver_descriptor_parser_expected_numeric_component_t const * const ccsemver_descriptor_parser_expected_numeric_component_ptr =
  &ccsemver_descriptor_parser_expected_numeric_component_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_expected_numeric_component_t const ccsemver_condition_parser_expected_numeric_component_stru = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  &(ccsemver_descriptor_parser_expected_numeric_component_stru.descriptor)
};

ccsemver_condition_parser_expected_numeric_component_t const * const ccsemver_condition_parser_expected_numeric_component_ptr =
  &ccsemver_condition_parser_expected_numeric_component_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_numeric_component (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected numeric component";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_numeric_component (ccsemver_condition_parser_expected_numeric_component_t * C)
{
  ccsemver_condition_init_parser_invalid_input(&(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_numeric_component (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_expected_numeric_component_ptr;
}

bool
ccsemver_condition_is_parser_expected_numeric_component (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_expected_numeric_component_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected X-Range numeric component.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_xrange_numeric_component;

static ccsemver_descriptor_parser_expected_xrange_numeric_component_t ccsemver_descriptor_parser_expected_xrange_numeric_component_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_invalid_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_xrange_numeric_component
};

ccsemver_descriptor_parser_expected_xrange_numeric_component_t const * const ccsemver_descriptor_parser_expected_xrange_numeric_component_ptr =
  &ccsemver_descriptor_parser_expected_xrange_numeric_component_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_expected_xrange_numeric_component_t const ccsemver_condition_parser_expected_xrange_numeric_component_stru = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  &(ccsemver_descriptor_parser_expected_xrange_numeric_component_stru.descriptor)
};

ccsemver_condition_parser_expected_xrange_numeric_component_t const * const ccsemver_condition_parser_expected_xrange_numeric_component_ptr =
  &ccsemver_condition_parser_expected_xrange_numeric_component_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_xrange_numeric_component (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected X-Range numeric component";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_xrange_numeric_component (ccsemver_condition_parser_expected_xrange_numeric_component_t * C)
{
  ccsemver_condition_init_parser_invalid_input(&(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_xrange_numeric_component (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_expected_xrange_numeric_component_ptr;
}

bool
ccsemver_condition_is_parser_expected_xrange_numeric_component (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_expected_xrange_numeric_component_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected identifier.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_identifier;

static ccsemver_descriptor_parser_expected_identifier_t ccsemver_descriptor_parser_expected_identifier_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_invalid_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_identifier
};

ccsemver_descriptor_parser_expected_identifier_t const * const ccsemver_descriptor_parser_expected_identifier_ptr =
  &ccsemver_descriptor_parser_expected_identifier_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_expected_identifier_t const ccsemver_condition_parser_expected_identifier_stru = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  &(ccsemver_descriptor_parser_expected_identifier_stru.descriptor)
};

ccsemver_condition_parser_expected_identifier_t const * const ccsemver_condition_parser_expected_identifier_ptr =
  &ccsemver_condition_parser_expected_identifier_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_identifier (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected identifier";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_identifier (ccsemver_condition_parser_expected_identifier_t * C)
{
  ccsemver_condition_init_parser_invalid_input(&(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_identifier (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_expected_identifier_ptr;
}

bool
ccsemver_condition_is_parser_expected_identifier (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_expected_identifier_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, number out of range.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_number_out_of_range;

static ccsemver_descriptor_parser_number_out_of_range_t ccsemver_descriptor_parser_number_out_of_range_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= &(ccsemver_descriptor_parser_invalid_input_stru.descriptor),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_number_out_of_range
};

ccsemver_descriptor_parser_number_out_of_range_t const * const ccsemver_descriptor_parser_number_out_of_range_ptr =
  &ccsemver_descriptor_parser_number_out_of_range_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_number_out_of_range_t const ccsemver_condition_parser_number_out_of_range_stru = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  &(ccsemver_descriptor_parser_number_out_of_range_stru.descriptor)
};

ccsemver_condition_parser_number_out_of_range_t const * const ccsemver_condition_parser_number_out_of_range_ptr =
  &ccsemver_condition_parser_number_out_of_range_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_number_out_of_range (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: number out of range";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_number_out_of_range (ccsemver_condition_parser_number_out_of_range_t * C)
{
  ccsemver_condition_init_parser_invalid_input(&(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_number_out_of_range (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_number_out_of_range_ptr;
}

bool
ccsemver_condition_is_parser_number_out_of_range (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_number_out_of_range_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Module initialisation.
 ** ----------------------------------------------------------------- */

void
ccsemver_library_init (void)
{
  static bool	to_be_initialised = true;

  if (to_be_initialised) {
    to_be_initialised = false;
    ccsemver_descriptor_parser_error_stru.descriptor.parent	= &(cce_descriptor_runtime_error_ptr->descriptor);
  }
}

/* end of file */
