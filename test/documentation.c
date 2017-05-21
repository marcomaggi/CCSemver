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
 ** Helpers.
 ** ----------------------------------------------------------------- */

#include <errno.h>

#undef SEMVER_FWRITE_STACK_BUFLEN
#define SEMVER_FWRITE_STACK_BUFLEN	2

static int
semver_id_fwrite (const semver_id_t * idp, FILE * stream)
/* Serialise the identifier to the  STREAM.  When successful: return the
   number  of  bytes written.   When  unsuccessful:  return -1  and  set
   "errno". */
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_id_pwrite(idp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_id_pwrite(idp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

static int
semver_fwrite (const semver_t * versionp, FILE * stream)
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_pwrite(versionp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_pwrite(versionp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

static int
semver_comp_fwrite (const semver_comp_t * compp, FILE * stream)
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_comp_pwrite(compp, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_comp_pwrite(compp, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

static int
semver_range_fwrite (const semver_range_t * rangep, FILE * stream)
{
  size_t	buffer_len = SEMVER_FWRITE_STACK_BUFLEN;
  char		buffer_ptr[buffer_len];
  size_t	needed_count;

  needed_count = (size_t)semver_range_pwrite(rangep, buffer_ptr, buffer_len);
  if (0 == needed_count) {
    return 0;
  } else if (needed_count < buffer_len) {
    errno = 0;
    return fwrite(buffer_ptr, sizeof(char), needed_count, stream);
  } else {
    size_t	buffer_len = needed_count+1;
    char *	buffer_ptr;
    errno = 0;
    buffer_ptr = (char *)malloc(buffer_len);
    if (NULL == buffer_ptr) {
      return -1;
    } else {
      size_t	actual_count  = semver_range_pwrite(rangep, buffer_ptr, buffer_len);
      size_t	written_count;
      errno = 0;
      written_count = fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      free(buffer_ptr);
      return written_count;
    }
  }
}

const char *
semver_op_string (enum semver_op op)
{
  switch (op) {
  case SEMVER_OP_EQ:
    return "SEMVER_OP_EQ";
  case SEMVER_OP_LT:
    return "SEMVER_OP_LT";
  case SEMVER_OP_LE:
    return "SEMVER_OP_LE";
  case SEMVER_OP_GT:
    return "SEMVER_OP_GT";
  case SEMVER_OP_GE:
    return "SEMVER_OP_GE";
  default:
    return "invalid value";
  }
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing identifiers.
 ** ----------------------------------------------------------------- */

void
doc_example_identifiers_1_1 (void)
{
  semver_id_t	id;

  semver_id_ctor(&id);
  {
    /* Do something with "id" here. */
  }
  semver_id_dtor(&id);
}

void
doc_example_identifiers_1_2 (void)
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

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_2_1 (void)
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
doc_example_identifiers_2_2 (void)
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
doc_example_identifiers_2_3 (void)
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
doc_example_identifiers_2_4 (void)
/* Parsing a compound identifier with build metadata. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7+x86-64";
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
doc_example_identifiers_2_5 (void)
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
doc_example_identifiers_2_6 (void)
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
      printf("len=%lu, actual_count=%lu, id=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_id_dtor(&id);
}

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_3_1 (void)
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

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_4_1 (void)
/* Comparing identifiers: A < B. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str_A[] = "1.2.3";
  static const char	input_str_B[] = "1.2.4";
  semver_id_t	id_A, id_B;
  size_t	offset_A = 0, offset_B = 0;
  char		rv;

  rv = semver_id_read(&id_A, input_str_A, strlen(input_str_A), &offset_A);
  if (0 == rv) {
    rv = semver_id_read(&id_B, input_str_B, strlen(input_str_B), &offset_B);
    if (0 == rv) {
      char	compar = semver_id_comp(id_A, id_B);
      printf("compar=%d\n", (int)compar);
    }
    semver_id_dtor(&id_B);
  }
  semver_id_dtor(&id_A);
}

void
doc_example_identifiers_4_2 (void)
/* Comparing identifiers: A < B. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str_A[] = "1.4.0";
  static const char	input_str_B[] = "1.2.3";
  semver_id_t	id_A, id_B;
  size_t	offset_A = 0, offset_B = 0;
  char		rv;

  rv = semver_id_read(&id_A, input_str_A, strlen(input_str_A), &offset_A);
  if (0 == rv) {
    rv = semver_id_read(&id_B, input_str_B, strlen(input_str_B), &offset_B);
    if (0 == rv) {
      char	compar = semver_id_comp(id_A, id_B);
      printf("compar=%d\n", (int)compar);
    }
    semver_id_dtor(&id_B);
  }
  semver_id_dtor(&id_A);
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing numeric components.
 ** ----------------------------------------------------------------- */

void
doc_example_numeric_components_1_1 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_buf[] = "123";
  int		num;
  size_t	input_off = 0;
  char		rv;

  rv = semver_num_read(&num, input_buf, strlen(input_buf), &input_off);
  if (0 == rv) {
    printf("num=%d\n", num);
  }
}

void
doc_example_numeric_components_1_2 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_buf[] = "x";
  int		num;
  size_t	input_off = 0;
  char		rv;

  rv = semver_num_read(&num, input_buf, strlen(input_buf), &input_off);
  if (0 == rv) {
    printf("num=%d\n", num);
  }
}

void
doc_example_numeric_components_1_3 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_buf[] = "X";
  int		num;
  size_t	input_off = 0;
  char		rv;

  rv = semver_num_read(&num, input_buf, strlen(input_buf), &input_off);
  if (0 == rv) {
    printf("num=%d\n", num);
  }
}

void
doc_example_numeric_components_1_4 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_buf[] = "*";
  int		num;
  size_t	input_off = 0;
  char		rv;

  rv = semver_num_read(&num, input_buf, strlen(input_buf), &input_off);
  if (0 == rv) {
    printf("num=%d\n", num);
  }
}

void
doc_example_numeric_components_1_5 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_buf[] = "";
  int		num;
  size_t	input_off = 0;
  char		rv;

  rv = semver_num_read(&num, input_buf, strlen(input_buf), &input_off);
  if (0 == rv) {
    printf("num=%d\n", num);
  }
}

/* ------------------------------------------------------------------ */

void
doc_example_numeric_components_2_1 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 123;
  int	B = 456;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)semver_num_comp(A, B));
}

