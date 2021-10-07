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

#include <algorithm>
#include <sstream>
#include <string>

#include "fixed.h"
#include "debug.h"
#include "findent_types.h"
#include "free.h"
#include "simpleostream.h"

void Fixed::build_statement(Fortranline &line, bool &f_more, bool &pushback)
{
   //
   // adds line to c_lines
   // adds line (stripped from comments, preprocessor stuff and 
   //    continuation stuff)  to full_statement
   // f_more true: more lines are to expected
   //        false: this line is complete
   //

   pushback = 0;
   f_more   = false;

   if (line.blank_or_comment())
   {
      c_lines.push_back(line);

      if (c_lines.size() ==1)
	 f_more = false;   // do not expect continuation lines
      else
	 f_more = true;   // but here we do
      D(O("FULL_fs_return");O(line.str());O(full_statement);O(f_more););
      return;
   }

   std::string s = line.strnomp();
   std::string sl = s.substr(0,5);

   // 
   // special attention for garbage lines:
   //
   if(!cleanfive(s))
   {
      c_lines.push_back(line);
      f_more = true;
      return;
   }

   if (s.length() >6)
      sl = sl+' '+s.substr(6);

   //
   // this is a line with code
   //

   // 
   // check if this is the first line with proper fortran
   //
   bool first_fortran_line = true;
   for(size_t i = 0; i< c_lines.size(); ++i)
      if (c_lines[i].fortran())
      {
	 first_fortran_line = false;
	 break;
      }
   if (first_fortran_line)
   {
      //
      // this is the first line with fortran content
      //
      c_lines.push_back(line);
      full_statement += trim(sl);
      D(O("fs_build");O(full_statement););
      full_statement = rtrim(remove_trailing_comment(full_statement));
      D(O("fs_build");O(full_statement););

      // -------------------------------------
      if(relabeling)
      {
	 size_t i = 0;
	 int    l = 0;
	 //while((line.str()[i] == ' ' || line.str()[i] == '\t') && i < line.str().size())
	 while(i < sl.size() && (sl.at(i) == ' ' || sl.at(i) == '\t'))
	 {
	    i++;
	    l++;
	 }
	 for(size_t i=0; i<full_statement.size(); i++)
	    insert_in_full_pos(c_lines.size()-1,l+i);

      }

      if (!f_more)
	 f_more = wizard();     // is there a continuation line in the future?
      return;
   }

   //
   // this is possibly a continuation line
   //
   if (!line.fixedcontinuation())
   {
      //
      // this is not a continuation line
      // push it back, we will see it later
      //
      pushback = 1;
      f_more   = 0;          // do not look for further continuation lines
      D(O("FULL_fs_return");O(line.str());O(full_statement);O(f_more););
      return;
   }
   //
   // this is a continuation line
   //
   c_lines.push_back(line);
   full_statement += rtrim((rtrim(sl)+"      ").substr(6));
   D(O("fs_build");O(full_statement););
   full_statement  = rtrim(remove_trailing_comment(full_statement));
   D(O("fs_build");O(full_statement););

   // -------------------------------------------------------------

   if(relabeling)
   {
      size_t m = full_pos.size();

      int k = 6;
      for (size_t i = m; i < full_statement.size(); i++)
      {
	 insert_in_full_pos(c_lines.size()-1,k++);
      }
   }


   if(!f_more)
      f_more = wizard();   // look for more continuation lines
   D(O("FULL_fs_return");O(line.str());O(full_statement);O(f_more););
}           // end of build_statement


