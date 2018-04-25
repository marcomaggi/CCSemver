/*
  Part of: CCSemver
  Contents: parser functions for semvers

  Abstract

	Parser functions for semvers.

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

#include <ccsemver-internals.h>
#include <string.h>
#include <stdio.h>

static void ccsemver_sv_parse_full    (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));

static void ccsemver_sv_parse_range (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Constructors and destructors.
 ** ----------------------------------------------------------------- */

static void
ccsemver_sv_delete_after_new (ccsemver_sv_t * sv)
{
  ccsemver_id_delete(&(sv->prerelease));
  ccsemver_id_delete(&(sv->build));
  memset(sv, 0, sizeof(ccsemver_sv_t));
  free(sv);
}

ccsemver_sv_t *
ccsemver_sv_new (cce_destination_t upper_L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(upper_L, input);
  {
    cce_location_t		L[1];
    cce_error_handler_t		sv_H[1];

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      ccsemver_sv_t *	sv = cce_sys_malloc_guarded(L, sv_H, sizeof(ccsemver_sv_t));
      sv->delete = ccsemver_sv_delete_after_new;
      ccsemver_sv_parse_full(L, sv, input);
      cce_run_cleanup_handlers(L);
      return sv;
    }
  }
}

ccsemver_sv_t *
ccsemver_sv_new_range (cce_destination_t upper_L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(upper_L, input);
  {
    cce_location_t		L[1];
    cce_error_handler_t		sv_H[1];

    if (cce_location(L)) {
      cce_run_error_handlers_raise(L, upper_L);
    } else {
      ccsemver_sv_t *	sv;
      sv         = cce_sys_malloc_guarded(L, sv_H, sizeof(ccsemver_sv_t));
      sv->delete = ccsemver_sv_delete_after_new;
      ccsemver_sv_parse_range(L, sv, input);
      cce_run_cleanup_handlers(L);
      return sv;
    }
  }
}

/* ------------------------------------------------------------------ */

static void
ccsemver_sv_delete_after_init (ccsemver_sv_t * sv)
{
  ccsemver_id_delete(&(sv->prerelease));
  ccsemver_id_delete(&(sv->build));
  memset(sv, 0, sizeof(ccsemver_sv_t));
}

ccsemver_sv_t *
ccsemver_sv_init (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);
  sv->delete = ccsemver_sv_delete_after_init;
  ccsemver_sv_parse_full(L, sv, input);
  return sv;
}

ccsemver_sv_t *
ccsemver_sv_init_range (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);
  sv->delete = ccsemver_sv_delete_after_init;
  ccsemver_sv_parse_range(L, sv, input);
  return sv;
}

/* ------------------------------------------------------------------ */

void
ccsemver_sv_delete (ccsemver_sv_t * sv)
{
  if (sv->delete) {
    sv->delete(sv);
  }
}


/** --------------------------------------------------------------------
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
static void
ccsemver_handler_sv_function (cce_condition_t const * C CCE_UNUSED, cce_handler_t * H)
{
  ccsemver_sv_delete(H->pointer);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsemver_cleanup_handler_sv_init (cce_location_t * L, cce_cleanup_handler_t * H, ccsemver_sv_t * sv)
{
  H->handler.function	= ccsemver_handler_sv_function;
  H->handler.pointer	= sv;
  cce_register_cleanup_handler(L, &(H->handler));
}

void
ccsemver_error_handler_sv_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_sv_t * sv)
{
  H->handler.function	= ccsemver_handler_sv_function;
  H->handler.pointer	= sv;
  cce_register_error_handler(L, &(H->handler));
}

/* ------------------------------------------------------------------ */

ccsemver_sv_t *
ccsemver_sv_new_guarded_cleanup (cce_destination_t L, cce_cleanup_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_sv_t *	sv = ccsemver_sv_new(L, input);
  ccsemver_cleanup_handler_sv_init(L, H, sv);
  return sv;
}

ccsemver_sv_t *
ccsemver_sv_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_sv_t *	sv = ccsemver_sv_new(L, input);
  ccsemver_error_handler_sv_init(L, H, sv);
  return sv;
}

/* ------------------------------------------------------------------ */

ccsemver_sv_t *
ccsemver_sv_init_guarded_cleanup (cce_destination_t L, cce_cleanup_handler_t * H, ccsemver_sv_t * sv, ccsemver_input_t * input)
{
  ccsemver_sv_init(L, sv, input);
  ccsemver_cleanup_handler_sv_init(L, H, sv);
  return sv;
}

ccsemver_sv_t *
ccsemver_sv_init_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_sv_t * sv, ccsemver_input_t * input)
{
  ccsemver_sv_init(L, sv, input);
  ccsemver_error_handler_sv_init(L, H, sv);
  return sv;
}


/** --------------------------------------------------------------------
 ** Parser functions.
 ** ----------------------------------------------------------------- */

