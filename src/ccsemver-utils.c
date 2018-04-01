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

#include <ccsemver-internals.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#undef CCSEMVER_FWRITE_STACK_BUFLEN
#define CCSEMVER_FWRITE_STACK_BUFLEN	64

size_t
ccsemver_id_fwrite (const ccsemver_id_t * idp, FILE * stream)
/* Serialise the identifier to the  STREAM.  When successful: return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   set "errno" to an error code. */
{
  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  needed_count = (size_t )ccsemver_id_write(idp, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    errno = 0;
    return fwrite(stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	dyn_buffer_len = needed_count+1;
    char *	dyn_buffer_ptr;
    errno = 0;
    dyn_buffer_ptr = (char *)malloc(dyn_buffer_len);
    if (NULL == dyn_buffer_ptr) {
      return 0;
    } else {
      size_t	actual_count  = ccsemver_id_write(idp, dyn_buffer_ptr, dyn_buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(dyn_buffer_ptr, sizeof(char), actual_count, stdout);
      free(dyn_buffer_ptr);
      return written_count;
    }
  }
}

size_t
ccsemver_fwrite (const ccsemver_t * versionp, FILE * stream)
/* Serialise the  version to  the STREAM.   When successful:  return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   set "errno" to an error code. */
{
  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  needed_count = (size_t )ccsemver_write(versionp, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    errno = 0;
    return fwrite(stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	dyn_buffer_len = needed_count+1;
    char *	dyn_buffer_ptr;
    errno = 0;
    dyn_buffer_ptr = (char *)malloc(dyn_buffer_len);
    if (NULL == dyn_buffer_ptr) {
      return 0;
    } else {
      size_t	actual_count  = ccsemver_write(versionp, dyn_buffer_ptr, dyn_buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(dyn_buffer_ptr, sizeof(char), actual_count, stdout);
      free(dyn_buffer_ptr);
      return written_count;
    }
  }
}

size_t
ccsemver_comp_fwrite (const ccsemver_comp_t * compp, FILE * stream)
/* Serialise the comparator to the  STREAM.  When successful: return the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   set "errno" to an error code. */
{
  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  needed_count = (size_t )ccsemver_comp_write(compp, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    errno = 0;
    return fwrite(stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	dyn_buffer_len = needed_count+1;
    char *	dyn_buffer_ptr;
    errno = 0;
    dyn_buffer_ptr = (char *)malloc(dyn_buffer_len);
    if (NULL == dyn_buffer_ptr) {
      return 0;
    } else {
      size_t	actual_count  = ccsemver_comp_write(compp, dyn_buffer_ptr, dyn_buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(dyn_buffer_ptr, sizeof(char), actual_count, stdout);
      free(dyn_buffer_ptr);
      return written_count;
    }
  }
}

size_t
ccsemver_range_fwrite (const ccsemver_range_t * rangep, FILE * stream)
/* Serialise  the range  to  the STREAM.   When  successful: return  the
   number of bytes written and  set "errno" to zero.  When unsuccessful:
   set "errno" to an error code. */
{
  size_t	stk_buffer_len = CCSEMVER_FWRITE_STACK_BUFLEN;
  char		stk_buffer_ptr[stk_buffer_len];
  size_t	needed_count;

  needed_count = (size_t )ccsemver_range_write(rangep, stk_buffer_ptr, stk_buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < stk_buffer_len) {
    errno = 0;
    return fwrite(stk_buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	dyn_buffer_len = needed_count+1;
    char *	dyn_buffer_ptr;
    errno = 0;
    dyn_buffer_ptr = (char *)malloc(dyn_buffer_len);
    if (NULL == dyn_buffer_ptr) {
      return 0;
    } else {
      size_t	actual_count  = ccsemver_range_write(rangep, dyn_buffer_ptr, dyn_buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(dyn_buffer_ptr, sizeof(char), actual_count, stdout);
      free(dyn_buffer_ptr);
      return written_count;
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