void Fixed::output(lines_t &lines,lines_t *freelines)
{
   D(O("entering Fixed::output"););
   //
   // output lines input: fixed format, output: fixed format
   //
   // lines optionally start with comments and/or preprocessor lines
   // lines ends with a fortran line
   //
   const std::string continuations = 
      "123456789" 
      "abcdefghijklmnopqrstuvwxyz" 
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   bool to_mycout            = (freelines == 0);
   unsigned int old_indent   = 0;
   unsigned int first_indent = 0;
   char prevquote            = ' ';
   bool had_first            = 0;

   std::ostringstream os;
   size_t cindex = 0;

   while(!lines.empty())
   {
      mycout.reset();

      if(lines.front().written())
      {
	 lines.pop_front();
	 continue;
      }

      if(output_pre(lines,freelines))
	 continue;

      if (lines.empty())
	 return;

      is_omp  = lines.front().omp();

      if(is_omp)
      {
	 ompstr = lines.front().str().substr(0,2);
	 cmpstr = "!$ ";
      }
      else
      {
	 ompstr = "";
	 cmpstr = "";
      }

      if (lines.front().blank())    
      {
	 //
	 // a completely blank line, that is simple:
	 //
	 if(to_mycout)
	    mycout << ompstr << fi->endline;
	 else
	 {
	    os << cmpstr;
	    if(had_first && is_omp)
	       os << " & &";   // to keep gfortran and ifort happy
	    freelines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      if (lines.front().comment())
      {
	 //
	 // output indentation blanks:
	 //
	 if (lines.front().firstchar() == '!')
	 {
	    if (lines.front()[0] != '!' || is_omp)
	       //
	       // take care of the situation that cur_indent == 0
	       // but the comment does not start in column 1
	       //
	    {
	       if(to_mycout)
		  //mycout << insert_omp(blanks(M(std::max(fi->cur_indent+6,1))));
		  mycout << insert_omp(blanks(M(std::max(cur_indent+6,1))));
	       else
	       {
		  os << cmpstr << ' ';
		  if(had_first && is_omp)
		     os << " & &";   // to keep gfortran and ifort happy
	       }
	    }
	 }
	 //
	 // output comment:
	 //
	 if(to_mycout)
	    mycout << lines.front().trim() << fi->endline;
	 else
	 {
	    int l=1;
	    if (!is_omp && toupper(lines.front().firstchar()) == 'D')
	       l = 0;
	    os << "!" << lines.front().trim().substr(l);
	    freelines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      //
      // this is a line with (part of) a fortran statement
      // possibly a continuation line
      //

      std::string s = lines.front().strnomp();

      //
      // garbage in, garbage out:
      //

      if(!cleanfive(s))
      {
	 //
	 // garbage in, garbage out
	 //
	 if(to_mycout)
	    mycout << insert_omp(lines.front().rtrim()) << fi->endline;
	 else
	 {
	    os << cmpstr << lines.front().rtrim();
	    freelines->push_back(F(os.str()));
	    os.str("");
	 }

	 lines.pop_front();
	 continue;
      }

      //
      // this should be a normal line with code, maybe preceded with a label
      //

      if(s.length() > 6)   // line is [label][c]statement
      {
	 bool iscontinuation = lines.front().fixedcontinuation();
	 if (iscontinuation)
	 {
	    if (fi->flags.conchar != ' ')
	    {
	       if (fi->flags.conchar == '0')
	       {
		  s.at(5) = continuations[cindex];
		  cindex++;
		  if (cindex >= continuations.length())
		     cindex = 0;
	       }
	       else
		  s.at(5) = fi->flags.conchar;
	    }
	 }
	 //
	 // output label field including possible continuation character
	 //
	 std::string label;
	 int         nlabel;
	 if(is_omp)
	 {
	    unsigned int n;
	    getstr_sp(s.substr(2,3),label,n);
	    nlabel = 3;
	 }
	 else
	 {
	    unsigned int n;
	    getstr_sp(s.substr(0,5),label,n);
	    nlabel = 5;
	 }
	 if (fi->flags.label_left)
	    label = label + std::string(nlabel-label.size(),' ');
	 else
	    label = std::string(nlabel-label.size(),' ') + label;

	 if(to_mycout)
	    //mycout << insert_omp(s.substr(0,6));
	    mycout << ompstr + label+s[5];
	 else
	    //os << cmpstr << s.substr(0,5);
	    os << cmpstr << label + ' ';

	 //
	 // try to honour current indentation
	 // if this is a continuation line, count the number
	 // of spaces after column 6.
	 //
	 if (iscontinuation)
	 {
	    std::string s6 = s.substr(6)+'x';
	    old_indent     = s6.find_first_not_of(' ');
	    if (!to_mycout)
	       os << '&';
	 }
	 else
	 {
	    //
	    // count the number of spaces after column 6 of the first line
	    //
	    std::string s6 = s.substr(6)+'x';
	    first_indent = s6.find_first_not_of(' ');
	    cindex = 0;
	 }
	 int adjust_indent = old_indent - first_indent;
	 if (adjust_indent < 0)
	    adjust_indent = 0;

	 switch(prevquote)
	 {
	    case ' ' :   // no dangling strings, output with indent
	       if(to_mycout)
		  //mycout << blanks(M(std::max(adjust_indent+fi->cur_indent,0))) 
		  mycout << blanks(M(std::max(adjust_indent+cur_indent,0))) 
		     << trim(s.substr(6));
	       else
		  os << trim(s.substr(6));
	       break;
	    default:  // dangling string, output asis
	       if(to_mycout)
		  mycout << s.substr(6);
	       else
		  os << s.substr(6);
	       break;
	 }
	 //
	 // investigate if this line terminates a string
	 //
	 prevquote = fixedmissingquote(prevquote + s);
	 if(to_mycout)
	    mycout << fi->endline;
	 else
	 {
	    char c;
	    if (wizard(lines))       // more fortran lines follow
	    {
	       c = ' ';
	       if(iscontinuation)
		  c = prevquote;
	       if (is_omp) // an hack
		  freelines->push_back(F('!'+add_amp(os.str().substr(1),c)));
	       else
		  freelines->push_back(F(add_amp(os.str(),c)));
	    }
	    else
	       freelines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 had_first = 1;     // here, the 1st line of a statement has been processed
	 continue;
      }
      //
      // output a line that does not fulfill above conditions
      //
      if(to_mycout)
	 mycout << insert_omp(s) << fi->endline;
      else
      {
	 os << cmpstr << s;
	 if (wizard(lines))       // more fortran lines follow
	 {
	    if (is_omp) // an hack
	       freelines->push_back(F('!'+add_amp(os.str().substr(1),prevquote)));
	    else
	       freelines->push_back(F(add_amp(os.str(),prevquote)));
	 }
	 else
	    freelines->push_back(F(os.str()));
	 os.str("");
      }
      lines.pop_front();
      had_first = 1;     // here, the 1st line of a statement has been processed
   }
}     // end of output


void Fixed::output_converted(lines_t &lines)
{
   D(O("fixed::output_converted "););
   lines_t freelines;

   output(lines, &freelines);

   Globals oldgl = *gl;
   gl->global_format = FREE;
   gl->global_line_length = 0;

   Free f(fi);
   lines_t::iterator it = freelines.begin();
   //
   // clean all freelines:
   //
   while(it != freelines.end()) 
   {
      D(O((*it)););
      it->clean(1);
      it++;
   }
   f.cur_indent  = cur_indent;
   f.labellength = labellength;
   f.output(freelines);
   (*gl) = oldgl;
}    // end of output_converted


bool Fixed::wizard()
{
   //
   // Look ahead to see if the next fixed-format fortran line is a continuation
   // return true if a continuation is found, false otherwize.
   // For free format, always return 0.
   //

   if (gl->global_format == FREE)
      return 0;

   Fortranline line(gl);
   bool eof;

   while(1)
   {
      line = getnext(eof,0);
      if (eof)
	 return 0;


      if (line.pre())
      {
	 bool p_more = 0;
	 while(1)
	 {
	    fi->handle_pre_light(line,p_more);
	    if (p_more)
	    {
	       line = getnext(eof,0);
	       if (eof)
		  return 0;
	    }
	    else
	       break;
	 }
	 if(eof)
	    return 0;
	 continue;
      }


      if (line.fortran() && cleanfive(line.strnomp()))
      {
	 //
	 // return 1 if this is a fixed fortran continuation line
	 //

	 return line.fixedcontinuation();
      }
   }
   return 0;
}     // end of wizard


bool Fixed::wizard(lines_t lines)
{
   // finds out if lines.front() gets a continuation line

   if(lines.empty())
      return 0;

   lines_t::iterator it = lines.begin();
   it++;
   while(it != lines.end())
   {
      if (it-> fortran() && cleanfive(it->strnomp()))
      {
	 D(O(it->str());O(it->fixedcontinuation()););
	 return it->fixedcontinuation();
      }
      it++;
   }
   return 0;
}

std::string Fixed::add_amp(const std::string &s,const char p)
{
   //
   // examples:
   // s = "abc ! xyz",  p = ' '   :  result = "abc& ! xyz"
   // s = "ab'c ! xyz", p = ' '   :  result = "ab'c ! xyz&"
   // s = "abc ! xyz",  p = '\''  :  result = "abc ! xyz&"
   // s = "ab'c ! xyz", p = '\''  :  result = "ab'c& ! xyz"
   // p can be ' ', '\'', '"'
   //
   std::string slt = rtrim(remove_trailing_comment(s,p));
   return slt + "&" + s.substr(slt.length());
}
