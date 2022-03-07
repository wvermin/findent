# -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021,2022 Willem Vermin wvermin@gmail.com
#-# 
#-# License: BSD-3-Clause
#-#  Redistribution and use in source and binary forms, with or without
#-#  modification, are permitted provided that the following conditions
#-#  are met:
#-#  1. Redistributions of source code must retain the above copyright
#-#     notice, this list of conditions and the following disclaimer.
#-#  2. Redistributions in binary form must reproduce the above copyright
#-#     notice, this list of conditions and the following disclaimer in the
#-#     documentation and/or other materials provided with the distribution.
#-#  3. Neither the name of the copyright holder nor the names of its
#-#     contributors may be used to endorse or promote products derived
#-#     from this software without specific prior written permission.
#-#   
#-#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#-#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#-#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#-#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
#-#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#-#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#-#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#-#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#-#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#-#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#-#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([findent], [4.2.0], [wvermin@gmail.com])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/findent.cpp])
AC_CONFIG_HEADERS([config.h])

AC_SUBST([make_include], ["include"])

AC_ARG_WITH(windows, [AS_HELP_STRING([--with-windows],[Build findent.exe @<:@default=no@:>@])],[],[with_windows=no])

mingw32="i686-w64-mingw32-g++"
AS_IF( [test "x$with_windows" = xyes],[AC_CHECK_PROG([MINGW32],[$mingw32],[$mingw32],[none])]) 

AS_IF( [test "x$MINGW32" = xnone],
      [AC_MSG_ERROR([Not found: $mingw32, set MINGW32 to a working mingw C++ compiler])])

AS_IF( [test "x$with_windows" = xyes],
       [CXX=$MINGW32
        AC_MSG_NOTICE([mingw c++ compiler: $MINGW32])
        AC_MSG_NOTICE([Will create WINDOWS executable findent.exe])
	AC_SUBST([EXEEXT],[.exe])])

AM_CONDITIONAL([MAKEMAN],[test "x$with_windows" != "xyes"])
AM_CONDITIONAL([MAKEWIN],[test "x$with_windows" = "xyes"])

AC_ARG_WITH(esope, [AS_HELP_STRING([--with-esope],[Build with esope support @<:@default=no@:>@])],[],[with_esope=no])
AM_CONDITIONAL([USEESOPE],[test "x$with_esope" = "xyes"])

# test if bison available etc...

AC_ARG_WITH(bison, [AS_HELP_STRING([--with-bison],[Use bison to create parser @<:@default=use-bison-if-available@:>@])],[],[with_bison=use-bison-if-available])

usebison="$with_bison"

AS_IF( [test "x$with_bison" != "xno"],
      [ m4_include([m4/ax_prog_bison.m4]) ] )

AS_IF( [test "x$with_bison" = "xyes"],
       [AX_PROG_BISON([],[AC_MSG_ERROR([No valid bison found. Try --without-bison])])])

AS_IF( [test "x$with_bison" = "xuse-bison-if-available"],
       [AX_PROG_BISON(AC_MSG_NOTICE([Will use bison if necessary to create parser.]) 
	[usebison="yes"],
        AC_MSG_NOTICE([No bison found. Will use pre-built parser.])
	[usebison="no"])])

AM_CONDITIONAL([USEBISON],[test "x$usebison" = "xyes"])

# test if flex available etc...

AC_ARG_VAR([LEX],[The 'Fast lexical analyser generator' to use])

AC_ARG_WITH(flex, [AS_HELP_STRING([--with-flex],[Use flex to create lexer @<:@default=use-flex-if-available@:>@])],[],[with_flex=use-flex-if-available])

useflex="$with_flex"

AS_IF( [test "x$with_flex" != "xno"],
    [ m4_include([m4/ax_prog_flex.m4]) ] )

AS_IF( [test "x$with_flex" = "xyes"],
       [AX_PROG_FLEX([],[AC_MSG_ERROR([No valid flex found. Try --without-flex])])])

