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

#include <string>
#include <iostream>
#include <sstream>
#include "functions.h"

std::string stoupper(const std::string &s)
{
   std::string sl = s;
   int l     = sl.size();
   for (int i=0; i<l; i++)
      sl[i] = toupper(sl[i]);
   return sl;
}

std::string stolower(const std::string &s)
{
   std::string sl = s;
   int l     = sl.size();
   for (int i=0; i<l; i++)
      sl[i] = tolower(sl[i]);
   return sl;
}

std::string remove_blanks(const std::string &s)
{
   std::string sl = "";
   size_t l = s.length();
   for (size_t i=0; i<l; i++)
   {
      switch (s[i])
      {
	 case ' ':
	 case '\t':
	    break;
	 default:
	    sl = sl + s[i];
      }
   }
   return sl;
}

bool cleanfive(const std::string &s)
   //
   // returns 1, if columns 1-5 contain only [0-9 \t]
   // else returns 0
   //
{
   int l = std::min((int)s.size(), 5);
   for (int i=0; i<l; i++)
   {
      char c = s[i];
      if (c == '\t')
	 return 1;
      if (c == ' ' || (c >='0' && c <='9'))
	 continue;
      return 0;
   }
   return 1;
}

bool isalnumplus(const char c)
   //
   // returns true if c is alfanum or in " \t_"
   //
{
   if (std::isalnum(c))
      return 1;
   else 
      switch (c)
      {
	 case ' ':
	 case '\t':
	 case '_':
	    return 1;
      }
   return 0;
}

bool isfixedcmtp(const std::string &s)
{
   //
   // returns 1 if this is a fixed empty line or fixed comment line or preprocessor line
   //                                         or debug line ('d' or 'D' in column 1)
   //
   if (s == "" || trim(s) == "")
      return 1;
   char c = firstchar(s);
   char cts = firstchar(trim(s));
   return (cts == 0 || c == 'C' || c == 'c' || cts == '!' || c == '*' \
	 || cts == '#' || c == 'd' || c == 'D' || trim(s).substr(0,2) == "??"); 
}

int num_leading_spaces(const std::string &s)
{
   size_t p = s.find_first_not_of(' ');
   if (p == std::string::npos)
      return s.size();
   return p;
}

std::string leading_blanks(const std::string &s)
{
   std::string r;
   size_t p = 0;
   while(p < s.size() && (s[p] == ' ' || s[p] == '\t'))
      r += s[p++];
   return r;
}

char fixedmissingquote(const std::string &s)
{
   //
   // investigate if this line terminates a string
   // returns ' ' if there is no unterminated string
   // returns '"' if this is an unterminated string, a '"' is missing
   // returns '\'' if this is an unterminated string, a '\'' is missing
   //
   bool instring = 0;
   char q = ' ';
   for(unsigned int i=0; i<s.size(); i++)
   {
      if (instring)
      {
	 if (s[i] == q)
	    instring = 0;
      }
      else
      {
	 switch(s[i])
	 {
	    case '"': case '\'':
	       instring = 1;
	       q = s[i];
	 }
      }
   }
   if (instring)
      return q;
   return ' ';
}

std::string ltab2sp(const std::string &s)
{
   //
   // converts leading white space and white space after a statement label
   //   to spaces and removes trailing white space
   // if line starts with 0-5 spaces or digits followed by a tab, followed
   //   by 1-9, this is counted as 5 spaces, it appears that many compilers
   //   assume that 
   //   <tab>1  <some code>
   //   is a continuation statement, if the continuation character is 1-9
   //   If the character is not within 1-9, it is assumed that 
   //   this character is the first of a statement, so in this case
   //   this is counted as 6 spaces
   //

   const std::string d1_9  =  "123456789";
   const std::string db0_9 = " 0123456789";
   size_t p = s.substr(0,6).find_first_of('\t');
   if (p == std::string::npos)
      return rtrim(s);                  // no tab in first 6 columns

   if (s.find_first_not_of(db0_9) == p) //  all space or numeric before tab? 
   {
      if (s.length() == p+1)                                   // ' 100 T'  ->' 100', ' T'-> ''
	 return rtrim(s.substr(0,p));
      if (d1_9.find(s[p+1]) == std::string::npos)              // ' 10 Tx'  -> ' 10   x'
	 return rtrim(s.substr(0,p)+blanks(6-p)+s.substr(p+1));
      return    rtrim(s.substr(0,p)+blanks(5-p)+s.substr(p+1));// ' 10 T1x' -> ' 10  1x'
   }
   else
      return rtrim(s);
}

std::string remove_trailing_comment(const std::string &s,const char prevquote)
{
   //
   // removes trailing comment, but only if not part of an
   // unterminated string.
   // e.g:
   // 'print *, " Hi! and goodbye'
   // becomes:
   // 'print *, " Hi! and goodbye'
   // but
   // 'print *, " Hi! and goodbye" ! say goodbye'
   // becomes
   // 'print *, " Hi! and goodbye" '
   //

   std::string so = s;
   bool instring = prevquote != ' ';
   char q        = prevquote;
   for (unsigned int i=0; i<s.size(); i++)
   {
      if (instring)
      {
	 if(so[i] == q)
	    instring = 0;
      }
      else
      {
	 switch(so[i])
	 {
	    case '"': case '\'':
	       instring = 1;
	       q = so[i];
	       break;
	    case '!':
	       so.erase(i,std::string::npos);
	       return so;
	 }
      }
   }
   return so;
}              // end of remove_trailing_comment

// get integer (l) from string (s). n will become the number 
// of characters used
// examples:
//     "0123456789"
// s   " 1 2 3   "  : l: 123, n:9
// s   " 1 2 3   p" : l: 123, n:9
//
void getint_sp(const std::string &s, unsigned long int &l, unsigned int &n)
{
   l = 0;
   n = 0;
   bool r = 0;
   for (size_t i = 0; i<s.length(); i++)
   {
      char c = s.at(i);
      switch (c)
      {
	 case ' ': case '\t':
	    n++;
	    break;
	 case '0': case '1': case '2': case '3': case '4': 
	 case '5': case '6': case '7': case '8': case '9': 
	    l = l*10 + c - '0';
	    n++;
	    break;
	 default:
	    r = 1;
	    break;
      }
      if(r)
	 break;
   }
} // end of getint_sp

// given s, l will contain digits from s, ignoring space
// n will contain number of characters used from s
//    s        l     n
// " 3 6 " -> "36"  5
// "36p"   -> "36"  2
//
void getstr_sp(const std::string &s, std::string &l, unsigned int &n)
{
   l              = "";
   n              = 0;
   bool r         = false;
   bool firstzero = false;
   for (size_t i = 0; i<s.length(); i++)
   {
      char c = s.at(i);
      switch (c)
      {
	 case ' ': case '\t':
	    n++;
	    break;
	 case '0': case '1': case '2': case '3': case '4': 
	 case '5': case '6': case '7': case '8': case '9': 
	    if (c == '0')
	    {
	       if(l.size() == 0)
	       {
		  n++;
		  firstzero = true;
		  break;
	       }
	    }
	    l += c;
	    n++;
	    break;
	 default:
	    r = 1;
	    break;
      }
      if(r)
	 break;
   }
   if (firstzero && l.size() == 0)
      l = "0";
} // end of getstr_sp

