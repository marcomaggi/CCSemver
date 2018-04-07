## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL \
			  meta/slackstuff/config.in	\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README UNLICENSE
AM_CFLAGS		= -I$(srcdir)/src $(AX_CFLAGS)

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccsemver.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccsemver.pc

## --------------------------------------------------------------------

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/ccsemver.texi
doc_ccsemver_TEXINFOS	= doc/macros.texi

#page
#### libraries

ccsemver_CURRENT	= @ccsemver_VERSION_INTERFACE_CURRENT@
ccsemver_REVISION	= @ccsemver_VERSION_INTERFACE_REVISION@
ccsemver_AGE		= @ccsemver_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ccsemver.h

lib_LTLIBRARIES		= libccsemver.la
libccsemver_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS)
libccsemver_la_LDFLAGS	= -version-info $(ccsemver_CURRENT):$(ccsemver_REVISION):$(ccsemver_AGE) \
			  $(CCEXCEPTIONS_LIBS)
libccsemver_la_SOURCES	= \
	src/ccsemver.h			\
	src/ccsemver-internals.h	\
	src/ccsemver-version.c		\
	src/ccsemver-conditions.c	\
	src/ccsemver-memory.c		\
	src/ccsemver-input.c		\
	src/ccsemver-core.c		\
	src/ccsemver-comp.c		\
	src/ccsemver-id.c		\
	src/ccsemver-num.c		\
	src/ccsemver-range.c		\
	src/ccsemver-utils.c

#page
#### tests

check_PROGRAMS	= \
	tests/version		\
	tests/comp		\
	tests/documentation	\
	tests/match		\
	tests/range		\
	tests/semver		\
	tests/utils

if CCSEMVER_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccsemver_tests_cppflags		= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) $(CCTESTS_CFLAGS)
ccsemver_tests_ldadd		= libccsemver.la $(CCEXCEPTIONS_LIBS) $(CCTESTS_LIBS)

tests_version_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_version_LDADD		= $(ccsemver_tests_ldadd)

tests_comp_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_comp_LDADD		= $(ccsemver_tests_ldadd)

tests_documentation_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_documentation_LDADD	= $(ccsemver_tests_ldadd)

tests_match_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_match_LDADD		= $(ccsemver_tests_ldadd)

tests_range_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_range_LDADD		= $(ccsemver_tests_ldadd)

tests_semver_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_semver_LDADD		= $(ccsemver_tests_ldadd)

tests_utils_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_utils_LDADD		= $(ccsemver_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

#page
#### testing under GDB

$(builddir)/.gdbinit:
	ln -sv "$(top_srcdir)/gdbinit" "$(builddir)/.gdbinit"

# This rule is meant to be used as:
#
#   $ make all gdb-test FILE=tests/inspection CFLAGS='-g'
#
# after having created an appropriate file "$(top_srcdir)/.gdbinit".
#
gdb-test: $(builddir)/.gdbinit
	libtool --mode=execute gdb $$FILE

#page
#### Static analysis with Clang's Static Analyzer
#
# See the documentation for the command line tool at:
#
#    <http://clang-analyzer.llvm.org/scan-build.html>
#
# To run the tool we must do:
#
#    $ make clean
#    $ make clang-static-analysis
#
# The  program  "scan-build"  works  by  overriding  the  CC  and  CXX
# environment variables.  Automake defines CXX as:
#
#    CXX = g++ -std=c++11
#
# so we override CXXFLAGS to include the language specification.
#

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make CXXFLAGS='-std=c++11'

### end of file
