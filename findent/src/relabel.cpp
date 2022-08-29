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
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <assert.h>
#include "findent_types.h"
#include "fortran.h"
#include "relabel.h"
#include "debug.h"
#include "functions.h"
#include "fortranline.h"

typedef std::deque<lines_t> lines_store_t;

static lines_store_t                           c_lines_store;
static intpairs_store_t                        full_pos_store;
static strings_t                               full_statement_store;
static std::map <std::string,std::string>      labels;
static std::map <std::string,int>              labeldefs;
static std::map <std::string,std::deque<int> > labeluses;
static std::string                             label_num;
static std::string                             subfunprog,subfunprogname;
static std::string                             error_type;

static std::string check_label_pos(const std::string &fs, lines_t &cl, const intpairs_t &fp, int &cor, int p, 
      const std::string &prefix, bool relabel_it, bool is_stlabel);

static std::string rel_check_labels(void);

static std::string ints2string(const ints_t &list);

static long long int seed;
static double        myrandom(void);
static unsigned int  myrandint(unsigned int n);
static void          smyrandom(unsigned long long i);
static void          mypermutate(int *a, unsigned int n);
static void          permutate_labels(void);

//generate random number [0-1)
// From Communications of the ACM, Vol 31 Oct 1988 number 10
// pp 1192..1201
// Stephen K. Park and Keith W. Miller
//
// Not thread-safe because of seed
// 
// Could have used drand48(), but I need reproducible random
// numbers to be able to check correct functioning of permutate_labels()
//
double myrandom()
{
   const int a=16807;
   const int m=2147483647;
   seed = (a*seed) % m;
   return (double)seed/m;
}

// returns random integer m, such that 0 <= m < n
unsigned int myrandint(unsigned int n)
{
   return myrandom()*n;
}

// set seed
void smyrandom(unsigned long long i)
{
   seed = i;
}

// random permutate array a.
// https://en.wikipedia.org/wiki/Random_permutation#Fisher-Yates_shuffles
// n (input) number of elements in a
void mypermutate(int *a, unsigned int n)
{
   (void) mypermutate;  // not called, only used as inspiration for permutate_labels()
   int x;
   for (unsigned int i=0; i<n-1; i++)
   {
      unsigned int j = i + myrandint(n-i);
      x    = a[i];
      a[i] = a[j];
      a[j] = x;
   }
}

void permutate_labels(void)
{
   std::map<std::string,std::string>::iterator it;
   std::vector<std::string> l,m;

   int n = labels.size();
   for (it = labels.begin(); it!=labels.end(); ++it)
   {
      l.push_back(it->first);  // use it->second if you want to use generated labels
      m.push_back(it->first);
   }
   std::string x;

   for (int i=0; i<n-1; ++i)
   {
      unsigned int j = i + myrandint(n-i);
      x    = l[i];
      l[i] = l[j];
      l[j] = x;
   }

   for (int i=0; i<n; ++i)
      labels[m[i]] = l[i];

   l.clear();
   m.clear();
}

void Fortran::init_relabel()
{
   c_lines_store.clear();
   full_pos_store.clear();
   full_statement_store.clear();
   labels.clear();
   labeldefs.clear();
   labeluses.clear();
   if (fi->flags.do_shuffle)
   {
      smyrandom(1);
      myrandom(); 
      myrandom();
   }
}

//
// In this text, SUBROUTINE stands for SUBROUTINE/FUNCTION/PROGRAM
//               END stands for END/END FUNCTION/END SUBROUTINE/END PROGRAM/CONTAINS
// read complete input and do relabel processing.
//
// Method: read until SUBROUTINE is encountered. Then: read until END storing the
// source lines. Then relabel, and find the next SUBROUTINE.
//
// Since relabeling is a process that can easily destroy the
// semantics of a program, relabeling is switched off after: 
// - encountering SUBROUTINE  or END  as part of a multi-statement line,
//   examples:
//     end ; subroutine foo
//     subroutine foo(x) ; real x
//     continue ; end
// - finding a label that is used but not defined
// - encountering an SUBROUTINE statement after an SUBROUTINE statement before an END statement
//
// Note: 
// a program like:
//   program prog
//   goto 10
//   10 end program
// is allowed according to the standard, but this label will not been seen in the following case:
//   program prog
//   goto 10
//   contains
//   subroutine sub
//   end subroutine
//   10 end program
// (Also gfortran gives an error: "Error: Label 10 referenced at (1) is never defined",
//  so we are in good company, albeit that ifort compiles the thing correctly)
//

