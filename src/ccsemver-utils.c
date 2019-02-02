/*
 * This  is free  and  unencumbered software  released  into the  public
 * domain.
 *
 * Anyone  is free  to copy,  modify,  publish, use,  compile, sell,  or
 * distribute this software, either in source code form or as a compiled
 * binary, for  any purpose,  commercial or  non-commercial, and  by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of  this software  dedicate any  and  all copyright  interest in  the
 * software  to the  public  domain.  We make  this  dedication for  the
 * benefit of the public at large and  to the detriment of our heirs and
 * successors.  We  intend  this  dedication  to  be  an  overt  act  of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE  SOFTWARE IS  PROVIDED "AS  IS",  WITHOUT WARRANTY  OF ANY  KIND,
 * EXPRESS OR  IMPLIED, INCLUDING BUT  NOT LIMITED TO THE  WARRANTIES OF
 * MERCHANTABILITY,    FITNESS   FOR    A    PARTICULAR   PURPOSE    AND
 * NONINFRINGEMENT.  IN  NO EVENT  SHALL THE AUTHORS  BE LIABLE  FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR  OTHERWISE, ARISING FROM,  OUT OF  OR IN CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccsemver-internals.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#undef CCSEMVER_FWRITE_STACK_BUFLEN
#define CCSEMVER_FWRITE_STACK_BUFLEN	64


long
ccsemver_strtol (cce_destination_t L, char const * input_str, char ** endptr)
{
  long	num;

  errno =  0;
  num   = strtol(input_str, endptr, 10);
  if (0 == errno) {
    return num;
  } else if (ERANGE == errno) {
    errno = 0;
    cce_raise(L, ccsemver_condition_new_parser_number_out_of_range());
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}


size_t
ccsemver_fwrite (cce_destination_t L, void * bufptr, size_t item_size, size_t item_num, FILE * stream)
{
  size_t	rv;
  errno = 0;
  rv = fwrite(bufptr, item_size, item_num, stream);
  if (! ferror(stream)) {
    return rv;
  } else {
    cce_raise(L, cce_condition_new_errno_clear());
  }
}


size_t
ccsemver_id_fwrite (cce_destination_t upper_L, ccsemver_id_t const * const id, FILE * stream)
/* Serialise  the struct  to the  STREAM.  When  successful: return  the
   number of bytes written; otherwise raise an exception. */
{

  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  /* Remember  that  "ccsemver_id_write()"  writes the  null  byte,  but
     returns  a  number   of  characters  that  does   not  include  the
     terminating null! */
  needed_count = (size_t)ccsemver_id_write(id, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    return ccsemver_fwrite(upper_L, stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      size_t	dyn_buffer_len	= 1 + needed_count;
      char *	dyn_buffer_ptr	= cce_sys_malloc_guarded(L, H, dyn_buffer_len);
      /* Remember that  "ccsemver_id_write()" writes the null  byte, but
	 returns  a  number of  characters  that  does not  include  the
	 terminating null! */
      size_t	actual_count	= ccsemver_id_write(id, dyn_buffer_ptr, dyn_buffer_len);
      size_t	rv		= ccsemver_fwrite(L, dyn_buffer_ptr, sizeof(char), actual_count, stream);
      cce_run_body_handlers(L);
      return rv;
    }
  }
}


size_t
ccsemver_sv_fwrite (cce_destination_t upper_L, ccsemver_sv_t const * const id, FILE * stream)
/* Serialise  the struct  to the  STREAM.  When  successful: return  the
   number of bytes written; otherwise raise an exception. */
{

  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  /* Remember  that  "ccsemver_sv_write()"  writes the  null  byte,  but
     returns  a  number   of  characters  that  does   not  include  the
     terminating null! */
  needed_count = (size_t)ccsemver_sv_write(id, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    return ccsemver_fwrite(upper_L, stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      size_t	dyn_buffer_len	= 1 + needed_count;
      char *	dyn_buffer_ptr	= cce_sys_malloc_guarded(L, H, dyn_buffer_len);
      /* Remember that  "ccsemver_sv_write()" writes the null  byte, but
	 returns  a  number of  characters  that  does not  include  the
	 terminating null! */
      size_t	actual_count	= ccsemver_sv_write(id, dyn_buffer_ptr, dyn_buffer_len);
      size_t	rv		= ccsemver_fwrite(L, dyn_buffer_ptr, sizeof(char), actual_count, stream);
      cce_run_body_handlers(L);
      return rv;
    }
  }
}


size_t
ccsemver_cmp_fwrite (cce_destination_t upper_L, ccsemver_cmp_t const * const id, FILE * stream)
/* Serialise  the struct  to the  STREAM.  When  successful: return  the
   number of bytes written; otherwise raise an exception. */
{

  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  /* Remember  that  "ccsemver_cmp_write()"  writes the  null  byte,  but
     returns  a  number   of  characters  that  does   not  include  the
     terminating null! */
  needed_count = (size_t)ccsemver_cmp_write(id, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    return ccsemver_fwrite(upper_L, stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      size_t	dyn_buffer_len	= 1 + needed_count;
      char *	dyn_buffer_ptr	= cce_sys_malloc_guarded(L, H, dyn_buffer_len);
      /* Remember that "ccsemver_cmp_write()" writes  the null byte, but
	 returns  a  number of  characters  that  does not  include  the
	 terminating null! */
      size_t	actual_count	= ccsemver_cmp_write(id, dyn_buffer_ptr, dyn_buffer_len);
      size_t	rv		= ccsemver_fwrite(L, dyn_buffer_ptr, sizeof(char), actual_count, stream);
      cce_run_body_handlers(L);
      return rv;
    }
  }
}


size_t
ccsemver_range_fwrite (cce_destination_t upper_L, ccsemver_range_t const * const id, FILE * stream)
/* Serialise  the struct  to the  STREAM.  When  successful: return  the
   number of bytes written; otherwise raise an exception. */
{

  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  /* Remember that  "ccsemver_range_write()" writes  the null  byte, but
     returns  a  number   of  characters  that  does   not  include  the
     terminating null! */
  needed_count = (size_t)ccsemver_range_write(id, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    return ccsemver_fwrite(upper_L, stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    cce_location_t	L[1];
    cce_clean_handler_t	H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      size_t	dyn_buffer_len	= 1 + needed_count;
      char *	dyn_buffer_ptr	= cce_sys_malloc_guarded(L, H, dyn_buffer_len);
      /* Remember  that "ccsemver_range_write()"  writes the  null byte,
	 but returns  a number of  characters that does not  include the
	 terminating null! */
      size_t	actual_count	= ccsemver_range_write(id, dyn_buffer_ptr, dyn_buffer_len);
      size_t	rv		= ccsemver_fwrite(L, dyn_buffer_ptr, sizeof(char), actual_count, stream);
      cce_run_body_handlers(L);
      return rv;
    }
  }
}


const char *
ccsemver_op_string (ccsemver_op_t op)
{
  switch (op) {
  case CCSEMVER_OP_EQ:
    return "CCSEMVER_OP_EQ";
  case CCSEMVER_OP_LT:
    return "CCSEMVER_OP_LT";
  case CCSEMVER_OP_LE:
    return "CCSEMVER_OP_LE";
  case CCSEMVER_OP_GT:
    return "CCSEMVER_OP_GT";
  case CCSEMVER_OP_GE:
    return "CCSEMVER_OP_GE";
  default:
    return "invalid value";
  }
}

/* end of file */
