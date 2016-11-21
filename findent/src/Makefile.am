AM_LDFLAGS =

PARYFILE   = parser.y
PARCFILE   = parser.cpp
PARHFILE   = parser.h
PARCFILES  = $(PARCFILE) $(PARHFILE)

LEXLFILE   = lexer.l
LEXCFILE   = lexer.cpp
LEXCFILES  = $(LEXCFILE)

$(PARHFILE): $(PARCFILE)

$(PARCFILE): $(PARYFILE)
	$(YACC) -o $@ -d $<
	if test -f $(PARCFILE).h ; then mv $(PARCFILE).h $(PARHFILE) ; fi
	if test -f $(PARHFILE)pp ; then mv $(PARHFILE)pp $(PARHFILE) ; fi

$(LEXCFILES): $(LEXLFILE)
	$(LEX) -i -o $@ $<

MOSTLYCLEANFILES =

CLEANFILES = $(LEXCFILES) $(PARCFILES) findent findent.exe test/*.try.f test/a.out test/*.mod findent.1 $(PARCFILE).h $(PARHFILE)pp

DISTCLEANFILES = 

MAINTAINERCLEANFILES = $(CLEANFILES)

if MAKEMAN

man1_MANS = findent.1

findent.1: findent
	./findent -H > $@

endif

findent.o: parser.h

bin_PROGRAMS    = findent
findent_SOURCES = findent.cpp lexer.cpp parser.cpp line_prep.cpp pre_analyzer.cpp
