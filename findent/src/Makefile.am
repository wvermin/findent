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
 
AM_LDFLAGS =

if MAKEWIN
AM_CXXFLAGS = -static
else
   AM_CXXFLAGS = -fPIC
endif

if USEESOPE
AM_CXXFLAGS += -DUSEESOPE
endif

if USEBISON
parser.hpp parser.cpp: $(top_srcdir)/src/parser.y
	$(YACC) -o parser.cpp -d $(top_srcdir)/src/parser.y
	if test -f parser.cpp.h ; then mv parser.cpp.h parser.hpp ; fi
	if test -f parser.h ; then mv parser.h parser.hpp ; fi

builtparser: parser.cpp
	mv parser.cpp builtparser.cpp
	mv parser.hpp builtparser.hpp

else

parser.cpp: builtparser.cpp 
	@echo ""
	@echo "********************************************************"
	@echo "*** At configuration time, bison (or yacc) was not   ***"
	@echo "*** available, or it was specified not to use bison. ***"
	@echo "*** I will use the pre-built parser.                 ***"
	@echo "********************************************************"
	cp builtparser.cpp $@

parser.hpp: builtparser.hpp
	cp builtparser.hpp $@
endif

if USEFLEX
mylexer.$(OBJEXT): lexer.cpp

lexer.cpp: lexer.l parser.hpp
	$(LEX) -i -o lexer.cpp $(top_srcdir)/src/lexer.l

builtlexer: lexer.cpp
	mv lexer.cpp builtlexer.cpp

else

lexer.cpp: builtlexer.cpp
	@echo ""
	@echo "*******************************************************"
	@echo "*** At configuration time, flex (or lex) was not    ***"
	@echo "*** available, or it was specified not to use flex. ***"
	@echo "*** I will use the pre-built lexer.                 ***"
	@echo "*******************************************************"
	cp builtlexer.cpp $@
endif

MOSTLYCLEANFILES =

CLEANFILES = findent findent.exe findent.1 \
	     parser.cpp.h parser.hpp parser_report.txt \
	     parser.cpp lexer.cpp env.sh \
	     $(INCS)


MAINTAINERCLEANFILES = builtparser.cpp builtparser.hpp builtlexer.cpp

if MAKEMAN

man1_MANS = findent.1

findent.1: findent
	: | ./findent -H > $@

endif

findent.$(OBJEXT): parser.hpp

vim_plugin.$(OBJEXT): vim_fortran.inc vim_findent.inc vim_help.inc


gedit_plugin.$(OBJEXT): gedit_external.inc gedit_plugin.inc gedit_plugin_py.inc \
   gedit_help.inc

emacs_plugin.$(OBJEXT): emacs_findent.inc emacs_help.inc

readme_plugin.$(OBJEXT): readme.inc

docs.$(OBJEXT):    vim_fortran.inc vim_findent.inc vim_help.inc \
   gedit_external.inc gedit_plugin.inc gedit_plugin_py.inc gedit_help.inc \
   emacs_findent.inc emacs_help.inc \
   readme.inc makefdeps.inc changelog.inc

DEPSDIR   = $(top_srcdir)/deps
EMACSDIR  = $(top_srcdir)/emacs
GEDITDIR  = $(top_srcdir)/gedit
READMEDIR = $(top_srcdir)/doc
TOCPP     = $(top_srcdir)/src/tocpp.sh
VIMDIR    = $(top_srcdir)/vim

vim_fortran.inc: $(VIMDIR)/fortran.vim $(TOCPP)
	$(TOCPP) < $(VIMDIR)/fortran.vim > $@

vim_findent.inc: $(VIMDIR)/findent.vim $(TOCPP)
	$(TOCPP) < $(VIMDIR)/findent.vim > $@

vim_help.inc: $(VIMDIR)/README $(TOCPP)
	$(TOCPP) < $(VIMDIR)/README > $@

gedit_external.inc: $(GEDITDIR)/findent-gedit $(TOCPP)
	$(TOCPP) < $(GEDITDIR)/findent-gedit > $@

gedit_plugin.inc: $(GEDITDIR)/findent.plugin $(TOCPP)
	$(TOCPP) < $(GEDITDIR)/findent.plugin > $@

gedit_plugin_py.inc: $(GEDITDIR)/findent.py $(TOCPP)
	$(TOCPP) < $(GEDITDIR)/findent.py > $@

gedit_help.inc: $(GEDITDIR)/README $(TOCPP)
	$(TOCPP) < $(GEDITDIR)/README > $@

emacs_findent.inc: $(EMACSDIR)/findent.el $(TOCPP)
	$(TOCPP) < $(EMACSDIR)/findent.el > $@

emacs_help.inc: $(EMACSDIR)/README $(TOCPP)
	$(TOCPP) < $(EMACSDIR)/README > $@

readme.inc: $(READMEDIR)/README $(TOCPP)
	$(TOCPP) < $(READMEDIR)/README > $@

makefdeps.inc: $(DEPSDIR)/makefdeps $(TOCPP)
	$(TOCPP) < $(DEPSDIR)/makefdeps > $@

changelog.inc: $(top_srcdir)/ChangeLog $(TOCPP)
	$(TOCPP) < $(top_srcdir)/ChangeLog > $@

INCS = vim_fortran.inc vim_findent.inc vim_help.inc gedit_external.inc gedit_plugin.inc \
       gedit_plugin_py.inc gedit_help.inc emacs_findent.inc emacs_help.inc readme.inc makefdeps.inc \
       changelog.inc

bin_PROGRAMS    = findent

findent_SOURCES = \
		  debug.cpp              debug.h           \
		  docs.cpp               docs.h            \
		  findent.cpp            findent.h         \
		  findent_types.h   \
		  findentclass.cpp       findentclass.h    \
		  findentrun.cpp         findentrun.h      \
		  fixed.cpp              fixed.h           \
		  flags.cpp              flags.h           \
		  fortran.cpp            fortran.h         \
		  fortranline.cpp        fortranline.h     \
		  free.cpp               free.h            \
		  functions.cpp          functions.h       \
		  globals.cpp            globals.h         \
		  lexer.h           \
		  line_prep.cpp          line_prep.h       \
		  mylexer.cpp                              \
		  myparser.cpp                             \
		  parser.hpp        \
		  pre_analyzer.cpp       pre_analyzer.h    \
		  prop.cpp               prop.h            \
		  relabel.cpp            relabel.h         \
		  simpleostream.h        simpleostream.cpp \
		  version.h


BUILT_SOURCES = parser.cpp parser.hpp lexer.cpp env.sh

# env.sh is used in the test directory

env.sh:
	> $@
	echo export SRCDIR=\"$(top_srcdir)\" >> $@
	echo export FC=\"$(FC)\" >> $@
if MAKEWIN
	echo export WINDOWS=\"yes\" >> $@
else
	echo export WINDOWS=\"no\" >> $@
endif

if USEESOPE
	echo export ESOPE=\"yes\" >> $@
else
	echo export ESOPE=\"no\" >> $@
endif

EXTRA_DIST  = lexer.l parser.y tocpp.sh \
	      builtparser.cpp builtparser.hpp builtlexer.cpp