void Fortran::do_relabel()
{
   enum
   {
      outside = 1,
      inside,
      error,
      flush   = 101,
      store,
      doit,
   };

   int state        = outside;
   bool error_found = 0;
   label_num = number2string(fi->flags.relabel_start);

   //
   // following loop is over the complete input
   //
   while(1)
   {
      D(O("error_found: ");O(error_found););
      if(end_of_file)
      {
	 rel_flush(1);
	 break;
      }
      get_full_statement();
      if (error_found)
      {
	 rel_flush(1);
	 continue;
      }
      std::string rest  = full_statement;
      intpairs_t  prest = full_pos;

      D(O(full_statement);O("full in relabel"););
      DL(
	    for (size_t i=0; i<c_lines.size(); i++)
	    D(O(c_lines[i].str());O(i);O("c_lines"););
	);

      int item_nr   = 0;
      bool do_flush = true;
      //
      // following loop is over the parts of a multi-statement line
      //

      while(1)
      {
	 item_nr++;

	 Line_prep p = Line_prep(rest,&prest);
	 propstruct props = parseline(p); 
	 intpairs_t pc          = p.get_pos_encoded();
	 strings_t labels_found = props.labels;
	 ints_t label_pos       = props.label_pos;
	 std::string labstring;
	 propstruct  labs;

	 int first_line, last_line;
	 first_line = 10000000;
	 last_line  = 0;
	 int pregion;


	 switch(state)
	 {
	    case outside:
	       subfunprog = "";
	       switch(props.kind)
	       {
		  case PROCEDURE:
		     if (subfunprog.size() == 0)
			subfunprog = "procedure";
		     //fall through
		  case SUBROUTINE:
		     if (subfunprog.size() == 0)
			subfunprog = "subroutine";
		     // fall through
		  case FUNCTION:
		     if (subfunprog.size() == 0)
			subfunprog = "function";
		     // fall through
		  case PROGRAM:
		     if (subfunprog.size() == 0)
			subfunprog = "program";
		     subfunprogname = props.name;
		     if (item_nr == 1 && p.get_line_rest().size() == 0)
		     {
			state     = inside;
			if (fi->flags.relabel_reset)
			   label_num = number2string(fi->flags.relabel_start);
			labels.clear();
			labeldefs.clear();
			labeluses.clear();
			rel_flush(1);
			break;
		     }
		     state = error;
		     error_type += subfunprog;
		     error_type += " statement part of multi statement line ("
			+ number2string(num_lines) + ");";
		     if (item_nr == 1)
			rel_store();
		     break;
		  default:
		     rel_flush(1);
		     break;
	       }
	       break;  // end case outside
	    case inside:
	       D(O(props.kind););
	       switch(props.kind)
	       {
		  case SUBROUTINE:
		  case FUNCTION:
		  case PROGRAM:
		     //
		     // an unexpected SUBROUTINE statement
		     //
		     state = error;
		     error_type += "misplaced ";
		     switch(props.kind)
		     {
			case SUBROUTINE:
			   error_type += "subroutine";
			   break;
			case FUNCTION:
			   error_type += "function";
			   break;
			case PROGRAM:
			   error_type += "program";
			   break;
		     }

		     error_type += " statement (" + number2string(num_lines) + ");";
		     if(item_nr == 1)
			rel_store();
		     break;
		  case CALL:
		  case IO:
		  case GOTO_COMPUTED:
		  case GOTO_ASSIGNED:
		     D(O("io or computed/assigned goto"););
		     D(O(props.lrvalue);O(props.lrpos););
		     labstring = props.lrvalue;
		     D(O("labstring");O(labstring););
		     switch(props.kind)
		     {
			case IO:
			   labs = parseio(labstring);
			   break;
			case CALL:
			   labs = parsecall(labstring);
			   break;
			default:
			   labs = parselabels(labstring);
			   break;
		     }
		     switch (labs.kind)
		     {
			case CALLLIST:
			case LABELLIST:
			case IOLIST:
			   D(O("CALL/LABEL/IO_LIST");O(props.lrpos););
			   D(O(p.get_line_encoded()););
			   DL(
				 for(size_t i =0; i<labs.labels.size(); ++i)
				 D(O(i);O(labs.labels[i]););
			     );
			   labels_found = labs.labels;
			   label_pos.clear();
			   for (size_t i = 0; i< labels_found.size(); ++i)
			   {
			      label_pos.push_back(props.lrpos + labs.label_pos[i]);
			      D(O(props.lrpos);O(labs.label_pos[i])O(props.lrpos + labs.label_pos[i]););
			   }
			   break;
			default:
			   D(O(labs.kind););
			   D(O("labs_error"););
			   state = error;
			   error_type += "error in ";
			   switch(props.kind)
			   {
			      case CALL:
				 error_type += "call parameters";
				 break;
			      case IO:
				 error_type += "I/O parameters list";
				 break;
			      case GOTO_COMPUTED:
				 error_type += "computed GOTO list";
				 break;
			      case GOTO_ASSIGNED:
				 error_type += "assigned GOTO list";
				 break;
			   }
			   error_type += " (" + number2string(num_lines) + ");";
			   break;
		     }
		     // fall through
		  case DO:
		  case GOTO:
		  case NWAYIF:
		  case ASSIGN:
		  case READ:
		  case PRINT:
		     pregion = c_lines[0].preregion();
		     D(O("do-goto-nwayif-assign-read");O(labels_found.size()););
		     for (size_t num = 0; num < labels_found.size(); num++)
		     {
			std::string l = labels_found[num];
			D(O(l););
			std::string lclean;
			unsigned int n;
			getstr_sp(l,lclean,n);
			if (labels.count(lclean) == 0)
			   labels[lclean] = "";
			labeluses[lclean].push_back(num_lines);
			DL({
			      for(size_t i = 0; i<pc.size(); i++)
			      {
			      D(O(i);O(pc[i].first);O(pc[i].second););
			      }
			      });
			DL(
			      D(O(c_lines.size()););
			      for (size_t i = 0; i < c_lines.size(); ++i)
			      D(O(c_lines[i]););
			  );
			// where is this label to be found in c_lines_store?
			int line_in_c_lines    = pc[label_pos[num]].first;
			int col_in_c_lines     = pc[label_pos[num]].second;

			if (!c_lines[line_in_c_lines].pre())
			{
			   D(O(c_lines[line_in_c_lines].str()););
			   if(first_line > line_in_c_lines)
			      first_line = line_in_c_lines;
			   if(last_line < line_in_c_lines)
			      last_line = line_in_c_lines;
			   D(O("first, last");O(first_line);O(last_line););
			}

			D(O(label_pos[num]);O(line_in_c_lines);O(col_in_c_lines););
			D(O(rest);O("re"););
			D(O(c_lines[line_in_c_lines].str());O("cu"););
			// now get the value of the label from c_lines:
			unsigned int m;
			std::string checklabel;
			getstr_sp(c_lines[line_in_c_lines].str().substr(col_in_c_lines), checklabel, m);
			D(O(props.kind);O(p.get_line_encoded()););
			if (checklabel != lclean)
			{
			   D(O("label_error"););
			   D(O(l);O(checklabel););
			   state = error;
			   error_type += "label not on one line: "+ l + "(" 
			      +number2string(num_lines) + ")";
			   break;
			}
		     }
		     // fall through
		  default:
		     {
			D(O("first, last");O(first_line);O(last_line););
			for(int i = first_line; i <=last_line; ++i)
			{
			   D(O(pregion);O(c_lines[i].preregion());O(c_lines[i].str()););
			   if (c_lines[i].preregion() != pregion)
			   {
			      D(O("intervening preprocessor statement");O(pregion);O(c_lines[i].preregion()););
			      state = error;
			      error_type += "intervening preprocessor statement ("+
				 number2string(num_lines) +");";
			      break;
			   }
			}
		     }
		     if(props.stlabel.size()>0)
		     {
			//
			// check for consistency of label
			//
			int dummy;
			std::string n = check_label_pos(rest, c_lines, prest, dummy, 0, "", false,true);
			D(O(rest);O(n););
			if (n.size() == 0 )
			{
			   state = error;
			   error_type += "internal error;";
			}
			else
			{
			   bool rc = rel_collect(props.stlabel);
			   if (!rc)
			   {
			      state = error;
			      D(O("label out of range"););
			      error_type += "label would go out of range: "
				 +props.stlabel + " -> "+label_num+" ("+number2string(num_lines) + ");";
			   }
			}
		     }
		     if(item_nr == 1)
			rel_store();
		     break;
	       }
	       if(state == error)
	       {
		  D(O("state = error");O(error_type););
		  break;
	       }
	       switch(props.kind)
	       {
		  case END:
		  case ENDPROGRAM:
		  case ENDFUNCTION:
		  case ENDSUBROUTINE:
		  case ENDPROCEDURE:
		  case CONTAINS:
		     //
		     // only valid if this is not part of a multiline statement
		     //
		     D(O("end found"););
		     if (item_nr != 1 || p.get_line_rest().size() != 0)
		     {
			std::string x = "END";
			if (props.kind == CONTAINS)
			   x = "CONTAINS";
			state = error;
			error_type += x+" in multi statement line ("+number2string(num_lines)+");";
		     }

		     if (rel_check_labels().size() != 0)
		     {
			state = error;
			error_type += "undefined label: " + rel_check_labels() + " (";
			error_type += ints2string(labeluses[rel_check_labels()]);
			error_type += ");";
		     }
		     if(state == error)
			do_flush = false;
		     else
		     {
			state = outside;
			rel_doit();
			rel_flush(0);
			break;
		     }
		     break;
	       }
	       break;  // end case inside
	    case error:
	       if(do_flush)
		  rel_flush(1);
	       error_found = 1;
	       break;
	 }
	 if(error_found || state == error)
	 {
	    D(O("Relabel impossible");O(error_type););
	    break;
	 }
	 rest  = p.get_line_rest();
	 D(O(rest););
	 if (rest.size() == 0)
	    break;
	 prest = p.get_pos_rest();
      }
   }
   switch(fi->flags.query_relabel)
   {
      case 0:
	 return;
      case 1:
      case 2:
	 rel_summary();
	 break;
   }
}

