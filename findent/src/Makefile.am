AM_LDFLAGS =

PARFILES      =      parser.cpp      parser.h
BUILDPARFILES = buildparser.cpp buildparser.h

LEXFILES      =      lexer.cpp
BUILDLEXFILES = buildlexer.cpp

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

CLEANFILES = $(LEXFILES) $(PARFILES) findent findent.exe test/*.try.f test/a.out test/*.mod findent.1 parser.cpp.h parser.hpp parser_report.txt


DISTCLEANFILES = dummylexer dummyparser

MAINTAINERCLEANFILES = $(BUILDPARFILES) $(BUILDLEXFILES)

if MAKEMAN

man1_MANS = findent.1

findent.1: findent
	./findent -H > $@

endif

findent.o: parser.h

bin_PROGRAMS    = findent
findent_SOURCES = findent.cpp lexer.cpp parser.cpp \
		  line_prep.cpp pre_analyzer.cpp debug.h \
		  line_prep.h pre_analyzer.h version.h \
		  findent.h simpleostream.h

EXTRA_DIST  = lexer.l parser.y
