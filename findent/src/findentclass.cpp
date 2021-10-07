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

#include <stdio.h>
#include <unistd.h>
#include "findentclass.h"
#include "fortranline.h"
#include "debug.h"

int Findent::determine_fix_or_free(strings_t *input)
{
   int rc;
   int n = 0;
   const int nmax = 4000;
   Fortranline line(gl);
   std::string s;
   bool eof;
   bool p_more = 0;
   bool skip = 0;

   strings_t::iterator it = input->begin();

   D(O(" "););
   while (n < nmax)
   {
      n++;
      if(it == input->end())
      {
	 s = "";
	 eof = 1;
      }
      else
      {
	 s = *it;
	 it++;
	 eof  = 0;
      }
      line = Fortranline(gl,s);
      D(O(' '););
      if (eof)
      {
	 //
	 // to avoid to have to type twice a dot to
	 // end input from terminal:
	 //

	 break;
      }

      handle_pre_light(line,p_more);
      if(p_more)
      {
	 skip = 1;
	 continue;
      }
      if(skip)
      {
	 skip = 0;
	 continue;
      }

      rc = guess_fixedfree(line);
      switch(rc)
      {
	 case UNSURE : 
	    break;
	 case PROBFREE : 
	    break;
	 case FREE : 
	    return FREE;
	 case FIXED : 
	    return FIXED;
      }
   }
   return FIXED;
}          // end of determine_fix_or_free


void Findent::handle_pre_light(Fortranline &line, bool &p_more)
{
   //
   // handles preprocessor lines and their continuations:
   //
   // line (input):   line to handle
   // p_more (inout):   input: if true, line is a continuation
   //                   output: if continuation is expected, p=true

   if (!p_more)   // this is the first line of a preprocessor sequence
      pregentype = line.getpregentype();

   if(pregentype == COCO)
      p_more = (line.lastchar() == '&');
   else
      p_more = (line.lastchar() == '\\');

}         // end of handle_pre_light

int Findent::guess_fixedfree(Fortranline &line)
{
   //
   // sometimes, program sources contain carriage control characters
   // such as ^L
   // I cannot get the lexer to respond to [:cntrl:]
   // so I handle that here:
   //
   std::string s = line.str();

   if (firstchar(s) != '\t')
      if(firstchar(s) < 32)
	 return UNSURE;

   lexer_set(ltab2sp(s),FINDFORMAT);
   int rc = yylex();
   return rc;
}           // end of guess_fixedfree


void Findent::handle_reading_from_tty()
{
   reading_from_tty = isatty(fileno(stdin));
   if (reading_from_tty)
   {
      std::cerr << "! Warning: reading from terminal"                << std::endl;
      std::cerr << "! End this session by typing a single dot ('.')" << std::endl;
      std::cerr << "!     on a new line"                             << std::endl;
      std::cerr << "! "                                              << std::endl;
      std::cerr << "! Examples of typical usage:"                    << std::endl;
      std::cerr << "!   help:    findent -h"                         << std::endl;
      std::cerr << "!   indent:  findent < in.f > out.f"             << std::endl;
      std::cerr << "!   convert: findent -ofree < prog.f > prog.f90" << std::endl;
   }
}                // end of handle_reading_from_tty

int Findent::what_to_return()
{
   if (flags.return_format)
      switch(input_format)
      {
	 case FREE:
	    return 2;
	    break;
	 case FIXED:
	    return 4;
	    break;
      }
   return 0;
}              // end of what_to_return

#if 0
void Findent::init_indent()
{
   //
   // fills the indent-stack until indent 0
   // if flags.all_indent <= 0: build indent_stack with a number of start_indent's
   //
   while(!indent.empty())
      indent.pop_back();
   int l=0;
   if(flags.all_indent > 0)
   {
      for (l = start_indent%flags.all_indent; l<start_indent; l+=flags.all_indent)
      {
	 push_indent(l);
      }
   }
   else
   {
      for (int i=0; i<100; i++)
	 push_indent(start_indent);
   }
   push_indent(start_indent);

}             // end of init_indent
#endif


std::string Findent::handle_dos(const std::string &s)
{
   //
   // determine if the input is dos format:
   // side effect: sets endline if not already been set
   //
   std::string sl;
   sl = s;
   if (!endline_set)
   {
      if (sl != "" && lastchar(sl) == '\r')
      {
	 endline = "\r\n";
      }
      endline_set = 1;
   }
   if (sl != "" && lastchar(sl) =='\r')
      sl.erase(sl.length()-1);
   return sl;
}         // end of handle_dos

#if 0
int Findent::guess_indent(Fortranline line)
{
   //
   // count spaces at start of line, correct for tabs and & and label
   //
   int si         = 0;
   bool ready     = 0;
   const int tabl = 8;

   if (line.format() == FIXED)
   {
      std::string s = line.str();
      si             = s.find_first_not_of(' ') -6;
      if (si <0)
	 si = 0;
      return si;
   }

   std::string s = line.str();
   for (unsigned int j=0; j<s.length(); j++)
   {
      switch (s[j])
      {
	 case ' ' : case '0' : case '1' : case '2' : case '3' : case '4' : 
	 case '5' : case '6' : case '7' : case '8' : case '9' :
	    si ++;
	    break;
	 case '&' :
	    si++;
	    ready = 1;
	    break;
	 case '\t' :
	    si = (si/tabl)*tabl + tabl;
	    break;
	 default:
	    ready = 1;
	    break;
      }
      if(ready)
	 break;
   }
   return si;
}                // end of guess_indent
#endif


std::string Findent::type2str(const int t)
{
   switch (t)
   {
      case USE:             return "use";
      case INCLUDE:         return "inc";
      case INCLUDE_CPP:     return "cpp";
      case INCLUDE_CPP_STD: return "std";
      case INCLUDE_COCO:    return "coc";
      case MODULE:          return "mod";
      case SUBMODULE:       return "sub";
   }
   return "";
}

void Findent::output_deps()
{
   std::set<std::pair<int,std::string> >:: iterator it;
   for (it = includes.begin(); it != includes.end(); ++it)
   {
      // eliminate use if corresponding (sub)module is made
      if (it->first == USE)
      {
	 if (includes.find(make_pair(MODULE,it->second)) != includes.end())
	    continue;
	 if (includes.find(make_pair(SUBMODULE,it->second)) != includes.end())
	    continue;
      }
      std::cout << type2str(it->first) << " " << it->second << std::endl;
   }
}
