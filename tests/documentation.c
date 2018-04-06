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
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing identifiers.
 ** ----------------------------------------------------------------- */

static void
doc_example_identifiers_1_1 (void)
{
  ccsemver_id_t	id;

  ccsemver_id_ctor(&id);
  {
    /* Do something with "id" here. */
  }
  ccsemver_id_dtor(&id);
}

static void
doc_example_identifiers_1_2 (void)
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

static void
doc_example_identifiers_2_1 (void)
/* Parsing a single numeric identifier. */
{
  printf("--- %s:\n", __func__);
  static char const	input_str[] = "123";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
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

static void
doc_example_identifiers_2_2 (void)
/* Parsing a single alphabetic identifier. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "alpha";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
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

static void
doc_example_identifiers_2_3 (void)
/* Parsing a compound identifier. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
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

static void
doc_example_identifiers_2_4 (void)
/* Parsing a compound identifier with build metadata. */
{
  printf("--- %s:\n", __func__);
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

static void
doc_example_identifiers_2_5 (void)
/* Parsing the empty string. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
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

static void
doc_example_identifiers_3_1 (void)
/* Writing a compound identifier, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
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

static void
doc_example_identifiers_3_2 (void)
/* Writing a compound identifier, *not* enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
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

static void
doc_example_identifiers_4_1 (void)
/* Comparing identifiers: A < B. */
{
  printf("--- %s:\n", __func__);
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

static void
doc_example_identifiers_4_2 (void)
/* Comparing identifiers: A < B. */
{
  printf("--- %s:\n", __func__);
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


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing numeric components.
 ** ----------------------------------------------------------------- */

static void
doc_example_numeric_components_1_1 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "123";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

static void
doc_example_numeric_components_1_2 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "x";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

static void
doc_example_numeric_components_1_3 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "X";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

static void
doc_example_numeric_components_1_4 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "*";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

static void
doc_example_numeric_components_1_5 (void)
{
  printf("--- %s:\n", __func__);
  char const		input_str[] = "";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  long			num;
  char			rv;

  rv = ccsemver_num_parse(&num, &input);
  if (0 == rv) {
    printf("num=%ld\n", num);
  }
}

/* ------------------------------------------------------------------ */

static void
doc_example_numeric_components_2_1 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 123;
  int	B = 456;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

static void
doc_example_numeric_components_2_2 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 456;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

static void
doc_example_numeric_components_2_3 (void)
{
  printf("--- %s:\n", __func__);
  int	A = 123;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}

static void
doc_example_numeric_components_2_4 (void)
{
  printf("--- %s:\n", __func__);
  int	A = CCSEMVER_NUM_X;
  int	B = 123;
  printf("A=%d, B=%d, compar=%d\n", A, B, (int)ccsemver_num_comp(A, B));
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing versions.
 ** ----------------------------------------------------------------- */

static void
doc_example_versions_1_1 (void)
{
  ccsemver_t	version;

  ccsemver_ctor(&version);
  {
    /* Do something with "version" here. */
  }
  ccsemver_dtor(&version);
}

static void
doc_example_versions_1_2 (void)
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

static void
doc_example_versions_2_1 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", ");
    printf("build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_versions_2_2 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", ");
    printf("build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_versions_2_3 (void)
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7+x86-64";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", ");
    printf("build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_versions_2_4 (void)
/* Parsing the empty string. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_t		sv;
  char			rv;

  rv = ccsemver_read(&sv, &input);
  if (0 == rv) {
    printf("major=%ld, minor=%ld, patch=%ld, ",
	   sv.major, sv.minor, sv.patch);
    printf("prerelease=");
    ccsemver_id_fwrite(&sv.prerelease, stdout);
    printf(", ");
    printf("build=");
    ccsemver_id_fwrite(&sv.build, stdout);
    printf("\n");
  } else {
    printf("invalid empty string\n");
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

static void
doc_example_versions_3_1 (void)
/* Writing a sv, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3-alpha.7";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
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

static void
doc_example_versions_4_1 (void)
/* Comparing versions: A < B. */
{
  printf("--- %s:\n", __func__);
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


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing comparators.
 ** ----------------------------------------------------------------- */

static void
doc_example_comparators_1_1 (void)
{
  ccsemver_comp_t	comp;

  ccsemver_comp_ctor(&comp);
  {
    /* Do something with "comp" here. */
  }
  ccsemver_comp_dtor(&comp);
}

static void
doc_example_comparators_1_2 (void)
{
  ccsemver_comp_t *	compp;

  compp = malloc(sizeof(ccsemver_comp_t));
  assert(NULL != compp);
  {
    ccsemver_comp_ctor(compp);
    {
      /* Do something with "compp" here. */
    }
    ccsemver_comp_dtor(compp);
  }
  free(compp);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_2_1_1 (void)
/* Parsing equal comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(comp.op));
    ccsemver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  ccsemver_comp_dtor(&comp);
}

static void
doc_example_comparators_2_1_2 (void)
/* Parsing less than comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "<1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(comp.op));
    ccsemver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  ccsemver_comp_dtor(&comp);
}

static void
doc_example_comparators_2_1_3 (void)
/* Parsing greater than comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = ">1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(comp.op));
    ccsemver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  ccsemver_comp_dtor(&comp);
}

static void
doc_example_comparators_2_1_4 (void)
/* Parsing less than or equal to comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "<=1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(comp.op));
    ccsemver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  ccsemver_comp_dtor(&comp);
}

static void
doc_example_comparators_2_1_5 (void)
/* Parsing greater than or equal to comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = ">=1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    printf("operation=%s, ", ccsemver_op_string(comp.op));
    ccsemver_fwrite(&comp.version, stdout);
    printf("\n");
  }
  ccsemver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_2_2_1 (void)
/* Extending a comparator. */
{
  printf("--- %s:\n", __func__);
  char const	input_str_A[] = ">=1.2.3";
  char const	input_str_B[] = "<3.0.0";
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
  ccsemver_comp_t	comp;
  char			rv;

  rv = ccsemver_comp_read(&comp, &input_A);
  if (0 == rv) {
    rv = ccsemver_comp_and(&comp, &input_B);
    if (0 == rv) {
      for (ccsemver_comp_t * iter = &comp; iter; iter = iter->next) {
	printf("operation=%s, ", ccsemver_op_string(iter->op));
	ccsemver_fwrite(&(iter->version), stdout);
	printf("\n");
      }
      printf("comparator: ");
      ccsemver_comp_fwrite(&comp, stdout);
      printf("\n");
    }
  }
  ccsemver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_3_1 (void)
/* Writing a comparator, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "<=1.2.3";
  ccsemver_input_t	input = ccsemver_input_new(input_str, strlen(input_str), 0);
  ccsemver_comp_t	comp;
  char			rv;
  size_t		buffer_len = 32;
  char			buffer_ptr[buffer_len];

  memset(buffer_ptr, 0, buffer_len);

  rv = ccsemver_comp_read(&comp, &input);
  if (0 == rv) {
    size_t	needed_count;
    size_t	actual_count;
    needed_count = (size_t)ccsemver_comp_write(&comp, buffer_ptr, buffer_len);
    if (0 < needed_count) {
      actual_count = (needed_count < buffer_len)? needed_count : buffer_len;
      printf("len=%lu, actual_count=%lu, comp=",
	     strlen(input_str), actual_count);
      fwrite(buffer_ptr, sizeof(char), actual_count, stdout);
      printf("\n");
    }
  }
  ccsemver_comp_dtor(&comp);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_4_1_1 (void)
/* Equality comparator, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[] = "1.2.3";
  char const	cmp_str[]  = "1.2.3";
  ccsemver_input_t	sv_input = {
    .str	= sv_str,
    .len	= strlen(sv_str),
    .off	= 0
  };
  ccsemver_input_t	cmp_input = {
    .str	= cmp_str,
    .len	= strlen(cmp_str),
    .off	= 0
  };
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_comparators_4_1_2 (void)
/* Equality comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[] = "1.0.3";
  char const	cmp_str[]  = "1.2.3";
  ccsemver_input_t	sv_input = {
    .str	= sv_str,
    .len	= strlen(sv_str),
    .off	= 0
  };
  ccsemver_input_t	cmp_input = {
    .str	= cmp_str,
    .len	= strlen(cmp_str),
    .off	= 0
  };
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_4_2_1 (void)
/* Less than comparator, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[] = "1.2.3";
  char const	cmp_str[]  = "<2.0.0";
  ccsemver_input_t	sv_input = {
    .str	= sv_str,
    .len	= strlen(sv_str),
    .off	= 0
  };
  ccsemver_input_t	cmp_input = {
    .str	= cmp_str,
    .len	= strlen(cmp_str),
    .off	= 0
  };
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_comparators_4_2_2 (void)
/* Less than comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "2.0.0";
  char const	cmp_str[] = "<1.2.3";
  ccsemver_input_t	sv_input = {
    .str	= sv_str,
    .len	= strlen(sv_str),
    .off	= 0
  };
  ccsemver_input_t	cmp_input = {
    .str	= cmp_str,
    .len	= strlen(cmp_str),
    .off	= 0
  };
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_4_3_1 (void)
/* Greater than comparator, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "2.0.0";
  char const	cmp_str[] = ">1.2.3";
  ccsemver_input_t	sv_input = {
    .str	= sv_str,
    .len	= strlen(sv_str),
    .off	= 0
  };
  ccsemver_input_t	cmp_input = {
    .str	= cmp_str,
    .len	= strlen(cmp_str),
    .off	= 0
  };
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_comparators_4_3_2 (void)
/* Greater than comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "1.2.3";
  char const	cmp_str[] = ">2.0.0";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_4_4_1 (void)
/* Less than or equal to comparator, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "2.0.0";
  char const	cmp_str[] = "<=2.0.0";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_comparators_4_4_2 (void)
/* Less than or equal to comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "2.0.0";
  char const	cmp_str[] = "<=1.2.3";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

/* ------------------------------------------------------------------ */

static void
doc_example_comparators_4_5_1 (void)
/* Greater than or equal to comparator, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "2.0.0";
  char const	cmp_str[] = ">=1.2.3";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}

static void
doc_example_comparators_4_5_2 (void)
/* Greater than or equal to comparator, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[]  = "1.2.3";
  char const	cmp_str[] = ">=2.0.0";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str,  strlen(sv_str),  0);
  ccsemver_input_t	cmp_input = ccsemver_input_new(cmp_str, strlen(cmp_str), 0);
  ccsemver_t		sv;
  ccsemver_comp_t	cmp;
  char			rv;

  rv = ccsemver_read(&sv, &sv_input);
  if (0 == rv) {
    rv = ccsemver_comp_read(&cmp, &cmp_input);
    if (0 == rv) {
      int	result = (int)ccsemver_match(&sv, &cmp);
      printf("matches?=%s\n", (result)? "yes" : "no");
    }
    ccsemver_comp_dtor(&cmp);
  }
  ccsemver_dtor(&sv);
}


/** --------------------------------------------------------------------
 ** Documentation snippets: parsing ranges.
 ** ----------------------------------------------------------------- */

static void
doc_example_ranges_1_1 (void)
{
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input  = ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    /* Do something with "rn" here. */
  }
  ccsemver_range_dtor(&rn);
}

static void
doc_example_ranges_1_2 (void)
{
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input  = ccsemver_input_new(input_str, strlen(input_str),  0);
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

static void
doc_example_ranges_2_1 (void)
/* Reading a simple range. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.3";
  ccsemver_input_t	input  = ccsemver_input_new(input_str, strlen(input_str),  0);
  ccsemver_range_t	rn;
  char			rv;

  rv = ccsemver_range_read(&rn, &input);
  if (0 == rv) {
    printf("range: ");
    ccsemver_comp_fwrite(&rn.comp, stdout);
    printf("\n");
  }
  ccsemver_range_dtor(&rn);
}

static void
doc_example_ranges_2_2 (void)
/* Reading a complex range. */
{
  printf("--- %s:\n", __func__);
  char const	input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	input  = ccsemver_input_new(input_str, strlen(input_str),  0);
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

static void
doc_example_ranges_3_1 (void)
/* Writing a range, enough room in the buffer. */
{
  printf("--- %s:\n", __func__);
  char const		input_str[] = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	input  = ccsemver_input_new(input_str, strlen(input_str),  0);
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

static void
doc_example_ranges_4_1 (void)
/* Matching, matches. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[] = "1.4.6";
  char const	rn_str[]  = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str, strlen(sv_str),  0);
  ccsemver_input_t	rn_input  = ccsemver_input_new(rn_str, strlen(rn_str),  0);
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

static void
doc_example_ranges_4_2 (void)
/* Matching, does not match. */
{
  printf("--- %s:\n", __func__);
  char const	sv_str[] = "1.2.8";
  char const	rn_str[]  = "1.2.7 || >=1.2.9 <2.0.0";
  ccsemver_input_t	sv_input  = ccsemver_input_new(sv_str, strlen(sv_str),  0);
  ccsemver_input_t	rn_input  = ccsemver_input_new(rn_str, strlen(rn_str),  0);
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


/** --------------------------------------------------------------------
 ** Main.
 ** ----------------------------------------------------------------- */

int
main (void)
{
  ccsemver_init();

  doc_example_identifiers_1_1();
  doc_example_identifiers_1_2();
  doc_example_identifiers_2_1();
  doc_example_identifiers_2_2();
  doc_example_identifiers_2_3();
  doc_example_identifiers_2_4();
  doc_example_identifiers_2_5();
  doc_example_identifiers_3_1();
  doc_example_identifiers_3_2();
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
