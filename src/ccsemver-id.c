/*
  Part of: CCSemver
  Contents: parser functions for identifiers

  Abstract

	Parser functions for identifiers.

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
#include <ctype.h>
#include <stdio.h>


/** --------------------------------------------------------------------
 ** Constructors and destructors.
 ** ----------------------------------------------------------------- */

void
ccsemver_id_ctor (ccsemver_id_t *self)
{
  self->next	= NULL;
  self->len	= 0;
  self->raw	= NULL;
  self->num	= 0;
  self->numeric	= true;
}

void
ccsemver_id_dtor (ccsemver_id_t * self)
{
  if (self->next) {
    ccsemver_id_dtor(self->next);
    free(self->next);
    self->next = NULL;
  }
}


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

char
ccsemver_id_read (ccsemver_id_t * self, char const * str, size_t len, size_t * offset)
{
#undef NEXT_CHAR
#define NEXT_CHAR		str[*offset]
  size_t	i       = 0;
  bool		is_zero = false;

  ccsemver_id_ctor(self);
  for (;
       (*offset < len) && (isalnum(NEXT_CHAR) || ('-' == NEXT_CHAR));
       ++i, ++(*offset)) {
    if (!isdigit(NEXT_CHAR)) {
      is_zero       = false;
      self->numeric = false;
    } else {
      if (i == 0) {
	is_zero = ('0' == NEXT_CHAR);
      } else if (is_zero) {
	return 1;
      }
    }
  }
/*
  while (*offset < len) {
    if (isalnum(NEXT_CHAR) || ('-' == NEXT_CHAR)) {
      if (!isdigit(NEXT_CHAR)) {
        is_zero       = false;
        self->numeric = false;
      } else {
        if (i == 0) {
          is_zero = ('0' == NEXT_CHAR);
        } else if (is_zero) {
          return 1;
        }
      }
      ++i, ++(*offset);
      continue;
    }
    break;
  }
*/
  if (!i) {
    return 1;
  }
  self->raw = str + *offset - i;
  self->len = i;
  if (!is_zero && self->numeric) {
    /* Here we  want to parse  a raw  number, not a  "numeric component"
       with "x",  "X" or  "*" elements.  So  we use  "strtol()" directly
       rather than "ccsemver_num_read()". */
    self->num = strtol(self->raw, NULL, 10);
  }
  if ('.' == NEXT_CHAR) {
    self->next = (ccsemver_id_t *) malloc(sizeof(ccsemver_id_t));
    if (NULL == self->next) {
      return 1;
    } else {
      ++(*offset);
      return ccsemver_id_read(self->next, str, len, offset);
    }
  }
  return 0;
}


/** --------------------------------------------------------------------
 ** Comparison.
 ** ----------------------------------------------------------------- */

char
ccsemver_id_comp (ccsemver_id_t const * self, ccsemver_id_t const * other)
{
  char s;

  if (!self->len && other->len) {
    return 1;
  }
  if (self->len && !other->len) {
    return -1;
  }
  if (!self->len) {
    return 0;
  }

  if (self->num && other->num) {
    if (self->num > other->num) {
      return 1;
    }
    if (self->num < other->num) {
      return -1;
    }
  }

  s = (char) memcmp(self->raw, other->raw, self->len > other->len ? self->len : other->len);

  if (s != 0) {
    return s;
  }

  if (!self->next && other->next) {
    return 1;
  }
  if (self->next && !other->next) {
    return -1;
  }
  if (!self->next) {
    return 0;
  }

  return ccsemver_id_comp(self->next, other->next);
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_id_write (ccsemver_id_t const * self, char *buffer, size_t len)
{
  char next[1024];

  if (self->next) {
    return snprintf(buffer, len, "%.*s.%.*s", (int) self->len, self->raw, ccsemver_id_write(self->next, next, 1024), next);
  }
  return snprintf(buffer, len, "%.*s", (int) self->len, self->raw);
}

/* end of file */
