// $Id: findent.h 136 2016-11-06 11:12:38Z willem_vermin $
#ifndef FINDENT_H
#define FINDENT_H

#include "line_prep.h"

int yyparse();
void yyerror(const char *c);
extern "C" int yylex();
void lexer_set(class line_prep p);
void lexer_set(std::string s);

std::string lexer_getname();
std::string lexer_getstlabel();
std::string lexer_geti_number();

struct propstruct
{
   int kind;
   std::string name;
   std::string label;
   std::string dolabel;
} ;

propstruct is_it    (class line_prep p, int what);
propstruct parseline(class line_prep p);

void lexer_enable(int k);

#include "debug.h"
#endif

