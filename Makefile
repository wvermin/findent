# $Id: Makefile 66 2015-06-22 11:44:54Z willem_vermin $
PROG      = findent

ifdef MINGW
   CXX = i586-mingw32msvc-g++
   EXE       = $(PROG).exe
else
   CXX = g++
   EXE       = $(PROG)
endif

CHMOD     = chmod
CP        = cp
JAR       = jar
JAVAC     = javac
LD        = $(CXX)
LEX       = flex
RM        = rm
SED       = sed
YACC      = bison

all:        $(EXE)

jfindent jfindent.jar:  Jfindent.java
	$(JAVAC)        Jfindent.java
	$(JAR)   cfe $@ Jfindent Jfindent*.class
	$(CHMOD) +x  $@
	$(CP)        $@ jfindent.jar

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

$(PARHFILE): $(PARCFILE)

$(PARCFILE): $(PARYFILE)
	$(YACC) -o $@ --report=all --report-file=$(PARREPORTFILE) --defines=$(PARHFILE) $<

$(LEXCFILES): $(LEXLFILE)
	$(LEX) -i -o $@ $<

distclean:
	$(RM) -f $(LEXCFILES) *.$(OBJSUFFIX) $(PARCFILES)
	$(RM) -f $(PARPFILE) $(LEXPFILE) $(PARREPORTFILE)
	$(RM) -f test/*.f.try.f test/a.out test/*.mod
	$(RM) -f *.class

clean: distclean
	$(RM) -f $(PROG) $(EXE) findent.exe jfindent jfindent.jar

test:	all
	cd test; ./tester ../$(EXE)

.SUFFIXES:

.PHONY: test

