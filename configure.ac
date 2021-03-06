# @configure_input@
#

AC_PREREQ([2.69])
MMUX_PKG_VERSIONS([0],[1],[0],[devel.0])
AC_INIT([CCSemver],[MMUX_PACKAGE_VERSION],
  [marco.maggi-ipsu@poste.it],
  [ccsemver],[http://github.com/marcomaggi/ccsemver])
MMUX_INIT
MMUX_LIBTOOL_LIBRARY_VERSIONS([ccsemver],1,0,0)
AC_REVISION([0.2])
AC_COPYRIGHT([This is free and unencumbered software released into the public domain.

Anyone  is  free  to  copy,  modify, publish,  use,  compile,  sell,  or
distribute this  software, either in source  code form or as  a compiled
binary, for any purpose, commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of
this software dedicate any and all copyright interest in the software to
the public domain. We make this dedication for the benefit of the public
at large  and to the  detriment of our  heirs and successors.  We intend
this dedication  to be an overt  act of relinquishment in  perpetuity of
all present and future rights to this software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR   IMPLIED,  INCLUDING   BUT  NOT   LIMITED  TO   THE  WARRANTIES   OF
MERCHANTABILITY, FITNESS  FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION  OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT  OF OR  IN CONNECTION WITH  THE SOFTWARE OR  THE USE  OR OTHER
DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([meta/autotools])
AC_CONFIG_AUX_DIR([meta/autotools])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.16 foreign subdir-objects])
AM_MAINTAINER_MODE

AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P

LT_PREREQ([2.4])
LT_INIT


#### basic system inspection

MMUX_CHECK_TARGET_OS
AX_IS_RELEASE([git-directory])

AC_LANG([C])
MMUX_LANG_C11

AM_PROG_AS
AM_PROG_CC_C_O
AC_HEADER_STDC
AC_HEADER_STDBOOL

CCLIB_CONFIG_SETUP

AC_CACHE_SAVE


#### external libraries

PKG_PREREQ([0.29])
PKG_INSTALLDIR
MMUX_CHECK_PKG_CONFIG_MACROS
PKG_CHECK_MODULES([CCEXCEPTIONS],[ccexceptions >= 0.10.0])
PKG_CHECK_MODULES([CCMEMORY],[ccmemory >= 0.3.0])
PKG_CHECK_MODULES([CCSTRUCTS],[ccstructs >= 0.4.0])
PKG_CHECK_MODULES([CCTESTS],[cctests >= 0.5.0],,[AC_MSG_WARN([package CCTests not found])])

AM_CONDITIONAL([MMUX_TESTING_ENABLED],[test -n "$CCTESTS_LIBS"])

MMUX_PKG_CONFIG_FIND_INCLUDE_FILE([ccexceptions],[ccexceptions.h])
MMUX_PKG_CONFIG_FIND_INCLUDE_FILE([ccmemory],[ccmemory.h])
MMUX_PKG_CONFIG_FIND_INCLUDE_FILE([ccstructs],[ccstructs.h])
MMUX_PKG_CONFIG_FIND_INCLUDE_FILE([ccstructs],[ccnames.h])
MMUX_PKG_CONFIG_FIND_INCLUDE_FILE([cctests],[cctests.h])

AC_CACHE_SAVE


#### finish

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile]
   [meta/scripts/ccsemver.pc]
   [meta/slackware/slack-desc])
MMUX_OUTPUT
AC_OUTPUT

### end of file
