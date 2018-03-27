/*
  Part of: CCTemplate
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

#ifndef CCTEMPLATE_H
#define CCTEMPLATE_H 1


/** --------------------------------------------------------------------
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  CCSEMVER_UNUSED  indicates  that a  function,  function
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

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccsemver_decl char const *	ccsemver_version_string			(void);
ccsemver_decl int		ccsemver_version_interface_current	(void);
ccsemver_decl int		ccsemver_version_interface_revision	(void);
ccsemver_decl int		ccsemver_version_interface_age		(void);


/** --------------------------------------------------------------------
 ** Preprocessor constants.
 ** ----------------------------------------------------------------- */

#define CCSEMVER_NUM_X (-1)


/** --------------------------------------------------------------------
 ** Type definitions.
 ** ----------------------------------------------------------------- */

typedef struct semver		semver_t;
typedef struct semver_id	semver_id_t;
typedef struct semver_comp	semver_comp_t;
typedef struct semver_range	semver_range_t;

enum semver_op {
  SEMVER_OP_EQ = 0,
  SEMVER_OP_LT,
  SEMVER_OP_LE,
  SEMVER_OP_GT,
  SEMVER_OP_GE,
};


/** --------------------------------------------------------------------
 ** Programming interface.
 ** ----------------------------------------------------------------- */

ccsemver_decl char semver_num_read(int *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4),leaf));
ccsemver_decl char semver_num_comp(int self, int other)
  __attribute__((leaf,const));

struct semver_id {
  bool numeric;
  int num;
  size_t len;
  const char *raw;
  struct semver_id *next;
};

ccsemver_decl void semver_id_ctor(semver_id_t *self)
  __attribute__((nonnull(1),leaf));
ccsemver_decl void semver_id_dtor(semver_id_t *self)
  __attribute__((nonnull(1),leaf));
ccsemver_decl char semver_id_read(semver_id_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));
ccsemver_decl int  semver_id_pwrite(const semver_id_t *self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));
ccsemver_decl char semver_id_pcomp(const semver_id_t *self, const semver_id_t *other)
  __attribute__((nonnull(1,2),leaf));

#define semver_id_write(self, buffer, len) semver_id_pwrite(&(self), buffer, len)
#define semver_id_comp(self, other) semver_id_pcomp(&(self), &(other))

struct semver {
  int major, minor, patch;
  semver_id_t prerelease, build;
  size_t len;
  const char *raw;
};

ccsemver_decl void semver_ctor(semver_t *self)
  __attribute__((nonnull(1)));
ccsemver_decl void semver_dtor(semver_t *self)
  __attribute__((nonnull(1)));
ccsemver_decl char semver_read(semver_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));
ccsemver_decl int  semver_pwrite(const semver_t *self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));
ccsemver_decl char semver_pcomp(const semver_t *self, const semver_t *other)
  __attribute__((nonnull(1,2)));

#define semver_write(self, buffer, len) semver_pwrite(&(self), buffer, len)
#define semver_comp(self, other) semver_pcomp(&(self), &(other))

struct semver_comp {
  struct semver_comp *next;
  enum semver_op op;
  semver_t version;
};

ccsemver_decl void semver_comp_ctor(semver_comp_t *self)
  __attribute__((nonnull(1)));
ccsemver_decl void semver_comp_dtor(semver_comp_t *self)
  __attribute__((nonnull(1)));
ccsemver_decl char semver_comp_read(semver_comp_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));
ccsemver_decl char semver_and(semver_comp_t *self, const char *str, size_t len)
  __attribute__((nonnull(1,2)));
ccsemver_decl char semver_comp_and(semver_comp_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));
ccsemver_decl int  semver_comp_pwrite(const semver_comp_t *self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));
ccsemver_decl char semver_pmatch(const semver_t *self, const semver_comp_t *comp)
  __attribute__((nonnull(1,2)));

#define semver_comp_write(self, buffer, len) semver_comp_pwrite(&(self), buffer, len)
#define semver_match(self, comp) semver_pmatch(&(self), &(comp))

struct semver_range {
  struct semver_range *next;
  semver_comp_t comp;
};

ccsemver_decl void semver_range_dtor(semver_range_t *self)
  __attribute__((nonnull(1)));
ccsemver_decl char semver_range_read(semver_range_t *self, const char *str, size_t len, size_t *offset)
  __attribute__((nonnull(1,2,4)));
ccsemver_decl int  semver_range_pwrite(const semver_range_t *self, char *buffer, size_t len)
  __attribute__((nonnull(1,2)));
ccsemver_decl char semver_prmatch(const semver_t *self, const semver_range_t *range)
  __attribute__((nonnull(1,2)));

#define semver_range_write(self, buffer, len) semver_range_pwrite(&(self), buffer, len)
#define semver_rmatch(self, comp) semver_prmatch(&(self), &(comp))

ccsemver_decl size_t semver_id_fwrite (const semver_id_t * idp, FILE * stream);
ccsemver_decl size_t semver_fwrite (const semver_t * versionp, FILE * stream);
ccsemver_decl size_t semver_comp_fwrite (const semver_comp_t * compp, FILE * stream);
ccsemver_decl size_t semver_range_fwrite (const semver_range_t * rangep, FILE * stream);
ccsemver_decl const char * semver_op_string (enum semver_op op);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCTEMPLATE_H */

/* end of file */