std::string rel_check_labels()
{
   for (std::map<std::string,std::string>::iterator it=labels.begin(); it!=labels.end(); ++it)
   {
      D(O("map:check ");O(it->first);O(" -> ");O(it->second););
      if (it->second.size() == 0)
	 return it->first;
   }
   return "";
}

bool Fortran::rel_collect(const std::string &label)
{
   if (label.size() == 0)
      return true;
   D(O(label);O(label_num););
   if (label_num.size() > 5)
      return false;
   std::string mylabel;
   unsigned int n;
   getstr_sp(label,mylabel,n);    // remove leading zero's embedding blanks ...
   D(O(label);O(mylabel););
   if(labels[mylabel].size() > 0)   // this label was already defined, maybe in a preprocessor suite.
      return true;
   labels[mylabel]    = label_num;
   labeldefs[mylabel] = num_lines;
   unsigned long int x = string2number<unsigned long int>(label_num) + fi->flags.relabel_increment;
   label_num = number2string(x);
   D(O(label);O(mylabel);O(label_num);O(x););
   return true;
}

void Fortran::rel_store()
{
   full_statement_store.push_back(full_statement);
   full_pos_store.push_back(full_pos);
   c_lines_store.push_back(c_lines);
}

void Fortran::rel_flush(bool do_c_lines)
{
   while(c_lines_store.size() > 0)
   {
      while(c_lines_store[0].size() > 0)
      {
	 Fortranline *line = &c_lines_store[0].front();
	 D(O("WRITE_rel");O(line->written());O(line->str()););
	 if(!line->written())
	    out->push_back(line->str());
	 c_lines_store[0].pop_front();
      }
      c_lines_store.pop_front();
   }
   c_lines_store.clear();
   full_statement_store.clear();
   full_pos_store.clear();

   if(do_c_lines)
      while (c_lines.size() > 0)
      {
	 Fortranline *line = &c_lines.front();
	 D(O("WRITE_rel");O(line->written());O(line->str()););
	 if(!line->written())
	    out->push_back(line->str());
	 c_lines.pop_front();
      }
}

