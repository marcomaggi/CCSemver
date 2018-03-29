/*
  Part of: CCSemver
  Contents: memory allocator
  Date: Mar 29, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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

#include "ccsemver-internals.h"


/** --------------------------------------------------------------------
 ** Default allocator.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1)))
static void *
ccsemver_default_malloc (ccsemver_allocator_t const * const A CCSEMVER_UNUSED, cce_destination_t L, size_t const size)
{
  return cce_sys_malloc(L, size);
}

__attribute__((__nonnull__(1,2)))
static void
ccsemver_default_free (ccsemver_allocator_t const * const A CCSEMVER_UNUSED, void * ptr)
{
  free(ptr);
}

static ccsemver_allocator_t const ccsemver_default_allocator_stru = {
  .malloc	= ccsemver_default_malloc,
  .free		= ccsemver_default_free,
};


/** --------------------------------------------------------------------
 ** Public API.
 ** ----------------------------------------------------------------- */

ccsemver_allocator_t const * const ccsemver_default_allocator = &ccsemver_default_allocator_stru;
ccsemver_allocator_t const *       ccsemver_current_allocator = &ccsemver_default_allocator_stru;

void *
ccsemver_malloc (cce_destination_t L, size_t size)
{
  return ccsemver_current_allocator->malloc(ccsemver_current_allocator, L, size);
}

void
ccsemver_free (void * ptr)
{
  ccsemver_current_allocator->free(ccsemver_current_allocator, ptr);
}

/* end of file */
