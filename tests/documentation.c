/*
  Part of: CCSemver
  Contents: tests for code included in the documentation


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

#include <ccsemver.h>
#include <cctests.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing identifiers.
 ** ----------------------------------------------------------------- */

#if 0
void
doc_example_identifiers_1_1 (cce_destination_t upper_L)
{
  ccsemver_id_t	id;

  ccsemver_id_ctor(&id);
  {
    /* Do something with "id" here. */
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_1_2 (cce_destination_t upper_L)
{
  ccsemver_id_t *	idp;

  idp = malloc(sizeof(ccsemver_id_t));
  assert(NULL != idp);
  {
    ccsemver_id_ctor(idp);
    {
      /* Do something with "idp" here. */
    }
    ccsemver_id_dtor(idp);
  }
  free(idp);
}

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_2_1 (cce_destination_t upper_L)
/* Parsing a single numeric identifier. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "123";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_id_t		id;
  char			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    printf("numeric?=%d, num=%ld, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_2_2 (cce_destination_t upper_L)
/* Parsing a single alphabetic identifier. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "alpha";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_id_t		id;
  char			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    printf("numeric?=%d, num=%ld, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_2_3 (cce_destination_t upper_L)
/* Parsing a compound identifier. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_id_t		id;
  char			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    for (ccsemver_id_t * iter = &id; iter; iter = iter->next) {
      printf("numeric?=%d, num=%ld, len=%lu, raw=%s\n",
	     (int)iter->numeric, iter->num, iter->len, iter->raw);
    }
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_2_4 (cce_destination_t upper_L)
/* Parsing a compound identifier with build metadata. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7+x86-64";
  ccsemver_input_t	input = {
    .str	= input_str,
    .len	= strlen(input_str),
    .off	= 6
  };
  ccsemver_id_t		id;
  char			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    for (ccsemver_id_t * iter = &id; iter; iter = iter->next) {
      printf("numeric?=%d, num=%ld, len=%lu, raw=%s\n",
	     (int)iter->numeric, iter->num, iter->len, iter->raw);
    }
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_2_5 (cce_destination_t upper_L)
/* Parsing the empty string. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_id_t		id;
  char			rv;

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    printf("numeric?=%d, num=%ld, len=%lu, raw=%s, next=%p\n",
	   (int)id.numeric, id.num, id.len, id.raw,
	   (void *)id.next);
  } else {
    printf("invalid empty string\n");
  }
  ccsemver_id_dtor(&id);
}

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_3_1 (cce_destination_t upper_L)
/* Writing a compound identifier, enough room in the buffer. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input = {
    .str	= input_str,
    .len	= strlen(input_str),
    .off	= 6
  };
  ccsemver_id_t		id;
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];
  char			rv;

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_id_write(&id, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("input_len=%lu, buffer_len=%lu, needed_count=%lu, actual_count=%lu, id=",
	     strlen(input_str), buffer_len, needed_count, actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_id_dtor(&id);
}

void
doc_example_identifiers_3_2 (cce_destination_t upper_L)
/* Writing a compound identifier, *not* enough room in the buffer. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input = {
    .str	= input_str,
    .len	= strlen(input_str),
    .off	= 6
  };
  ccsemver_id_t		id;
  char			rv;
  size_t		buffer_len = 3;
  char			buffer_ptr[buffer_len];

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_id_read(&id, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_id_write(&id, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("input_len=%lu, buffer_len=%lu, needed_count=%lu, actual_count=%lu, id=",
	     strlen(input_str), buffer_len, needed_count, actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_id_dtor(&id);
}

/* ------------------------------------------------------------------ */

void
doc_example_identifiers_4_1 (cce_destination_t upper_L)
/* Comparing identifiers: A < B. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str_A[] = "1.2.3";
  char const	input_str_B[] = "1.2.4";
  ccsemver_input_t	input_A = {
    .str	= input_str_A,
    .len	= strlen(input_str_A),
    .off	= 0
  };
  ccsemver_input_t	input_B = {
    .str	= input_str_B,
    .len	= strlen(input_str_B),
    .off	= 0
  };
  ccsemver_id_t		id_A, id_B;
  char			rv;

  rv = ccsemver_id_read(&id_A, &input_A);
  if (0 == rv) {
    rv = ccsemver_id_read(&id_B, &input_B);
    if (0 == rv) {
      char	compar = ccsemver_id_comp(&id_A, &id_B);
      printf("compar=%d\n", (int)compar);
    }
    ccsemver_id_dtor(&id_B);
  }
  ccsemver_id_dtor(&id_A);
}

void
doc_example_identifiers_4_2 (cce_destination_t upper_L)
/* Comparing identifiers: A < B. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str_A[] = "1.4.0";
  char const	input_str_B[] = "1.2.3";
  ccsemver_input_t	input_A = {
    .str	= input_str_A,
    .len	= strlen(input_str_A),
    .off	= 0
  };
  ccsemver_input_t	input_B = {
    .str	= input_str_B,
    .len	= strlen(input_str_B),
    .off	= 0
  };
  ccsemver_id_t		id_A, id_B;
  char			rv;

  rv = ccsemver_id_read(&id_A, &input_A);
  if (0 == rv) {
    rv = ccsemver_id_read(&id_B, &input_B);
    if (0 == rv) {
      char	compar = ccsemver_id_comp(&id_A, &id_B);
      printf("compar=%d\n", (int)compar);
    }
    ccsemver_id_dtor(&id_B);
  }
  ccsemver_id_dtor(&id_A);
}
#endif


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing numeric components.
 ** ----------------------------------------------------------------- */

#if 0
void
doc_example_numeric_components_1_1 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "123";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

void
doc_example_numeric_components_1_2 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "x";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

void
doc_example_numeric_components_1_3 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "X";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

void
doc_example_numeric_components_1_4 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "*";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

void
doc_example_numeric_components_1_5 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

/* ------------------------------------------------------------------ */

void
doc_example_numeric_components_2_1 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  int	A = 123;
  int	B = 456;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

void
doc_example_numeric_components_2_2 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  int	A = 456;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

void
doc_example_numeric_components_2_3 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  int	A = 123;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

void
doc_example_numeric_components_2_4 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  int	A = CCSEMVER_NUM_X;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}
#endif


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing versions.
 ** ----------------------------------------------------------------- */

#if 0
void
doc_example_versions_1_1 (cce_destination_t upper_L)
{
  ccsemver_t	version;

  ccsemver_ctor(&version);
  {
    /* Do something with "version" here. */
  }
  ccsemver_dtor(&version);
}

void
doc_example_versions_1_2 (cce_destination_t upper_L)
{
  ccsemver_t *	sv;

  sv = malloc(sizeof(ccsemver_t));
  assert(NULL != sv);
  {
    ccsemver_ctor(sv);
    {
      /* Do something with "sv" here. */
    }
    ccsemver_dtor(sv);
  }
  free(sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_2_1 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

void
doc_example_versions_2_2 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

void
doc_example_versions_2_3 (cce_destination_t upper_L)
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7+x86-64";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

void
doc_example_versions_2_4 (cce_destination_t upper_L)
/* Parsing the empty string. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  } else {
    printf("invalid empty string\n");
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_3_1 (cce_destination_t upper_L)
/* Writing a sv, enough room in the buffer. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_write(&sv, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, id=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_versions_4_1 (cce_destination_t upper_L)
/* Comparing versions: A < B. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str_A[] = "1.2.3";
  char const	input_str_B[] = "1.2.4";
  ccsemver_input_t	input_A = {
    .str	= input_str_A,
    .len	= strlen(input_str_A),
    .off	= 0
  };
  ccsemver_input_t	input_B = {
    .str	= input_str_B,
    .len	= strlen(input_str_B),
    .off	= 0
  };
  ccsemver_t		sv_A, sv_B;
  char			rv;

  rv = ccsemver_read(&sv_A, &input_A);
  if (0 == rv) {
    rv = ccsemver_read(&sv_B, &input_B);
    if (0 == rv) {
      char	compar = ccsemver_comp(&sv_A, &sv_B);
      printf("compar=%d\n", (int)compar);
    }
    ccsemver_dtor(&sv_B);
  }
  ccsemver_dtor(&sv_A);
}
#endif


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing comparators.
 ** ----------------------------------------------------------------- */

#if 0
void
doc_example_comparators_1_1 (cce_destination_t upper_L)
{
  ccsemver_cmp_t	cmp;

  ccsemver_cmp_ctor(&cmp);
  {
    /* Do something with "comp" here. */
  }
  ccsemver_cmp_dtor(&cmp);
}

void
doc_example_comparators_1_2 (cce_destination_t upper_L)
{
  ccsemver_cmp_t *	cmp;

  cmp = malloc(sizeof(ccsemver_cmp_t));
  assert(NULL != cmp);
  {
    ccsemver_cmp_ctor(cmp);
    {
      /* Do something with "cmp" here. */
    }
    ccsemver_cmp_dtor(cmp);
  }
  free(cmp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_2_1_1 (cce_destination_t upper_L)
/* Parsing equal comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(cmp.op));
    ccsemver_fwrite(&cmp.version, stdout);
    printf("\n");
  }
  ccsemver_cmp_dtor(&cmp);
}

void
doc_example_comparators_2_1_2 (cce_destination_t upper_L)
/* Parsing less than comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "<1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(cmp.op));
    ccsemver_fwrite(&cmp.version, stdout);
    printf("\n");
  }
  ccsemver_cmp_dtor(&cmp);
}

void
doc_example_comparators_2_1_3 (cce_destination_t upper_L)
/* Parsing greater than comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = ">1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(cmp.op));
    ccsemver_fwrite(&cmp.version, stdout);
    printf("\n");
  }
  ccsemver_cmp_dtor(&cmp);
}

void
doc_example_comparators_2_1_4 (cce_destination_t upper_L)
/* Parsing less than or equal to comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "<=1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(cmp.op));
    ccsemver_fwrite(&cmp.version, stdout);
    printf("\n");
  }
  ccsemver_cmp_dtor(&cmp);
}

void
doc_example_comparators_2_1_5 (cce_destination_t upper_L)
/* Parsing greater than or equal to comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = ">=1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(cmp.op));
    ccsemver_fwrite(&cmp.version, stdout);
    printf("\n");
  }
  ccsemver_cmp_dtor(&cmp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_2_2_1 (cce_destination_t upper_L)
/* Extending a comparator. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str_A[] = ">=1.2.3";
  char const		input_str_B[] = "<3.0.0";
  ccsemver_input_t	input_A =
    ccsemver_input_new(input_str_A, strlen(input_str_A), 0);
  ccsemver_input_t	input_B =
    ccsemver_input_new(input_str_B, strlen(input_str_B), 0);
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_cmp_read(&cmp, &input_A);
  if (0 == rv) {
    rv = ccsemver_cmp_and(&cmp, &input_B);
    if (0 == rv) {
      for (ccsemver_cmp_t * iter = &cmp; iter; iter = iter->next) {
	printf("operation=%s, ", ccsemver_op_string(iter->op));
	ccsemver_fwrite(&(iter->version), stdout);
	printf("\n");
      }
      printf("comparator: ");
      ccsemver_cmp_fwrite(&cmp, stdout);
      printf("\n");
    }
  }
  ccsemver_cmp_dtor(&cmp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_3_1 (cce_destination_t upper_L)
/* Writing a comparator, enough room in the buffer. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "<=1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_cmp_t	cmp;
  char			rv;
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_cmp_read(&cmp, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_cmp_write(&cmp, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, comp=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_cmp_dtor(&cmp);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_1_1 (cce_destination_t upper_L)
/* Equality comparator, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "1.2.3";
  char const		cmp_str[] = "1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_comparators_4_1_2 (cce_destination_t upper_L)
/* Equality comparator, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "1.0.3";
  char const		cmp_str[] = "1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_2_1 (cce_destination_t upper_L)
/* Less than comparator, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "1.2.3";
  char const		cmp_str[] = "<2.0.0";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_comparators_4_2_2 (cce_destination_t upper_L)
/* Less than comparator, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "2.0.0";
  char const		cmp_str[] = "<1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_3_1 (cce_destination_t upper_L)
/* Greater than comparator, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "2.0.0";
  char const		cmp_str[] = ">1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_comparators_4_3_2 (cce_destination_t upper_L)
/* Greater than comparator, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "1.2.3";
  char const		cmp_str[] = ">2.0.0";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_4_1 (cce_destination_t upper_L)
/* Less than or equal to comparator, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "2.0.0";
  char const		cmp_str[] = "<=2.0.0";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_comparators_4_4_2 (cce_destination_t upper_L)
/* Less than or equal to comparator, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "2.0.0";
  char const		cmp_str[] = "<=1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

void
doc_example_comparators_4_5_1 (cce_destination_t upper_L)
/* Greater than or equal to comparator, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "2.0.0";
  char const		cmp_str[] = ">=1.2.3";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_comparators_4_5_2 (cce_destination_t upper_L)
/* Greater than or equal to comparator, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[]  = "1.2.3";
  char const		cmp_str[] = ">=2.0.0";
  ccsemver_input_t	sv_input  =
    ccsemver_input_new(sv_str, strlen(sv_str), 0);
  ccsemver_input_t	cmp_input =
    ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_cmp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_cmp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_cmp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}
#endif


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing ranges.
 ** ----------------------------------------------------------------- */

#if 0
void
doc_example_ranges_1_1 (cce_destination_t upper_L)
{
  char const		input_str[] = "1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    /* Do something with "rn" here. */
  }
  ccsemver_range_dtor(&rn);
}

void
doc_example_ranges_1_2 (cce_destination_t upper_L)
{
  char const		input_str[] = "1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t *	rnp;
  char			rv;

  rnp = (ccsemver_range_t *) malloc(sizeof(ccsemver_range_t));
  assert(NULL != rnp);
  {
    rv = ccsemver_range_read(rnp, &input);
    if (0 == rv) {
      /* Do something with "rnp" here. */
    }
    ccsemver_range_dtor(rnp);
  }
  free(rnp);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_2_1 (cce_destination_t upper_L)
/* Reading a simple range. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "1.2.3";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    printf("range: ");
    ccsemver_cmp_fwrite(&rn.comp, stdout);
    printf("\n");
  }
  ccsemver_range_dtor(&rn);
}

void
doc_example_ranges_2_2 (cce_destination_t upper_L)
/* Reading a complex range. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    printf("range: ");
    ccsemver_range_fwrite(&rn, stdout);
    printf("\n");
  }
  ccsemver_range_dtor(&rn);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_3_1 (cce_destination_t upper_L)
/* Writing a range, enough room in the buffer. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	input =
    ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_range_write(&rn, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, range=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_range_dtor(&rn);
}

/* ------------------------------------------------------------------ */

void
doc_example_ranges_4_1 (cce_destination_t upper_L)
/* Matching, matches. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[] = "1.4.6";
  char const		rn_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	sv_input =
    ccsemver_input_new(sv_str, strlen(sv_str),  0);
  ccsemver_input_t	rn_input =
    ccsemver_input_new(rn_str, strlen(rn_str),  0);
  ccsemver_t		sv;
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_range_read(&rn, &rn_input);
    if (0 == rv) {
      int	result = (int)ccsemver_range_match(&sv, &rn);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_range_dtor(&rn);
  }
  ccsemver_dtor(&sv);
}

void
doc_example_ranges_4_2 (cce_destination_t upper_L)
/* Matching, does not match. */
{
  printf("\n\n--- %s:\n", __func__);
  char const		sv_str[] = "1.2.8";
  char const		rn_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	sv_input =
    ccsemver_input_new(sv_str, strlen(sv_str),  0);
  ccsemver_input_t	rn_input =
    ccsemver_input_new(rn_str, strlen(rn_str),  0);
  ccsemver_t		sv;
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_range_read(&rn, &rn_input);
    if (0 == rv) {
      int	result = (int)ccsemver_range_match(&sv, &rn);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_range_dtor(&rn);
  }
  ccsemver_dtor(&sv);
}
#endif


/** --------------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------------- */

int
main (void)
{
  ccsemver_library_init();
  cctests_init("documentation examples");
  {
#if 0
    cctests_begin_group("identifiers examples");
    {
      cctests_run(doc_example_identifiers_1_1);
      cctests_run(doc_example_identifiers_1_2);
      cctests_run(doc_example_identifiers_2_1);
      cctests_run(doc_example_identifiers_2_2);
      cctests_run(doc_example_identifiers_2_3);
      cctests_run(doc_example_identifiers_2_4);
      cctests_run(doc_example_identifiers_2_5);
      cctests_run(doc_example_identifiers_3_1);
      cctests_run(doc_example_identifiers_3_2);
      cctests_run(doc_example_identifiers_4_1);
      cctests_run(doc_example_identifiers_4_2);
    }
    cctests_end_group();

    cctests_begin_group("numeric components examples");
    {
      cctests_run(doc_example_numeric_components_1_1);
      cctests_run(doc_example_numeric_components_1_2);
      cctests_run(doc_example_numeric_components_1_3);
      cctests_run(doc_example_numeric_components_1_4);
      cctests_run(doc_example_numeric_components_1_5);
      cctests_run(doc_example_numeric_components_2_1);
      cctests_run(doc_example_numeric_components_2_2);
      cctests_run(doc_example_numeric_components_2_3);
      cctests_run(doc_example_numeric_components_2_4);
    }
    cctests_end_group();

    cctests_begin_group("semver examples");
    {
      cctests_run(doc_example_versions_1_1);
      cctests_run(doc_example_versions_1_2);
      cctests_run(doc_example_versions_2_1);
      cctests_run(doc_example_versions_2_2);
      cctests_run(doc_example_versions_2_3);
      cctests_run(doc_example_versions_2_4);
      cctests_run(doc_example_versions_3_1);
      cctests_run(doc_example_versions_4_1);
    }
    cctests_end_group();

    cctests_begin_group("comparators examples");
    {
      cctests_run(doc_example_comparators_1_1);
      cctests_run(doc_example_comparators_1_2);
      cctests_run(doc_example_comparators_2_1_1);
      cctests_run(doc_example_comparators_2_1_2);
      cctests_run(doc_example_comparators_2_1_3);
      cctests_run(doc_example_comparators_2_1_4);
      cctests_run(doc_example_comparators_2_1_5);
      cctests_run(doc_example_comparators_2_2_1);
      cctests_run(doc_example_comparators_3_1);
      cctests_run(doc_example_comparators_4_1_1);
      cctests_run(doc_example_comparators_4_1_2);
      cctests_run(doc_example_comparators_4_2_1);
      cctests_run(doc_example_comparators_4_2_2);
      cctests_run(doc_example_comparators_4_3_1);
      cctests_run(doc_example_comparators_4_3_2);
      cctests_run(doc_example_comparators_4_4_1);
      cctests_run(doc_example_comparators_4_4_2);
      cctests_run(doc_example_comparators_4_5_1);
      cctests_run(doc_example_comparators_4_5_2);
    }
    cctests_end_group();

    cctests_begin_group("identifiers examples");
    {
      cctests_run(doc_example_ranges_1_1);
      cctests_run(doc_example_ranges_1_2);
      cctests_run(doc_example_ranges_2_1);
      cctests_run(doc_example_ranges_2_2);
      cctests_run(doc_example_ranges_3_1);
      cctests_run(doc_example_ranges_4_1);
      cctests_run(doc_example_ranges_4_2);
    }
    cctests_end_group();
#endif
  }
  cctests_final();
}

/* end of file */
