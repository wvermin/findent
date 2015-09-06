// $Id: debug.h 71 2015-07-30 11:49:09Z willem_vermin $
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

