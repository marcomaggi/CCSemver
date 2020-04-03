/*
  Part of: CCSemver
  Contents: private header file
  Date: Mar 27, 2018

  Abstract

	This  header  file is  for  internal  definitions.  It  must  be
	included by all the source files in this package.

  Copyright (C) 2018, 2020 Marco Maggi <mrc.mgg@gmail.com>

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

#ifndef CCSEMVER_INTERNALS_H
#define CCSEMVER_INTERNALS_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "ccsemver.h"


/** --------------------------------------------------------------------
 ** Preprocessor macros.
 ** ----------------------------------------------------------------- */



/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */




/** --------------------------------------------------------------------
 ** Function prototypes and inline functions.
 ** ----------------------------------------------------------------- */

cclib_private_decl long ccsemver_strtol (cce_destination_t L, char const * input_str, char ** endptr)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#endif /* CCSEMVER_INTERNALS_H */

/* end of file */
