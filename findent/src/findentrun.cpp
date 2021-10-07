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

#include "findentclass.h"
#include "free.h"
#include "fixed.h"
#include "fortran.h"
#include "debug.h"
#include "globals.h"


int Findent::run()
{

   handle_reading_from_tty();

   // copy whole input to input_buffer
   while(1)
   {
      bool eof;
      std::string s;
      getline(std::cin,s);
      eof = (std::cin.eof() && s == "");
      s = handle_dos(s);
      if (!eof && reading_from_tty)
	 eof = (s == ".");
      D(O(s););
      if (eof)
	 break;
      input_buffer.push_back(s);
   }

   input = &input_buffer;

   if (input_format == UNKNOWN)
      input_format = determine_fix_or_free(input);

   D(O(FIXED);O(FREE);O(input_format););

   gl->global_format      = input_format;

   if (flags.only_fix_free)
   {
      switch(input_format)
      {
	 case FREE:
	    std::cout << "free" << std::endl;
	    break;
	 case FIXED:
	    std::cout << "fixed" << std::endl;
	    break;
	 default:
	    std::cout << "free" << std::endl;
	    break;
      }
      return what_to_return();
   }

   strings_t relabel_out;

   //flags.relabel = 1;    // TTDD

   if(flags.relabel)
   {
      Fortran *relabel;
      switch (input_format)
      {
	 case FREE: relabel = new Free(this); break;
	 default:   relabel = new Fixed(this); break;
      }
      relabel->relabeling = true;
      relabel->input = input;
      relabel->out   = &relabel_out;
      relabel->init_relabel();
      relabel->do_relabel();
      input = &relabel_out;
      delete relabel;
   }

   if (flags.query_relabel != 0)
      return 0;

   Fortran *indent;

   switch (input_format)
   {
      case FREE: indent = new Free(this); break;
      default:   indent = new Fixed(this); break;
   }


   D(O("startindent ");O(flags.start_indent););
   indent->start_indent = flags.start_indent;

   output_format = flags.output_format;
   if(output_format == 0)
      output_format = input_format;

   // default we want labels right aligned for FIXED and left aligned
   // for FREE
   if(!flags.label_left_set)
   {
      flags.label_left = false;
      if(output_format == FREE)
	 flags.label_left = true;
   }

   indent->init_indent();
   indent->input = input;

   if (flags.last_usable_only)
   {
      mycout.setoutput(0);
      indent->handle_last_usable_only();
      delete indent;
      return what_to_return();
   }

   if (flags.last_indent_only || flags.deps)
      mycout.setoutput(0);

   indent->cur_indent = indent->start_indent;

   D(O("size: ");O(input->size()););

   while(1)
   {
      indent->full_statement = "";
      D(O("Full_statement1");O(indent->full_statement););
      indent->get_full_statement();

      D(O("FULL_statement2");O(indent->full_statement););
      DL({
	    for (size_t i = 0; i<indent->c_lines.size(); i++)
	    D(O("FULL_c_lines");O(indent->c_lines[i].str()););
	    });

      indent->indent_and_output();
      if (indent->end_of_file)
      {
	 if(flags.last_indent_only)
	    std::cout << num_leading_spaces(mycout.get()) << endline;
	 if(flags.deps)
	    output_deps();
	 delete indent;
	 return what_to_return();
      }
   }
   return what_to_return();  // never reached
}
