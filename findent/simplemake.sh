#!/bin/sh
# -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021 Willem Vermin wvermin@gmail.com
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
#
# this script tries to compile findent, without using the
# configure ; make ; make install 
# suite.
#
# definitions:
CPP='g++'               # C++ compiler
CPPFLAGS='-O2'          # compile flags
EXE='findent'           # name of program to create
#
echo "C++ compiler:  $CPP"
echo "compile flags: $CPPFLAGS"
echo "program:       $EXE"
SRCS="
debug.cpp
docs.cpp
findentclass.cpp
findent.cpp
findentrun.cpp
fixed.cpp
flags.cpp
fortran.cpp
fortranline.cpp
free.cpp
functions.cpp
globals.cpp
line_prep.cpp
mylexer.cpp
myparser.cpp
pre_analyzer.cpp
prop.cpp
relabel.cpp
simpleostream.cpp
"
echo "creating include files ..."
echo vim_help.inc
src/tocpp.sh < vim/README > src/vim_help.inc || exit 1
echo vim_findent.inc
src/tocpp.sh < vim/findent.vim > src/vim_findent.inc || exit 1
echo vim_fortran.inc
src/tocpp.sh < vim/fortran.vim > src/vim_fortran.inc || exit 1
echo gedit_help.inc
src/tocpp.sh < gedit/README > src/gedit_help.inc || exit 1
echo gedit_external.inc
src/tocpp.sh < gedit/findent-gedit > src/gedit_external.inc || exit 1
echo gedit_plugin.inc
src/tocpp.sh < gedit/findent.plugin > src/gedit_plugin.inc || exit 1
echo gedit_plugin_py.inc
src/tocpp.sh < gedit/findent.py > src/gedit_plugin_py.inc || exit 1
echo emacs_help.inc
src/tocpp.sh < emacs/README > src/emacs_help.inc || exit 1
echo emacs_findent.inc
src/tocpp.sh < emacs/findent.el > src/emacs_findent.inc || exit 1
echo readme.inc
src/tocpp.sh < doc/README > src/readme.inc || exit 1
echo makefdeps.inc
src/tocpp.sh < deps/makefdeps > src/makefdeps.inc || exit 1
echo changelog.inc
src/tocpp.sh < ChangeLog > src/changelog.inc || exit 1


echo "compiling, this can take some time ..."
cd src                         || exit 1
cp builtparser.hpp parser.hpp  || exit 1
cp builtparser.cpp parser.cpp  || exit 1
cp builtlexer.cpp lexer.cpp    || exit 1
$CPP $CPPFLAGS -o $EXE $SRCS   || exit 1
cd ..
cp src/$EXE .                  || exit 1
echo "succesfully created: $EXE"
echo "copy this file to the desired directory, for example"
echo "  cp $EXE \$HOME/bin"
echo "or"
echo "  cp $EXE \$HOME/.local/bin"
echo "or"
echo "  cp $EXE /usr/local/bin"
echo
echo "The man page can be obtained by running:"
echo "  findent --manpage > findent.1"
echo "copy the man page to a suitable location, for example:"
echo "  cp findent.1 /usr/local/share/man/man1/findent.1"
echo
echo "To install wfindent:"
echo "  Edit the file scripts/wfindent.tmpl"
echo "    Replace the string 'findent-location' (around line 43)"
echo "    by the location of findent, e.g. '/usr/local/bin/findent'"
echo "  Copy scripts/wfindent.tmpl to the desired directory, for example"
echo "    cp scripts/wfindent.tmpl /usr/local/bin/wfindent"
echo "To install the man page of wfindent:"
echo "  cp scripts/wfindent.1 /usr/local/share/man/man1/wfindent.1"
echo
