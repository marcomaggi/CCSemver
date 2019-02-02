## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL \
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README UNLICENSE
AM_CFLAGS		= $(MMUX_CFLAGS) -I$(srcdir)/src

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccsemver.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccsemver.pc


#### documentation

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ccsemver.texi
doc_ccsemver_TEXINFOS	= doc/macros.texi


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
	src/ccsemver.h				\
	src/ccsemver-internals.h		\
	src/ccsemver-version.c			\
	src/ccsemver-conditions.c		\
	src/ccsemver-memory.c			\
	src/ccsemver-input.c			\
	src/ccsemver-semver.c			\
	src/ccsemver-comparators.c		\
	src/ccsemver-identifiers.c		\
	src/ccsemver-numeric-components.c	\
	src/ccsemver-ranges.c			\
	src/ccsemver-utils.c


#### tests

check_PROGRAMS	= \
	tests/version		\
	tests/numbers		\
	tests/identifiers	\
	tests/comparators	\
	tests/documentation	\
	tests/match		\
	tests/ranges		\
	tests/semver		\
	tests/utils

if CCSEMVER_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccsemver_tests_cppflags		= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) $(CCTESTS_CFLAGS)
ccsemver_tests_ldadd		= libccsemver.la $(CCEXCEPTIONS_LIBS) $(CCTESTS_LIBS)

tests_version_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_version_LDADD		= $(ccsemver_tests_ldadd)

tests_numbers_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_numbers_LDADD		= $(ccsemver_tests_ldadd)

tests_identifiers_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_identifiers_LDADD		= $(ccsemver_tests_ldadd)

tests_comparators_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_comparators_LDADD		= $(ccsemver_tests_ldadd)

tests_documentation_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_documentation_LDADD	= $(ccsemver_tests_ldadd)

tests_match_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_match_LDADD		= $(ccsemver_tests_ldadd)

tests_ranges_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_ranges_LDADD		= $(ccsemver_tests_ldadd)

tests_semver_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_semver_LDADD		= $(ccsemver_tests_ldadd)

tests_utils_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_utils_LDADD		= $(ccsemver_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
