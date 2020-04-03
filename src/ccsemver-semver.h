/*
  Part of: CCSemver
  Contents: subordinate public header file, semantic version parser
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

#ifndef CCSEMVER_SEMVER_H
#define CCSEMVER_SEMVER_H 1


/** --------------------------------------------------------------------
 ** Parsing semantic versions: type definitions.
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

  /* The number  of characters  in the input  string that where  parsed to  form this
     version. */
  size_t	len;

  /* Pointer to the character in the input string representing the first character of
     this component.  This pointer is valid as long as the input string handed to the
     constructor function is not mutated. */
  char const *	raw;
};


/** --------------------------------------------------------------------
 ** Parsing semantic versions: constructors and destructors.
 ** ----------------------------------------------------------------- */

cclib_decl ccsemver_sv_t * ccsemver_sv_new  (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_sv_t * ccsemver_sv_init (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_sv_t * ccsemver_sv_new_range  (cce_destination_t L, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_sv_t * ccsemver_sv_init_range (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

/* ------------------------------------------------------------------ */

cclib_decl void ccsemver_sv_delete (ccsemver_sv_t * sv)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);

cclib_decl void ccsemver_sv_reset (ccsemver_sv_t * sv)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1);


/** --------------------------------------------------------------------
 ** Parsing semantic versions: exception handlers.
 ** ----------------------------------------------------------------- */

cclib_decl void ccsemver_clean_handler_sv_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_sv_t * sv)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

cclib_decl void ccsemver_error_handler_sv_init   (cce_location_t * L, cce_error_handler_t * H, ccsemver_sv_t * sv)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3);

#define ccsemver_handler_sv_init(L,H,sv)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_clean_handler_sv_init, \
	   cce_error_handler_t		*: ccsemver_error_handler_sv_init)(L,H,sv)


/** --------------------------------------------------------------------
 ** Parsing semantic versions: guarded constructors.
 ** ----------------------------------------------------------------- */

cclib_decl ccsemver_sv_t * ccsemver_sv_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_sv_t * ccsemver_sv_new_guarded_error   (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_sv_new_guarded(L,H,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_sv_new_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_sv_new_guarded_error)(L,H,input)

/* ------------------------------------------------------------------ */

cclib_decl ccsemver_sv_t * ccsemver_sv_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H,
								ccsemver_sv_t * sv, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl ccsemver_sv_t * ccsemver_sv_init_guarded_error   (cce_destination_t L, cce_error_handler_t   * H,
								ccsemver_sv_t * sv, ccsemver_input_t * input)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccsemver_sv_init_guarded(L,H,sv,input)				\
  _Generic((H),								\
	   cce_clean_handler_t	*: ccsemver_sv_init_guarded_clean, \
	   cce_error_handler_t		*: ccsemver_sv_init_guarded_error)(L,H,sv,input)



/** --------------------------------------------------------------------
 ** Parsing semantic versions: operations.
 ** ----------------------------------------------------------------- */

cclib_decl int  ccsemver_sv_write (ccsemver_sv_t const * sv, char * buffer, size_t len)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);

cclib_decl int  ccsemver_sv_comp  (ccsemver_sv_t const * sv1, ccsemver_sv_t const * sv2)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCSEMVER_SEMVER_H */

/* end of file */
