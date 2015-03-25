#ifndef FINDENT_H
#define FINDENT_H
void lexer_init();
int yyparse();
void yyerror(const char *c);
extern "C" int yylex();
void lexer_pop();
void lexer_push(const std::string s, int start);
void lexer_set(const string s, int start);

extern int  lexer_position;
extern bool stlabel_found;

#define O(x) {cerr << " [" << x << "] ";}
#define Y O(yytext)
// macro to activate or deactivate debug output:
//#define D(x) {cerr << "WD: "<< __FILE__ <<":"<<__LINE__<<" "; x; cerr << endl; }
#define D(x) ;
#endif
