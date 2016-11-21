// $Id: debug.h 136 2016-11-06 11:12:38Z willem_vermin $
#ifndef DEBUG_H
#define DEBUG_H
#define O(x) {std::cerr << " [" << x << "] ";}
#define Y O(yytext);O(YY_START)
// macro to activate or deactivate debug output:
#ifdef DEBUG
#define D(x) {std::cerr << "WD: "<< __FILE__ <<":"<<__LINE__<<" "; x; std::cerr << std::endl; }
#else
#define D(x)
#endif
#endif

