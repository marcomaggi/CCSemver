## mmux-lang-c11.m4 --
#
# Define the appropriate flags to  use the C11 standard language.  Store
# the appropriate compiler flags into the variable AX_CFLAGS.
#
# If the  C compiler is  GCC: set  the variable AX_CFLAGS  to additional
# warning flags.
#
# This macro depends upon the macros:
#
#       AX_CHECK_COMPILE_FLAG
#       AX_GCC_VERSION
#
# This macro is meant to be used as:
#
#       AC_LANG([C])
#       MMUX_LANG_C11
#       AC_SUBST(AX_CFLAGS)
#

AC_DEFUN([MMUX_LANG_C11],[
  AC_REQUIRE([AX_CHECK_COMPILE_FLAG])
  AC_REQUIRE([AX_GCC_VERSION])
  AC_PROG_CC_C99
  AX_CHECK_COMPILE_FLAG([-std=c11],[],[AC_MSG_ERROR([*** Compiler does not support -std=c11])],[-pedantic])

  AS_VAR_SET(AX_CFLAGS,[-std=c11])
  AS_VAR_APPEND(AX_CFLAGS,[" -Wall -Wextra"])
  AX_GCC_VERSION
  AS_IF([test "x$GCC" = "xyes"],
    [AS_CASE("$GCC_VERSION",
       [7.*],[AS_VAR_APPEND(AX_CFLAGS,[" -pedantic -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2 -Wmisleading-indentation"])],
       [8.*],[AS_VAR_APPEND(AX_CFLAGS,[" -pedantic -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2 -Wmisleading-indentation"])])])
  ])


### end of file
# Local Variables:
# mode: autoconf
# End:
