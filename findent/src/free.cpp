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
#include <string>
#include <sstream>
#include <deque>

#include "debug.h"
#include "findent_types.h"
#include "fixed.h"
#include "free.h"
#include "simpleostream.h"

static int get_paren_align(const std::string& line, int& state);
static int align_state;
static int align_indent;

void Free::build_statement(Fortranline &line, bool &f_more, bool &pushback)
{
   //
   // adds line to c_lines
   // adds line (stripped from comments, preprocessor stuff and 
   //    continuation stuff)  to full_statement
   // f_more 1: more lines are to expected
   //        0: this line is complete
   //

   pushback = 0;

   if (!line.blank_or_comment())
   {
      //
      // sl becomes the first input_line_length characters of line
      // and then trimmed left and right:
      //

      std::string sl = line.trimmed_line();

      if(line.firstchar() == '&')
	 sl.erase(0,1);

      full_statement = full_statement + sl;

      //
      // remove trailing comment and trailing white space
      //

      full_statement = rtrim(remove_trailing_comment(full_statement));

      // 
      // If the last character = '&', a continuation is expected.
      //

      f_more = ( lastchar(full_statement) == '&');
      if (f_more)            // chop off '&' from full_statement :
	 full_statement.erase(full_statement.length()-1);

   }
   c_lines.push_back(line);
   D(O(full_statement));
   DL(
	 for (size_t i=0; i<c_lines.size(); i++)
	 D(O(c_lines[i].str());O("!cur ");O(i););
     );

   //---------------------------------------------------------------------

   if(relabeling)
   {
      size_t k = 0;
      bool r   = 1;
      std::string s  = line.str();
      k = line.str().size() - line.strnomp().size();

      while(k < s.size() && r)
      {
	 switch(s.at(k))
	 {
	    case ' ':
	    case '\t':
	       k++;
	       break;
	    case '&':
	       k++;
	       r = 0;
	       break;
	    default:
	       r = 0;
	       break;
	 }
      }

      size_t m = full_pos.size();
      for (size_t i = m; i < full_statement.size(); i++)
      {
	 insert_in_full_pos(c_lines.size()-1,k++);
      }

      DL(
	    D(O(full_statement);O("!full"););
	    D(O(line.str());O("!line"););
	    for (size_t i=0; i<c_lines.size(); i++)
	    D(O(c_lines[i].str());O("!cur ");O(i););

	    for(size_t i=0; i<full_statement.size(); i++)
	    {
	    int k = full_pos[i].first; int l = full_pos[i].second;
	    D(O(full_statement[i]);O(c_lines[k].str()[l]);O(k);O(l);O(i););
	    if (full_statement[i] != c_lines[k].str()[l])
	    D(O("Fatal error"););
	    }
	);
   }
   //---------------------------------------------------------------------

}           // end of build_statement


