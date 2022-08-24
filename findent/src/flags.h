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

#ifndef FLAGS_H
#define FLAGS_H
class Flags
{
   void set_defaults(void);
   void set_default_indents();

   public:

   Flags()
   {
      set_defaults();
   }

   int get_flags(int argc, char *argv[]);

   enum {
      DO_NOTHING = 1000,
      DO_CHANGELOG,
      DO_CHANGETEAM,
      DO_CONCHAR,
      DO_DEPS,
      DO_EMACS_FINDENT,
      DO_EMACS_HELP,
      DO_END_ENV,
      DO_GEDIT_EXTERNAL,
      DO_GEDIT_HELP,
      DO_GEDIT_PLUGIN,
      DO_GEDIT_PLUGIN_PY,
      DO_INCLUDE_LEFT,
      DO_ALIGN_PAREN,
      DO_INDENT,
      DO_INDENT_CONTAINS,
      DO_INPUT_FORMAT,
      DO_LABEL_LEFT,
      DO_LAST_INDENT,
      DO_LAST_USABLE,
      DO_MAKEFDEPS,
      DO_MANPAGE,
      DO_OMP,
      DO_QUERY_RELABEL,
      DO_README,
      DO_REFACTOR_END,
      DO_RELABEL,
      DO_RELABEL_RESET,
      DO_SEGMENT,
      DO_SAFE,
      DO_USAGE,
      DO_VERSION,
      DO_VIM_FINDENT,
      DO_VIM_FORTRAN,
      DO_VIM_HELP,
      DO_WS_REMRED
   };

   bool apply_indent         ; // 1: output indented line, else output original lines
   bool auto_firstindent     ;
   char conchar              ; // continuation character output by fixed2fixed, see manpage
   bool deps                 ; // output dependencies only
   bool end_env_found        ; // signal that parsing flags from environment has been done
   bool include_left         ; // 1: put include on the start of the line
   bool include_left_default ;
   bool indent_continuation  ;
   bool indent_ampersand     ; // indent continuation lines starting with ampersand, too
   bool indent_contain       ;
   bool input_format_gnu     ;
   bool label_left           ; // 1: put statement labels on the start of the line
   bool label_left_set       ;
   bool last_indent_only     ;
   bool last_usable_only     ;
   int  max_indent           ; // maximum allowed indent
   bool only_fix_free        ; // 1: determine only if fixed or free (-q)
   bool refactor_end         ; // 1: refactor end statements
   bool relabel              ; // 1: relabel when possible
   bool relabel_reset        ; // 1: restart from relabel_start in every function/subroutine/program
   int  relabel_start        ; // startnumber of relabeling
   int  relabel_increment    ; // increment of relabeling
   bool return_format        ; // 1: return 2 if format==free, 4 if format==fixed
   bool safe                 ; // 1: disable flages like -q, default: 0
   bool ws_remred            ; // 1: remove redundand space
   bool upcase_end_type      ; // 1: use 'END SUBROUTINE' etc in stead of 'end subroutine'
   bool honour_omp           ; // 1: treat openmp continuation lines as code,
   //                             0: treat them as comment
   int  all_indent;
   int  associate_indent;
   int  block_indent;
   int  case_indent;
   int  changeteam_indent;
   int  cont_indent;
   bool align_paren;
   int  contains_indent;
   int  critical_indent;
   int  default_indent;
   int  do_indent;
   int  entry_indent;
   int  enum_indent;
   int  forall_indent;
   int  if_indent;
   int  input_format;
   int  input_line_length;
   int  interface_indent;
   int  module_indent;
   int  output_format;
   int  query_relabel;
   int  routine_indent;
#ifdef USEESOPE
   int  segment_indent;
#endif
   int  select_indent; 
   int  start_indent;
   int  type_indent;
   int  where_indent;
};
#endif
