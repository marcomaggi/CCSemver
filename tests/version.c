/*
  Part of: CCSemver
  Contents: test for version functions
  Date: Mar 27, 2018

  Abstract

	Test file for version functions.

  Copyright (C) 2018, 2020 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the UNLICENSE file.
*/

#include <ccsemver.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  ccsemver_library_init();

  printf("version number string: %s\n", ccsemver_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ccsemver_version_interface_current(),
	 ccsemver_version_interface_revision(),
	 ccsemver_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