void Free::output(lines_t &lines, lines_t *fixedlines)
{
   //
   // output lines input: free format, output: free format
   //

   std::ostringstream os;
   std::string endline = fi->endline;
   bool to_mycout      = (fixedlines == 0);
   bool isfirst        = 1;
   const char conchar  = '&';

   bool expect_continuation = 0;
   bool prev_expect_continuation;

   bool mylabel_left = fi->flags.label_left;
   if(!to_mycout) // want labels left if converting to fixed
      mylabel_left   = true;
   align_state  = 0;
   align_indent = -1;

   while (!lines.empty())
   {
      mycout.reset();

      if(lines.front().written())
      {
	 lines.pop_front();
	 continue;
      }

      is_omp = lines.front().omp();

      if(is_omp)
      {
	 ompstr = "!$ ";     // omp sentinel for free format
	 cmpstr = "!$";      // omp sentinel for fixed format
      }
      else
      {
	 ompstr = "";
	 cmpstr = "";
      }

      if(output_pre(lines,fixedlines))
	 continue;

      if (lines.empty())
	 return;

      if (lines.front().blank())    
      {
	 //
	 // a completely blank line, that is simple:
	 //
	 if(to_mycout)
	    mycout << ompstr << endline;
	 else
	 {
	    if (lines.front().omp())
	       fixedlines->push_back(F(cmpstr));
	    else
	       fixedlines->push_back(F(""));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      //
      // handle comment lines:
      //
      if (lines.front().comment())
      {
	 if (lines.front()[0] != '!' || lines.front().omp())
	 {
	    //
	    // take care of the situation that cur_indent == 0
	    // but the comment does not start in column 1
	    //
	    if(to_mycout)
	       //mycout << insert_omp(blanks(M(std::max(fi->cur_indent,1))),ompstr);
	       mycout << insert_omp(blanks(M(std::max(cur_indent,1))),ompstr);
	    else
	       //os << cmpstr << blanks(std::max(fi->cur_indent,1));
	       os << cmpstr << blanks(std::max(cur_indent,1));
	 }

	 if(to_mycout)
	 {
	    mycout << lines.front().trim() << endline;
	 }
	 else
	 {
	    os << lines.front().trim();
	    fixedlines->push_back(F(rtrim(os.str())));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      //
      // this is a line with (part of) a fortran statement
      // possibly a continuation line
      //

      //
      // If the line ends with a '&', the next line with fortran code
      // is a real continuation line. 
      // If this line does not end with '&', the next line with fortran code
      // is not a real continuation line: possibly this line is garbage.
      // 
      // Important because of indenting real continuation lines.
      //

      prev_expect_continuation = expect_continuation;
      //
      // TODO: not bulletproof with regard to unfinished strings
      //
      expect_continuation = 
	 (lastchar(lines.front().remove_trailing_comment()) == '&');


      if(isfirst)
      {
	 //
	 // found the first statement line
	 // if label: handle it
	 //
	 isfirst = 0;
	 std::string lineout;
	 if (mylabel_left && labellength > 0)
	 {
	    //
	    // put label at start of line
	    //

	    std::string firstline = lines.front().trim();

	    // it is possible, that the statement label is split across more lines.
	    // as in:  123&
	    //      :   &4   continue
	    // or the label is directly followed by an &:
	    // as in:  123&
	    //      :    &  continue
	    // In that case the actual value of labellength is not usable here, we
	    // will handle it separately.
	    //
	    if (labellength > (int)firstline.size() && lastchar(firstline) == '&')
	    {
	       if(to_mycout)
	       {
		  lineout = firstline;
		  mycout << lineout << endline;
	       }
	       else
	       {
		  os << rm_last_amp(firstline) << blanks(6); // this is likely to create havoc
		  fixedlines->push_back(F(os.str()));
		  os.str("");
	       }
	    }
	    else
	    {

	       std::string label     = firstline.substr(0,labellength);
	       firstline             = trim(firstline.substr(labellength));

	       //int l = M(std::max(fi->cur_indent - labellength,1));  // put at least one space after label
	       int l = M(std::max(cur_indent - labellength,1));  // put at least one space after label
	       if(to_mycout)
	       {
		  if (lines.front().omp())
		     l = M(std::max((int)(l-ompstr.length()),1));
		  lineout = ompstr + label + blanks(l) + firstline;
		  //mycout << ompstr << label << blanks(l) << firstline << endline;
		  mycout << lineout << endline;
	       }
	       else
	       {
		  os << insert_omp(label,cmpstr) << blanks(6) << rm_last_amp(firstline);
		  fixedlines->push_back(F(os.str()));
		  os.str("");
	       }
	    }

	    if (fi->flags.align_paren)
	       align_indent = get_paren_align(lineout,align_state);
	    D(O("align_indent");O(align_indent););
	    lines.pop_front();
	    continue;
	 }
	 //
	 // this is the first code line without label. Always indent
	 // (even if it starts with '&')
	 // Or it is the first line with a label, and mylabel_left = false
	 // In that case, the number of spaces between the label and the rest of
	 // the line should be made equal to one and the whole line should
	 // be indented.
	 //

	 if(to_mycout)
	 {
	    std::string line = lines.front().trim();
	    if (labellength > 0)
	       line = line.substr(0,labellength) + ' ' + ltrim(line.substr(labellength));
	    std::string lineout;
	    //mycout << insert_omp(blanks(M(std::max(cur_indent,0))),ompstr) <<
	    //  line << endline;
	    if (align_indent >=0)
	    {
	       lineout = insert_omp(blanks(M(std::max(align_indent,0))),ompstr)+line;
	       D(O("align_indent");O(align_indent););
	    }
	    else
	       lineout = insert_omp(blanks(M(std::max(cur_indent,0))),ompstr)+line; 
	    if (fi->flags.align_paren)
	    {
	       align_indent = get_paren_align(lineout,align_state);
	    }
	    D(O("align_indent");O(align_indent);O(lineout););

	    mycout << lineout << endline;
	 }
	 else
	 {
	    os << insert_omp(blanks(5),cmpstr);
	    if(lines.front().firstchar() == '&')
	       os << conchar << rm_last_amp(ltrim(lines.front().trim().substr(1)));
	    else
	       os << ' ' << rm_last_amp(lines.front().rtrim());
	    fixedlines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      //
      // handle next lines, these should be continuation lines
      //
      //
      // if continuation starts with '&', use current indentation
      // else use current indentation + flags.cont_indent 
      //

      if (lines.front().firstchar() == '&')  // continuation line starting with '&'
      {
	 if(to_mycout)
	    //mycout << insert_omp(blanks(M(std::max(fi->cur_indent,0))),ompstr) <<
	    mycout << insert_omp(blanks(M(std::max(cur_indent,0))),ompstr) <<
	       lines.front().trim() << endline;
	 else
	 {
	    os << insert_omp(blanks(5),cmpstr) << conchar << rm_last_amp(lines.front().trim().substr(1));
	    fixedlines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }

      //
      // continutation not starting with '&'
      //
      D(O("align_indent");O(align_indent););
      if (align_indent >= 0)
      {
	 int l;
	 l = M(std::max(align_indent,0));
	 std::string lineout = insert_omp(blanks(l),ompstr) + lines.front().trim();
	 mycout << lineout << endline;
	 align_indent = get_paren_align(ltrim(lineout),align_state);
	 D(O("align_indent");O(align_indent);O(lineout););
	 lines.pop_front();
	 continue;
      }
      else if (fi->flags.indent_continuation)    // indentation of continuation lines is requested
      {
	 int l;
	 if (prev_expect_continuation)
	    l = M(std::max(cur_indent+fi->flags.cont_indent,0));
	 else
	    l = M(std::max(cur_indent,0));

	 if(to_mycout)
	    mycout << insert_omp(blanks(l),ompstr) << lines.front().trim() << endline;
	 else
	 {
	    os << insert_omp(blanks(5),cmpstr) << conchar << rm_last_amp(lines.front().rtrim());
	    fixedlines->push_back(F(os.str()));
	    os.str("");
	 }
	 lines.pop_front();
	 continue;
      }
      //
      // this line is a continuation line, without preceding '&'
      // and it has been requested that no indentation is done for 
      // continuation lines
      //
      if(to_mycout)
	 mycout << insert_omp(lines.front().rtrim(),ompstr) << endline;
      else
      {
	 os << insert_omp(blanks(5),cmpstr) << conchar << rm_last_amp(lines.front().rtrim());
	 fixedlines->push_back(F(os.str()));
	 os.str("");
      }
      lines.pop_front();
   }
}      // end of output

void Free::output_converted(lines_t &lines)
{
   D(O("free::output_converted "););
   lines_t fixedlines;

   output(lines, &fixedlines);

   Globals oldgl = *gl;
   gl->global_format      = FIXED;
   gl->global_line_length = 0;

   Fixed f(fi);

   lines_t::iterator it = fixedlines.begin();
   //
   // clean all fixedlines:
   //
   while(it != fixedlines.end()) 
   {
      it->clean(1);
      it++;
   }
   f.cur_indent  = cur_indent;
   f.labellength = labellength;
   f.output(fixedlines);
   (*gl) = oldgl;

}   // end of output_converted

std::string Free::rm_last_amp(const std::string &s)
{
   //
   // removes trailing &, also as in
   // x = 10 & ! comment
   // also removes trailing space in result
   //

   std::string slt = rtrim(remove_trailing_comment(s));
   std::string so = rtrim(slt);
   if (*so.rbegin() == '&')
      return  rtrim(so.erase(so.length() -1)+s.substr(slt.length()));
   else
      return rtrim(s);
}   // end of rm_last_amp

#ifdef PUSH
#undef PUSH
#endif
#ifdef POP
#undef POP
#endif

#define PUSH(i) \
   do {                               \
      indent = previndent + i;        \
      stack.push_back(indent);        \
   } while(0)

#define POP \
   do {                               \
      if (stack.size() > 0)           \
      {                               \
	 stack.pop_back();            \
	 if (stack.size() > 0)        \
	 indent = stack.back();       \
	 else                         \
	 indent = startindent;        \
      }                               \
   } while(0)

/* to be used in a continuation, and you want things aligned as
 * call sub(x, &
 *          abc, &
 *          [ 1, &
 *            42, &
 *            5], &
 *          z)
 *
 * Usage:
 * Call the function with the first line ('call sub(x, &'), with 
 * second parameter zero, example
 *    int state = 0;
 *    int indent = get_paren_align(line,state);
 * The return value is equal to the number of spaces one should prepend
 * to the next line, in our example that would be 9.
 * If the return value == -1, there is no unmatched open parenthesis,
 * it is up to you to act accordingly.
 * Keep calling this function, until the end of the continuation lines,
 * taking care to not alter the second parameter: this is managed by
 * the function for internal affairs.
 * The function is comment-aware.
 */
static int get_paren_align(const std::string& line, int& state)
{
   enum states{start = 0, start1, inquote, lparen, lparen1};
   static std::deque<int> stack;
   static char quote;
   static int nextstate;
   static int indent;
   static int previndent;
   static int startindent;
   if (state == 0)
   {
      stack.clear();
      quote     = 0;
      state     = start1;
      nextstate = start1;
      previndent= 0;
      indent    = 0;
      for (int i=0; i<(int)line.size(); ++i)
	 if(line[i] != ' ')
	 {
	    startindent = i;
	    break;
	 }
   }
   for (int i=0; i<(int)line.size(); ++i)
   {
      char c = line[i];
      switch (state)
      {
	 case start1:
	    switch(c)
	    {
	       case '"': case '\'':
		  quote = c;
		  nextstate = state;
		  state = inquote;
		  break;
	       case '(': case '[':
		  PUSH(i);
		  state = lparen;
		  break;
	       case '!':
		  goto endline;
		  break;
	    }
	    break;
	 case inquote:
	    switch(c)
	    {
	       case '"': case '\'':
		  if (quote == c)
		     state = nextstate;
		  break;
	    }
	    break;
	 case lparen:
	    switch(c)
	    {
	       case '"': case '\'':
		  POP;
		  PUSH(i);
		  quote = c;
		  nextstate = lparen1;
		  state = inquote;
		  break;
	       case '(': case '[':
		  PUSH(i);
		  state = lparen;
		  break;
	       case ')': case ']':
		  POP;
		  if(stack.size() == 0)
		     state = start1;
		  else
		     state = lparen;
		  break;
	       case '!':
		  goto endline;
		  break;
	       default:
		  if (c != '/' && ! isspace(c))
		  {
		     POP;
		     PUSH(i);
		     state = lparen1;
		  }
		  break;
	    }
	    break;
	 case lparen1:
	    switch(c)
	    {
	       case '"': case '\'':
		  quote = c;
		  nextstate = lparen1;
		  state = inquote;
		  break;
	       case '(': case '[':
		  PUSH(i);
		  state = lparen;
		  break;
	       case ')': case ']':
		  POP;
		  if(stack.size() == 0)
		     state = start1;
		  else
		     state = lparen1;
		  break;
	       case '!':
		  goto endline;
		  break;
	    }
	    break;
      }
   }
endline:
   previndent = indent;
   if (stack.size() == 0)
      return -1;
   else
      return indent;
}
#undef PUSH
#undef POP

