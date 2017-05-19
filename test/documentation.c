/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#include <semver.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing identifiers.
 ** ----------------------------------------------------------------- */

void
doc_example_identifiers_1 (void)
{
  semver_id_t	id;

  semver_id_ctor(&id);
  {
    /* Do something with "id" here. */
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_2 (void)
{
  semver_id_t *	idp;

  idp = malloc(sizeof(semver_id_t));
  assert(NULL != idp);
  {
    semver_id_ctor(idp);
    {
      /* Do something with "idp" here. */
    }
    semver_id_dtor(idp);
  }
  free(idp);
}

void
doc_example_identifiers_3 (void)
/* Parsing a single numeric identifier. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "123";
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("numeric?=%d, num=%d, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_4 (void)
/* Parsing a single alphabetic identifier. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "alpha";
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("numeric?=%d, num=%d, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_5 (void)
/* Parsing a compound identifier. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7";
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    for (semver_id_t * iter = &id; iter; iter = iter->next) {
      printf("numeric?=%d, num=%d, len=%lu, raw=%s\n",
	     (int)iter->numeric, iter->num, iter->len, iter->raw);
    }
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_6 (void)
/* Parsing the empty string. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "";
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("numeric?=%d, num=%d, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  } else {
    printf("invalid empty string\n");
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_7 (void)
/* Writing a compound identifier, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7";
  size_t	buffer_len = 32;
  char		buffer_ptr[buffer_len];
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)semver_id_write(id, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("actual_count=%lu, id=", actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_8 (void)
/* Writing a compound identifier, *not* enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7";
  size_t	buffer_len = 6;
  char		buffer_ptr[buffer_len];
  semver_id_t	id;
  size_t	offset = 0;
  char		rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = semver_id_read(&id, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)semver_id_write(id, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("actual_count=%lu, id=", actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_id_dtor(&id);
}


/** --------------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------------- */

int main(void) {
  doc_example_identifiers_1();
  doc_example_identifiers_2();
  doc_example_identifiers_3();
  doc_example_identifiers_4();
  doc_example_identifiers_5();
  doc_example_identifiers_6();
  doc_example_identifiers_7();
  doc_example_identifiers_8();

  return EXIT_SUCCESS;
}

/* end of file */