void
doc_example_numeric_components_2_2 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 456;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)semver_num_comp(A, B));
}

void
doc_example_numeric_components_2_3 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 123;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)semver_num_comp(A, B));
}

void
doc_example_numeric_components_2_4 (void)
{
  printf("--- %s:\n", __func__);
  int	A = SEMVER_NUM_X;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)semver_num_comp(A, B));
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing versions.
 ** ----------------------------------------------------------------- */

void
doc_example_versions_1_1 (void)
{
  semver_t	version;

  semver_ctor(&version);
  {
    /* Do something with "version" here. */
  }
  semver_dtor(&version);
}

void
doc_example_versions_1_2 (void)
{
  semver_t *	versionp;

  versionp = malloc(sizeof(semver_t));
  assert(NULL != versionp);
  {
    semver_ctor(versionp);
    {
      /* Do something with "versionp" here. */
    }
    semver_dtor(versionp);
  }
  free(versionp);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_2_1 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3";
  semver_t	version;
  size_t	offset = 0;
  char		rv;

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("major=%d, minor=%d, patch=%d, ",
	   version.major, version.minor, version.patch);
    printf("prerelease=");
    semver_id_fwrite(&version.prerelease, stdout);
    printf(", ");
    printf("build=");
    semver_id_fwrite(&version.build, stdout);
    printf("\n");
  }
  semver_dtor(&version);
}

void
doc_example_versions_2_2 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7";
  semver_t	version;
  size_t	offset = 0;
  char		rv;

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("major=%d, minor=%d, patch=%d, ",
	   version.major, version.minor, version.patch);
    printf("prerelease=");
    semver_id_fwrite(&version.prerelease, stdout);
    printf(", ");
    printf("build=");
    semver_id_fwrite(&version.build, stdout);
    printf("\n");
  }
  semver_dtor(&version);
}

