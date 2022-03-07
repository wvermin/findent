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

#ifndef FORTRANLINE_H
#define FORTRANLINE_H

#include <deque>

#include "functions.h"
#include "lexer.h"
#include "parser.hpp"
#include "globals.h"
#include "debug.h"

class Fortranline
{
   //
   // NOTE:
   // some functions assume that clean() is called
   //
   std::string orig_line;
   std::string orig_without_omp;
   //
   // Some functions are used repeatedly, the results are
   // cached here
   //
   char        firstchar_cache;    bool firstchar_cached;
   std::string ltrim_cache;        bool ltrim_cached;
   bool        omp_cache;          bool omp_cached;
   int         scanfixpre_cache;   bool scanfixpre_cached;
   std::string trim_cache;         bool trim_cached;
   std::string incfile_cache;      bool Written;

   bool     is_clean;
   Globals* gl;
   int      local_format;
   bool     local_gnu_format;
   int      Preregion;


   void init(void)
   {
      local_format      = gl->global_format;
      local_gnu_format  = gl->global_gnu_format;

      is_clean          = false;

      firstchar_cached  = false;
      ltrim_cached      = false;
      omp_cached        = false;
      scanfixpre_cached = false;
      trim_cached       = false;
   }

   void init(Globals* g)
   {
      gl = g;
      init();
   }

   void do_clean(void);
   bool do_omp(void);

   public:

   void print(void);

   void clean(void)
   {
      if (is_clean)
	 return;
      else
	 do_clean();
   }

   void clean(bool force)
   {
      if (force)
	 do_clean();
      else
	 clean();
   }

   Fortranline()
   {
      Written  = false;
      Preregion = 0;
   }

   Fortranline(Globals* g)
   {
      Written  = false;
      Preregion = 0;
      init(g);
   }

   Fortranline(Globals*g, const std::string &s)
   {
      Written  = false;
      Preregion = 0;
      init(g);
      orig_line = s;
   }

   void written(bool w)
   {
      Written = w;
   }

   bool written(void)
   {
      return Written;
   }

   void preregion(int p)
   {
      Preregion = p;
   }

   int preregion(void)
   {
      return Preregion;
   }

   std::string g_format2txt(void)
   {
      switch(g_format())
      {
	 case UNKNOWN:
	    return "unknown";
	 case FIXED:
	    return "fixed";
	 case FREE:
	    return "free";
	 default:
	    return "";
      }
   }

   void g_format(const int what)    { gl->global_format = what; }

   int g_format(void)               { return gl->global_format; }

   void line_length(const int what) { gl->global_line_length=what; }

   int line_length(void)            { return gl->global_line_length; }

   void gnu_format(bool what)       { gl->global_gnu_format=what; }

   bool gnu_format(void)            { return gl->global_gnu_format; }

   std::string str(void)            { return orig_line; }

   std::string strnomp(void)      
   {
      clean();
      return orig_without_omp; 
   }

   friend std::ostream& operator <<(std::ostream &os,Fortranline &obj);

   void str(const std::string &s)
   {
      orig_line = s;
      init();
   }

   int format(void) const
   {
      if (local_format == UNKNOWN)
	 return gl->global_format;
      return local_format;
   }


   std::string trimmed_line(void)
   {
      //
      // result is different for FIXED or FREE, see below:
      //
      switch(format())
      {
	 case FIXED:
	    return ::rtrim(orig_line);
	    break;
	 case FREE:
	 default:
	    if (omp())
	       return ::trim(orig_without_omp);
	    else
	       return ::trim(orig_line);
	    break;
      }
   }

   std::string rtrim(void) 
   {
      if (omp())
	 return ::rtrim(orig_without_omp);
      else
	 return ::rtrim(orig_line);
   }

   std::string ltrim(void)
   {
      if (!ltrim_cached)
      {
	 if (omp())
	    ltrim_cache = ::ltrim(orig_without_omp);
	 else
	    ltrim_cache = ::ltrim(orig_line);
	 ltrim_cached = 1;
      }
      return ltrim_cache;
   }