void
ccsemver_sv_parse_full (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
{
  sv->major	= 0;
  sv->minor	= 0;
  sv->patch	= 0;
  sv->len	= 0;
  sv->raw	= input->str + input->off;
  memset(&(sv->prerelease), 0, sizeof(ccsemver_id_t));
  memset(&(sv->build),      0, sizeof(ccsemver_id_t));

  /* Skip the initial "v" character, if any. */
  ccsemver_input_parse_v(input);

  sv->major = ccsemver_parse_number(L, input);

  if (ccsemver_input_parse_dot(input)) {
    sv->minor = ccsemver_parse_number(L, input);

    if (ccsemver_input_parse_dot(input)) {
      sv->patch = ccsemver_parse_number(L, input);

      /* Parse the  "prerelease tag", if  any.  It  is fine if  there is
	 none. */
      if (ccsemver_input_parse_dash(input)) {
	ccsemver_id_init(L, input, &(sv->prerelease));
      }

      /* Parse the  "build metadata", if  any.  It  is fine if  there is
	 none. */
      if (ccsemver_input_parse_plus(input)) {
	ccsemver_id_init(L, input, &(sv->build));
      }

      sv->len = input->str + input->off - sv->raw;
    } else {
      /* Missing patch level is an error. */
      cce_raise(L, ccsemver_condition_new_parser_invalid_input());
    }
  } else {
    /* Missing minor version is an error. */
    cce_raise(L, ccsemver_condition_new_parser_invalid_input());
  }
}

void
ccsemver_sv_parse_range (cce_destination_t L, ccsemver_sv_t * sv, ccsemver_input_t * input)
/* Parse  a partial  semantic version.   It  must start  with the  major
   version number  as numeric  component.  The  minor version  number is
   optional.   The  patch level  is  optional.   The prerelease  tag  is
   optional.  The build metadata is optional. */
{
  sv->major	= CCSEMVER_NUM_X;
  sv->minor	= CCSEMVER_NUM_X;
  sv->patch	= CCSEMVER_NUM_X;
  sv->len	= 0;
  sv->raw	= input->str + input->off;
  memset(&(sv->prerelease), 0, sizeof(ccsemver_id_t));
  memset(&(sv->build),      0, sizeof(ccsemver_id_t));

  sv->major = ccsemver_parse_numeric_component(L, input);

  /* Parse the minor number, if any. It is fine if there is none. */
  if (ccsemver_input_parse_dot(input)) {
    sv->minor = ccsemver_parse_numeric_component(L, input);

    /* If the major number is a wild card: the minor nummber must either
       be absent or  be a wildcard too.  The semver  "x.x" is fine.  The
       semver "x.2" is an error, because the 2 would be ignored. */
    if ((CCSEMVER_NUM_X == sv->major) && (CCSEMVER_NUM_X != sv->minor)) {
      cce_raise(L, ccsemver_condition_new_parser_expected_xrange_numeric_component());
    }

    /* Parse the patch level, if any. It is fine if there is none. */
    if (ccsemver_input_parse_dot(input)) {
      sv->patch = ccsemver_parse_numeric_component(L, input);

      /* If the minor number is a wild card: the patch level must either
	 be absent  or be a wildcard  too.  The semver "1.x.x"  is fine.
	 The  semver  "1.x.3"  is  an  error, because  the  3  would  be
	 ignored. */
      if ((CCSEMVER_NUM_X == sv->minor) && (CCSEMVER_NUM_X != sv->patch)) {
	cce_raise(L, ccsemver_condition_new_parser_expected_xrange_numeric_component());
      }

      /* Parse the  "prerelease tag", if  any.  It  is fine if  there is
	 none. */
      if (ccsemver_input_parse_dash(input)) {
	ccsemver_id_init(L, input, &(sv->prerelease));
      }

      /* Parse the  "build metadata", if  any.  It  is fine if  there is
	 none. */
      if (ccsemver_input_parse_plus(input)) {
	ccsemver_id_init(L, input, &(sv->build));
      }

      sv->len = input->str + input->off - sv->raw;
    }
  }
}


/** --------------------------------------------------------------------
 ** Comparision.
 ** ----------------------------------------------------------------- */

int
ccsemver_sv_comp (ccsemver_sv_t const * sv1, ccsemver_sv_t const * sv2)
{
  int	result;

  if (0 != (result = ccsemver_num_comp(sv1->major, sv2->major))) {
    return result;
  } else if (0 != (result = ccsemver_num_comp(sv1->minor, sv2->minor))) {
    return result;
  } else if (0 != (result = ccsemver_num_comp(sv1->patch, sv2->patch))) {
    return result;
  } else if (0 != (result = ccsemver_id_comp(&(sv1->prerelease), &(sv2->prerelease)))) {
    return result;
  } else {
    return ccsemver_id_comp(&(sv1->build), &(sv2->build));
  }
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_sv_write (ccsemver_sv_t const * sv, char * buffer, size_t len)
{
  char	prerelease[256], build[256];
  int	rv;

  if (sv->prerelease.len && sv->build.len) {
    rv = snprintf(buffer, len, "%ld.%ld.%ld-%.*s+%.*s",
		  sv->major, sv->minor, sv->patch,
		  ccsemver_id_write(&(sv->prerelease), prerelease, 256), prerelease,
		  ccsemver_id_write(&(sv->build), build, 256), build);
  } else if (sv->prerelease.len) {
    rv = snprintf(buffer, len, "%ld.%ld.%ld-%.*s",
		  sv->major, sv->minor, sv->patch,
		  ccsemver_id_write(&(sv->prerelease), prerelease, 256), prerelease);
  } else if (sv->build.len) {
    rv = snprintf(buffer, len, "%ld.%ld.%ld+%.*s",
		  sv->major, sv->minor, sv->patch,
		  ccsemver_id_write(&(sv->build), build, 256), build);
  } else {
    rv = snprintf(buffer, len, "%ld.%ld.%ld", sv->major, sv->minor, sv->patch);
  }
  return rv;
}

/* end of file */
