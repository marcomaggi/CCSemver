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

ccsemver_decl void ccsemver_init (void)
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
 ** Forward type definitions.
 ** ----------------------------------------------------------------- */

typedef struct ccsemver_t		ccsemver_t;
typedef struct ccsemver_id_t		ccsemver_id_t;
typedef struct ccsemver_comp_t		ccsemver_comp_t;
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
 ** Memory allocation
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
 ** Number parsers.
 ** ----------------------------------------------------------------- */

#define CCSEMVER_NUM_X	((long)(-1))

ccsemver_decl char ccsemver_num_parse (long * nump, char const * input_str, size_t input_len, size_t * input_offp)
  __attribute__((nonnull(1,2,4),leaf));

ccsemver_decl char ccsemver_num_comp (long self, long other)
  __attribute__((leaf,const));


/** --------------------------------------------------------------------
 ** Identifiers.
 ** ----------------------------------------------------------------- */

struct ccsemver_id_t {
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

ccsemver_decl void ccsemver_id_ctor (ccsemver_id_t *self)
  __attribute__((nonnull(1),leaf));

ccsemver_decl void ccsemver_id_dtor (ccsemver_id_t *self)
  __attribute__((nonnull(1),leaf));

ccsemver_decl char ccsemver_id_read (ccsemver_id_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));

ccsemver_decl int  ccsemver_id_write (ccsemver_id_t const * self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));

ccsemver_decl char ccsemver_id_comp (ccsemver_id_t const * self, ccsemver_id_t const * other)
  __attribute__((nonnull(1,2),leaf));


/** --------------------------------------------------------------------
 ** Semantic versions.
 ** ----------------------------------------------------------------- */

struct ccsemver_t {
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

ccsemver_decl void ccsemver_ctor(ccsemver_t *self)
  __attribute__((nonnull(1)));

ccsemver_decl void ccsemver_dtor(ccsemver_t *self)
  __attribute__((nonnull(1)));

ccsemver_decl char ccsemver_read(ccsemver_t *self, char const *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));

ccsemver_decl int  ccsemver_write (ccsemver_t const * self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));

ccsemver_decl char ccsemver_comp (ccsemver_t const * self, ccsemver_t const * other)
  __attribute__((nonnull(1,2)));


/** --------------------------------------------------------------------
 ** Comparators.
 ** ----------------------------------------------------------------- */

struct ccsemver_comp_t {
  ccsemver_comp_t *	next;
  ccsemver_op_t		op;
  ccsemver_t		version;
};

ccsemver_decl void ccsemver_comp_ctor(ccsemver_comp_t *self)
  __attribute__((nonnull(1)));

ccsemver_decl void ccsemver_comp_dtor(ccsemver_comp_t *self)
  __attribute__((nonnull(1)));

ccsemver_decl char ccsemver_comp_read(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));

ccsemver_decl char ccsemver_and(ccsemver_comp_t *self, const char *str, size_t len)
  __attribute__((nonnull(1,2)));

ccsemver_decl char ccsemver_comp_and(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));

ccsemver_decl int  ccsemver_comp_write (ccsemver_comp_t const * self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));

ccsemver_decl char ccsemver_match (ccsemver_t const * self, ccsemver_comp_t const * comp)
  __attribute__((nonnull(1,2)));


/** --------------------------------------------------------------------
 ** Ranges.
 ** ----------------------------------------------------------------- */

struct ccsemver_range_t {
  ccsemver_range_t *	next;
  ccsemver_comp_t	comp;
};

ccsemver_decl void ccsemver_range_dtor(ccsemver_range_t *self)
  __attribute__((nonnull(1)));

ccsemver_decl char ccsemver_range_read(ccsemver_range_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));

ccsemver_decl int  ccsemver_range_write (ccsemver_range_t const * self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));

ccsemver_decl char ccsemver_range_match (ccsemver_t const * self, ccsemver_range_t const * range)
  __attribute__((nonnull(1,2)));

ccsemver_decl size_t ccsemver_id_fwrite (const ccsemver_id_t * idp, FILE * stream);

ccsemver_decl size_t ccsemver_fwrite (const ccsemver_t * versionp, FILE * stream);

ccsemver_decl size_t ccsemver_comp_fwrite (const ccsemver_comp_t * compp, FILE * stream);

ccsemver_decl size_t ccsemver_range_fwrite (const ccsemver_range_t * rangep, FILE * stream);

ccsemver_decl const char * ccsemver_op_string (ccsemver_op_t op);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCSEMVER_H */

/* end of file */