void Fortran::rel_doit()
{
   if (fi->flags.do_shuffle)
   {
      //
      //  here we shuffle labels
      //
      permutate_labels();
   }
   switch(fi->flags.query_relabel)
   {
      case 0:
	 rel_apply();
	 break;
      case 2:
	 rel_report();
	 break;
   }
}

void Fortran::rel_summary()
{
   if(error_type.size() == 0)
      std::cout << "!No relabeling errors found" << std::endl;
   else
      std::cout << "!Relabeling error: " << error_type << std::endl;
}

void Fortran::rel_report()
{
   std::cout << "!" << subfunprog << " " << subfunprogname << ":" << std::endl;
   std::cout << "!original(def'd )     new (used)" << std::endl;

   for (std::map <std::string,std::string>::iterator it=labels.begin(); it != labels.end(); ++it)
   {
      std::cout << "!   " << std::setw(5) << it->first << 
	 "(" << std::setw(6) << labeldefs[it->first] << ") -> " << 
	 std::setw(5) << it->second 
	 << "(";
      std::cout << ints2string(labeluses[it->first]) << ")" << std::endl;
   }
}

std::string ints2string(const ints_t &list)
{
   std::string s;
   for (size_t i = 0; i < list.size(); ++i)
   {
      s += number2string(list[i]);
      if (i != list.size()-1)
	 s += ",";
   }
   return s;
}

