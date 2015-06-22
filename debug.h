// $Id: debug.h 65 2015-06-22 10:01:15Z willem_vermin $
#ifndef DEBUG_H
#define DEBUG_H
#define O(x) {cerr << " [" << x << "] ";}
#define Y O(yytext);O(YY_START)
// macro to activate or deactivate debug output:
#ifdef DEBUG
#define D(x) {cerr << "WD: "<< __FILE__ <<":"<<__LINE__<<" "; x; cerr << endl; }
#else
#define D(x)
#endif
#endif

