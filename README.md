# ccsemver

[![Build Status](https://travis-ci.org/marcomaggi/ccsemver.svg?branch=master)](https://travis-ci.org/marcomaggi/ccsemver)
[![Coverity passed](https://scan.coverity.com/projects/12801/badge.svg)](https://scan.coverity.com/projects/marcomaggi-ccsemver)
[![codecov](https://codecov.io/gh/marcomaggi/ccsemver/branch/master/graph/badge.svg)](https://codecov.io/gh/marcomaggi/ccsemver)


## Introduction

This  package  installs a  C11  language  library implementing  semantic
versioning as described at:

[https://semver.org/](https://semver.org/)

The package is a fork of the project Libsv by Lucas Abel, whose original
source is available at:

[https://github.com/uael/sv](https://github.com/uael/sv)

The  library  targets  POSIX  systems.   This  package  depends  on  the
packages:  CCExceptions,  at least  version  0.9.0;  CCMemory, at  least
version  0.2.2; CCStructs,  at least  version  0.3.0.  To  run the  test
suite: this package  depends upon the package CCTests,  at least version
0.4.1.

The package uses the GNU Autotools and it is tested, using Travis CI, on
both Ubuntu GNU+Linux  systems and OS X systems.  The  package relies on
`pkg-config` to find the dependencies installed on the system.


## License

Copyright (c) 2017, 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it><br/>
Copyright (c) 2017 Lucas Abel <https://github.com/uael/><br/>

This is free and unencumbered software released into the public domain.

Anyone  is  free  to  copy,  modify, publish,  use,  compile,  sell,  or
distribute this  software, either in source  code form or as  a compiled
binary, for any purpose, commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of
this software dedicate any and all copyright interest in the software to
the  public domain.   We make  this dedication  for the  benefit of  the
public at  large and to the  detriment of our heirs  and successors.  We
intend  this  dedication  to  be  an  overt  act  of  relinquishment  in
perpetuity  of all  present and  future  rights to  this software  under
copyright law.

THE SOFTWARE IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR   IMPLIED,  INCLUDING   BUT  NOT   LIMITED  TO   THE  WARRANTIES   OF
MERCHANTABILITY, FITNESS  FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION  OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT  OF OR  IN CONNECTION WITH  THE SOFTWARE OR  THE USE  OR OTHER
DEALINGS IN THE SOFTWARE.

For        more        information,        please        refer        to
[http://unlicense.org](http://unlicense.org).


## Install

To install from a proper release tarball, do this:

```
$ cd ccsemver-0.1.0
$ mkdir build
$ cd build
$ ../configure
$ make
$ make check
$ make install
```

to inspect the available configuration options:

```
$ ../configure --help
```

The Makefile is designed to allow parallel builds, so we can do:

```
$ make -j4 all && make -j4 check
```

which,  on  a  4-core  CPU,   should  speed  up  building  and  checking
significantly.

The Makefile supports the DESTDIR  environment variable to install files
in a temporary location, example: to see what will happen:

```
$ make -n install DESTDIR=/tmp/ccsemver
```

to really do it:

```
$ make install DESTDIR=/tmp/ccsemver
```

After the  installation it is  possible to verify the  installed library
against the test suite with:

```
$ make installcheck
```

From a repository checkout or snapshot  (the ones from the Github site):
we  must install  the GNU  Autotools  (GNU Automake,  GNU Autoconf,  GNU
Libtool), then  we must first run  the script `autogen.sh` from  the top
source directory, to generate the needed files:

```
$ cd ccsemver
$ sh autogen.sh

```

notice  that  `autogen.sh`  will   run  the  programs  `autoreconf`  and
`libtoolize`; the  latter is  selected through the  environment variable
`LIBTOOLIZE`,  whose  value  can  be  customised;  for  example  to  run
`glibtoolize` rather than `libtoolize` we do:

```
$ LIBTOOLIZE=glibtoolize sh autogen.sh
```

After this  the procedure  is the same  as the one  for building  from a
proper release tarball, but we have to enable maintainer mode:

```
$ ../configure --enable-maintainer-mode [options]
$ make
$ make check
$ make install
```

## Usage

Read the documentation generated from  the Texinfo sources.  The package
installs the documentation  in Info format; we can  generate and install
documentation in HTML format by running:

```
$ make html
$ make install-html
```


## Credits

The original  Libsv project was created  by Lucas Abel, with  very small
contributions by Marco Maggi.  The  forked project CCSemver is developed
and maintained  by Marco Maggi; if  this package exists it's  because of
the great GNU software tools that he uses all the time.


## Bugs, vulnerabilities and contributions

Bug  and vulnerability  reports are  appreciated, all  the vulnerability
reports  are  public; register  them  using  the  Issue Tracker  at  the
project's GitHub  site.  For  contributions and  patches please  use the
Pull Requests feature at the project's GitHub site.


## Resources

The latest release of this package can be downloaded from:

[https://bitbucket.org/marcomaggi/ccsemver/downloads](https://bitbucket.org/marcomaggi/ccsemver/downloads)

development takes place at:

[http://github.com/marcomaggi/ccsemver/](http://github.com/marcomaggi/ccsemver/)

and as backup at:

[https://bitbucket.org/marcomaggi/ccsemver/](https://bitbucket.org/marcomaggi/ccsemver/)

the documentation is available online:

[http://marcomaggi.github.io/docs/ccsemver.html](http://marcomaggi.github.io/docs/ccsemver.html)

the GNU Project software can be found here:

[http://www.gnu.org/](http://www.gnu.org/)

the package CCExceptions is available at:

[https://github.com/marcomaggi/ccexceptions/](https://github.com/marcomaggi/ccexceptions/)

the package CCMemory is available at:

[https://github.com/marcomaggi/ccmemory/](https://github.com/marcomaggi/ccmemory/)

the package CCStructs is available at:

[https://github.com/marcomaggi/ccstructs/](https://github.com/marcomaggi/ccstructs/)

the package CCTests is available at:

[https://github.com/marcomaggi/cctests/](https://github.com/marcomaggi/cctests/)

the original project is available at:

[https://github.com/uael/sv](https://github.com/uael/sv)


## Badges and static analysis

### Travis CI

Travis CI is  a hosted, distributed continuous  integration service used
to build and test software projects  hosted at GitHub.  We can find this
project's dashboard at:

[https://travis-ci.org/marcomaggi/ccsemver](https://travis-ci.org/marcomaggi/ccsemver)

Usage of this  service is configured through the  file `.travis.yml` and
additional scripts are under the directory `meta/travis-ci`.


### Coverity Scan

Coverity Scan is  a service providing the results of  static analysis on
open source coding projects.  We can find this project's dashboard at:

[https://scan.coverity.com/projects/marcomaggi-ccsemver](https://scan.coverity.com/projects/marcomaggi-ccsemver)

Usage of this  service is implemented with make rules;  see the relevant
section in the file `Makefile.am`.  To access the service a unique token
is needed: this token is private and is owned by the current maintainer.


### Clang's Static Analyzer

The Clang Static Analyzer is a source code analysis tool that finds bugs
in C, C++, and Objective-C programs.  It is distributed along with Clang
and we can find it at:

[http://clang-analyzer.llvm.org/](http://clang-analyzer.llvm.org/)

Usage of this  service is implemented with make rules;  see the relevant
section in the file `Makefile.am`.


### Codecov

Codecov is a service providing code  coverage reports.  We can find this
project's dashboard at:

[https://codecov.io/gh/marcomaggi/ccsemver](https://codecov.io/gh/marcomaggi/ccsemver)

Usage of  this service is  implemented through direct  interface between
GitHub and Codecov  sites; it configured through  the file `codecov.yml`
and appropriate entries in Travis CI's matrix of builds.

### Codacy

Codacy is  an online service  providing code  review.  We can  find this
project's dashboard at:

[https://www.codacy.com/app/marcomaggi/ccsemver](https://www.codacy.com/app/marcomaggi/ccsemver)

Usage of this service is  implemented through direct integration between
GitHub and Codacy sites.
