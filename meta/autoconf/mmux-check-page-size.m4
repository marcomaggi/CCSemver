### mm-check-page-size.m4 --
#
# Inspect   the  system   for   the  page   size.    Set  the   variable
# "mmux_cv_pagesize" to the value.
#
AC_DEFUN([MMUX_CHECK_PAGESIZE],
  [AC_CACHE_CHECK([page size],
     [mmux_cv_pagesize],
     [AC_COMPUTE_INT([mmux_cv_pagesize],[sysconf(_SC_PAGESIZE)],[
       #include <unistd.h>
     ],[AS_VAR_SET(mmux_cv_pagesize,[4096])])])])

### end of file
# Local Variables:
# mode: autoconf
# End:
