/*
  Part of: CCSemver
  Contents: subordinate public header file, exceptional-condition object-types
  Date: Apr 23, 2019

  Abstract

	This a public  header file subordinate to  the main one.  It  should never be
	included independently.

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

#ifndef CCSEMVER_CONDITIONS_H
#define CCSEMVER_CONDITIONS_H 1


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_error_t	ccsemver_descriptor_parser_error_t;
typedef struct ccsemver_condition_parser_error_t	ccsemver_condition_parser_error_t;

struct ccsemver_descriptor_parser_error_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_error_t {
  cce_condition_runtime_error_t		runtime_error;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_error_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_error (cce_destination_t L, ccsemver_condition_parser_error_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_error (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_error (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, no input.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_no_input_t	ccsemver_descriptor_parser_no_input_t;
typedef struct ccsemver_condition_parser_no_input_t	ccsemver_condition_parser_no_input_t;

struct ccsemver_descriptor_parser_no_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_no_input_t {
  ccsemver_condition_parser_error_t	parser_error;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_no_input_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_no_input (cce_destination_t L, ccsemver_condition_parser_no_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_no_input (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_no_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, input is empty.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_empty_input_t	ccsemver_descriptor_parser_empty_input_t;
typedef struct ccsemver_condition_parser_empty_input_t	ccsemver_condition_parser_empty_input_t;

struct ccsemver_descriptor_parser_empty_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_empty_input_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_empty_input_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_empty_input (cce_destination_t L, ccsemver_condition_parser_empty_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_empty_input (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_empty_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, unexpected end-of-input.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_end_of_input_t	ccsemver_descriptor_parser_end_of_input_t;
typedef struct ccsemver_condition_parser_end_of_input_t		ccsemver_condition_parser_end_of_input_t;

struct ccsemver_descriptor_parser_end_of_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_end_of_input_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_end_of_input_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_end_of_input (cce_destination_t L, ccsemver_condition_parser_end_of_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_end_of_input (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_end_of_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, invalid input offset.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_invalid_input_offset_t	ccsemver_descriptor_parser_invalid_input_offset_t;
typedef struct ccsemver_condition_parser_invalid_input_offset_t		ccsemver_condition_parser_invalid_input_offset_t;

struct ccsemver_descriptor_parser_invalid_input_offset_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_invalid_input_offset_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_invalid_input_offset_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_invalid_input_offset
  (cce_destination_t L, ccsemver_condition_parser_invalid_input_offset_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_invalid_input_offset (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_invalid_input_offset (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_invalid_input_t	ccsemver_descriptor_parser_invalid_input_t;
typedef struct ccsemver_condition_parser_invalid_input_t	ccsemver_condition_parser_invalid_input_t;

struct ccsemver_descriptor_parser_invalid_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_invalid_input_t {
  ccsemver_condition_parser_error_t	parser_error;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_invalid_input_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_invalid_input (cce_destination_t L, ccsemver_condition_parser_invalid_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_invalid_input (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_invalid_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_number_t	ccsemver_descriptor_parser_expected_number_t;
typedef struct ccsemver_condition_parser_expected_number_t	ccsemver_condition_parser_expected_number_t;

struct ccsemver_descriptor_parser_expected_number_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_number_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_number_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_expected_number (cce_destination_t L, ccsemver_condition_parser_expected_number_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_number (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_number (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, expected numeric component.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_numeric_component_t	ccsemver_descriptor_parser_expected_numeric_component_t;
typedef struct ccsemver_condition_parser_expected_numeric_component_t	ccsemver_condition_parser_expected_numeric_component_t;

struct ccsemver_descriptor_parser_expected_numeric_component_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_numeric_component_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_numeric_component_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_expected_numeric_component
  (cce_destination_t L, ccsemver_condition_parser_expected_numeric_component_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_numeric_component (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_numeric_component (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, expected X-Range numeric component.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_xrange_numeric_component_t
  ccsemver_descriptor_parser_expected_xrange_numeric_component_t;
typedef struct ccsemver_condition_parser_expected_xrange_numeric_component_t
  ccsemver_condition_parser_expected_xrange_numeric_component_t;

struct ccsemver_descriptor_parser_expected_xrange_numeric_component_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_xrange_numeric_component_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_xrange_numeric_component_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_expected_xrange_numeric_component
  (cce_destination_t L, ccsemver_condition_parser_expected_xrange_numeric_component_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_xrange_numeric_component (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_xrange_numeric_component (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, expected identifier.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_identifier_t	ccsemver_descriptor_parser_expected_identifier_t;
typedef struct ccsemver_condition_parser_expected_identifier_t	ccsemver_condition_parser_expected_identifier_t;

struct ccsemver_descriptor_parser_expected_identifier_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_identifier_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_expected_identifier_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_expected_identifier
  (cce_destination_t L, ccsemver_condition_parser_expected_identifier_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_identifier (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_identifier (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Exceptional-condition objects: parser error, number out of range.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_number_out_of_range_t		ccsemver_descriptor_parser_number_out_of_range_t;
typedef struct ccsemver_condition_parser_number_out_of_range_t		ccsemver_condition_parser_number_out_of_range_t;

struct ccsemver_descriptor_parser_number_out_of_range_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_number_out_of_range_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void cce_descriptor_set_parent_to(ccsemver_descriptor_parser_number_out_of_range_t) (cce_descriptor_t * D)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_condition_init_parser_number_out_of_range
  (cce_destination_t L, ccsemver_condition_parser_number_out_of_range_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_number_out_of_range (cce_destination_t L)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_number_out_of_range (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCSEMVER_CONDITIONS_H */

/* end of file */
