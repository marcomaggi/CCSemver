/*
  Part of: CCSemver
  Contents: CCExceptions condition objects definitions
  Date: Apr  1, 2018

  Abstract

	This file  defines the CCExceptions  condition objects used by  CCSemver.  It
	also holds the library initialisation function.

  This is free and unencumbered software released into the public domain.

  Anyone is  free to copy,  modify, publish, use,  compile, sell, or  distribute this
  software, either  in source  code form or  as a compiled  binary, for  any purpose,
  commercial or non-commercial, and by any means.

  In  jurisdictions that  recognize copyright  laws, the  author or  authors of  this
  software dedicate  any and  all copyright  interest in the  software to  the public
  domain.  We make this dedication for the benefit  of the public at large and to the
  detriment of our  heirs and successors.  We  intend this dedication to  be an overt
  act  of relinquishment  in perpetuity  of  all present  and future  rights to  this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING  BUT NOT  LIMITED TO  THE WARRANTIES  OF MERCHANTABILITY,  FITNESS FOR  A
  PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO  EVENT SHALL THE AUTHORS  BE LIABLE
  FOR ANY CLAIM, DAMAGES  OR OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT, TORT
  OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
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

static ccsemver_descriptor_parser_error_t ccsemver_descriptor_parser_error = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_error
};

static ccsemver_condition_parser_error_t const ccsemver_condition_parser_error = {
  .runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccsemver_descriptor_parser_error)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_error_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_error);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_error (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "error parsing input string";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_error (cce_destination_t L CCSEMVER_UNUSED, ccsemver_condition_parser_error_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_error (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_error;
}

bool
ccsemver_condition_is_parser_error (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_error));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, no input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_no_input;

static ccsemver_descriptor_parser_no_input_t ccsemver_descriptor_parser_no_input = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_error),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_no_input
};

static ccsemver_condition_parser_no_input_t const ccsemver_condition_parser_no_input = {
  .parser_error.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccsemver_descriptor_parser_no_input)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_no_input_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_no_input);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_no_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: no input available";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_no_input (cce_destination_t L, ccsemver_condition_parser_no_input_t * C)
{
  ccsemver_condition_init_parser_error(L, &(C->parser_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_no_input (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_no_input;
}

bool
ccsemver_condition_is_parser_no_input (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_no_input));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, input is empty.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_empty_input;

static ccsemver_descriptor_parser_empty_input_t ccsemver_descriptor_parser_empty_input = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_no_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_empty_input
};

static ccsemver_condition_parser_empty_input_t const ccsemver_condition_parser_empty_input = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccsemver_descriptor_parser_empty_input)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_empty_input_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_empty_input);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_empty_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: input is empty";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_empty_input (cce_destination_t L, ccsemver_condition_parser_empty_input_t * C)
{
  ccsemver_condition_init_parser_no_input(L, &(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_empty_input (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_empty_input;
}

bool
ccsemver_condition_is_parser_empty_input (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_empty_input));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, end of input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_end_of_input;

static ccsemver_descriptor_parser_end_of_input_t ccsemver_descriptor_parser_end_of_input = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_no_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_end_of_input
};

static ccsemver_condition_parser_end_of_input_t const ccsemver_condition_parser_end_of_input = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccsemver_descriptor_parser_end_of_input)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_end_of_input_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_end_of_input);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_end_of_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: unexpected end of input";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_end_of_input (cce_destination_t L, ccsemver_condition_parser_end_of_input_t * C)
{
  ccsemver_condition_init_parser_no_input(L, &(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_end_of_input (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_end_of_input;
}

bool
ccsemver_condition_is_parser_end_of_input (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_end_of_input));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, invalid input offset.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_invalid_input_offset;

static ccsemver_descriptor_parser_invalid_input_offset_t ccsemver_descriptor_parser_invalid_input_offset = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_no_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_invalid_input_offset
};

static ccsemver_condition_parser_invalid_input_offset_t const ccsemver_condition_parser_invalid_input_offset = {
  .parser_no_input.parser_error.runtime_error.error.root.condition.descriptor =
    cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input_offset)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_invalid_input_offset_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input_offset);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_invalid_input_offset (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: invalid offset in input string";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_invalid_input_offset (cce_destination_t L, ccsemver_condition_parser_invalid_input_offset_t * C)
{
  ccsemver_condition_init_parser_no_input(L, &(C->parser_no_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_invalid_input_offset (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_invalid_input_offset;
}

bool
ccsemver_condition_is_parser_invalid_input_offset (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input_offset));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, invalid input.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_invalid_input;

static ccsemver_descriptor_parser_invalid_input_t ccsemver_descriptor_parser_invalid_input = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_error),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_invalid_input
};

static ccsemver_condition_parser_invalid_input_t const ccsemver_condition_parser_invalid_input = {
  .parser_error.runtime_error.error.root.condition.descriptor = cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_invalid_input_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_invalid_input (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: invalid input";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_invalid_input (cce_destination_t L, ccsemver_condition_parser_invalid_input_t * C)
{
  ccsemver_condition_init_parser_error(L, &(C->parser_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_invalid_input (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_invalid_input;
}

bool
ccsemver_condition_is_parser_invalid_input (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_number;

static ccsemver_descriptor_parser_expected_number_t ccsemver_descriptor_parser_expected_number = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_number
};

static ccsemver_condition_parser_expected_number_t const ccsemver_condition_parser_expected_number = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  cce_descriptor_pointer(ccsemver_descriptor_parser_expected_number)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_number_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_expected_number);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_number (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected number";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_number (cce_destination_t L, ccsemver_condition_parser_expected_number_t * C)
{
  ccsemver_condition_init_parser_invalid_input(L, &(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_number (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_expected_number;
}

bool
ccsemver_condition_is_parser_expected_number (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_expected_number));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected numeric component.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_numeric_component;

static ccsemver_descriptor_parser_expected_numeric_component_t ccsemver_descriptor_parser_expected_numeric_component = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_numeric_component
};

static ccsemver_condition_parser_expected_numeric_component_t const ccsemver_condition_parser_expected_numeric_component = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  cce_descriptor_pointer(ccsemver_descriptor_parser_expected_numeric_component)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_numeric_component_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_expected_numeric_component);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_numeric_component (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected numeric component";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_numeric_component (cce_destination_t L, ccsemver_condition_parser_expected_numeric_component_t * C)
{
  ccsemver_condition_init_parser_invalid_input(L, &(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_numeric_component (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_expected_numeric_component;
}

bool
ccsemver_condition_is_parser_expected_numeric_component (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_expected_numeric_component));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected X-Range numeric component.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_xrange_numeric_component;

static ccsemver_descriptor_parser_expected_xrange_numeric_component_t ccsemver_descriptor_parser_expected_xrange_numeric_component = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_xrange_numeric_component
};

static ccsemver_condition_parser_expected_xrange_numeric_component_t const ccsemver_condition_parser_expected_xrange_numeric_component = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  cce_descriptor_pointer(ccsemver_descriptor_parser_expected_xrange_numeric_component)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_xrange_numeric_component_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_expected_xrange_numeric_component);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_xrange_numeric_component (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected X-Range numeric component";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_xrange_numeric_component (cce_destination_t L, ccsemver_condition_parser_expected_xrange_numeric_component_t * C)
{
  ccsemver_condition_init_parser_invalid_input(L, &(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_xrange_numeric_component (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_expected_xrange_numeric_component;
}

bool
ccsemver_condition_is_parser_expected_xrange_numeric_component (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_expected_xrange_numeric_component));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected identifier.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_expected_identifier;

static ccsemver_descriptor_parser_expected_identifier_t ccsemver_descriptor_parser_expected_identifier = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_expected_identifier
};

static ccsemver_condition_parser_expected_identifier_t const ccsemver_condition_parser_expected_identifier = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  cce_descriptor_pointer(ccsemver_descriptor_parser_expected_identifier)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_identifier_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_expected_identifier);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_expected_identifier (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: expected identifier";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_expected_identifier (cce_destination_t L, ccsemver_condition_parser_expected_identifier_t * C)
{
  ccsemver_condition_init_parser_invalid_input(L, &(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_expected_identifier (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_expected_identifier;
}

bool
ccsemver_condition_is_parser_expected_identifier (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_expected_identifier));
}


/** --------------------------------------------------------------------
 ** Condition objects: parser error, number out of range.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_number_out_of_range;

static ccsemver_descriptor_parser_number_out_of_range_t ccsemver_descriptor_parser_number_out_of_range = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= cce_descriptor_pointer(ccsemver_descriptor_parser_invalid_input),
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_number_out_of_range
};

static ccsemver_condition_parser_number_out_of_range_t const ccsemver_condition_parser_number_out_of_range = {
  .parser_invalid_input.parser_error.runtime_error.error.root.condition.descriptor =
  cce_descriptor_pointer(ccsemver_descriptor_parser_number_out_of_range)
};

void
cce_descriptor_set_parent_to(ccsemver_descriptor_parser_number_out_of_range_t) (cce_descriptor_t * const D)
{
  D->parent = cce_descriptor_pointer(ccsemver_descriptor_parser_number_out_of_range);
}

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_number_out_of_range (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "while parsing: number out of range";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_number_out_of_range (cce_destination_t L, ccsemver_condition_parser_number_out_of_range_t * C)
{
  ccsemver_condition_init_parser_invalid_input(L, &(C->parser_invalid_input));
}

cce_condition_t const *
ccsemver_condition_new_parser_number_out_of_range (cce_destination_t L CCSEMVER_UNUSED)
{
  return (cce_condition_t const *) &ccsemver_condition_parser_number_out_of_range;
}

bool
ccsemver_condition_is_parser_number_out_of_range (cce_condition_t const * C)
{
  return cce_condition_is(C, cce_descriptor_pointer(ccsemver_descriptor_parser_number_out_of_range));
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
    cce_descriptor_set_parent_to(cce_descriptor_runtime_error_t)(cce_descriptor_pointer(ccsemver_descriptor_parser_error));
  }
}

/* end of file */
