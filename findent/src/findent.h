// $Id: findent.h 71 2015-07-30 11:49:09Z willem_vermin $
#ifndef FINDENT_H
#define FINDENT_H

using namespace std;

#include "line_prep.h"

int yyparse();
void yyerror(const char *c);
extern "C" int yylex();
void lexer_set(class line_prep p);
void lexer_set(string s);

string lexer_getname();
string lexer_getstlabel();
string lexer_geti_number();

struct propstruct
{
   int kind;
   string name;
   string label;
   string dolabel;
} ;

propstruct is_it    (class line_prep p, int what);
propstruct parseline(class line_prep p);

void lexer_enable(int k);

#include "debug.h"
#endif

