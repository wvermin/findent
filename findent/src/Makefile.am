AM_LDFLAGS =

PARFILES      =      parser.cpp      parser.h
BUILDPARFILES = buildparser.cpp buildparser.h

LEXFILES      =      lexer.cpp
BUILDLEXFILES = buildlexer.cpp

AM_CXXFLAGS = -fPIC

if USEBISON
parser.cpp parser.h: parser.y
	$(YACC) -o parser.cpp -d $<
	if test -f parser.cpp.h ; then mv parser.cpp.h parser.h ; fi
	if test -f parser.hpp ; then mv parser.hpp parser.h ; fi
	if test -f buildparser.cpp.in ; \
	   then  cp parser.cpp buildparser.cpp.in ; fi
	if test -f buildparser.h.in ; \
	   then cp parser.h buildparser.h.in ; fi
else
dummyparser: parser.y
	@echo ""
	@echo "********************************************************"
	@echo "*** At configuration time, bison (or yacc) was not   ***"
	@echo "*** available, or it was specified not to use bison. ***"
	@echo "*** I will use the pre-build parser.                 ***"
	@echo "********************************************************"
	touch $@

parser.cpp: buildparser.cpp dummyparser 
	cp $< $@

parser.h: buildparser.h dummyparser
	cp $< $@
endif

if USEFLEX
lexer.cpp: lexer.l
	$(LEX) -i -o $@ $<
	if test -f buildlexer.cpp.in ; \
	   then cp lexer.cpp buildlexer.cpp.in ; fi
else
dummylexer: lexer.l
	@echo ""
	@echo "*******************************************************"
	@echo "*** At configuration time, flex (or lex) was not    ***"
	@echo "*** available, or it was specified not to use flex. ***"
	@echo "*** I will use the pre-build lexer.                 ***"
	@echo "*******************************************************"
	touch $@

lexer.cpp: buildlexer.cpp dummylexer
	cp $< $@
endif

MOSTLYCLEANFILES =

CLEANFILES = $(LEXFILES) $(PARFILES) findent findent.exe findent.1 parser.cpp.h parser.hpp parser_report.txt *.inc


DISTCLEANFILES = dummylexer dummyparser

MAINTAINERCLEANFILES = $(BUILDPARFILES) $(BUILDLEXFILES)

if MAKEMAN

man1_MANS = findent.1

findent.1: findent
	./findent -H > $@

endif

findent.o: parser.h

vim_plugin.o: vim_fortran.inc vim_findent.inc vim_help.inc

gedit_plugin.o: gedit_external.inc gedit_plugin.inc gedit_plugin_py.inc \
   gedit_help.inc

TOCPP    = $(top_srcdir)/src/tocpp.sh
VIMDIR   = $(top_srcdir)/vim
GEDITDIR = $(top_srcdir)/gedit

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

bin_PROGRAMS    = findent

findent_SOURCES = findent.cpp lexer.cpp parser.cpp \
		  line_prep.cpp pre_analyzer.cpp debug.h \
		  line_prep.h pre_analyzer.h version.h \
		  findent.h simpleostream.h \
		  vim_plugin.cpp vim_plugin.h \
		  gedit_plugin.cpp gedit_plugin.h 

EXTRA_DIST  = lexer.l parser.y tocpp.sh
