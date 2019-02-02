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

static void ccsemver_id_parse (cce_destination_t L, ccsemver_input_t * input, ccsemver_id_t * id)
  __attribute__((__nonnull__(1,2,3)));


/** --------------------------------------------------------------------
 ** Constructors and destructors.
 ** ----------------------------------------------------------------- */

static void
ccsemver_id_delete_after_new (ccsemver_id_t * id)
{
  if (id->next) {
    ccsemver_id_delete(id->next);
  }
  free(id);
  memset(id, 0, sizeof(ccsemver_id_t));
}

ccsemver_id_t *
ccsemver_id_new (cce_destination_t upper_L, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(upper_L, input);
  {
    cce_location_t		L[1];
    ccsemver_id_t *		id;
    cce_error_handler_t		id_H[1];

    if (cce_location(L)) {
      cce_run_catch_handlers_raise(L, upper_L);
    } else {
      id         = cce_sys_malloc_guarded(L, id_H, sizeof(ccsemver_id_t));
      id->delete = ccsemver_id_delete_after_new;
      ccsemver_id_parse(L, input, id);
      cce_run_body_handlers(L);
    }
    return id;
  }
}

/* ------------------------------------------------------------------ */

static void
ccsemver_id_delete_after_init (ccsemver_id_t * id)
{
  if (id->next) {
    ccsemver_id_delete(id->next);
  }
  memset(id, 0, sizeof(ccsemver_id_t));
}

ccsemver_id_t *
ccsemver_id_init (cce_destination_t L, ccsemver_id_t * id, ccsemver_input_t * input)
{
  ccsemver_input_assert_more_input(L, input);
  id->delete = ccsemver_id_delete_after_init;
  ccsemver_id_parse(L, input, id);
  return id;
}

/* ------------------------------------------------------------------ */

void
ccsemver_id_delete (ccsemver_id_t * id)
{
  if (id->delete) {
    id->delete(id);
  }
}

void
ccsemver_id_reset (ccsemver_id_t * id)
/* Reset the struct  to an initial, empty state.  This  function must be
   used when we embed  a struct of this type into  another struct and we
   might or  might not apply a  constructor to it: this  function resets
   the struct to a safe, empty state. */
{
  id->delete	= NULL;
  id->numeric	= false;
  id->num	= 0;
  id->len	= 0;
  id->raw	= NULL;
  id->next	= NULL;
}


/** --------------------------------------------------------------------
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
static void
ccsemver_handler_id_function (cce_condition_t const * C CCE_UNUSED, cce_handler_t * H)
{
  ccsemver_id_delete(H->pointer);
  if (0) { fprintf(stderr, "%s: done\n", __func__); }
}

void
ccsemver_clean_handler_id_init (cce_location_t * L, cce_clean_handler_t * H, ccsemver_id_t * id)
{
  H->handler.function	= ccsemver_handler_id_function;
  H->handler.pointer	= id;
  cce_register_clean_handler(L, H);
}

void
ccsemver_error_handler_id_init (cce_location_t * L, cce_error_handler_t * H, ccsemver_id_t * id)
{
  H->handler.function	= ccsemver_handler_id_function;
  H->handler.pointer	= id;
  cce_register_error_handler(L, H);
}

/* ------------------------------------------------------------------ */

ccsemver_id_t *
ccsemver_id_new_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_id_t *	id = ccsemver_id_new(L, input);
  ccsemver_clean_handler_id_init(L, H, id);
  return id;
}

ccsemver_id_t *
ccsemver_id_new_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_input_t * input)
{
  ccsemver_id_t *	id = ccsemver_id_new(L, input);
  ccsemver_error_handler_id_init(L, H, id);
  return id;
}

/* ------------------------------------------------------------------ */

ccsemver_id_t *
ccsemver_id_init_guarded_clean (cce_destination_t L, cce_clean_handler_t * H, ccsemver_id_t * id, ccsemver_input_t * input)
{
  ccsemver_id_init(L, id, input);
  ccsemver_clean_handler_id_init(L, H, id);
  return id;
}

ccsemver_id_t *
ccsemver_id_init_guarded_error (cce_destination_t L, cce_error_handler_t * H, ccsemver_id_t * id, ccsemver_input_t * input)
{
  ccsemver_id_init(L, id, input);
  ccsemver_error_handler_id_init(L, H, id);
  return id;
}


