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

#include "fortran.h"
#include "pre_analyzer.h"
#include "line_prep.h"
#include "functions.h"
#include "flags.h"

#define Cur_indent   cur_indent
#define FLAGS        fi->flags
#define Endline      fi->endline
#define End_of_file  end_of_file
#define Getnext      getnext
#define Includes     fi->includes
#define Curline      (*curline)

void Fortran::init_indent()
{
   //
   // fills the indent-stack until indent 0
   // if flags.all_indent <= 0: build indent_stack with a number of start_indent's
   //
   while(!indent.empty())
      indent.pop_back();
   int l=0;
   if(FLAGS.all_indent > 0)
   {
      for (l = start_indent%FLAGS.all_indent; l<start_indent; l+=FLAGS.all_indent)
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

//Fortranline Fortran::mygetline(bool &eof, bool keep_lines)
Fortranline Fortran::mygetline(bool &eof)
{
   std::string s = "";
   eof = 0;

   if (input->empty())
   {
      eof = 1;
   }
   else
   {
      s = input->front();
      input->pop_front();
   }
   //}

lines_read ++;

return Fortranline(gl,s);
}              // end of mygetline

Fortranline Fortran::getnext(bool &eof, bool use_wb)
{
   Fortranline line(gl);
   eof = 0;
   if (use_wb && !wizardbuffer.empty())
   {
      line = wizardbuffer.front();
      D(O("wizardbuffer_pop");O(line.str());O(wizardbuffer.size()););
      num_lines++;
      wizardbuffer.pop_front();
      //if (reading_from_tty && line.str() == ".")
      //	 eof = 1;
   }
   else
   {
      line = mygetline(eof);
      if (!eof)
	 num_lines++;
   }

   //
   // remove trailing white space
   // FIXED: convert leading tab to space
   //

   line.clean();

   if(!use_wb && !eof)
   {
      num_lines--;
      wizardbuffer.push_back(line);
      D(O("wizardbuffer_push");O(line.str());O(wizardbuffer.size()););
   }

   if (!nbseen)
   {
      nbseen = !line.blank_or_comment() 
	 && (line.getpregentype() == 0)
	 && prevlastchar          != '\\'
	 && line.lastchar()       != '\\'; 

      if (FLAGS.auto_firstindent && nbseen)
      {
	 start_indent = guess_indent(line);
	 cur_indent   = start_indent;
	 init_indent();
	 indent_handled = 1;
      }
      prevlastchar = line.lastchar();
   }

   D(O(line.str());O(use_wb););
   DL(
	 if(use_wb)
	 D(O("getnext___");O(line.str()););
     );

   return line;
}

int Fortran::guess_indent(Fortranline line)
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

bool Fortran::output_pre(lines_t &lines, lines_t *outlines)
{
   //
   // if the first line of lines is a preprocessor line
   // output this line and the continuation lines
   // popping lines
   //

   bool to_mycout = outlines == 0;

   if(lines.empty())
      return 0;
   if (lines.front().pre())
   {
      bool p_more = 0;
      while(1)
      {
	 fi->handle_pre_light(lines.front(),p_more);
	 if (lines.front().pre())
	    if (to_mycout)
	       mycout << lines.front().trim() << Endline;
	    else
	       outlines->push_back(F(lines.front().trim()));
	 else
	    if (to_mycout)
	       mycout << lines.front().str() << Endline;
	    else
	       outlines->push_back(F(lines.front().str()));
	 lines.pop_front();
	 if (lines.empty() || !p_more)
	    break;
      }
      return 1;
   }
   else
      return 0;
}     // end of output_pre


void Fortran::handle_last_usable_only()
{
   //
   // search for the last line that is usable to start indenting
   // using findent.
   //
   int usable_line = 1;

   init_indent();
   while(1)
   {
      //int prev         = fi->num_lines;
      int prev         = num_lines;
      get_full_statement();
      Line_prep p(full_statement);
      propstruct props = parseline(p);
      switch (props.kind)
      {
	 case BLANK:
	 case CASE:
	 case CASEDEFAULT:
	 case CLASSDEFAULT:
	 case CLASSIS:
	 case CONTAINS:
	 case ELSE:
	 case ELSEIF:
	 case ELSEWHERE:
	 case ENTRY:
	 case RANK:
	 case RANKDEFAULT:
	 case TYPEIS:
	    break;
	 default: 
	    usable_line = prev+1;
	    break;
      }
      if (End_of_file)
      {
	 std::cout << usable_line << Endline;
	 return;
      }
   }
}             // end of last_usable_only

void Fortran::get_full_statement()
{
   //
   // this function collects 'full_statement': a complete
   // fortran line, concatenated from possible continuation lines,
   // comments and preprocessor lines removed
   //
   // full_statement is used to determine the indentation
   //
   // Also, every line is unmodified stored in 'c_lines'
   // 
   // If the input starts with a comment or a preprocessor line,
   // full_statement = "", c_lines will contain the line, possibly
   // followed by preprocessor continuation lines as in:
   /* #define foo \   */
   //           bar
   //
   // A totally empty line is treated as a comment line,
   //
   // Comment lines and preprocessor lines can be embedded in 
   // fortran continuation lines as in:
   //
   //    subroutine compute( &
   // #ifdef one
   //      x &
   // #else
   //      x,y &
   // #endif
   //     )
   //
   // In this example, full_statement will contain:
   //
   //    subroutine compute(x,y)
   // ( this is only correct if 'one' is not defined, but it 
   //   will not prevent correct indenting)
   //

   //
   // if things get hairy: try a finite state machine
   //

   // 
   // see fortran.h for definitions of some variables
   //

   full_statement = "";
   if(relabeling)
      full_pos.clear();
   indent_handled = 0;
   pregion = 0;

   if (first_call)
   {
      Curline     = Getnext(End_of_file);
      first_call  = 0;
   }

   c_lines.clear();
   if(relabeling)
      full_pos.clear();

   while(1)
   {
      D(O("State:");O(state);O(fs_store.size());O(Curline.str()););
      switch(state)
      {
	 case start:
	    if (fs_store.empty())
	    {
	       full_statement = "";
	       if(relabeling)
		  full_pos.clear();
	    }
	    else
	    {
	       full_statement = fs_store.back();
	       if(relabeling)
		  full_pos    = fp_store.back();
	    }

	    if (End_of_file) 
	    {
	       state = end_start;
	       break;
	    }
	    pregentype = Curline.getpregentype();
	    if(pregentype == CPP || pregentype == COCO)
	    {
	       state = in_pre;
	       break;
	    }

	    if (FLAGS.deps)
	    {
	       switch(pregentype)
	       {
		  case INCLUDE_CPP: case INCLUDE_CPP_STD: case INCLUDE_COCO:
		     Includes.insert(std::make_pair(pregentype,Curline.getinclude()));
	       }
	    }

	    if (is_findentfix(Curline))
	    {
	       state = in_ffix;
	       break;
	    }

	    state = in_fortran;
	    break;

	 case in_ffix:
	    Curline.preregion(pregion);
	    c_lines.push_back(Curline);
	    full_statement = rtrim(remove_trailing_comment(Curline.rest()));
	    // todo full_pos
	    Curline = Getnext(End_of_file);
	    state = start;
	    return;

	 case in_fortran:
	    if(End_of_file) { state = end_fortran; break; }

	    Curline.preregion(pregion);
	    build_statement(Curline, f_more, pushback);
	    D(O("Full_statement ");O(Curline.str());O(f_more);O(pushback););

	    if (f_more)
	    {
	       Curline = Getnext(End_of_file); 
	       if (End_of_file) { state = end_fortran; break; }
	       pregentype = Curline.getpregentype();
	       if (pregentype == CPP || pregentype == COCO)
	       {
		  p_more = 0;
		  while (1)
		  {
		     handle_pre(Curline,p_more);
		     Curline.preregion(pregion);
		     c_lines.push_back(Curline);
		     if(p_more)
		     {
			Curline = Getnext(End_of_file); 
			if (End_of_file) { state = end_fortran; break; }
		     }
		     else
			break;
		  }
		  Curline = Getnext(End_of_file);
	       }
	       if (FLAGS.deps)
	       {
		  switch(pregentype)
		  {
		     case INCLUDE_CPP: case INCLUDE_CPP_STD: case INCLUDE_COCO:
			Includes.insert(std::make_pair(pregentype,Curline.getinclude()));
		  }
	       }
	       state = in_fortran;
	       break;
	    }
	    state = in_fortran_1;
	    //
	    // need state in_fortran_1 to get correct result from last_usable
	    //
	    return;

	 case in_fortran_1:
	    if (!pushback)
	       Curline = Getnext(End_of_file);
	    state = start;
	    break;

	 case in_pre:
	    p_more = 0;
	    while(1)
	    {
	       handle_pre(Curline,p_more);
	       Curline.preregion(pregion);
	       c_lines.push_back(Curline);
	       if(p_more)
	       {
		  Curline = Getnext(End_of_file); 
		  if (End_of_file) { state = end_pre; break; }
	       }
	       else
		  break;
	    }
	    Curline = Getnext(End_of_file);
	    state = start;
	    break;

	 case end_start:
	 case end_fortran:
	 case end_pre:
	    state = start;
	    return;
      }
   }
}           // end of get_full_statement

bool Fortran::is_findentfix(Fortranline &line)
{
   bool rc = 0;
   switch (line.scanfixpre())
   {
      case FINDENTFIX:
	 rc = 1;
	 break;
      case P_ON:
	 ppp.on();     // debug.h, debug.cpp
	 break;
      case P_OFF:
	 ppp.off();
	 break;
   }
   return rc;
}

void Fortran::handle_pre(Fortranline &line, bool &p_more)
{
   int ifelse;

   if((!p_more) && ( pregentype == CPP || pregentype == COCO))
   {
      int pretype    = line.scanfixpre();
      switch(pretype)
      {
	 case CPP:
	 case COCO:
	 case FINDENTFIX:
	    break;
	 default:
	    ifelse = prea.analyze(pretype);
	    switch(ifelse)
	    {
	       case Pre_analyzer::PRE_IF:
		  pregion++;
		  push_all();
		  break;

	       case Pre_analyzer::PRE_ELIF:
		  pregion++;
		  top_all();
		  break;

	       case Pre_analyzer::PRE_ELSE:
		  pregion++;
		  top_all();
		  // fall through
	       case Pre_analyzer::PRE_ENDIF:
		  pregion++;
		  pop_all();
		  break;

	       case Pre_analyzer::PRE_ENDIFE:
		  pregion++;
		  break;

	       default:
		  return;
		  break;
	    }

	    switch(ifelse) // full_statement needs apart treatment:
	    {
	       case Pre_analyzer::PRE_IF:
		  fs_store.push_back(full_statement);
		  D(O("full in if");O(full_statement););
		  cl_store.push_back(c_lines);
		  if(relabeling)
		     fp_store.push_back(full_pos);
		  break;

	       case Pre_analyzer::PRE_ELIF:
		  if(fs_store.empty())
		  {
		     full_statement = "";
		     cl_store.clear();
		     if(relabeling)
			full_pos.clear();
		  }
		  else
		  {
		     full_statement = fs_store.back();

		     lines_t x      = cl_store.back();
		     for (size_t i = 0; i<x.size(); ++i)
		     {
			x[i].written(true);
			D(O("marked as written");O(x[i].written());O(x[i].str()););
		     }
		     while(x.size())
		     {
			c_lines.push_back(x[0]);
			x.pop_front();
		     }
		     if(relabeling)
			full_pos    = fp_store.back();
		  }
		  break;

	       case Pre_analyzer::PRE_ELSE:
		  if(fs_store.empty())
		  {
		     full_statement = "";
		     cl_store.clear();
		     if(relabeling)
			full_pos.clear();
		     D(O("full in else");O(full_statement););
		  }
		  else
		  {
		     full_statement = fs_store.back();
		     D(O("full in else");O(full_statement););
		     lines_t x      = cl_store.back();
		     for (size_t i = 0; i<x.size(); ++i)
		     {
			x[i].written(true);
			D(O("marked as written");O(x[i].written());O(x[i].str()););
		     }
		     while(x.size())
		     {
			c_lines.push_back(x[0]);
			x.pop_front();
		     }

		     if(relabeling)
			full_pos    = fp_store.back();
		  }
		  break;

	       case Pre_analyzer::PRE_ENDIF:
	       case Pre_analyzer::PRE_ENDIFE:
		  D(O("#endif stack:");O(fs_store.size()););
		  if(!fs_store.empty())
		  {
		     fs_store.pop_back();
		     cl_store.pop_back();
		     if(relabeling)
			fp_store.pop_back();
		  }
		  break;
	    }
	    break;
      }
   }
   if(pregentype == CPP)
      p_more = (line.lastchar() == '\\');
   else if(pregentype == COCO)
      p_more = (line.lastchar() == '&');
   else
      p_more = 0;

}       // end of handle_pre

void Fortran::indent_and_output()
{
   std::string rest = full_statement;
   bool first_time  = 1;
   while(1)
   {
      Line_prep p(rest);
      propstruct props = parseline(p); 
      labellength = props.stlabel.size();
      if (labellength > 0)
	 //
	 // if there was a previous labeled do, handle it:
	 //
      {
	 int ilabel = string2number<int>(props.stlabel);
	 while ( top_dolabel() == ilabel )
	 {
	    pop_indent();
	    pop_dolabel();
	    Cur_indent = top_indent();
	    //fi->indent_handled = 1;
	    indent_handled = 1;
	 }
      }
      //
      // if the previous non-blank line was PROCEDURE (module procedure)
      // then determine if this was a procedure with executable content
      // if so: take delayed action with respect to indenting
      //
      if (prev_props.kind == PROCEDURE)
      {
	 switch (props.type)
	 {
	    case EXEC:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.routine_indent);
	       empty_dolabels();
	       push_rprops(prev_props);
	       break;
	    default:
	       break;
	 }
      }
      Cur_indent = top_indent();
      refactor_end_found = 0;
      if (FLAGS.deps)   // determine dependencies
      {
	 switch(props.kind)  /* handle includes etc */
	 {
	    case INCLUDE:
	    case INCLUDE_CPP:
	    case INCLUDE_CPP_STD:
	    case INCLUDE_COCO:
	       D(O("INCLUDE");O(props.stringvalue);O(FLAGS.deps););
	       Includes.insert(std::make_pair(props.kind,trim(props.stringvalue)));
	       break;
	    case USE:
	    case MODULE:
	       D(O("USE/MODULE");O(props.name);O(FLAGS.deps););
	       Includes.insert(std::make_pair(props.kind,trim(stolower(props.name))));
	       break;
	    case SUBMODULE:
	       D(O("SUBMODULE");O(props.lrvalue+":"+props.name);O(FLAGS.deps););
	       Includes.insert(std::make_pair(props.kind,trim(stolower(props.lrvalue))+":"
			+trim(stolower(props.name))));
	       Includes.insert(std::make_pair(USE,trim(stolower(props.lrvalue))));
	 }
      }
      else
      {
	 //
	 // for every entity that is eligible for refactoring it's end
	 // e.g. subroutine
	 // we will push props on the rprops stack
	 // for every corresponding end-entity (e.g. endsubroutine) we will
	 // pop the rprops stack
	 // 
	 switch(props.kind)   //determine indent, refactor
	 {
	    case TYPE:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.type_indent);
	       empty_dolabels();
	       push_rprops(props);
	       break;
	    case INTERFACE:
	       D(O("INTERFACE_NAME");O(props.name););
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.interface_indent);
	       empty_dolabels();
	       push_rprops(props);
	       break;
	    case SUBROUTINE:
	    case FUNCTION:
	    case PROGRAM:
	    case BLOCKDATA:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.routine_indent);
	       empty_dolabels();
	       push_rprops(props);
	       break;
	    case MODULE:
	    case SUBMODULE:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.module_indent);
	       empty_dolabels();
	       push_rprops(props);
	       break;
	    case BLOCK:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.block_indent);
	       break;
	    case CHANGETEAM:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.changeteam_indent);
	       break;
	    case CRITICAL:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.critical_indent);
	       break;
	    case ENUM:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.enum_indent);
	       empty_dolabels();
	       break;
	    case ABSTRACTINTERFACE:
	       D(O("ABSTRACTINTERFACE"););
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.interface_indent);
	       empty_dolabels();
	       push_rprops(props);
	       break;
	    case DO:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.do_indent);
	       if (props.labels.size() != 0)
		  push_dolabel(string2number<int>(props.labels[0]));
	       break;
	    case SELECTCASE:
	    case SELECTRANK:
	    case SELECTTYPE:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.select_indent);
	       break;
	    case CASE:
	    case CASEDEFAULT:
	    case RANK:
	    case RANKDEFAULT:
	    case CLASSDEFAULT:
	    case CLASSIS:
	    case TYPEIS:
	       Cur_indent -= FLAGS.case_indent;
	       break;
	    case END:
	    case ENDBLOCKDATA:
	    case ENDFUNCTION:
	    case ENDMODULE:
	    case ENDPROCEDURE:
	    case ENDPROGRAM:
	    case ENDSUBROUTINE:
	    case ENDSUBMODULE:
	    case ENDINTERFACE:
	    case ENDTYPE:
	       if (!indent_handled)
		  Cur_indent = pop_indent();
	       cur_rprop = top_rprops();
	       pop_rprops();
	       refactor_end_found = (cur_rprop.kind != ABSTRACTINTERFACE);
	       break;
	    case ENDASSOCIATE:
	    case ENDBLOCK:
	    case ENDCRITICAL:
	    case ENDDO:
	    case ENDENUM:
	    case ENDFORALL:
	    case ENDIF:
	    case ENDSELECT:
	    case ENDTEAM:
	    case ENDWHERE:
	       if (!indent_handled)
		  Cur_indent = pop_indent();
	       break;
	    case PROCEDURE:  // in fact: moduleprocedure
	       //
	       // depending on what follows this will be 
	       // recognized as a module procedure with content
	       // or only a moduleprocedure specification
	       //
	       break;
	    case CONTAINS:
	       if (FLAGS.indent_contain)
		  Cur_indent -= FLAGS.contains_indent;
	       else
	       {
		  Cur_indent = start_indent;
		  pop_indent();
		  push_indent(Cur_indent);
	       }
	       break;
	    case IF:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.if_indent);
	       break;
	    case ELSE:
	       Cur_indent -= FLAGS.if_indent;
	       break;
	    case ELSEIF:
	       Cur_indent -= FLAGS.if_indent;
	       break;
	    case ELSEWHERE:
	       Cur_indent -= FLAGS.where_indent;
	       break;
	    case ENTRY:
	       Cur_indent -= FLAGS.entry_indent;
	       break;
	    case WHERE:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.where_indent);
	       break;
	    case ASSOCIATE:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.associate_indent);
	       break;
	    case FORALL:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.forall_indent);
	       break;
	    case INCLUDE:
	       D(O("INCLUDE");O(FLAGS.include_left);O(start_indent););
	       if (FLAGS.include_left)
		  Cur_indent = start_indent;
	       else
		  Cur_indent = top_indent();
	       break;
