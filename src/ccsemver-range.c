/*
  Part of: CCSemver
  Contents: parsing ranges and creating range objects


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
#include <stdlib.h>
#include <stdio.h>


/** --------------------------------------------------------------------
 ** Constructor and destructor.
 ** ----------------------------------------------------------------- */

static void
ccsemver_range_init (ccsemver_range_t * self)
{
  self->next = NULL;
  ccsemver_comp_ctor(&self->comp);
}

void
ccsemver_range_dtor (ccsemver_range_t * self)
{
  if (self->next) {
    ccsemver_range_dtor(self->next);
    free(self->next);
    self->next = NULL;
  }
}


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
skip_blank_characters (ccsemver_input_t * input)
{
  while (ccsemver_input_more(input) &&
	 ((' '  == ccsemver_input_next(input)) ||
	  ('\t' == ccsemver_input_next(input)) ||
	  ('\r' == ccsemver_input_next(input)) ||
	  ('\n' == ccsemver_input_next(input)))) {
    ++(input->off);
  }
}

char
ccsemver_range_read (ccsemver_range_t * self, ccsemver_input_t * input)
/* Parse a range from the input string.  A range is defined as follows:
 *
 *    <range> := <comparator> ("||" <range>)
 */
{
  ccsemver_range_init(self);

  /* Read the first comparator. */
  if (ccsemver_comp_read(&self->comp, input)) {
    return 1;
  }

  skip_blank_characters(input);

  /* Check if there is a "||" operator after the first comparator. */
  if (looking_at_OR(input)) {
    input->off += 2;
    skip_blank_characters(input);
    self->next = (ccsemver_range_t *) malloc(sizeof(ccsemver_range_t));
    if (self->next) {
      return ccsemver_range_read(self->next, input);
    } else {
      return 1;
    }
  } else {
    return 0;
  }
}


/** --------------------------------------------------------------------
 ** Matching.
 ** ----------------------------------------------------------------- */

char
ccsemver_range_match (ccsemver_t const * self, ccsemver_range_t const * range)
{
  return (char)  (ccsemver_match(self, &range->comp) ? 1 : range->next ? ccsemver_range_match(self, range->next) : 0);
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_range_write (ccsemver_range_t const * self, char * buffer, size_t len)
{
  char comp[1024], next[1024];

  if (self->next) {
    return snprintf(buffer, len, "%.*s || %.*s",
		    ccsemver_comp_write(&(self->comp), comp, 1024), comp,
		    ccsemver_range_write(self->next, next, 1024), next);
  }
  return snprintf(buffer, len, "%.*s", ccsemver_comp_write(&(self->comp), comp, 1024), comp);
}

/* end of file */
