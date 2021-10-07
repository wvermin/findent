/* -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021 Willem Vermin wvermin@gmail.com
#-# 
#-# License: BSD-3-Clause
#-#  Redistribution and use in source and binary forms, with or without
#-#  modification, are permitted provided that the following conditions
#-#  are met:
#-#  1. Redistributions of source code must retain the above copyright
#-#     notice, this list of conditions and the following disclaimer.
#-#  2. Redistributions in binary form must reproduce the above copyright
#-#     notice, this list of conditions and the following disclaimer in the
#-#     documentation and/or other materials provided with the distribution.
#-#  3. Neither the name of the copyright holder nor the names of its
#-#     contributors may be used to endorse or promote products derived
#-#     from this software without specific prior written permission.
#-#   
#-#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#-#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#-#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#-#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
#-#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#-#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#-#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#-#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#-#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#-#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#-#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG
//#define DEBUG
#endif


#include <iostream>
#include <string>
#include <typeinfo>

#define O(x) {std::cerr << " [" << x << "] ";}
#define Y O(yytext);O(YY_START)
// macro to activate or deactivate debug output:
#ifdef DEBUG
#define D(x) {std::cerr << "WD: "<< __func__<<" "<<__FILE__ <<":"<<__LINE__<<" "; x; std::cerr << std::endl; }
#define DL(x) x
#else
#define D(x)
#define DL(x)
#endif

#define XSTR(x) STR(x)
#define STR(x) #x
#define FL  (std::string(__func__) + (" " __FILE__ ":" XSTR(__LINE__) " "))
//
// ad-hoc debugging
// turn on by  !findentfix:p-on
// turn off by !findentfix:p-off
// or in the code: ppp.on() or ppp.off()
//
// a typical debug line would be:
// ppp<<FL<<full_statement<<endchar;
//

extern  char endchar;

class Debugostream 

{
   private:
      bool output;
   public:
      Debugostream()
      {
	 output = 0;
      }
      void off()
      {
	 output = 0;
      }
      void on()
      {
	 output = 1;
      }
      template< typename T > Debugostream& operator<<( T val )
      { 
	 if (output)
	 {
	    std::string name = typeid(val).name();
	    bool b = (typeid(val) == typeid(std::string));
	    if (name.length() > 2)
		  b = b || (name.substr(name.length()-1)=="c");

	    if (b)
	       std::cout << "[";
	    std::cout << val; 
	    if (b)
	       std::cout << "]";
	 }
	 return *this; 
      }
};

extern Debugostream ppp;
#endif
