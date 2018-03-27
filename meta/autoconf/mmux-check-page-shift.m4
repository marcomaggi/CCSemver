### mmux-check-page-shift.m4 --
#
# MM_CHECK_PAGESHIFT($pagesize)
#
# Determine the number of bits to  right-shift a pointer value to obtain
# the index of  the page (of size  $PAGESIZE) it is in.   Defaults to 12
# which is the value for a page size of 4096.  The test assumes that the
# page size is an exact power of 2.
#
# Set the variable "mmux_cv_pageshift" to the value.
#
AC_DEFUN([MMUX_CHECK_PAGESHIFT],
  [AC_CACHE_CHECK([page shift bit count],
     [mmux_cv_pageshift],
     [AC_RUN_IFELSE([AC_LANG_SOURCE([
        int main (void)
        {
           int count=0;
	   int roller=$1 - 1;
           FILE *f = fopen ("conftest.val", "w");
           while (roller) {
             ++count;
	     roller >>= 1;
           }
           fprintf(f, "%d", count);
           return ferror (f) || fclose (f) != 0;
        }])],
        [mmux_cv_pageshift=`cat conftest.val`],
        [mmux_cv_pageshift=12])
      rm -f conftest.val])])])

### end of file
# Local Variables:
# mode: autoconf
# End:
