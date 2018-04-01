/*
 * This  is free  and  unencumbered software  released  into the  public
 * domain.
 *
 * Anyone  is free  to copy,  modify,  publish, use,  compile, sell,  or
 * distribute this software, either in source code form or as a compiled
 * binary, for  any purpose,  commercial or  non-commercial, and  by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of  this software  dedicate any  and  all copyright  interest in  the
 * software  to the  public  domain.  We make  this  dedication for  the
 * benefit of the public at large and  to the detriment of our heirs and
 * successors.  We  intend  this  dedication  to  be  an  overt  act  of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE  SOFTWARE IS  PROVIDED "AS  IS",  WITHOUT WARRANTY  OF ANY  KIND,
 * EXPRESS OR  IMPLIED, INCLUDING BUT  NOT LIMITED TO THE  WARRANTIES OF
 * MERCHANTABILITY,    FITNESS   FOR    A    PARTICULAR   PURPOSE    AND
 * NONINFRINGEMENT.  IN  NO EVENT  SHALL THE AUTHORS  BE LIABLE  FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR  OTHERWISE, ARISING FROM,  OUT OF  OR IN CONNECTION  WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#include <ccsemver-internals.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _MSC_VER
# define snprintf(s, maxlen, fmt, ...) _snprintf_s(s, _TRUNCATE, maxlen, fmt, __VA_ARGS__)
#endif

static void ccsemver_xrevert(ccsemver_t *semver) {
  if (semver->major == CCSEMVER_NUM_X) {
    semver->major = semver->minor = semver->patch = 0;
  } else if (semver->minor == CCSEMVER_NUM_X) {
    semver->minor = semver->patch = 0;
  } else if (semver->patch == CCSEMVER_NUM_X) {
    semver->patch = 0;
  }
}

static ccsemver_comp_t  *ccsemver_xconvert(ccsemver_comp_t *self) {
  if (self->version.major == CCSEMVER_NUM_X) {
    self->op = CCSEMVER_OP_GE;
    ccsemver_xrevert(&self->version);
    return self;
  }
  if (self->version.minor == CCSEMVER_NUM_X) {
    ccsemver_xrevert(&self->version);
    self->op = CCSEMVER_OP_GE;
    self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (self->next == NULL) {
      return NULL;
    }
    ccsemver_comp_ctor(self->next);
    self->next->op = CCSEMVER_OP_LT;
    self->next->version = self->version;
    ++self->next->version.major;
    return self->next;
  }
  if (self->version.patch == CCSEMVER_NUM_X) {
    ccsemver_xrevert(&self->version);
    self->op = CCSEMVER_OP_GE;
    self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (self->next == NULL) {
      return NULL;
    }
    ccsemver_comp_ctor(self->next);
    self->next->op = CCSEMVER_OP_LT;
    self->next->version = self->version;
    ++self->next->version.minor;
    return self->next;
  }
  self->op = CCSEMVER_OP_EQ;
  return self;
}

static char parse_partial(ccsemver_t *self, const char *str, size_t len, size_t *offset) {
  ccsemver_ctor(self);
  self->major = self->minor = self->patch = CCSEMVER_NUM_X;
  if (*offset < len) {
    self->raw = str + *offset;
    if  (ccsemver_num_read(&self->major, str, len, offset)) {
      return 0;
    }
    if (*offset >= len || str[*offset] != '.') {
      return 0;
    }
    ++*offset;
    if  (ccsemver_num_read(&self->minor, str, len, offset)) {
      return 1;
    }
    if (*offset >= len || str[*offset] != '.') {
      return 0;
    }
    ++*offset;
    if  (ccsemver_num_read(&self->patch, str, len, offset)) {
      return 1;
    }
    if ((str[*offset] == '-' && ccsemver_id_read(&self->prerelease, str, len, (++*offset, offset)))
      || (str[*offset] == '+' && ccsemver_id_read(&self->build, str, len, (++*offset, offset)))) {
      return 1;
    }
    self->len = str + *offset - self->raw;
    return 0;
  }
  return 0;
}

static char parse_hiphen(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset) {
  ccsemver_t partial;

  if (parse_partial(&partial, str, len, offset)) {
    return 1;
  }
  self->op = CCSEMVER_OP_GE;
  ccsemver_xrevert(&self->version);
  self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
  if (self->next == NULL) {
    return 1;
  }
  ccsemver_comp_ctor(self->next);
  self->next->op = CCSEMVER_OP_LT;
  if (partial.minor == CCSEMVER_NUM_X) {
    self->next->version.major = partial.major + 1;
  } else if (partial.patch == CCSEMVER_NUM_X) {
    self->next->version.major = partial.major;
    self->next->version.minor = partial.minor + 1;
  } else {
    self->next->op = CCSEMVER_OP_LE;
    self->next->version = partial;
  }

  return 0;
}

static char parse_tidle(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset) {
  ccsemver_t partial;

  if (parse_partial(&self->version, str, len, offset)) {
    return 1;
  }
  ccsemver_xrevert(&self->version);
  self->op = CCSEMVER_OP_GE;
  partial = self->version;
  if (partial.major != 0) {
    ++partial.major;
    partial.minor = partial.patch = 0;
  } else if (partial.minor != 0) {
    ++partial.minor;
    partial.patch = 0;
  } else {
    ++partial.patch;
  }
  self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
  if (self->next == NULL) {
    return 1;
  }
  ccsemver_comp_ctor(self->next);
  self->next->op = CCSEMVER_OP_LT;
  self->next->version = partial;
  return 0;
}

static char parse_caret(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset) {
  ccsemver_t partial;

  if (parse_partial(&self->version, str, len, offset)) {
    return 1;
  }
  ccsemver_xrevert(&self->version);
  self->op = CCSEMVER_OP_GE;
  partial = self->version;
  if (partial.minor || partial.patch) {
    ++partial.minor;
    partial.patch = 0;
  } else {
    ++partial.major;
    partial.minor = partial.patch = 0;
  }
  self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
  if (self->next == NULL) {
    return 1;
  }
  ccsemver_comp_ctor(self->next);
  self->next->op = CCSEMVER_OP_LT;
  self->next->version = partial;
  return 0;
}

void ccsemver_comp_ctor(ccsemver_comp_t *self) {
#ifndef _MSC_VER
  *self = (ccsemver_comp_t) {0};
#else
  self->next = NULL;
  self->op = CCSEMVER_OP_EQ;
  ccsemver_ctor(&self->version);
#endif
}

void ccsemver_comp_dtor(ccsemver_comp_t *self) {
  if (self && self->next) {
    ccsemver_comp_dtor(self->next);
    free(self->next);
    self->next = NULL;
  }
}

char ccsemver_comp_read(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset) {
  ccsemver_comp_ctor(self);
  while (*offset < len) {
    switch (str[*offset]) {
      case '^':
        ++*offset;
        if (parse_tidle(self, str, len, offset)) {
          return 1;
        }
        self = self->next;
        goto next;
      case '~':
        ++*offset;
        if (parse_caret(self, str, len, offset)) {
          return 1;
        }
        self = self->next;
        goto next;
      case '>':
        ++*offset;
        if (*offset < len && str[*offset] == '=') {
          ++*offset;
          self->op = CCSEMVER_OP_GE;
        } else {
          self->op = CCSEMVER_OP_GT;
        }
        if (parse_partial(&self->version, str, len, offset)) {
          return 1;
        }
        ccsemver_xrevert(&self->version);
        goto next;
      case '<':
        ++*offset;
        if (*offset < len && str[*offset] == '=') {
          ++*offset;
          self->op = CCSEMVER_OP_LE;
        } else {
          self->op = CCSEMVER_OP_LT;
        }
        if (parse_partial(&self->version, str, len, offset)) {
          return 1;
        }
        ccsemver_xrevert(&self->version);
        goto next;
      case '=':
        ++*offset;
        self->op = CCSEMVER_OP_EQ;
        if (parse_partial(&self->version, str, len, offset)) {
          return 1;
        }
        ccsemver_xrevert(&self->version);
        goto next;
      default:
        goto range;
    }
  }
  range:
  if (parse_partial(&self->version, str, len, offset)) {
    return 1;
  }
  if (*offset < len && str[*offset] == ' '
    && *offset + 1 < len && str[*offset + 1] == '-'
    && *offset + 2 < len && str[*offset + 2] == ' ') {
    *offset += 3;
    if (parse_hiphen(self, str, len, offset)) {
      return 1;
    }
    self = self->next;
  } else {
    self = ccsemver_xconvert(self);
    if (self == NULL) {
      return 1;
    }
  }
  next:
  if (*offset < len && str[*offset] == ' '
    && *offset < len + 1 && str[*offset + 1] != ' ' && str[*offset + 1] != '|') {
    ++*offset;
    if (*offset < len) {
      self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
      if (self->next == NULL) {
        return 1;
      }
      return ccsemver_comp_read(self->next, str, len, offset);
    }
    return 1;
  }
  return 0;
}

char ccsemver_and(ccsemver_comp_t *self, const char *str, size_t len) {
  if (len) {
    size_t offset = 0;
    ccsemver_comp_t *tail = NULL;

    if (self->next == NULL) {
      self->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
      if (self->next == NULL) {
        return 1;
      }
      return ccsemver_comp_read(self->next, str, len, &offset);
    }
    tail = self->next;
    while (tail->next) tail = tail->next;
    tail->next = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (tail->next == NULL) {
      return 1;
    }
    return ccsemver_comp_read(tail->next, str, len, &offset);
  }
  return 1;
}

char ccsemver_comp_and(ccsemver_comp_t *self, const char *str, size_t len, size_t *offset) {
  if (0 == len) {
    return 1;
  } else {
    ccsemver_comp_t * newp;

    newp = (ccsemver_comp_t *) malloc(sizeof(ccsemver_comp_t));
    if (NULL == newp) {
      return 1;
    } else {
      char rv = ccsemver_comp_read(newp, str, len, offset);
      if (rv) {
	ccsemver_comp_dtor(newp);
	free(newp);
	return rv;
      }
    }

    if (NULL == self->next) {
      self->next = newp;
    } else {
      ccsemver_comp_t * tailp = self->next;

      while (tailp->next) { tailp = tailp->next; }
      tailp->next = newp;
    }
    return 0;
  }
}

char
ccsemver_match (ccsemver_t const * self, ccsemver_comp_t const *comp)
{
  char result = ccsemver_comp(self, &(comp->version));

  if (result < 0 && comp->op != CCSEMVER_OP_LT && comp->op != CCSEMVER_OP_LE) {
    return 0;
  }
  if (result > 0 && comp->op != CCSEMVER_OP_GT && comp->op != CCSEMVER_OP_GE) {
    return 0;
  }
  if (result == 0 && comp->op != CCSEMVER_OP_EQ && comp->op != CCSEMVER_OP_LE && comp->op != CCSEMVER_OP_GE) {
    return 0;
  }
  if (comp->next) {
    return ccsemver_match(self, comp->next);
  }
  return 1;
}

int
ccsemver_comp_write (ccsemver_comp_t const * self, char *buffer, size_t len)
{
  char *op = "";
  char semver[256], next[1024];

  switch (self->op) {
  case CCSEMVER_OP_EQ:
    break;
  case CCSEMVER_OP_LT:
    op = "<";
    break;
  case CCSEMVER_OP_LE:
    op = "<=";
    break;
  case CCSEMVER_OP_GT:
    op = ">";
    break;
  case CCSEMVER_OP_GE:
    op = ">=";
    break;
  }
  ccsemver_write(&(self->version), semver, 256);
  if (self->next) {
    return snprintf(buffer, len, "%s%s %.*s", op, semver, ccsemver_comp_write(self->next, next, 1024), next);
  }
  return snprintf(buffer, len, "%s%s", op, semver);
}

/* end of file */