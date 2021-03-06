/*
  Part of: CCSemver
  Contents: parsing ranges and creating range objects


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

#include <ccsemver-internals.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void ccsemver_range_parse (cce_destination_t L, ccsemver_range_t * range, ccsemver_input_t * input)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Constructor and destructor.
 ** ----------------------------------------------------------------- */

static void
ccsemver_range_delete_after_new (ccsemver_range_t * range)
{
  if (range->cmp.delete) {
    ccsemver_cmp_delete(&(range->cmp));
  }
  if (range->next) {
    ccsemver_range_delete(range->next);
  }
  memset(range, 0, sizeof(ccsemver_range_t));
  free(range);
}

ccsemver_range_t *
ccsemver_range_new (cce_destination_t upper_L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(upper_L, input);
  {
    cce_location_t		L[1];
    ccsemver_range_t *		range;
    cce_error_handler_t		range_H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      range		= cce_sys_malloc_guarded(L, range_H, sizeof(ccsemver_range_t));
      range->delete	= ccsemver_range_delete_after_new;
      range->next	= NULL;
      ccsemver_cmp_reset(&(range->cmp));
      ccsemver_range_parse(L, range, input);
      cce_run_body_handlers(L);
    }
    return range;
  }
}

/* ------------------------------------------------------------------ */

static void
ccsemver_range_delete_after_init (ccsemver_range_t * range)
{
  if (range->cmp.delete) {
    ccsemver_cmp_delete(&(range->cmp));
  }
  if (range->next) {
    ccsemver_range_delete(range->next);
  }
  memset(range, 0, sizeof(ccsemver_range_t));
}

ccsemver_range_t *
ccsemver_range_init (cce_destination_t L, ccsemver_range_t * range, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);
  range->delete = ccsemver_range_delete_after_init;
  range->next	= NULL;
  ccsemver_cmp_reset(&(range->cmp));
  ccsemver_range_parse(L, range, input);
  return range;
}

/* ------------------------------------------------------------------ */

void
ccsemver_range_delete (ccsemver_range_t * range)
{
  if (range->delete) {
    range->delete(range);
  }
}

void
ccsemver_range_reset (ccsemver_range_t * range)
/* Reset the struct  to an initial, empty state.  This  function must be
   used when we embed  a struct of this type into  another struct and we
   might or  might not apply a  constructor to it: this  function resets
   the struct to a safe, empty state. */
{
  range->delete	= NULL;
  range->next	= NULL;
  ccsemver_cmp_reset(&(range->cmp));
}


/** --------------------------------------------------------------------
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

void
ccsemver_clean_handler_range_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_range_t * range)
{
  cce_init_and_register_handler(L, H, cce_default_clean_handler_function,
				cce_resource_pointer(range), cce_resource_destructor(ccsemver_range_delete));
}

void
ccsemver_error_handler_range_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_range_t * range)
{
  cce_init_and_register_handler(L, H, cce_default_error_handler_function,
				cce_resource_pointer(range), cce_resource_destructor(ccsemver_range_delete));
}

/* ------------------------------------------------------------------ */

ccsemver_range_t *
ccsemver_range_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_range_t *	range = ccsemver_range_new(L, input);
  ccsemver_clean_handler_range_init(L, H, range);
  return range;
}

ccsemver_range_t *
ccsemver_range_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_range_t *	range = ccsemver_range_new(L, input);
  ccsemver_error_handler_range_init(L, H, range);
  return range;
}

/* ------------------------------------------------------------------ */

ccsemver_range_t *
ccsemver_range_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_range_t * range, ccsemver_input_t * input)
{
  ccsemver_range_init(L, range, input);
  ccsemver_clean_handler_range_init(L, H, range);
  return range;
}

ccsemver_range_t *
ccsemver_range_init_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_range_t * range, ccsemver_input_t * input)
{
  ccsemver_range_init(L, range, input);
  ccsemver_error_handler_range_init(L, H, range);
  return range;
}


/** --------------------------------------------------------------------
 ** Parser functions.
 ** ----------------------------------------------------------------- */

void
ccsemver_range_parse (cce_destination_t L, ccsemver_range_t * range, ccsemver_input_t * input)
/* Parse a range from the input string.  A range is defined as follows:
 *
 *    <range> := <comparator> (<blanks> "||" <blanks> <range>)
 */
{
  ccsemver_input_parse_blanks(input);

  /* Read the first comparator. */
  ccsemver_cmp_init(L, &(range->cmp), input);

  /* Check if  there is a "||"  operator after the first  comparator; if
     there  is:  parse  the  next  range.   Otherwise  leave  the  input
     unchanged. */
  if (ccsemver_input_parse_blanked_OR(input)) {
    range->next = ccsemver_range_new(L, input);
  }
}


/** --------------------------------------------------------------------
 ** Matching.
 ** ----------------------------------------------------------------- */

int
ccsemver_range_match (ccsemver_sv_t const * self, ccsemver_range_t const * range)
{
  return (char)  (ccsemver_match(self, &range->cmp) ? 1 : (range->next ? ccsemver_range_match(self, range->next) : 0));
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_range_write (ccsemver_range_t const * self, char * buffer, size_t len)
{
  char comp[1024], next[1024];

  if (self->next) {
    /* Remember that  "snprintf()" writes the  null byte, but  returns a
       number  of  characters  that  does not  include  the  terminating
       null! */
    return snprintf(buffer, len, "%.*s || %.*s",
		    ccsemver_cmp_write(&(self->cmp), comp, 1024), comp,
		    ccsemver_range_write(self->next, next, 1024), next);
  } else {
    /* Remember that  "snprintf()" writes the  null byte, but  returns a
       number  of  characters  that  does not  include  the  terminating
       null! */
    return snprintf(buffer, len, "%.*s", ccsemver_cmp_write(&(self->cmp), comp, 1024), comp);
  }
}

/* end of file */
