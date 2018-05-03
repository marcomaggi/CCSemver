/*
  Part of: CCSemver
  Contents: public header file
  Date: Mar 27, 2018

  Abstract

	This public header file must be included in all the source files
	that use the CCSemver facilities.

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

#ifndef CCSEMVER_H
#define CCSEMVER_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  CCSEMVER_UNUSED  indicates   that  a  function,  function
   argument or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCSEMVER_UNUSED;
   int foo (char unused_argument CCSEMVER_UNUSED);
   int unused_variable CCSEMVER_UNUSED;
*/
#ifdef __GNUC__
#  define CCSEMVER_UNUSED		__attribute__((__unused__))
#else
#  define CCSEMVER_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccsemver_decl		__attribute__((__dllexport__)) extern
#    else
#      define ccsemver_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccsemver_decl		__attribute__((__dllimport__)) extern
#    else
#      define ccsemver_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define ccsemver_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccsemver_decl		__attribute__((__visibility__("default"))) extern
#    define ccsemver_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define ccsemver_decl		extern
#    define ccsemver_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

ccsemver_decl void ccsemver_library_init (void)
  __attribute__((__constructor__));


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccsemver_decl char const *	ccsemver_version_string			(void);
ccsemver_decl int		ccsemver_version_interface_current	(void);
ccsemver_decl int		ccsemver_version_interface_revision	(void);
ccsemver_decl int		ccsemver_version_interface_age		(void);


