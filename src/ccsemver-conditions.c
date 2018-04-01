/*
  Part of: CCSemver
  Contents: CCExceptions condition objects definitions
  Date: Apr  1, 2018

  Abstract

	This  file defines  the CCExceptions  condition objects  used by
	CCSemver.  It also holds the library initialisation function.

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
 ** Condition objects: parser error.
 ** ----------------------------------------------------------------- */

static cce_condition_static_message_fun_t	ccsemver_condition_static_message_parser_error;

static ccsemver_descriptor_parser_error_t ccsemver_descriptor_parser_error_stru = {
  /* This  "parent" field  is  set below  by  the module  initialisation
     function. */
  .descriptor.parent		= NULL,
  .descriptor.delete		= NULL,
  .descriptor.final		= NULL,
  .descriptor.static_message	= ccsemver_condition_static_message_parser_error
};

ccsemver_descriptor_parser_error_t const * const ccsemver_descriptor_parser_error_ptr = &ccsemver_descriptor_parser_error_stru;

/* This is  the single  instance of  the "invalid  pathname" exceptional
   condition.  It is used by "cce_raise()" and "cce_retry()". */
static ccsemver_condition_parser_error_t const ccsemver_condition_parser_error_stru = {
  .runtime_error.error.root.condition.descriptor = &(ccsemver_descriptor_parser_error_stru.descriptor)
};

ccsemver_condition_parser_error_t const * const ccsemver_condition_parser_error_ptr = &ccsemver_condition_parser_error_stru;

/* ------------------------------------------------------------------ */

char const *
ccsemver_condition_static_message_parser_error (cce_condition_t const * C CCSEMVER_UNUSED)
{
  return "error parsing input string";
}

/* ------------------------------------------------------------------ */

void
ccsemver_condition_init_parser_error (ccsemver_condition_parser_error_t * C)
{
  cce_condition_init_runtime_error(&(C->runtime_error));
}

cce_condition_t const *
ccsemver_condition_new_parser_error (void)
{
  return (cce_condition_t const *) ccsemver_condition_parser_error_ptr;
}

bool
ccsemver_condition_is_parser_error (cce_condition_t const * C)
{
  return cce_is_condition(C, &(ccsemver_descriptor_parser_error_ptr->descriptor));
}


/** --------------------------------------------------------------------
 ** Module initialisation.
 ** ----------------------------------------------------------------- */

void
ccsemver_init (void)
{
  ccsemver_descriptor_parser_error_stru.descriptor.parent	= &(cce_descriptor_runtime_error_ptr->descriptor);
}

/* end of file */