AS_IF( [test "x$with_flex" = "xuse-flex-if-available"],
       [AX_PROG_FLEX(AC_MSG_NOTICE([Will use flex if necessary to create lexer.]) 
	[useflex="yes"],
        AC_MSG_NOTICE([No flex found. Will use pre-built lexer.])
	[useflex="no"])])

AM_CONDITIONAL([USEFLEX],[test "x$useflex" = "xyes"])


# test for working gfortran (only used in make check):

AC_PROG_FC([gfortran])
AS_IF( [test "x$FC" = "xgfortran"],
      [AC_MSG_NOTICE([gfortran will be used to compile test sources in 'make check'])],
      [AC_MSG_NOTICE([No gfortran compiler found. 'make check' will not try to compile test sources])])


AC_CONFIG_FILES([test/prelude])
AC_CONFIG_FILES([test/postlude])
AC_CONFIG_FILES([test/test-compile.sh],[chmod +x test/test-compile.sh])
AC_CONFIG_FILES([test/doit],[chmod +x test/doit])
AC_CONFIG_FILES([test/init.sh],[chmod +x test/init.sh])

AC_CONFIG_FILES([test/progfixed-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfixed-dos.f])
AC_CONFIG_FILES([test/progfixed.f.try.f.ref])
AC_CONFIG_FILES([test/progfixed.f])
AC_CONFIG_FILES([test/progfixed1.f.try.f.ref])
AC_CONFIG_FILES([test/progfixed1.f])
AC_CONFIG_FILES([test/progfixedfree-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfixedfree-dos.f])
AC_CONFIG_FILES([test/progfixedfree.f.try.f.ref])
AC_CONFIG_FILES([test/progfixedfree.f])
AC_CONFIG_FILES([test/progfree-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfree-dos.f])
AC_CONFIG_FILES([test/progfree.f.try.f.ref])
AC_CONFIG_FILES([test/progfree.f])
AC_CONFIG_FILES([test/progfree1.f.try.f.ref])
AC_CONFIG_FILES([test/progfree1.f])
AC_CONFIG_FILES([test/progfreefixed.f.try.f.ref])
AC_CONFIG_FILES([test/progfreefixed.f])
AC_CONFIG_FILES([test/esope.f.try.f.ref])
AC_CONFIG_FILES([test/esope.f])
AC_CONFIG_FILES([test/progrelabelfree.f])
AC_CONFIG_FILES([test/progrelabelfree.f.try.f.ref])
AC_CONFIG_FILES([test/progfixedrelabelfixed.f])
AC_CONFIG_FILES([test/progfixedrelabelfixed.f.try.f.ref])

AC_CONFIG_FILES([test/changelog.ref])
AC_CONFIG_FILES([test/emacs_findent.ref])
AC_CONFIG_FILES([test/emacs_help.ref])
AC_CONFIG_FILES([test/gedit_external.ref])
AC_CONFIG_FILES([test/gedit_help.ref])
AC_CONFIG_FILES([test/gedit_plugin.ref])
AC_CONFIG_FILES([test/gedit_plugin_py.ref])
AC_CONFIG_FILES([test/help.ref])
AC_CONFIG_FILES([test/makefdeps.ref])
AC_CONFIG_FILES([test/manpage.ref])
AC_CONFIG_FILES([test/readme.ref])
AC_CONFIG_FILES([test/vim_findent.ref])
AC_CONFIG_FILES([test/vim_fortran.ref])
AC_CONFIG_FILES([test/vim_help.ref])

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_SED

AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([iostream string deque map algorithm sstream iomanip set typeinfo utility],[],AC_MSG_ERROR([Exiting])) 
AC_CHECK_HEADERS([assert.h errno.h getopt.h inttypes.h stdio.h stdlib.h string.h unistd.h],[],AC_MSG_ERROR([Exiting])) 

AC_OUTPUT(Makefile src/Makefile scripts/Makefile test/Makefile vim/Makefile 
	  gedit/Makefile emacs/Makefile doc/Makefile deps/Makefile)
