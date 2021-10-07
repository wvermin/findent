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

#ifndef FINDENTCLASS_H
#define FINDENTCLASS_H

#include <set>
#include <utility>  // pair
#include <string>

#include "flags.h"
#include "prop.h"
#include "findent_types.h"
#include "debug.h"
#include "fortranline.h"
#include "globals.h"

class Findent
{

   public:
      Findent() {}

      Findent(Flags &f)
      {
	 //
	 // public
	 //
	 gl                 = new Globals;
	 cur_rprop          = empty_rprop;
	 endline            = "\n";
	 flags              = f;

	 gl->global_format      = UNKNOWN;
	 gl->global_gnu_format  = flags.input_format_gnu;;
	 gl->global_line_length = flags.input_line_length;
	 gl->global_omp         = flags.honour_omp;

	 //
	 // private
	 //
	 all_indent         = 0;
	 endline_set        = 0;
	 input_format       = flags.input_format;
	 output_format      = 0;
	 reading_from_tty   = 0;
      }

      ~Findent() { delete gl; }


      void         handle_pre_light(Fortranline &line, bool &p_more);
      void         init_indent();
      int          run();

      std::string  endline;
      Flags        flags;
      Globals*     gl;
      int          input_format;
      int          output_format;

      std::set <std::pair<int,std::string> > includes;
      strings_t *input;

   private:

      int          all_indent;
      prop_t       cur_rprop;
      bool         endline_set;
      bool         reading_from_tty;

      int          determine_fix_or_free(strings_t *input);
      int          guess_fixedfree(Fortranline &line);
      std::string  handle_dos(const std::string &s);
      void         handle_reading_from_tty();
      void         output_deps();
      std::string  type2str(const int t);
      int          what_to_return();


      // for handle_pre_light:
      int pregentype;
      strings_t input_buffer;
};

#endif
