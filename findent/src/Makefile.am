AM_LDFLAGS =

PARREPORTFILE  = parser_report.txt

PARYFILE   = parser.y
PARCFILE   = parser.cpp
PARHFILE   = parser.h
PARCFILES  = $(PARCFILE) $(PARHFILE)

LEXLFILE   = lexer.l
LEXCFILE   = lexer.cpp
LEXCFILES  = $(LEXCFILE)

$(PARHFILE): $(PARCFILE)

$(PARCFILE): $(PARYFILE)
	$(YACC) -o $@ --report=all --report-file=$(PARREPORTFILE) --defines=$(PARHFILE) $<

$(LEXCFILES): $(LEXLFILE)
	$(LEX) -i -o $@ $<

MOSTLYCLEANFILES =

CLEANFILES = $(LEXCFILES) $(PARCFILES) $(PARREPORTFILE) findent findent.exe test/*.try.f test/a.out test/*.mod findent.1

DISTCLEANFILES = 

MAINTAINERCLEANFILES = $(CLEANFILES)

if MAKEMAN

man1_MANS = findent.1

findent.1: findent
	./findent -H > $@

endif

findent.o: parser.h

bin_PROGRAMS    = findent
findent_SOURCES = findent.cpp lexer.cpp parser.cpp line_prep.cpp

TESTS = test.sh