#ifdef USEESOPE
	    case SEGMENT:
	       Cur_indent = top_indent();
	       push_indent(Cur_indent + FLAGS.segment_indent);
	       break;
	    case ENDSEGMENT:
	       if (!indent_handled)
		  Cur_indent = pop_indent();
	       break;
#endif
	    default:
	       Cur_indent = top_indent();
	 } // end determine indent and refactor

	 switch(props.kind)
	 {
	    case BLANK:
	       break;
	    default:
	       prev_props = props;
	 }
	 if(first_time)  // check to handle multi-statement line like x=1;y=3
	 {
	    output_line();
	    first_time = 0;
	 }
      }
      rest = p.get_line_rest();
      if (rest == "")
	 break;
   }
}               // end of indent_and_output

void Fortran::output_line()
{
   if (c_lines.empty())
      return;

   mycout.reset();

   handle_refactor();

   if (!FLAGS.apply_indent)
   {
      //
      // no indentation requested:
      //
      while (!c_lines.empty())
      {
	 if(!c_lines.front().written())
	    mycout << c_lines.front().str() << Endline;
	 c_lines.pop_front();
      }
      return;
   }

   if (fi->input_format == fi->output_format)
      output(c_lines);
   else
      output_converted(c_lines);
}           // end of output_line

