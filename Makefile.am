## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL \
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README UNLICENSE
AM_CPPFLAGS		= -I$(srcdir)/src
AM_CFLAGS		= $(MMUX_CFLAGS)
CLEANFILES		=

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/ccsemver.pc


#### documentation

EXTRA_DIST		+= doc/cclibraries.style.css
CLEANFILES		+= doc/cclibraries.css

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 --css-ref=cclibraries.css \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ccsemver.texi
doc_ccsemver_TEXINFOS	= \
	doc/macros.texi			\
	doc/ccexceptions-macros.texi	\
	doc/overview-linking.texi	\
	doc/overview-ccnames.texi	\
	doc/overview-ccexceptions.texi	\
	doc/overview-ccmemory.texi

## --------------------------------------------------------------------

doc/cctemplate.html/$(am__dirstamp): doc/$(am__dirstamp)
	@$(MKDIR_P) doc/cctemplate.html/
	@: > doc/cctemplate.html/$(am__dirstamp)

doc/cctemplate.html/cclibraries.css: doc/cclibraries.style.css doc/cctemplate.html/$(am__dirstamp)
	$(INSTALL) -m 0444 "$(top_srcdir)/doc/cclibraries.style.css" "$(builddir)/doc/cctemplate.html/cclibraries.css"

## --------------------------------------------------------------------

html-local: doc/cctemplate.html/cclibraries.css

install-html-local:
	$(MKDIR_P) "$(DESTDIR)$(htmldir)/cctemplate.html/"
	$(INSTALL) -m 0444 $(builddir)/doc/cctemplate.html/cclibraries.css "$(DESTDIR)$(htmldir)/cctemplate.html/"


#### libraries

ccsemver_CURRENT	= @ccsemver_VERSION_INTERFACE_CURRENT@
ccsemver_REVISION	= @ccsemver_VERSION_INTERFACE_REVISION@
ccsemver_AGE		= @ccsemver_VERSION_INTERFACE_AGE@
ccsemver_LINKER_VERSION	= -version-info $(ccsemver_CURRENT):$(ccsemver_REVISION):$(ccsemver_AGE)

include_HEADERS		= \
	src/ccsemver.h			\
	src/ccsemver-conditions.h	\
	src/ccsemver-semver.h

lib_LTLIBRARIES		= libccsemver.la
libccsemver_la_CPPFLAGS	= $(AM_CPPFLAGS)
libccsemver_la_CFLAGS	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
libccsemver_la_LDFLAGS	= $(ccsemver_LINKER_VERSION) $(AM_LDFLAGS) $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS)
libccsemver_la_SOURCES	= \
	src/ccsemver.h				\
	src/ccsemver-conditions.h		\
	src/ccsemver-semver.h			\
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

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccsemver_tests_cppflags		= $(AM_CPPFLAGS)
ccsemver_tests_cflags		= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCMEMORY_CFLAGS) $(CCSTRUCTS_CFLAGS)
ccsemver_tests_ldadd		= $(AM_LDFLAGS) libccsemver.la $(CCEXCEPTIONS_LIBS) $(CCMEMORY_LIBS) $(CCSTRUCTS_LIBS) $(CCTESTS_LIBS)

## --------------------------------------------------------------------

tests_version_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_version_CFLAGS		= $(ccsemver_tests_cflags)
tests_version_LDADD		= $(ccsemver_tests_ldadd)

tests_numbers_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_numbers_CFLAGS		= $(ccsemver_tests_cflags)
tests_numbers_LDADD		= $(ccsemver_tests_ldadd)

tests_identifiers_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_identifiers_CFLAGS	= $(ccsemver_tests_cflags)
tests_identifiers_LDADD		= $(ccsemver_tests_ldadd)

tests_comparators_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_comparators_CFLAGS	= $(ccsemver_tests_cflags)
tests_comparators_LDADD		= $(ccsemver_tests_ldadd)

tests_documentation_CPPFLAGS	= $(ccsemver_tests_cppflags)
tests_documentation_CFLAGS	= $(ccsemver_tests_cflags)
tests_documentation_LDADD	= $(ccsemver_tests_ldadd)

tests_match_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_match_CFLAGS		= $(ccsemver_tests_cflags)
tests_match_LDADD		= $(ccsemver_tests_ldadd)

tests_ranges_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_ranges_CFLAGS		= $(ccsemver_tests_cflags)
tests_ranges_LDADD		= $(ccsemver_tests_ldadd)

tests_semver_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_semver_CFLAGS		= $(ccsemver_tests_cflags)
tests_semver_LDADD		= $(ccsemver_tests_ldadd)

tests_utils_CPPFLAGS		= $(ccsemver_tests_cppflags)
tests_utils_CFLAGS		= $(ccsemver_tests_cflags)
tests_utils_LDADD		= $(ccsemver_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