/** --------------------------------------------------------------------
 ** Condition objects: parser error.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_error_t	ccsemver_descriptor_parser_error_t;
typedef struct ccsemver_condition_parser_error_t	ccsemver_condition_parser_error_t;

struct ccsemver_descriptor_parser_error_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_error_t {
  cce_condition_runtime_error_t		runtime_error;
};

ccsemver_decl void ccsemver_condition_init_parser_error (ccsemver_condition_parser_error_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_error (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_error (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, no input.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_no_input_t	ccsemver_descriptor_parser_no_input_t;
typedef struct ccsemver_condition_parser_no_input_t	ccsemver_condition_parser_no_input_t;

struct ccsemver_descriptor_parser_no_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_no_input_t {
  ccsemver_condition_parser_error_t	parser_error;
};

ccsemver_decl void ccsemver_condition_init_parser_no_input (ccsemver_condition_parser_no_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_no_input (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_no_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, input is empty.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_empty_input_t	ccsemver_descriptor_parser_empty_input_t;
typedef struct ccsemver_condition_parser_empty_input_t	ccsemver_condition_parser_empty_input_t;

struct ccsemver_descriptor_parser_empty_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_empty_input_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void ccsemver_condition_init_parser_empty_input (ccsemver_condition_parser_empty_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_empty_input (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_empty_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, unexpected end-of-input.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_end_of_input_t	ccsemver_descriptor_parser_end_of_input_t;
typedef struct ccsemver_condition_parser_end_of_input_t		ccsemver_condition_parser_end_of_input_t;

struct ccsemver_descriptor_parser_end_of_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_end_of_input_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void ccsemver_condition_init_parser_end_of_input (ccsemver_condition_parser_end_of_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_end_of_input (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_end_of_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, invalid input offset.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_invalid_input_offset_t	ccsemver_descriptor_parser_invalid_input_offset_t;
typedef struct ccsemver_condition_parser_invalid_input_offset_t		ccsemver_condition_parser_invalid_input_offset_t;

struct ccsemver_descriptor_parser_invalid_input_offset_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_invalid_input_offset_t {
  ccsemver_condition_parser_no_input_t	parser_no_input;
};

ccsemver_decl void ccsemver_condition_init_parser_invalid_input_offset (ccsemver_condition_parser_invalid_input_offset_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_invalid_input_offset (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_invalid_input_offset (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_invalid_input_t	ccsemver_descriptor_parser_invalid_input_t;
typedef struct ccsemver_condition_parser_invalid_input_t	ccsemver_condition_parser_invalid_input_t;

struct ccsemver_descriptor_parser_invalid_input_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_invalid_input_t {
  ccsemver_condition_parser_error_t	parser_error;
};

ccsemver_decl void ccsemver_condition_init_parser_invalid_input (ccsemver_condition_parser_invalid_input_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_invalid_input (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_invalid_input (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected number.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_number_t	ccsemver_descriptor_parser_expected_number_t;
typedef struct ccsemver_condition_parser_expected_number_t	ccsemver_condition_parser_expected_number_t;

struct ccsemver_descriptor_parser_expected_number_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_number_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void ccsemver_condition_init_parser_expected_number (ccsemver_condition_parser_expected_number_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_number (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_number (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected numeric component.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_numeric_component_t	ccsemver_descriptor_parser_expected_numeric_component_t;
typedef struct ccsemver_condition_parser_expected_numeric_component_t	ccsemver_condition_parser_expected_numeric_component_t;

struct ccsemver_descriptor_parser_expected_numeric_component_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_numeric_component_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void ccsemver_condition_init_parser_expected_numeric_component (ccsemver_condition_parser_expected_numeric_component_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_numeric_component (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_numeric_component (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected X-Range numeric component.
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

ccsemver_decl void ccsemver_condition_init_parser_expected_xrange_numeric_component
  (ccsemver_condition_parser_expected_xrange_numeric_component_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_xrange_numeric_component (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_xrange_numeric_component (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, expected identifier.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_expected_identifier_t	ccsemver_descriptor_parser_expected_identifier_t;
typedef struct ccsemver_condition_parser_expected_identifier_t	ccsemver_condition_parser_expected_identifier_t;

struct ccsemver_descriptor_parser_expected_identifier_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_expected_identifier_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void ccsemver_condition_init_parser_expected_identifier (ccsemver_condition_parser_expected_identifier_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_expected_identifier (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_expected_identifier (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Condition objects: parser error, number out of range.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_descriptor_parser_number_out_of_range_t		ccsemver_descriptor_parser_number_out_of_range_t;
typedef struct ccsemver_condition_parser_number_out_of_range_t		ccsemver_condition_parser_number_out_of_range_t;

struct ccsemver_descriptor_parser_number_out_of_range_t {
  cce_descriptor_t			descriptor;
};

struct ccsemver_condition_parser_number_out_of_range_t {
  ccsemver_condition_parser_invalid_input_t	parser_invalid_input;
};

ccsemver_decl void ccsemver_condition_init_parser_number_out_of_range (ccsemver_condition_parser_number_out_of_range_t * C)
  __attribute__((__nonnull__(1)));

ccsemver_decl cce_condition_t const * ccsemver_condition_new_parser_number_out_of_range (void)
  __attribute__((__returns_nonnull__));

ccsemver_decl bool ccsemver_condition_is_parser_number_out_of_range (cce_condition_t const * C)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Forward type definitions.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_input_t		ccsemver_input_t;
typedef struct ccsemver_sv_t		ccsemver_sv_t;
typedef struct ccsemver_id_t		ccsemver_id_t;
typedef struct ccsemver_cmp_t		ccsemver_cmp_t;
typedef struct ccsemver_range_t		ccsemver_range_t;

enum ccsemver_op_t {
  CCSEMVER_OP_EQ = 0,
  CCSEMVER_OP_LT,
  CCSEMVER_OP_LE,
  CCSEMVER_OP_GT,
  CCSEMVER_OP_GE,
};

typedef enum ccsemver_op_t		ccsemver_op_t;


/** --------------------------------------------------------------------
 ** Memory allocation.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_allocator_t	ccsemver_allocator_t;

typedef void *	ccsemver_malloc_fun_t (ccsemver_allocator_t const * A, cce_destination_t L, size_t size);
typedef void	ccsemver_free_fun_t   (ccsemver_allocator_t const * A, void * ptr);

struct ccsemver_allocator_t {
  ccsemver_malloc_fun_t *	malloc;
  ccsemver_free_fun_t *		free;
};

ccsemver_decl ccsemver_allocator_t const * const ccsemver_default_allocator;
ccsemver_decl ccsemver_allocator_t const *       ccsemver_current_allocator;

ccsemver_decl void * ccsemver_malloc (cce_destination_t L, size_t size)
  __attribute__((__nonnull__(1),__returns_nonnull__));

ccsemver_decl void   ccsemver_free   (void * ptr)
  __attribute__((__nonnull__(1)));


/** --------------------------------------------------------------------
 ** Parser input.
 ** ----------------------------------------------------------------- */