   std::string trim(void) 
   {
      if (!trim_cached)
      {
	 if (omp())
	    trim_cache = ::trim(orig_without_omp);
	 else
	    trim_cache = ::trim(orig_line);
	 trim_cached = 1;
      }
      return trim_cache;
   }

   char firstchar(void) 
   {
      // returns first char of ltrim(), 0 if length()=0
      if (!firstchar_cached)
      {
	 if (ltrim().length()>0)
	    firstchar_cache = ltrim()[0];
	 else
	    firstchar_cache = 0;
	 firstchar_cached = 1;
      }
      return firstchar_cache;
   }

   char operator [] (int i) const
   {
      //returns character in column k of the original line
      if(orig_line.length() > (unsigned) i)
	 return orig_line[i];
      else
	 return 0;
   }

   char lastchar(void) const
   {
      if (orig_line.length() > 0)
	 return *orig_line.rbegin();
      else
	 return 0;
   }

   std::string first2chars() { return ltrim().substr(0,2); }

   int scanfixpre(void)
   {
      if(!scanfixpre_cached)
      {
	 lexer_set(trim(),SCANFIXPRE);
	 scanfixpre_cache = yylex();
	 incfile_cache    = lexer_getinclude();
	 if (format() == FIXED)
	    if(scanfixpre_cache == FIXFINDENTFIX)
	       scanfixpre_cache = FINDENTFIX;
	 scanfixpre_cached = 1;
      }
      return scanfixpre_cache;
   }

   std::string getinclude()
   {
      if(scanfixpre_cache)
	 return incfile_cache;
      else
	 return "";
   }

   bool omp(void)
   {
      if (!omp_cached)
      {
	 omp_cache = do_omp();
	 omp_cached = 1;
      }
      return omp_cache;
   }

   std::string rest(void) 
   {
      if(scanfixpre()==FINDENTFIX)
	 return lexer_getrest();
      else
	 return "";
   }

   bool blank(void) { return (trim().length() == 0); }

   bool comment(void) 
   {
      switch (format())
      {
	 case FIXED:
	    if (omp())
	    {
	       return firstchar() == '!';
	    }
	    switch(::firstchar(orig_line))
	    {
	       case 'd':
	       case 'D':
	       case 'c':
	       case 'C':
	       case '!':
	       case '*':
		  return 1;
	    }
	    return firstchar() == '!';
	    break;

	 case FREE:
	    return firstchar() == '!';
	    break;
      }
      return 0;
   }

   bool blank_or_comment(void) { return blank() || comment(); }

   int getpregentype(void) 
   {
      int  pretype = scanfixpre();
      switch(pretype)
      {
	 case CPP_IF: case CPP_ENDIF: case CPP_ELSE: case CPP_ELIF: case CPP: 
	    return CPP;
	    break;
	 case COCO_IF: case COCO_ENDIF: case COCO_ELSE: case COCO_ELIF: case COCO: 
	    return COCO;
	    break;
	 case INCLUDE_CPP: case INCLUDE_CPP_STD: case INCLUDE_COCO:
	    return pretype;
	    break;
	 default:
	    return 0;
      }
   }

   bool precpp(void)                  { return firstchar() == '#'; }

   bool precoco(void)                 { return first2chars() == "??"; }

   bool pre(void)                     { return precpp() || precoco(); }

   bool blank_or_comment_or_pre(void) { return blank_or_comment() || pre(); }

   bool fortran(void)                 { return !blank_or_comment_or_pre(); }

   bool fixedcontinuation(void) const
   {
      //
      // returns 1 if the line, interpreted as a fixed format line,
      // contains a character other than 0 or ' ' in column 5
      //
      const std::string c = " 0";
      return c.find((*this)[5]) == std::string::npos;
   }

   std::string remove_trailing_comment(const char prevquote = ' ')
   {
      return ::rtrim(::remove_trailing_comment(orig_without_omp,prevquote));
   }

};

std::ostream& operator <<(std::ostream &os,Fortranline &obj);

typedef std::deque<Fortranline> lines_t;

typedef std::deque<Fortranline> linebuffer_t;

#endif