void Fortran::rel_apply()
{

   DL(
   for (std::map <std::string,std::string>::iterator it=labels.begin(); it != labels.end(); ++it)
   {
      D(O("map: ");O(it->first);O(" -> ");O(it->second););
   }
   );

   //
   // loop over stored lines
   //
   for (size_t line_nr=0; line_nr<full_statement_store.size(); line_nr++)
   {
      std::string line   = full_statement_store[line_nr];
      intpairs_t  pos    = full_pos_store[line_nr];
      lines_t     *lines = &c_lines_store[line_nr];

      D(O(line_nr);O(line););
      //
      // loop over statements of multi-statement line eg: goto 10; if(x) 30,40,50
      //
      while(1)
      {
	 Line_prep p = Line_prep(line,&pos);
	 D(O(line););
	 DL(
	       {
	       for (size_t i = 0; i<line.size(); i++)
	       {
	       std::string l = (*lines)[pos[i].first].str();
	       D(O(pos[i].first);O(pos[i].second);O(l[pos[i].second]););
	       }
	       });
	 DL(assert(line.size() == pos.size()););

	 DL(assert(p.get_line_rest().size() == p.get_pos_rest().size()););

	 propstruct props       = parseline(p); 
	 intpairs_t  pc         = p.get_pos_encoded();
	 std::string sc         = p.get_line_encoded();
	 strings_t labels_found = props.labels;
	 ints_t label_pos       = props.label_pos;
	 std::string labstring;
	 propstruct  labs;

	 int k;
	 if(pc.size()>0)
	    k   = pc[0].first;  // line nr in multi-line statement
	 else                   // happens with a blank line
	    k = 0;
	 int cor = 0;
	 size_t l = props.stlabel.size();
	 if (l > 0) // this line starts with a label
	 {
	    D(O(pos[0].first);O(pos[0].second); O(props.stlabel););
	    std::string n = check_label_pos(line, (*lines), pos, cor, 0, "", true, true);
	    D(O(cor);O(pos[0].first);O(pos[0].second););
	    (void)n;
	    D(O(n););
	 }

	 D(O(line.size());O(pos.size()););
	 DL(assert(pos.size() == line.size()););
	 if(cor)
	 {
	    for (size_t i = 0; i < pc.size(); ++i)
	    {
	       if(pc[i].first == k)
		  pc[i].second += cor;
	    }
	    for (size_t i = 0; i < pos.size(); ++i)
	    {
	       if(pos[i].first == k)
	       {
		  pos[i].second += cor;
		  D(O(i);O(cor);O(k);O(pos[i].second);O((*lines)[k][pos[i].second]););
	       }
	    }
	 }
	 DL(assert(pos.size() == line.size()););

	 int extracor = 0;
	 switch(props.kind)
	 {
	    case CALL:
	    case IO:
	    case GOTO_COMPUTED:
	    case GOTO_ASSIGNED:
	       D(O("computed/assigned goto"););
	       D(O(props.lrvalue);O(props.lrpos););
	       D(O(p.get_line_encoded()););
	       labstring = props.lrvalue;
	       switch(props.kind)
	       {
		  case IO:
		     labs = parseio(labstring);
		     break;
		  case CALL:
		     labs = parsecall(labstring);
		     break;
		  default:
		     labs = parselabels(labstring);
	       }
	       switch (labs.kind)
	       {
		  case CALLLIST:
		  case LABELLIST:
		  case IOLIST:
		     D(O("CALL/LABEL/IO_LIST");O(props.lrpos););
		     D(O(p.get_line_encoded()););
		     DL(
			   for(size_t i =0; i<labs.labels.size(); ++i)
			   D(O(i);O(labs.labels[i]););
		       );
		     labels_found = labs.labels;
		     label_pos.clear();
		     for (size_t i = 0; i< labels_found.size(); ++i)
		     {
			label_pos.push_back(props.lrpos + labs.label_pos[i]);
			D(O(props.lrpos);O(labs.label_pos[i])O(props.lrpos + labs.label_pos[i]););
		     }
		     break;
	       }
	       // fall through
	    case DO:
	    case GOTO:
	    case NWAYIF:
	    case ASSIGN:
	    case READ:
	    case PRINT:
	       for(size_t num = 0; num < labels_found.size(); ++num)
	       {
		  int k = pc[label_pos[num]].first;   // line nr in multiline statement
						      // note: label_pos gives position in encoded line
		  D(O(pc.size());O(sc.size());O(sc);O(num););
		  DL(assert(sc.size() == pc.size()););
		  int q              = pc[label_pos[num]].second;
		  std::string prefix = leading_blanks((*lines)[k].str().substr(q));

		  D(O((*lines)[k].str());O(sc);O(k);O(q);O(label_pos[num]););
		  D(O(prefix);O((*lines)[k].str().substr(q));O(label_pos[num]);O(labels_found[num].size()););

		  check_label_pos(sc, *lines, pc, extracor, label_pos[num], prefix, true, false);

		  D(O(labels_found[num]);O(sc);O(label_pos[num]););

		  int curk = pc[label_pos[num]].first;
		  D(O(curk);O(extracor);O(label_pos[num]);O(pc.size()););

		  if(extracor)
		  {
		     for (size_t i = label_pos[num]; i < pc.size(); ++i)
		     {
			if(pc[i].first == curk)
			   pc[i].second += extracor;
		     }
		     for (size_t i = 0; i < pos.size(); ++i)
		     {
			// sloppy correction of pos. If already visited locations in 
			// pos are to be used later on, more precise correction is in order,
			// as approximately in:
			//if(pos[i].first == curk && pos[i].second >= label_pos[num])
			if(pos[i].first == curk /*&& pos[i].second >= label_pos[num]-2*/) // todo why -2?
			   pos[i].second += extracor;
		     }
		  }
	       }
	       break;
	 }


	 line = p.get_line_rest();  // this is ok
	 D(O(pos.size());O(line.size());O(line););
	 //pos  = p.get_pos_rest(); // this is not ok, instead: do not use original pos, but shorten existing pos, using
	 // length of previous and current line.
	 size_t new_size = p.get_pos_rest().size();
	 while (pos.size() > new_size)
	    pos.pop_front();

	 if (line.size() == 0)
	    break;
      }
   }
}

