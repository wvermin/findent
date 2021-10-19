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

#ifndef FORTRAN_H
#define FORTRAN_H

#include "findent_types.h"
#include "findentclass.h"
#include "fortranline.h"
#include "simpleostream.h"
#include "pre_analyzer.h"
#include "debug.h"

class Fortran
{
   public:
      virtual ~Fortran(){ delete curline; }

      Fortran(Findent *f)
      {
	 fi = f;
	 gl = f->gl;
	 cur_rprop          = empty_rprop;
	 curline            = new Fortranline(gl);
	 prev_props         = empty_rprop;
	 refactor_end_found = 0;

	 // for get_full_statement:
	 f_more             = 0;
	 first_call         = 1;
	 state              = start;
	 num_lines          = 0;
	 start_indent       = 0;
	 lines_read         = 0;
	 nbseen             = 0;
	 end_of_file        = 0;
	 labellength        = 0;
	 relabeling         = false;
         mygetline_nr       = 0;
	 //
	 // for getnext:
	 //
	 prevlastchar       = 0;
	 cur_indent         = 0;
	 indent_handled     = 0;
      }

      virtual void build_statement(Fortranline &line, bool &f_more, bool &pushback) = 0;

      virtual void output(lines_t &lines,bool contains_hollerith,lines_t *freelines = 0) = 0;

      virtual void output_converted(lines_t &lines, bool ch) = 0;

      void         get_full_statement(void);

      void         do_relabel(void);
      void         init_relabel(void);
      int          get_num(void) { return num_lines; }
      void         handle_last_usable_only(void);
      void         indent_and_output(void);
      void         init_indent(void);
      int          start_indent;
      int          cur_indent;
      bool         end_of_file;
      int          labellength;
      bool         relabeling;
      int          pregion;

      std::string      full_statement;
      intpairs_t       full_pos;
      intpairs_store_t fp_store;

      void insert_in_full_pos(int i, int j)
      {
	 full_pos.push_back(intpair_t(i,j));
      }

      strings_t *input;
      strings_t *out;

      lines_t           c_lines;

   protected:

      std::deque <lines_t> cl_store;

      struct propstruct cur_rprop;
      Fortranline*      curline;
      ints_store_t      dolabels_store;
      ints_t            dolabels;        // to store labels, necessary for labelled do
      Findent*          fi;
      strings_t         fs_store;
      Globals*          gl;
      indent_store_t    indent_store;    // to store indent store
      bools_t           nbseen_store;    // to store nbseen
      Pre_analyzer      prea;
      struct propstruct prev_props;
      bool              refactor_end_found;
      rprops_store_t    rprops_store;
      rprops_t          rprops;          // to store routines (module, subroutine ...)

      int M(const int k)
      {
	 //
	 // used to delimit indentation
	 //
	 if (fi->flags.max_indent <= 0)
	    return k;
	 else
	    return std::min(fi->flags.max_indent,k);
      }

      Fortranline F(const std::string &s) { return Fortranline(gl,s); }


      int pop_indent(void) 
      { 
	 if (indent.empty()) 
	    return 0; 
	 indent.pop_back(); 
	 return top_indent(); 
      }


      int top_indent(void) 
      { 
	 if (indent.empty()) 
	    return 0; 
	 return indent.back(); 
      }

      int top_dolabel(void) 
      { 
	 if (dolabels.empty()) 
	    return -1; 
	 return dolabels.back(); 
      } 

      int pop_dolabel(void) 
      { 
	 if (dolabels.empty()) 
	    return -1; 
	 dolabels.pop_back(); 
	 return top_dolabel(); 
      }


      void empty_dolabels(void) 
      { 
	 while(!dolabels.empty()) 
	    dolabels.pop_back(); 
      }

      void push_rprops(struct propstruct p) { rprops.push_back(p); }

      void push_dolabel(int p) { dolabels.push_back(p); }

      struct propstruct top_rprops(void) 
      { 
	 if (rprops.empty()) 
	    return empty_rprop; 
	 return rprops.back();
      }

      struct propstruct pop_rprops(void) 
      { 
	 if (rprops.empty()) 
	    return empty_rprop; 
	 rprops.pop_back(); 
	 return top_rprops(); 
      }


      void push_all(void)
      {
	 dolabels_store.push_back(dolabels);
	 indent_store.push_back(indent);
	 nbseen_store.push_back(nbseen);
	 rprops_store.push_back(rprops);

      }         // end of push_all


      void top_all(void)
      {
	 if (!dolabels_store.empty())
	    dolabels = dolabels_store.back();
	 if (!indent_store.empty())
	    indent=indent_store.back();
	 if (!nbseen_store.empty())
	    nbseen=nbseen_store.back();
	 if (!rprops_store.empty())
	    rprops = rprops_store.back();
      }         // end of top_all

      void pop_all(void)
      {
	 if (!dolabels_store.empty())
	    dolabels_store.pop_back();
	 if (!indent_store.empty())
	    indent_store.pop_back();
	 if (!nbseen_store.empty())
	    nbseen_store.pop_back();
	 if (!rprops_store.empty())
	    rprops_store.pop_back();

      }        // end of pop_all

      void  handle_pre(Fortranline &line, bool &p_more);
      bool  is_findentfix(Fortranline &line);
      bool  output_pre(lines_t &lines, lines_t *outlines);
      void  output_line(bool ch);
      void  handle_refactor(void);
      Fortranline getnext(bool &eof, bool use_wb = 1);
      //Fortranline mygetline(bool &eof, bool keep_lines = 0);
      Fortranline mygetline(bool &eof);

   private:
      //
      // for get_full_statement and handle_pre:
      //
      bool first_call;
      bool f_more;
      bool p_more;
      int  pregentype;
      bool pushback;
      int  state;
      indents_t    indent;          // to store indents
      void push_indent(int p) { indent.push_back(p); }
      linebuffer_t wizardbuffer;    // to store wizard lines and push back lines from handle_fortran
      int          num_lines;
      size_t       mygetline_nr;
      int          lines_read;
      bool         nbseen;
      int          guess_indent(Fortranline line);
      void         rel_store(void);
      void         rel_flush(bool do_c_lines);
      void         rel_doit(void);
      void         rel_apply(void);
      void         rel_report(void);
      void         rel_summary(void);
      bool         rel_collect(const std::string &label);
      // for getnext:
      char         prevlastchar;
      bool         indent_handled;
      enum 
      {
	 start=1,
	 in_fortran, in_fortran_1,
	 in_pre, 
	 end_start, end_fortran, end_pre, 
	 in_ffix
      };

};

#endif