void Fortran::handle_refactor()
{
   if (FLAGS.refactor_end && refactor_end_found)
   {
      //
      // handle refactor routines
      //
      if (cur_rprop.kind != 0) // check if corresponding start is ok
      {
	 //
	 // modify line to match the corrsponding module, subroutine ... line  
	 // first, we have to locate that line, the back of the deque
	 // could have comment or empty lines
	 //
	 lines_t::reverse_iterator it = c_lines.rbegin();
	 while (it != c_lines.rend())
	 {
	    if ( it->blank_or_comment_or_pre() )
	       it++;
	    else
	       break;
	 }

	 // starting at position labellength + spaces, scan lines[0] until isalnumplus()
	 // returns false. The scanned characters will be replaced by something
	 // like: 'end subroutine mysub'
	 //
	 //std::string s = c_lines.back().trimmed_line();
	 std::string s = it->trimmed_line();
	 size_t startpos = s.find_first_not_of(' ',labellength);
	 size_t endpos   = s.length();
	 for (size_t i=startpos; i<s.length(); i++)
	 {
	    if (!isalnumplus(s[i]))
	    {
	       endpos = i;
	       break;
	    }
	 }
	 //
	 // correct endpos for trailing spaces:
	 //
	 for (size_t i=endpos-1; ;i--)
	 {
	    bool r;
	    switch(s[i])
	    {
	       case ' ':
	       case '\t':
		  r = 0;
		  break;
	       default:
		  endpos = i+1;
		  r = 1;
		  break;
	    }
	    if (r)
	       break;
	 }
	 std::string replacement = "end " + whatrprop(cur_rprop);
	 if (FLAGS.upcase_end_type)
	    replacement = stoupper(replacement);
	 if (cur_rprop.name != "")
	    replacement += " " + cur_rprop.name;
	 it->str(s.substr(0,startpos) + replacement + s.substr(endpos));
      }
   }
}


#undef Cur_indent
#undef FLAGS
#undef Endline
#undef End_of_file
#undef Getnext
#undef Curline
#undef Includes