//
// check if label at position p of statement(fs) c_lines(cl) and pos(fp)
// given info in full_pos_store
// returns the number of chars used for the label in c_lines. 
// 0 signifies an error
//
std::string check_label_pos(const std::string &fs, lines_t &cl, const intpairs_t &fp, int &cor, 
      int p, const std::string &prefix, bool relabel_it, bool is_stlabel)
{
   DL(assert(fs.size() == fp.size()););
   cor = 0;
   std::string label_in_f;
   unsigned int m;

   // get label from fs
   getstr_sp(fs.substr(p), label_in_f, m);
   D(O(m);O(label_in_f););
   if (m < 1)
      return "";

   // get label from c_lines
   std::string label_in_c;
   unsigned int n;
   int k = fp[p].first;
   int q = fp[p].second;

   Fortranline *clk = &cl[k];

   D(O(q);O(clk->str());O(clk->str().substr(q));O(is_stlabel););

   if (is_stlabel)
   {
      // if this is a statement label, find the position where it starts, including spaces and tabs
      while(q > 0)
      {
	 char c = clk->str()[q-1];
	 D(O(c););
	 if (c == ' ' || c == '\t')
	    q--;
	 else
	    break;
      }
   }

   getstr_sp(clk->str().substr(q), label_in_c, n);

   D(O(clk->str());O(relabel_it););
   D(O(clk->str().substr(q));O(relabel_it););
   D(O(label_in_f);O(label_in_c);O(p);O(k);O(q);O(relabel_it););
   if ((label_in_f != label_in_c) || label_in_f.size() == 0)
      return "";

   if (relabel_it && !clk->written())
   {
      //
      // replace in cl
      //

      int oldsize = clk->str().size();
      std::string sl = prefix+labels[label_in_f];
      D(O(label_in_f);O(sl);O("q n");O(q);O(n););
      // add as many blanks after the label as in the original case
      std::string trailing_blanks = "";
      for (size_t i = q+n-1; i!=0; --i)
      {
	 char c = clk->str()[i];
	 if (isblank(c))
	    trailing_blanks = trailing_blanks+c;
	 else
	    break;
      }
#ifdef equallength
      // if new label field is smaller than old one, add spaces
      // new label: sl+trailing_blanks.
      int e = n-sl.size()-trailing_blanks.size();
      if (e < 0)
	 e = 0;
      D(O(n);O(sl.size());O("bl.size");O(trailing_blanks.size());O(e););
      trailing_blanks = std::string(e,' ')+trailing_blanks;
#endif
      //
      // if we are handling a statement label, make sure that
      // it occupies at least 6 characters
      //

      if (is_stlabel && clk->format() == FIXED)
      {
	 sl = std::string(5-sl.size(),' ') + sl;
	 if(trailing_blanks.size() < 1)
	    trailing_blanks = ' ';
      }
      else
      {
	 //
	 // gfortran-8, compiling free format thinks that
	 //   goto i(10,20,30 ) 
	 // is erroneous, while
	 //   goto i(10,20,30)
	 // is ok.

	 // so, we will not add spaces between the label and ')'.

	 if (clk->str()[q+n] == ')')
	    trailing_blanks="";
      }
      // some peculiarities with repect to OpenMP:
      if (is_stlabel && clk->omp())
      {
	 sl = ltrim(sl);
	 if(clk->format() == FREE)
	    sl = " " + ltrim(sl); // space before label required
	 else 
	 {
	    // to make sure that the label fills the first 6 colums
	    int n = 3-sl.size();
	    if(n>0)
	       sl = std::string(n,' ')+sl;
	    trailing_blanks = " ";  // continuation character
	 }
      }

      std::string one   = clk->str().substr(0,q);
      std::string two   = sl+trailing_blanks;
      std::string three = clk->str().substr(q+n);
      D(O(one);O(two);O(three););
      clk->str(one+two+three);
      cor = clk->str().size()-oldsize;

      D(O(cor);O(p);O(fp.size());O(oldsize););
   }
   return label_in_f;
}