void
doc_example_versions_2_3 (void)
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7+x86-64";
  semver_t	version;
  size_t	offset = 0;
  char		rv;

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("major=%d, minor=%d, patch=%d, ",
	   version.major, version.minor, version.patch);
    printf("prerelease=");
    semver_id_fwrite(&version.prerelease, stdout);
    printf(", ");
    printf("build=");
    semver_id_fwrite(&version.build, stdout);
    printf("\n");
  }
  semver_dtor(&version);
}

void
doc_example_versions_2_4 (void)
/* Parsing the empty string. */
{

  printf("--- %s:\n", __func__);
  static const char	input_str[] = "";
  semver_t	version;
  size_t	offset = 0;
  char		rv;

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("major=%d, minor=%d, patch=%d, ",
	   version.major, version.minor, version.patch);
    printf("prerelease=");
    semver_id_fwrite(&version.prerelease, stdout);
    printf(", ");
    printf("build=");
    semver_id_fwrite(&version.build, stdout);
    printf("\n");
  } else {
    printf("invalid empty string\n");
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_3_1 (void)
/* Writing a version, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3-alpha.7";
  size_t	buffer_len = 32;
  char		buffer_ptr[buffer_len];
  semver_t	version;
  size_t	offset = 0;
  char		rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = semver_read(&version, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)semver_write(version, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, id=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_4_1 (void)
/* Comparing versions: A < B. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str_A[] = "1.2.3";
  static const char	input_str_B[] = "1.2.4";
  semver_t	version_A, version_B;
  size_t	offset_A = 0, offset_B = 0;
  char		rv;

  rv = semver_read(&version_A, input_str_A, strlen(input_str_A), &offset_A);
  if (0 == rv) {
    rv = semver_read(&version_B, input_str_B, strlen(input_str_B), &offset_B);
    if (0 == rv) {
      char	compar = semver_comp(version_A, version_B);
      printf("compar=%d\n", (int)compar);
    }
    semver_dtor(&version_B);
  }
  semver_dtor(&version_A);
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing comparators.
 ** ----------------------------------------------------------------- */

void
doc_example_comparators_1_1 (void)
{
  semver_comp_t	comp;

  semver_comp_ctor(&comp);
  {
    /* Do something with "comp" here. */
  }
  semver_comp_dtor(&comp);
}

void
doc_example_comparators_1_2 (void)
{
  semver_comp_t *	compp;

  compp = malloc(sizeof(semver_comp_t));
  assert(NULL != compp);
  {
    semver_comp_ctor(compp);
    {
      /* Do something with "compp" here. */
    }
    semver_comp_dtor(compp);
  }
  free(compp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_2_1_1 (void)
/* Parsing equal comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3";
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("operation=%s, ", semver_op_string(comp.op));
    semver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  semver_comp_dtor(&comp);
}

void
doc_example_comparators_2_1_2 (void)
/* Parsing less than comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "<1.2.3";
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("operation=%s, ", semver_op_string(comp.op));
    semver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  semver_comp_dtor(&comp);
}

void
doc_example_comparators_2_1_3 (void)
/* Parsing greater than comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = ">1.2.3";
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("operation=%s, ", semver_op_string(comp.op));
    semver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  semver_comp_dtor(&comp);
}

void
doc_example_comparators_2_1_4 (void)
/* Parsing less than or equal to comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "<=1.2.3";
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("operation=%s, ", semver_op_string(comp.op));
    semver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  semver_comp_dtor(&comp);
}

void
doc_example_comparators_2_1_5 (void)
/* Parsing greater than or equal to comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = ">=1.2.3";
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("operation=%s, ", semver_op_string(comp.op));
    semver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  semver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_2_2_1 (void)
/* Extending a comparator. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str_1[] = ">=1.2.3";
  static const char	input_str_2[] = "<3.0.0";
  semver_comp_t	comp;
  size_t	offset_1 = 0;
  size_t	offset_2 = 0;
  char		rv;

  rv = semver_comp_read(&comp, input_str_1, strlen(input_str_1), &offset_1);
  if (0 == rv) {
    rv = semver_comp_and(&comp, input_str_2, strlen(input_str_2), &offset_2);
    if (0 == rv) {
      for (semver_comp_t * iter = &comp; iter; iter = iter->next) {
	printf("operation=%s, ", semver_op_string(iter->op));
	semver_fwrite(&(iter->version), stdout);
	printf("\n");
      }
      printf("comparator: ");
      semver_comp_fwrite(&comp, stdout);
      printf("\n");
    }
  }
  semver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_3_1 (void)
/* Writing a comparator, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "<=1.2.3";
  size_t	buffer_len = 32;
  char		buffer_ptr[buffer_len];
  semver_comp_t	comp;
  size_t	offset = 0;
  char		rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = semver_comp_read(&comp, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)semver_comp_write(comp, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, comp=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_1_1 (void)
/* Equality comparator, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.2.3";
  static const char	compar_str[]  = "1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

void
doc_example_comparators_4_1_2 (void)
/* Equality comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.0.3";
  static const char	compar_str[]  = "1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_2_1 (void)
/* Less than comparator, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.2.3";
  static const char	compar_str[]  = "<2.0.0";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

void
doc_example_comparators_4_2_2 (void)
/* Less than comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "2.0.0";
  static const char	compar_str[]  = "<1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_3_1 (void)
/* Greater than comparator, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "2.0.0";
  static const char	compar_str[]  = ">1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

void
doc_example_comparators_4_3_2 (void)
/* Greater than comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.2.3";
  static const char	compar_str[]  = ">2.0.0";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_4_1 (void)
/* Less than or equal to comparator, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "2.0.0";
  static const char	compar_str[]  = "<=2.0.0";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

void
doc_example_comparators_4_4_2 (void)
/* Less than or equal to comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "2.0.0";
  static const char	compar_str[]  = "<=1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_5_1 (void)
/* Greater than or equal to comparator, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "2.0.0";
  static const char	compar_str[]  = ">=1.2.3";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}

void
doc_example_comparators_4_5_2 (void)
/* Greater than or equal to comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.2.3";
  static const char	compar_str[]  = ">=2.0.0";
  semver_t	version;
  semver_comp_t	comp;
  size_t	version_off = 0, comp_off = 0;
  char		rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_comp_read(&comp, compar_str, strlen(compar_str), &comp_off);
    if (0 == rv) {
      int	result = (int)semver_match(version, comp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_comp_dtor(&comp);
  }
  semver_dtor(&version);
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing ranges.
 ** ----------------------------------------------------------------- */

void
doc_example_ranges_1_1 (void)
{
  static const char	input_str[] = "1.2.3";
  semver_range_t	range;
  size_t		offset = 0;
  char			rv;

  rv = semver_range_read(&range, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    /* Do something with "range" here. */
  }
  semver_range_dtor(&range);
}

void
doc_example_ranges_1_2 (void)
{
  static const char	input_str[] = "1.2.3";
  semver_range_t *	rangep;
  size_t		offset = 0;
  char			rv;

  rangep = (semver_range_t *) malloc(sizeof(semver_range_t));
  assert(NULL != rangep);
  {
    rv = semver_range_read(rangep, input_str, strlen(input_str), &offset);
    if (0 == rv) {
      /* Do something with "rangep" here. */
    }
    semver_range_dtor(rangep);
  }
  free(rangep);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_2_1 (void)
/* Reading a simple range. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.3";
  semver_range_t	range;
  size_t		offset = 0;
  char			rv;

  rv = semver_range_read(&range, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("range: ");
    semver_comp_fwrite(&range.comp, stdout);
    printf("\n");
  }
  semver_range_dtor(&range);
}

void
doc_example_ranges_2_2 (void)
/* Reading a complex range. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  semver_range_t	range;
  size_t		offset = 0;
  char			rv;

  rv = semver_range_read(&range, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    printf("range: ");
    semver_range_fwrite(&range, stdout);
    printf("\n");
  }
  semver_range_dtor(&range);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_3_1 (void)
/* Writing a range, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  static const char	input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];
  semver_range_t	range;
  size_t		offset = 0;
  char			rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = semver_range_read(&range, input_str, strlen(input_str), &offset);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)semver_range_write(range, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, range=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  semver_range_dtor(&range);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_4_1 (void)
/* Matching, matches. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.4.6";
  static const char	range_str[]  = "1.2.7 || >=1.2.9 <2.0.0";
  semver_t		version;
  semver_range_t	range;
  size_t		version_off = 0, range_off = 0;
  char			rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_range_read(&range, range_str, strlen(range_str), &range_off);
    if (0 == rv) {
      int	result = (int)semver_rmatch(version, range);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_range_dtor(&range);
  }
  semver_dtor(&version);
}

void
doc_example_ranges_4_2 (void)
/* Matching, does not match. */
{
  printf("--- %s:\n", __func__);
  static const char	version_str[] = "1.2.8";
  static const char	range_str[]  = "1.2.7 || >=1.2.9 <2.0.0";
  semver_t		version;
  semver_range_t	range;
  size_t		version_off = 0, range_off = 0;
  char			rv;

  rv = semver_read(&version, version_str, strlen(version_str), &version_off);
  if (0 == rv) {
    rv = semver_range_read(&range, range_str, strlen(range_str), &range_off);
    if (0 == rv) {
      int	result = (int)semver_rmatch(version, range);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    semver_range_dtor(&range);
  }
  semver_dtor(&version);
}


/** --------------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------------- */

int
main (void)
{
  doc_example_identifiers_1_1();
  doc_example_identifiers_1_2();
  doc_example_identifiers_2_1();
  doc_example_identifiers_2_2();
  doc_example_identifiers_2_3();
  doc_example_identifiers_2_4();
  doc_example_identifiers_2_5();
  doc_example_identifiers_2_6();
  doc_example_identifiers_3_1();
  doc_example_identifiers_4_1();
  doc_example_identifiers_4_2();


  doc_example_numeric_components_1_1();
  doc_example_numeric_components_1_2();
  doc_example_numeric_components_1_3();
  doc_example_numeric_components_1_4();
  doc_example_numeric_components_1_5();
  doc_example_numeric_components_2_1();
  doc_example_numeric_components_2_2();
  doc_example_numeric_components_2_3();
  doc_example_numeric_components_2_4();

  doc_example_versions_1_1();
  doc_example_versions_1_2();
  doc_example_versions_2_1();
  doc_example_versions_2_2();
  doc_example_versions_2_3();
  doc_example_versions_2_4();
  doc_example_versions_3_1();
  doc_example_versions_4_1();

  doc_example_comparators_1_1();
  doc_example_comparators_1_2();
  doc_example_comparators_2_1_1();
  doc_example_comparators_2_1_2();
  doc_example_comparators_2_1_3();
  doc_example_comparators_2_1_4();
  doc_example_comparators_2_1_5();
  doc_example_comparators_2_2_1();
  doc_example_comparators_3_1();
  doc_example_comparators_4_1_1();
  doc_example_comparators_4_1_2();
  doc_example_comparators_4_2_1();
  doc_example_comparators_4_2_2();
  doc_example_comparators_4_3_1();
  doc_example_comparators_4_3_2();
  doc_example_comparators_4_4_1();
  doc_example_comparators_4_4_2();
  doc_example_comparators_4_5_1();
  doc_example_comparators_4_5_2();

  doc_example_ranges_1_1();
  doc_example_ranges_1_2();
  doc_example_ranges_2_1();
  doc_example_ranges_2_2();
  doc_example_ranges_3_1();
  doc_example_ranges_4_1();
  doc_example_ranges_4_2();

  return EXIT_SUCCESS;
}

/* end of file */