struct ccsemver_input_t {
  /* Pointer to the first character in the input string. */
  char const * const	str;

  /* Number of character in the input string. */
  size_t const		len;

  /* Offset of the next character to parse in the input string. */
  size_t		off;
};

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccsemver_input_t
ccsemver_input_new (char const * const str, size_t const len, size_t const off)
{
  ccsemver_input_t	input = {
    .str	= str,
    .len	= len,
    .off	= off
  };
  return input;
}

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__nonnull__(1)))
static inline char
ccsemver_input_next (ccsemver_input_t * input)
{
  return input->str[input->off];
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccsemver_input_step (ccsemver_input_t * input)
{
  ++(input->off);
}

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__nonnull__(1)))
static inline bool
ccsemver_input_more (ccsemver_input_t * input)
{
  return (input->len > input->off);
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline bool
ccsemver_input_at_end (ccsemver_input_t * input)
{
  return (input->len == input->off);
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline bool
ccsemver_input_invalid_offset (ccsemver_input_t * input)
{
  return (input->len < input->off);
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline bool
ccsemver_input_is_empty (ccsemver_input_t * input)
{
  return (0 == input->len);
}

/* ------------------------------------------------------------------ */

ccsemver_decl bool ccsemver_looking_at_blanked_dash (ccsemver_input_t const * input)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl char ccsemver_input_parse_next	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl void ccsemver_input_step_back	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl bool ccsemver_input_parse_blanks	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_v	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_dot	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_dash	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_plus	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_bar	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_equal	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_blanked_dash	(ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_blanked_OR	(ccsemver_input_t * that_input)
  __attribute__((__nonnull__(1)));

ccsemver_decl bool ccsemver_input_parse_comparator_separator (ccsemver_input_t * input)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_input_assert_more_input (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2)));


/** --------------------------------------------------------------------
 ** Number parsers.
 ** ----------------------------------------------------------------- */

#define CCSEMVER_NUM_X	((long)(-1))

ccsemver_decl long ccsemver_parse_numeric_component (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2),__leaf__));

ccsemver_decl long ccsemver_parse_number (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2),__leaf__));

ccsemver_decl int ccsemver_num_comp (long num1, long num2)
  __attribute__((__leaf__,__const__));


/** --------------------------------------------------------------------
 ** Identifiers.
 ** ----------------------------------------------------------------- */

typedef void ccsemver_id_delete_fun_t (ccsemver_id_t * id);

struct ccsemver_id_t {
  ccsemver_id_delete_fun_t *	delete;

  /* A boolean  value: true if the  this component is numeric;  false if
     this component is alphabetic. */
  bool			numeric;

  /* If  this  component  is  numeric:  this  field  holds  its  numeric
     representation.  Otherwise this field holds a meaningless value. */
  long			num;

  /* The number of  characters in the input string that  where parsed to
     form this component. */
  size_t		len;

  /* Pointer to the character in the input string representing the first
     character of this component. */
  char const *		raw;

  /* Pointer to the next node in the linked list of identifiers; NULL if
     this structure is the last node. */
  ccsemver_id_t *	next;
};

ccsemver_decl ccsemver_id_t * ccsemver_id_new  (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2),__returns_nonnull__));

ccsemver_decl ccsemver_id_t * ccsemver_id_init (cce_destination_t L, ccsemver_id_t * id, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_id_delete (ccsemver_id_t * id)
  __attribute__((__nonnull__(1)));

ccsemver_decl void ccsemver_id_reset (ccsemver_id_t * id)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl int  ccsemver_id_write (ccsemver_id_t const * id, char * buffer, size_t len)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl int  ccsemver_id_comp (ccsemver_id_t const * id1, ccsemver_id_t const * id2)
  __attribute__((__nonnull__(1,2),__leaf__));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_clean_handler_id_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_id_t * id)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl void ccsemver_error_handler_id_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_id_t * id)
  __attribute__((__nonnull__(1,2,3)));

