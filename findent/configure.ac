#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT(findent, 2.8.2, wvermin@gmail.com)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/findent.cpp])
AC_CONFIG_HEADERS([config.h])

AC_SUBST([make_include], ["include"])

AC_ARG_WITH(windows, [AS_HELP_STRING([--with-windows],[Build findent.exe @<:@default=no@:>@])],[],[with_windows=no])

mingw32="i586-mingw32msvc-g++"
AS_IF( [test "x$with_windows" = xyes],[AC_CHECK_PROG([MINGW32],[$mingw32],[$mingw32],[none])]) 

AS_IF( [test "x$MINGW32" = xnone],
      [AC_MSG_ERROR([Not found: $mingw32, set MINGW32 to a working mingw C++ compiler])])

AS_IF( [test "x$with_windows" = xyes],
       [CXX=$MINGW32
        AC_MSG_NOTICE([mingw c++ compiler: $MINGW32])
        AC_MSG_NOTICE([Will create WINDOWS executable findent.exe])])

AM_CONDITIONAL([MAKEMAN],[test "x$with_windows" != "xyes"])
AM_CONDITIONAL([MAKEWIN],[test "x$with_windows" = "xyes"])

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

AC_CONFIG_FILES([test/prelude])
AC_CONFIG_FILES([test/test-compile.sh],[chmod +x test/test-compile.sh])
AC_CONFIG_FILES([test/doit],[chmod +x test/doit])
AC_CONFIG_FILES([test/progfixed-dos.f])
AC_CONFIG_FILES([test/progfixed-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfixed.f])
AC_CONFIG_FILES([test/progfixed.f.try.f.ref])
AC_CONFIG_FILES([test/progfixedfree-dos.f])
AC_CONFIG_FILES([test/progfixedfree-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfixedfree.f])
AC_CONFIG_FILES([test/progfixedfree.f.try.f.ref])
AC_CONFIG_FILES([test/progfree-dos.f])
AC_CONFIG_FILES([test/progfree-dos.f.try.f.ref])
AC_CONFIG_FILES([test/progfree.f])
AC_CONFIG_FILES([test/progfree.f.try.f.ref])
AC_CONFIG_FILES([test/progfree1.f])
AC_CONFIG_FILES([test/progfree1.f.try.f.ref])
AC_CONFIG_FILES([test/progfixed1.f])
AC_CONFIG_FILES([test/progfixed1.f.try.f.ref])
AC_CONFIG_FILES([test/manpage.ref])
AC_CONFIG_FILES([test/help.ref])
AC_CONFIG_FILES([test/vim_fortran.ref])
AC_CONFIG_FILES([test/vim_findent.ref])
AC_CONFIG_FILES([test/gedit_help.ref])
AC_CONFIG_FILES([test/vim_help.ref])
AC_CONFIG_FILES([test/gedit_external.ref])
AC_CONFIG_FILES([test/gedit_plugin.ref])
AC_CONFIG_FILES([test/gedit_plugin_py.ref])
AC_CONFIG_FILES([test/emacs_help.ref])
AC_CONFIG_FILES([test/emacs_findent.ref])
AC_CONFIG_FILES([test/readme.ref])

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_SED

AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([iostream string stack vector cstdio queue algorithm sstream],[],AC_MSG_ERROR([Exiting])) 

AC_OUTPUT(Makefile src/Makefile scripts/Makefile test/Makefile vim/Makefile gedit/Makefile emacs/Makefile doc/Makefile)
