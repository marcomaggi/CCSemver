/*
  Part of: CCSemver
  Contents: public header file
  Date: Mar 27, 2018

  Abstract

	This public header file must be included in all the source files that use the
	CCSemver facilities.

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

#ifndef CCSEMVER_H
#define CCSEMVER_H 1

#ifdef __cplusplus
extern "C" {
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <ccmemory.h>
#include <ccstructs.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

cclib_decl void ccsemver_library_init (void)
  CCLIB_FUNC_ATTRIBUTE_CONSTRUCTOR;


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

cclib_decl char const *	ccsemver_version_string			(void);
cclib_decl int		ccsemver_version_interface_current	(void);
cclib_decl int		ccsemver_version_interface_revision	(void);
cclib_decl int		ccsemver_version_interface_age		(void);


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

cclib_decl ccsemver_allocator_t const * const ccsemver_default_allocator;
cclib_decl ccsemver_allocator_t const *       ccsemver_current_allocator;

cclib_decl void * ccsemver_malloc (cce_destination_t L, size_t size)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl void   ccsemver_free   (void * ptr)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);


/** --------------------------------------------------------------------
 ** Subordinate header files.
 ** ----------------------------------------------------------------- */

#include <ccsemver-conditions.h>


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

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
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

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline char
ccsemver_input_next (ccsemver_input_t * input)
{
  return input->str[input->off];
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccsemver_input_step (ccsemver_input_t * input)
{
  ++(input->off);
}

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccsemver_input_more (ccsemver_input_t * input)
{
  return (input->len > input->off);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccsemver_input_at_end (ccsemver_input_t * input)
{
  return (input->len == input->off);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccsemver_input_invalid_offset (ccsemver_input_t * input)
{
  return (input->len < input->off);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
  CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccsemver_input_is_empty (ccsemver_input_t * input)
{
  return (0 == input->len);
}

/* ------------------------------------------------------------------ */

cclib_decl bool ccsemver_looking_at_blanked_dash (ccsemver_input_t const * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl char ccsemver_input_parse_next	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccsemver_input_step_back	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl bool ccsemver_input_parse_blanks	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_v	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_dot	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_dash	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_plus	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_bar	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_equal	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_blanked_dash	(ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_blanked_OR	(ccsemver_input_t * that_input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl bool ccsemver_input_parse_comparator_separator (ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_input_assert_more_input (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);


/** --------------------------------------------------------------------
 ** Number parsers.
 ** ----------------------------------------------------------------- */

#define CCSEMVER_NUM_X	((long)(-1))

cclib_decl long ccsemver_parse_numeric_component (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_LEAF;

cclib_decl long ccsemver_parse_number (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_LEAF;

cclib_decl int ccsemver_num_comp (long num1, long num2)
  CCLIB_FUNC_ATTRIBUTE_LEAF
  CCLIB_FUNC_ATTRIBUTE_CONST;


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

cclib_decl ccsemver_id_t * ccsemver_id_new  (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_id_t * ccsemver_id_init (cce_destination_t L, ccsemver_id_t * id, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_id_delete (ccsemver_id_t * id)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccsemver_id_reset (ccsemver_id_t * id)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl int  ccsemver_id_write (ccsemver_id_t const * id, char * buffer, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl int  ccsemver_id_comp (ccsemver_id_t const * id1, ccsemver_id_t const * id2)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_LEAF;

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_clean_handler_id_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_id_t * id)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl void ccsemver_error_handler_id_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_id_t * id)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

#define ccsemver_handler_id_init(L,id_H,id)				\
  _Generic((id_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_id_init,	\
	   cce_error_handler_t	*: ccsemver_error_handler_id_init)(L,id_H,id)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_id_t * ccsemver_id_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_id_t * ccsemver_id_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_id_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_id_new_guarded_clean,	\
	   cce_error_handler_t	*: ccsemver_id_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_id_t * ccsemver_id_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
							      ccsemver_id_t * id, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_id_t * ccsemver_id_init_guarded_error (cce_destination_t L, cce_error_handler_t * H,
							      ccsemver_id_t * id, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_id_init_guarded(L,H,id,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_id_init_guarded_clean,	\
	   cce_error_handler_t	*: ccsemver_id_init_guarded_error)(L,H,id,input)



#include <ccsemver-semver.h>


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

cclib_decl ccsemver_cmp_t * ccsemver_cmp_new  (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl ccsemver_cmp_t * ccsemver_cmp_init (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_cmp_delete (ccsemver_cmp_t * cmp)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccsemver_cmp_reset (ccsemver_cmp_t * cmp)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_cmp_and   (cce_destination_t L, ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl int  ccsemver_cmp_write (ccsemver_cmp_t const * cmp, char * buffer, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl bool ccsemver_match (ccsemver_sv_t const * sv, ccsemver_cmp_t const * cmp)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_clean_handler_cmp_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_cmp_t * comp)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl void ccsemver_error_handler_cmp_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_cmp_t * comp)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

#define ccsemver_handler_cmp_init(L,comp_H,comp) \
  _Generic((comp_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_cmp_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_cmp_init)(L,&(comp_H->handler),comp)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_cmp_t * ccsemver_cmp_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_cmp_t * ccsemver_cmp_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_cmp_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_cmp_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_cmp_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_cmp_t * ccsemver_cmp_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								    ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_cmp_t * ccsemver_cmp_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								    ccsemver_cmp_t * cmp, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

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

cclib_decl ccsemver_range_t * ccsemver_range_new  (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl ccsemver_range_t * ccsemver_range_init (cce_destination_t L, ccsemver_range_t * range, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_range_delete (ccsemver_range_t * range)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccsemver_range_reset (ccsemver_range_t * range)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

/* ------------------------------------------------------------------ */

cclib_decl int ccsemver_range_write (ccsemver_range_t const * self, char * buffer, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl int ccsemver_range_match (ccsemver_sv_t const * sv, ccsemver_range_t const * range)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_clean_handler_range_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_range_t * range)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl void ccsemver_error_handler_range_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_range_t * range)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

#define ccsemver_handler_range_init(L,range_H,range)			\
  _Generic((range_H),							\
	   cce_clean_handler_t	*: ccsemver_clean_handler_range_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_range_init)(L,range_H,range)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_range_t * ccsemver_range_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_range_t * ccsemver_range_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_range_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_range_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_range_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_range_t * ccsemver_range_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								      ccsemver_range_t * range, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_range_t * ccsemver_range_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								      ccsemver_range_t * range, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_range_init_guarded(L,H,range,input)			\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_range_init_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_range_init_guarded_error)(L,H,range,input)


/** --------------------------------------------------------------------
 ** Input/output.
 ** ----------------------------------------------------------------- */

cclib_decl size_t ccsemver_id_fwrite (cce_destination_t L, ccsemver_id_t const * idp, FILE * stream)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl size_t ccsemver_sv_fwrite (cce_destination_t L, ccsemver_sv_t const * sv, FILE * stream)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl size_t ccsemver_cmp_fwrite  (cce_destination_t L, ccsemver_cmp_t const * cmp, FILE * stream)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl size_t ccsemver_range_fwrite (cce_destination_t L, ccsemver_range_t const * range, FILE * stream)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl char const * ccsemver_op_string (ccsemver_op_t op)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSEMVER_H */

/* end of file */