#define ccsemver_handler_id_init(L,id_H,id)				\
  _Generic((id_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_id_init,	\
	   cce_error_handler_t	*: ccsemver_error_handler_id_init)(L,id_H,id)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_id_t * ccsemver_id_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccsemver_decl ccsemver_id_t * ccsemver_id_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccsemver_id_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_id_new_guarded_clean,	\
	   cce_error_handler_t	*: ccsemver_id_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_id_t * ccsemver_id_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
							      ccsemver_id_t * id, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccsemver_decl ccsemver_id_t * ccsemver_id_init_guarded_error (cce_destination_t L, cce_error_handler_t * H,
							      ccsemver_id_t * id, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

#define ccsemver_id_init_guarded(L,H,id,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_id_init_guarded_clean,	\
	   cce_error_handler_t	*: ccsemver_id_init_guarded_error)(L,H,id,input)


/** --------------------------------------------------------------------
 ** Semantic versions.
 ** ----------------------------------------------------------------- */

typedef void ccsemver_sv_delete_fun_t (ccsemver_sv_t * sv);

struct ccsemver_sv_t {
  ccsemver_sv_delete_fun_t *	delete;

  /* The major version number. */
  long		major;

  /* The mino version number. */
  long		minor;

  /* The patch level. */
  long		patch;

  /* An identifier representing the prerelease tag. */
  ccsemver_id_t	prerelease;

  /* An identifier representing the build metadata. */
  ccsemver_id_t	build;

  /* The number of  characters in the input string that  where parsed to
     form this version. */
  size_t	len;

  /* Pointer to the character in the input string representing the first
     character of this component.  This pointer  is valid as long as the
     input string handed to the constructor function is not mutated. */
  char const *	raw;
};

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_sv_t * ccsemver_sv_new  (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2),__returns_nonnull__));

ccsemver_decl ccsemver_sv_t * ccsemver_sv_init (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_sv_t * ccsemver_sv_new_range  (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2),__returns_nonnull__));

