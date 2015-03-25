PROG      = findent

ifdef MINGW
   CPP       = i586-mingw32msvc-g++
   EXE       = $(PROG).exe
else
   CPP       = g++
   EXE       = $(PROG)
endif

all:        $(EXE)

CPPFLAGS  = -Wall -g -O2 
INCLUDES  = $(PROG).h $(PROG)_parser.h version.h
LD        = $(CPP)
YACC      = bison
LEX       = flex

$(EXE): $(PROG)_parser.o $(PROG)_lexer.o
	$(LD) -o $@ $(PROG)_parser.o $(PROG)_lexer.o
	
$(PROG)_parser.cpp $(PROG)_parser.h: $(PROG).y
	$(YACC) -o $(PROG)_parser.cpp --defines=$(PROG)_parser.h $<

$(PROG)_lexer.cpp: $(PROG).l
	$(LEX) -i -o $@ $<

%.o:     %.cpp $(INCLUDES) 
	$(CPP) -c $(CPPFLAGS) -o $@ $<

clean:
	rm -f $(PROG)_lexer.cpp *.o $(PROG) $(EXE) findent.exe $(PROG)_parser.h $(PROG)_parser.cpp
	rm -f test/*.f.try.f test/a.out test/*.mod

test:   all
	cd test; ./tester ../$(EXE)

.SUFFIXES:

.PHONY: test