/** --------------------------------------------------------------------
 ** Parser.
 ** ----------------------------------------------------------------- */

void
ccsemver_id_parse (cce_destination_t L, ccsemver_input_t * input, ccsemver_id_t * id)
{
  size_t	component_len = 0;
  /* This is set to true if the input string "0". */
  bool		is_zero       = false;

  id->num	= 0;
  id->numeric	= true;
  id->len	= 0;
  id->raw	= NULL;
  id->next	= NULL;

  for (;
       ccsemver_input_more(input) && (isalnum(ccsemver_input_next(input)) || ('-' == ccsemver_input_next(input)));
       ++component_len, ++(input->off)) {
    if (!isdigit(ccsemver_input_next(input))) {
      is_zero     = false;
      id->numeric = false;
    } else {
      if (0 == component_len) {
	is_zero = ('0' == ccsemver_input_next(input));
      } else if (is_zero) {
	/* We reject  identifier components  that are numeric  and start
	   with zero, for example "0123".

	   FIXME Is this a good idea?  (Marco Maggi; Apr 7, 2018) */
	cce_raise(L, ccsemver_condition_new_parser_expected_identifier());
      }
    }
  }

  if (0 == component_len) {
    /* If we are here: there is no identifier component at the beginning
       of the input string. */
    cce_raise(L, ccsemver_condition_new_parser_expected_identifier());
  }

  /* Here we know that the input string holds an identifier component of
     length "component_len". */
  id->raw = input->str + input->off - component_len;
  id->len = component_len;

  /* Only if  the identifier  component is  numeric we  convert it  to a
     number and  store it into  "id->num".  If the  component represents
     zero: we do nothing because "id->num" is already zero. */
  if ((! is_zero) && id->numeric) {
    /* Here we  want to parse  a raw  number, not a  "numeric component"
       with  "x", "X"  or  "*" elements.   We  use "strtol()"  directly,
       rather  than "ccsemver_parse_number()",  because we  have already
       determined that: there is more input; the next input is a number.

       Also, we have already determined that the numeric string starting
       at  "id->raw"  is a  positive  number,  so  the return  value  of
       "strtol()" is a positive number. */
    id->num = ccsemver_strtol(L, id->raw, NULL);
  }

  /* Is there another  component after this one?  If  the next character
     is a dot: there is. */
  if (ccsemver_input_parse_dot(input)) {
    id->next = ccsemver_id_new(L, input);
  }
}


/** --------------------------------------------------------------------
 ** Comparison.
 ** ----------------------------------------------------------------- */

int
ccsemver_id_comp (ccsemver_id_t const * id1, ccsemver_id_t const * id2)
{
  int	s;

  if (!id1->len && id2->len) {
    return 1;
  }
  if (id1->len && !id2->len) {
    return -1;
  }
  if (!id1->len) {
    return 0;
  }

  if (id1->num && id2->num) {
    if (id1->num > id2->num) {
      return 1;
    }
    if (id1->num < id2->num) {
      return -1;
    }
  }

  s = memcmp(id1->raw, id2->raw, id1->len > id2->len ? id1->len : id2->len);

  if (s != 0) {
    return s;
  }

  if (!id1->next && id2->next) {
    return 1;
  }
  if (id1->next && !id2->next) {
    return -1;
  }
  if (!id1->next) {
    return 0;
  }

  return ccsemver_id_comp(id1->next, id2->next);
}


/** --------------------------------------------------------------------
 ** Serialisation.
 ** ----------------------------------------------------------------- */

int
ccsemver_id_write (ccsemver_id_t const * id, char * buffer, size_t len)
{
  char next[1024];

  if (id->next) {
    /* Remember that  "snprintf()" writes the  null byte, but  returns a
       number  of  characters  that  does not  include  the  terminating
       null! */
    return snprintf(buffer, len, "%.*s.%.*s", (int) id->len, id->raw, ccsemver_id_write(id->next, next, 1024), next);
  }
  /* Remember  that "snprintf()"  writes the  null byte,  but returns  a
     number of characters that does not include the terminating null! */
  return snprintf(buffer, len, "%.*s", (int) id->len, id->raw);
}

/* end of file */