ccsemver_decl ccsemver_sv_t * ccsemver_sv_init_range (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_sv_delete (ccsemver_sv_t * sv)
  __attribute__((__nonnull__(1)));

ccsemver_decl void ccsemver_sv_reset (ccsemver_sv_t * sv)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl int  ccsemver_sv_write (ccsemver_sv_t const * sv, char * buffer, size_t len)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl int  ccsemver_sv_comp  (ccsemver_sv_t const * sv1, ccsemver_sv_t const * sv2)
  __attribute__((__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_clean_handler_sv_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_sv_t * sv)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl void ccsemver_error_handler_sv_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_sv_t * sv)
  __attribute__((__nonnull__(1,2,3)));

#define ccsemver_handler_sv_init(L,H,sv)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_clean_handler_sv_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_sv_init)(L,H,sv)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_sv_t * ccsemver_sv_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccsemver_decl ccsemver_sv_t * ccsemver_sv_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccsemver_sv_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_sv_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_sv_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_sv_t * ccsemver_sv_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccsemver_decl ccsemver_sv_t * ccsemver_sv_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

#define ccsemver_sv_init_guarded(L,H,sv,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_sv_init_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_sv_init_guarded_error)(L,H,sv,input)


/** --------------------------------------------------------------------
 ** Comparators.
 ** ----------------------------------------------------------------- */

typedef void ccsemver_cmp_delete_fun_t (ccsemver_cmp_t * comp);

struct ccsemver_cmp_t {
  ccsemver_cmp_delete_fun_t *	delete;

  /* Pointer to the next node in the simply-linked list; NULL if this is
     the last node. */
  ccsemver_cmp_t *		next;

  /* The operator with  which the semantic version in this  node must be
     compared with other versions. */
  ccsemver_op_t			op;

  /* The semantic version of this node. */
  ccsemver_sv_t			sv;
};

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_new  (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_init (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_cmp_delete (ccsemver_cmp_t * cmp)
  __attribute__((__nonnull__(1)));

ccsemver_decl void ccsemver_cmp_reset (ccsemver_cmp_t * cmp)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_cmp_and   (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl int  ccsemver_cmp_write (ccsemver_cmp_t const * cmp, char * buffer, size_t len)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl bool ccsemver_match (ccsemver_sv_t const * sv, ccsemver_cmp_t const * cmp)
  __attribute__((__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_clean_handler_cmp_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_cmp_t * comp)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl void ccsemver_error_handler_cmp_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_cmp_t * comp)
  __attribute__((__nonnull__(1,2,3)));

#define ccsemver_handler_cmp_init(L,comp_H,comp) \
  _Generic((comp_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_cmp_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_cmp_init)(L,&(comp_H->handler),comp)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccsemver_cmp_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_cmp_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_cmp_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								    ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccsemver_decl ccsemver_cmp_t * ccsemver_cmp_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								    ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

#define ccsemver_cmp_init_guarded(L,H,cmp,input)			\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_cmp_init_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_cmp_init_guarded_error)(L,H,cmp,input)


/** --------------------------------------------------------------------
 ** Ranges.
 ** ----------------------------------------------------------------- */

typedef void ccsemver_range_delete_fun_t (ccsemver_range_t * range);

struct ccsemver_range_t {
  ccsemver_range_delete_fun_t *	delete;

  /* Pointer to the next node in the simply-linked list; NULL if this is
     the last node. */
  ccsemver_range_t *	next;

  /* The comparator of this node. */
  ccsemver_cmp_t	cmp;
};

ccsemver_decl ccsemver_range_t * ccsemver_range_new  (cce_destination_t L, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl ccsemver_range_t * ccsemver_range_init (cce_destination_t L, ccsemver_range_t * range, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_range_delete (ccsemver_range_t * range)
  __attribute__((__nonnull__(1)));

ccsemver_decl void ccsemver_range_reset (ccsemver_range_t * range)
  __attribute__((__nonnull__(1)));

/* ------------------------------------------------------------------ */

ccsemver_decl int ccsemver_range_write (ccsemver_range_t const * self, char * buffer, size_t len)
  __attribute__((__nonnull__(1,2)));

ccsemver_decl int ccsemver_range_match (ccsemver_sv_t const * sv, ccsemver_range_t const * range)
  __attribute__((__nonnull__(1,2)));

/* ------------------------------------------------------------------ */

ccsemver_decl void ccsemver_clean_handler_range_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_range_t * range)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl void ccsemver_error_handler_range_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_range_t * range)
  __attribute__((__nonnull__(1,2,3)));

#define ccsemver_handler_range_init(L,range_H,range)			\
  _Generic((range_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_range_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_range_init)(L,range_H,range)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_range_t * ccsemver_range_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccsemver_decl ccsemver_range_t * ccsemver_range_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccsemver_range_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_range_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_range_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

ccsemver_decl ccsemver_range_t * ccsemver_range_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								      ccsemver_range_t * range, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccsemver_decl ccsemver_range_t * ccsemver_range_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								      ccsemver_range_t * range, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

#define ccsemver_range_init_guarded(L,H,range,input)			\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_range_init_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_range_init_guarded_error)(L,H,range,input)


/** --------------------------------------------------------------------
 ** Input/output.
 ** ----------------------------------------------------------------- */

ccsemver_decl size_t ccsemver_id_fwrite (cce_destination_t L, ccsemver_id_t const * idp, FILE * stream)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl size_t ccsemver_sv_fwrite (cce_destination_t L, ccsemver_sv_t const * sv, FILE * stream)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl size_t ccsemver_cmp_fwrite  (cce_destination_t L, ccsemver_cmp_t const * cmp, FILE * stream)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl size_t ccsemver_range_fwrite (cce_destination_t L, ccsemver_range_t const * range, FILE * stream)
  __attribute__((__nonnull__(1,2,3)));

ccsemver_decl char const * ccsemver_op_string (ccsemver_op_t op)
  __attribute__((__returns_nonnull__));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSEMVER_H */

/* end of file */
