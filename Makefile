PROG      = findent

ifdef MINGW
   CXX = i586-mingw32msvc-g++
   EXE       = $(PROG).exe
else
   CXX = g++
   EXE       = $(PROG)
endif

all:        $(EXE)

LD        = $(CXX)
YACC      = bison
LEX       = flex
SED       = sed
RM        = rm

ifdef DEBUG
   CXXFLAGS = -Wall -g -O0 -DDEBUG
else
   CXXFLAGS = -Wall -O3 
endif

ifdef STATIC
   CXXFLAGS += -static
   LDFLAGS   = -static
endif

PARNAME   = parser
LEXNAME   = lexer

PARREPORTFILE  = $(PARNAME)_report.txt

CPLUSSUFFIX  = cpp
BISONSUFFIX  = y
FLEXSUFFIX   = l
OBJSUFFIX    = o

PARYFILE   = $(PARNAME).$(BISONSUFFIX)
PARCFILE   = $(PARNAME).$(CPLUSSUFFIX)
PARHFILE   = $(PARNAME).h
PARCFILES  = $(PARCFILE) $(PARHFILE)
PAROBJ     = $(PARNAME).$(OBJSUFFIX)

LEXLFILE   = $(LEXNAME).$(FLEXSUFFIX)
LEXCFILE   = $(LEXNAME).$(CPLUSSUFFIX)
LEXCFILES  = $(LEXCFILE)
LEXOBJ     = $(LEXNAME).$(OBJSUFFIX)

FINDENTOBJS = $(PROG).$(OBJSUFFIX) line_prep.$(OBJSUFFIX)

EXEOBJS     = $(PAROBJ) $(LEXOBJ) $(FINDENTOBJS)

INCLUDES    = findent.h $(PARHFILE) debug.h version.h line_prep.h

%.$(OBJSUFFIX):    %.$(CPLUSSUFFIX) $(INCLUDES) 
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(EXE): $(EXEOBJS)
	$(LD) $(LDFLAGS) -o $@ $(EXEOBJS)

$(PARCFILES): $(PARYFILE)
	$(YACC) -o $@ --report=all --report-file=$(PARREPORTFILE) --defines=$(PARHFILE) $<

$(LEXCFILES): $(LEXLFILE)
	$(LEX) -i -o $@ $<

distclean:
	$(RM) -f $(LEXCFILES) *.$(OBJSUFFIX) $(PARCFILES)
	$(RM) -f $(PARPFILE) $(LEXPFILE) $(PARREPORTFILE)
	$(RM) -f test/*.f.try.f test/a.out test/*.mod

clean: distclean
	$(RM) -f $(PROG) $(EXE) findent.exe

test:	all
	cd test; ./tester ../$(EXE)

.SUFFIXES:

.PHONY: test

