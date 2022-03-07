/* -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021,2022 Willem Vermin wvermin@gmail.com
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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>
#include <iostream>
#include <sstream>
#include "simpleostream.h"

inline std::string blanks(const int x) 
{ 
   if (x > 0)
      return std::string((size_t) x ,' '); 
   else
      return "";
}

inline std::string ltrim(const std::string &str)
{
   const std::string whitespace = " \t";
   size_t strBegin = str.find_first_not_of(whitespace);
   if (strBegin == std::string::npos)
      return ""; // no content

   return str.substr(strBegin);
}

inline std::string rtrim(const std::string &str)
{
   const std::string whitespace = " \t";

   size_t strEnd = str.find_last_not_of(whitespace);
   if (strEnd == std::string::npos)
      return ""; // no content
   return str.substr(0,strEnd+1);
}

inline std::string trim(const std::string &str)
{
   const std::string whitespace = " \t";
   size_t strBegin = str.find_first_not_of(whitespace);
   if (strBegin == std::string::npos)
      return ""; // no content

   size_t strEnd = str.find_last_not_of(whitespace);
   size_t strRange = strEnd - strBegin + 1;

   return str.substr(strBegin, strRange);
}

inline char firstchar(const std::string &s)
{
   if (s.length() == 0)
      return 0;
   return s[0];
}

inline char lastchar(const std::string &s)
{
   size_t l = s.length();
   if (l == 0)
      return 0;
   return s[l-1];
}

inline bool myisalnum(char c)
{
   return(isalnum(c) || c == '_');
}

extern Simpleostream mycout; 

   template <typename T>
std::string number2string ( T number )
{
   std::ostringstream ss;
   ss << number;
   return ss.str();
}

   template <typename T>
T string2number ( const std::string &text )
{
   std::istringstream ss(text);
   T result;
   return ss >> result ? result : 0;
}

bool        cleanfive(const std::string &s);

char        firstchar(const std::string &s);

char        fixedmissingquote(const std::string &s);

bool        isalnumplus(const char c);

bool        isfixedcmtp(const std::string &s);

char        lastchar(const std::string &s);

std::string ltab2sp(const std::string& s);

int         num_leading_spaces(const std::string &s);

std::string leading_blanks(const std::string &s);

std::string remove_blanks(const std::string &s);

std::string remove_trailing_comment(const std::string &s, const char prevquote = ' ');

std::string stolower(const std::string &s);

std::string stoupper(const std::string &s);

void getint_sp(const std::string &s, unsigned long int &l, unsigned int &n);

void getstr_sp(const std::string &s, std::string &l, unsigned int &n);

#endif
