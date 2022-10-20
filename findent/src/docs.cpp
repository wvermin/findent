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
#include "docs.h"
#include "flags.h"
#include "version.h"
#include "functions.h"

bool Docs::print(int todo)
{
   switch(todo)
   {
      case Flags::DO_CHANGELOG:
         changelog();
         return 1;
      case Flags::DO_EMACS_FINDENT:
         emacs_findent();
         return 1;
      case Flags::DO_EMACS_HELP:
         emacs_help();
         return 1;
      case Flags::DO_GEDIT_EXTERNAL:
         gedit_external();
         return 1;
      case Flags::DO_GEDIT_HELP:
         gedit_help();
         return 1;
      case Flags::DO_GEDIT_PLUGIN:
         gedit_plugin();
         return 1;
      case Flags::DO_GEDIT_PLUGIN_PY:
         gedit_plugin_py();
         return 1;
      case Flags::DO_MAKEFDEPS:
         makefdeps();
         return 1;
      case Flags::DO_MANPAGE:
         usage(1);
         return 1;
      case Flags::DO_README:
         readme();
         return 1;
      case Flags::DO_USAGE:
         usage(0);
         return 1;
      case Flags::DO_VERSION:
         std::cout << "findent version "<<VERSION<<std::endl;
         return 1;
      case Flags::DO_VIM_FINDENT:
         vim_findent();
         return 1;
      case Flags::DO_VIM_FORTRAN:
         vim_fortran();
         return 1;
      case Flags::DO_VIM_HELP:
         vim_help();
         return 1;
      default:
         return 0;
   }
}

void Docs::usage(bool man)
{

   std::string no_env = "[NO_ENV]";
   if (man)
   {
      doman = 1;
      std::cout << ".\\\" DO NOT MODIFY THIS FILE! It was created by findent \\-H"                << std::endl;
      std::cout << ".TH FINDENT \"1\" \"2022\" \"findent\\-" << VERSION << "\" \"User Commands\"" << std::endl;
      std::cout << ".SH NAME"                                                                     << std::endl;
      std::cout << "findent \\- Indents, converts and relabels Fortran programs."                 << std::endl;
      std::cout << ".SH SYNOPSIS"                                                                 << std::endl;
      std::cout << ".B findent"                                                                   << std::endl;
      std::cout << "[\\fIOPTION\\fR]..."                                                          << std::endl;
      std::cout << ".PP"                                                                          << std::endl;
      std::cout << "Findent reads from STDIN and writes to STDOUT."                               << std::endl;
      std::cout << ".SH DESCRIPTION"                                                              << std::endl;
      std::cout << "Findent indents and optionally relabels a Fortran source."                    << std::endl;
      std::cout << "Findent can convert from fixed form to free form and vice-versa,"             << std::endl;
      std::cout << "and can supplement single END statements, see 'Refactor' below."              << std::endl;
      std::cout << "Comment lines with '!' in column one are not indented."                       << std::endl;
      std::cout << " You can correct findent related indenting errors by inserting comment"       << std::endl;
      std::cout << "lines: "                                                                      << std::endl;
      std::cout << " !  findentfix: <fortran statement>"                                          << std::endl;
      std::cout << " where <fortran statement> is for example DO, END, WHERE() etcetera."         << std::endl;
      std::cout << "Findent will adjust the indentation according to <fortran statement>."        << std::endl;
      std::cout << " Errors in OPTIONS are silently ignored."                                     << std::endl;
      std::cout << " Options marked with "+no_env+" are ignored if given via environment"         << std::endl;
      std::cout << "variable FINDENT_FLAGS."                                                      << std::endl;
      std::cout << " In the long options, you can replace '_' with '-'."                          << std::endl;
      std::cout << " Below: <n> denotes an unsigned decimal number."                              << std::endl;
      std::cout << "        <c> denotes a character."                                             << std::endl;
      std::cout << ".PP" << std::endl << ".SS \"General options:"                                 << std::endl;
   }
   else
   {
      doman = 0;
      std::cout << "findent [options]"                                                            << std::endl;
      std::cout << "   Findent reads from STDIN, writes to STDOUT."                               << std::endl;
      std::cout << "   Findent indents and optionally relabels a Fortran source."                 << std::endl;
      std::cout << "   Findent can convert from fixed form to free form and vice versa and"       << std::endl;
      std::cout << "   can supplement END statements, see 'Refactor' below."                      << std::endl;
      std::cout << "   Comment lines with '!' in column one are not indented."                    << std::endl;
      std::cout << "   You can correct findent related indenting errors by"                       << std::endl;
      std::cout << "    inserting comment lines: "                                                << std::endl;
      std::cout << "     !  findentfix: <fortran statement>"                                      << std::endl;
      std::cout << "    where <fortran statement> is for example DO, END, WHERE() etcetera."      << std::endl;
      std::cout << "    Findent will adjust the indentation according to <fortran statement>."    << std::endl;
      std::cout << " "                                                                            << std::endl;
      std::cout << "OPTIONS (errors are silently ignored):"                                       << std::endl;
      std::cout << " Options marked with "+no_env+" are ignored if given via environment"         << std::endl;
      std::cout << "   variable FINDENT_FLAGS."                                                   << std::endl;
      std::cout << " In the long options, you can replace '_' with '-'."                          << std::endl;
      std::cout << " Below: <n> denotes an unsigned decimal number."                              << std::endl;
      std::cout << "        <c> denotes a character."                                             << std::endl;
      std::cout                                                                                   << std::endl;
      std::cout << "  General options:"                                                           << std::endl;
   }

   manout(" "," ");
   manout("-h, --help                        ","print this text. "+no_env);
   manout("-H, --manpage                     ","print man page. "+no_env);
   manout("--readme                          ","print some background information. "+no_env);
   manout("--changelog                       ","print ChangeLog. "+no_env);
   manout("-v, --version                     ","prints findent version. "+no_env);
   manout("-q, --query_fix_free              ","guess free or fixed, prints 'fixed' or 'free' and exits. "+no_env);
   manout("--continuation=<c>                "," ' ': (default) do not change continuation characters.");
   manout(" "," '0': create numbered continuation characters.");
   manout(" "," other: use that continuation character.");
   manout(" "," default for conversion from free to fixed is '&'.");
   manout("--include_left=<n>                ","(0/1) 1: indent include statements to starting indent (default:0).");
   manout("-l<n>, --label_left=<n>           ","(0/1) 1: move statement labels to start of line.");
   manout(" ","      (default: free: 1, fixed: 0)");
   manout("-lastindent, --last_indent        ","prints computed indentation of last line.");
   manout(" ","      (for usage with vim) "+no_env);
   manout("-lastusable, --last_usable        ","prints line number of last line usable.");
   manout(" ","      as start for indenting(for usage with vim). "+no_env);
   manout("-iauto, --input_format=auto       ","determine automatically input format (free or fixed).");
   manout("-ifixed, --input_format=fixed     ","force input format fixed (default: auto).");
   manout("-ifree, --input_format=free       ","force input format free (default:auto).");
   manout("-i-, --indent=none                ","do not change indent (useful in combination with -R).");
   manout("-L<n>, --input_line_length=<n>    ","use only first <n> characters of each line.");
   manout(" ","      default=0: take whole lines.");
   manout("-L<n>g, --input_line_length=<n>g  ","same as above, but use gfortran convention.");
   manout(" ","      for counting the characters with tabbed lines.");
   manout(" ","      example: --input_line_length=72g.");
   manout("-M<n>, --max_indent=<n>           ","maximum output indent, default 100, 0: no limit.");
   manout("-ofixed, --output_format=fixed    ","force fixed format output.");
   manout("-ofree, --output_format=free      ","force free format output.");
   manout("-osame, --output_format=same      ","output format same is input format.");
   manout("--openmp=<n>                      ","1/0: do/don't indent openmp conditionals (default: 1).");
   manout(" ","      NOTE: for free format, the omp sentinel must be '!$ '.");
   manout("-Rr, --refactor_end               ","refactor subroutines etc: the END line.");
   manout(" "," of a subroutine, program etc. is, if possible, replaced by");
   manout(" "," 'end subroutine <name>' or");
   manout(" "," 'end function <name>' or");
   manout(" "," 'end procedure <name>' or");
   manout(" "," 'end program <name>' or");
   manout(" "," 'end block data <name>' or");
   manout(" "," 'end module <name>' or");
   manout(" "," 'end submodule <name>'");
   manout(" "," 'end interface <name>'");
   manout(" "," 'end type <name>'");
   manout(" "," where <name> is the name of the appropriate procedure, subroutine etc.");
   manout(" "," NOTE1: if the END line contains a continuation the results are undefined.");
   manout(" "," NOTE2: a line like 'end function fun' will be replaced by.");
   manout(" ","        'end subroutine sub' if the END line ends 'subroutine sub'.");
   manout(" "," NOTE3: the flag --refactor_procedures is deprecated, use --refactor_end.");
   manout("-RR, --refactor_end=upcase        ","same as -Rr, but 'END SUBROUTINE <name>'.");
   manout(" ","in stead of 'end subroutine <name>' etc.");
   manout("--relabel=shuffle                 ","shuffles existing labels,");
   manout(" "," primarily meant for demo and debugging.");
   manout(" "," see also the notes under --relabel below.");
   manout("--relabel, --relabel=<n1>,<n2>    ","renumber labels: n1 = starting index,");
   manout(" "," n2 is increment (default:1000,10).");
   manout(" "," if n1=0 or n2=0, relabeling is disabled.");
   manout(" "," see also RELABEL below.");
   manout(" "," NOTE1: use only SYNTAX-CORRECT SOURCES with this option.");
   manout(" "," NOTE2: CHECK the functionality of your program after using this option.");
   manout(" "," NOTE3: after finding an error (missing label, ill formatted label list, ...)");
   manout(" ","        findent will stop relabeling, but will continue indenting");
   manout(" ","        and/or converting.");
   manout("--relabel_reset=<n>           ","1: reset label numbering at start of subroutine/function/program.");
   manout(" ","0: do not reset label numbering (default: 1).");
   manout("--query_relabel[=<n>]             ","generate report. " + no_env);
   manout(" "," n=0, or flag omitted: do not generate report.");
   manout(" "," n=1: generate summary (only error message or OK message).");
   manout(" "," n=2: generate report of original and renumbered labels (default).");
   manout(" "," NOTE: if n >= 0, no output of the fortran source.");
   manout("--ws_remred[=<n>]                 ","replace redundant white space with one space.");
   manout(" "," n=0, or flag omitted: do not replace redundant spaces.");
   manout(" "," NOTE1: use with care, avoid pieces of code with not terminated");
   manout(" ","        strings and use Fortran code without syntax errors:");
   manout(" ","        changes are irreversible.");
   manout(" "," NOTE2: for statements containing an hollerith, this flag is ignored.");
   manout("--safe                            ","ignore next flags marked with "+no_env+" (used by wfindent).");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"Indenting options:" << std::endl;
   }
   else
   {
      std::cout << std::endl;
      std::cout << "  Indenting options:"                           << std::endl;
      std::cout << std::endl;
   }
   manout("-I<n>, --start_indent=<n>         ","starting  indent (default:0).");
   manout("-Ia, --start_indent=a             ","determine starting indent from first line.");
   Flags f;
   manout("-i<n>, --indent=<n>               ","all       indents except I,c,C,e (default: "+number2string(f.default_indent)+").");
   manout("-a<n>, --indent_associate=<n>     ","ASSOCIATE    indent.");
   manout("-b<n>, --indent_block=<n>         ","BLOCK        indent.");
   manout("-d<n>, --indent_do=<n>            ","DO           indent.");
   manout("-f<n>, --indent_if=<n>            ","IF           indent.");
   manout("-E<n>, --indent_enum=<n>          ","ENUM         indent.");
   manout("-F<n>, --indent_forall=<n>        ","FORALL       indent.");
   manout("-j<n>, --indent_interface=<n>     ","INTERFACE    indent.");
   manout("-m<n>, --indent_module=<n>        ","MODULE       indent.");
   manout("-r<n>, --indent_procedure=<n>     ","FUNCTION,");
   manout(" ",  " SUBROUTINE and PROGRAM indent.");
#ifdef USEESOPE
   manout("--indent_segment=<n>              ","SEGMENT      indent.");
#endif
   manout("-s<n>, --indent_select=<n>        ","SELECT       indent.");
   manout("-t<n>, --indent_type=<n>          ","TYPE         indent.");
   manout("-w<n>, --indent_where=<n>         ","WHERE        indent.");
   manout("-x<n>, --indent_critical=<n>      ","CRITICAL     indent.");
   manout("--indent_changeteam=<n>           ","CHANGE TEAM  indent.");
   manout("-C-, --indent_contains=restart,   ","restart indent after CONTAINS.");
   manout("-k<n>, --indent_continuation=<n>  ","continuation indent except   " );
   manout(" ","  for lines starting with '&' (see flag -K).");
   manout(" ","     NOTE: free to free only.");
   manout("-k-, --indent_continuation=none   ","continuation lines not preceded");
   manout(" ","  by '&' are untouched.");
   manout(" ","     NOTE: free to free only.");
   manout("-kd, --indent_continuation=default","continuation lines not preceded");
   manout(" ","  by '&' are default indented.");
   manout(" ","     NOTE: free to free only.");
   manout("-K, --indent_ampersand           ","indent lines starting with '&' (free only).");
   manout("--align_paren[=<n>]              ","align continuation lines not preceded");
   manout(" ","  by '&' with preceding unmatched left parenthesis.");
   manout(" "," n=0, or flag omitted: do not align.");
   manout(" "," n=1: do align (default).");
   manout(" ","     NOTE1: free to free only.");
   manout(" ","     NOTE1: for statements containing an hollerith, this flag is ignored.");
   manout("  ","Next defaults are: all - all/2.");
   manout("-c<n>, --indent_case=<n>          ","CASE      negative indent.");
   manout(" ","     NOTE: also for RANK, TYPEIS and CLASSIS.");
   manout("-C<n>, --indent_contains=<n>      ","CONTAINS  negative indent.");
   manout("-e<n>, --indent_entry=<n>         ","ENTRY     negative indent.");
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS" << std::endl;
   }
   std::cout << "Dependencies:"                << std::endl;
   manout("--deps                            ","output dependency information only, other flags are ignored. "+no_env);
   manout(" "," This can be used to generate a dependencies file for usage with make(1). ");
   manout(" "," The format of this information:");
   manout(" "," Fortran source      ->        findent output");
   manout(" ","  include \"file1\"    ->        inc file1");
   manout(" ","  #include \"file2\"   ->        cpp file2");
   manout(" ","  #include <file3>   ->        std file3");
   manout(" ","  ??include 'file4'  ->        coc file4");
   manout(" ","  use module1        ->        use module1");
   manout(" ","  submodule(m) subm  ->        use m");
   manout(" ","                               mod m:subm");
   manout(" ","  module module2     ->        mod module2");
   manout("--makefdeps                       ","outputs a sh(1) script that serves as a an example");
   manout(" "," to generate dependencies for use in make(1). "+no_env);
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"Environment:" << std::endl;
   }
   else
   {
      std::cout << "Environment:" << std::endl;
   }
   manout(" ","findent parses the environment variable FINDENT_FLAGS before the command-line flags.");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"Usage with vim:" << std::endl;
   }
   else
   {
      std::cout << "Usage with vim:" << std::endl;
   }
   manout("--vim_help                        "  ,"outputs directions to use findent in (g)vim. "+no_env);
   manout("--vim_fortran                     "  ,"outputs file 'fortran.vim', see --vim_help. "+no_env);
   manout("--vim_findent                     "  ,"outputs file 'findent.vim', see --vim_help. "+no_env); 
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"Usage with gedit:" << std::endl;
   }
   else
   {
      std::cout << "Usage with gedit:" << std::endl;
   }
   manout("--gedit_help                      "  ,"outputs directions to use findent in gedit. "+no_env);
   manout("--gedit_external                  "  ,"outputs script 'findent-gedit', see --gedit_help. "+no_env);
   manout("--gedit_plugin                    "  ,"outputs file 'findent.plugin', see --gedit_help. "+no_env);
   manout("--gedit_plugin_py                 "  ,"outputs file 'python.py', see --gedit_help. "+no_env);
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"Usage with emacs:" << std::endl;
   }
   else
   {
      std::cout << "  Usage with emacs:" << std::endl;
   }
   manout("--emacs_help                      "   ,"outputs directions to use findent in emacs. "+no_env);
   manout("--emacs_findent                   "   ,"outputs script 'findent.el', see --emacs_help. "+no_env);
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS \"RELABEL:" << std::endl;
   }
   else
   {
      std::cout << "RELABEL:" << std::endl;
   }
   manout(" ","The following constructs are candidates for relabeling:");
   manout(" "," - 100,110,120,130: label");
   manout(" "," - X:         integer or real or logical expression");
   manout(" "," - I:         identifier");
   manout(" "," - [,]:       optional comma");
   manout(" "," - ...:       not parsed by findent, can be anything except '=...'");
   manout(" "," - [IF]:      optional IF(...)");
   manout(" ","");
   manout(" "," 100 ...         ! statement or format label");
   manout(" "," IF(...) 110,120,130");
   manout(" "," IF(...) 110,120");
   manout(" "," DO 100[,] I=X,..."); 
   manout(" "," DO 100[,] WHILE(...)");
   manout(" "," DO 100[,] CONCURRENT(...)");
   manout(" "," DO 100");
   manout(" "," [IF] ACCEPT 100...");
   manout(" "," [IF] ASSIGN 100 TO I");
   manout(" "," [IF] BACKSPACE(...,ERR=100,...)");
   manout(" "," [IF] CALL I(...,*100,...,$110,...,&120,...)");
   manout(" "," [IF] CLOSE(...,ERR=100,...)");
   manout(" "," [IF] DECODE(...,100,...,ERR=110,...)");
   manout(" "," [IF] DELETE(...,ERR=110,...)");
   manout(" "," [IF] ENCODE(...,100,...,ERR=110,...)");
   manout(" "," [IF] ENDFILE(...,ERR=100,...)");
   manout(" "," [IF] FIND(...,ERR=110,...)");
   manout(" "," [IF] FLUSH(...,ERR=100,...)");
   manout(" "," [IF] GOTO 100");
   manout(" "," [IF] GOTO I[,](100,110,120)");
   manout(" "," [IF] GOTO(100,110,120)...");
   manout(" "," [IF] INQUIRE(...,ERR=100,...)");
   manout(" "," [IF] OPEN(...,ERR=100,...)");
   manout(" "," [IF] PRINT 100...");
   manout(" "," [IF] READ 100...");
   manout(" "," [IF] READ(...,100,...)");
   manout(" "," [IF] READ(...,ERR=100,...,END=110,EOR=120,...,FMT=130)...");
   manout(" "," [IF] REREAD 100...");
   manout(" "," [IF] REREAD(...,100,...)");
   manout(" "," [IF] REREAD(...,ERR=100,...,END=110,EOR=120,...,FMT=130)...");
   manout(" "," [IF] REWIND(...,ERR=100,...)");
   manout(" "," [IF] REWRITE(...,100) ...");
   manout(" "," [IF] REWRITE(...,ERR=100,...,FMT=110,...,EOR=120)...");
   manout(" "," [IF] TYPE 100...");
   manout(" "," [IF] WAIT(...,ERR=100,...,END=110,...,EOR=120,...)");
   manout(" "," [IF] WRITE(...,100) ...");
   manout(" "," [IF] WRITE(...,ERR=100,...,FMT=110,...,EOR=120)...");
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS" << std::endl;
   }
   std::cout << "Examples:"                    << std::endl;
   manout(" ","Indent: findent < in.f > out.f");
   manout(" ","        findent -i2 -r0 < in.f > out.f");
   manout(" ","");
   manout(" ","Relabel: findent --relabel < in.f > out.f");
   manout(" ","");
   manout(" ","Convert fixed to free form: findent -ofree < prog.f > prog.f90");
   manout(" ","");
   manout(" ","Convert free to fixed form: findent -ofixed < prog.f90 > prog.f");
   manout(" ","");
   manout(" ","Refactor 'end': findent -Rr < in.f90 > out.f90");
   manout(" "," ");
   if(doman)
   {
      std::cout << ".PP" << std::endl << ".SS" << std::endl;
   }
   std::cout << "BUGS:"                    << std::endl;
   manout("*","Also for free-format, findent is space-insensitive, while");
   manout(" ","the standard states that space must be used as general separator.");
   manout(" ","");
   manout("*","There are some issues with labels in a continuation");
   manout(" ","when converting from free to fixed format.");
   manout(" ","For example:");
   manout(" ","");
   manout(" ","\n123&\n 4 continue");
   manout(" ","");
   manout(" ","The problems arise because it is not possible to define a");
   manout(" ","statement label in a continuation in fixed format.");
   manout(" ","");
   manout("*","When converting from fixed format to free format, findent");
   manout(" ","discards white space in a string if the string contains a");
   manout(" ","continuation, e.g:");
   manout(" ","      print *,\"a");
   manout(" ","     +b\"");
   manout(" ","");
   manout(" ","is converted to:");
   manout(" ","   print *,\"a&");
   manout(" ","   &b\"");
   manout("*","Relabel issues");
   manout(" "," In some cases, findent refuses to relabel.");
   manout(" "," To see the cause, use the flag '--query-relabel=1', e.g.:");
   manout(" ","  findent --relabel=10,10 --query-relabel=1");
   manout(" "," If the fortran source contains label-related errors, or when");
   manout(" ","findent misbehaves, relabeling can create havoc. So: test.");


   if(doman)
   {
      std::cout << ".SH DONATIONS" << std::endl;
      std::cout << ".br"           << std::endl;
   }
   else
   {
      std::cout << "DONATIONS"     << std::endl;
   }
   manout(" ","Donations to support the maintenance of this program are welcome: ");
   manout(" ","https://paypal.me/wvermin");
   if(doman)
   {
      std::cout << ".SH COPYRIGHT" << std::endl;
      std::cout << ".br"           << std::endl;
   }
   else
   {
      std::cout << std::endl;
      std::cout << "COPYRIGHT"     << std::endl;
   }
   manout(" ","This is free software; see the source for copying conditions.");
   manout(" ","There is NO warranty; not even for MERCHANTABILITY or FITNESS");
   manout(" ","FOR A PARTICULAR PURPOSE.");
}

void Docs::replaceAll( std::string &s, const std::string &search, const std::string &replace ) 
{
   //
   // https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
   //
   for( size_t pos = 0; ; pos += replace.length() ) {
      //
      // Locate the substring to replace
      //
      pos = s.find( search, pos );
      if( pos == std::string::npos ) break;
      //
      // Replace by erasing and inserting
      //
      s.erase( pos, search.length() );
      s.insert( pos, replace );
   }
}

//
// doman == 1: output in man page format
// flag: " ": normal continuation line
//       otherwize : skip to new paragraph and use bold format
// txt: Line to output
//
void Docs::manout(const std::string flag, const std::string txt)
{

   std::string mantxt  = txt;
   if (doman)
   {
      std::string manflag = flag;
      replaceAll(mantxt,"-","\\-");
      replaceAll(manflag,"-","\\-");
      if (manflag == " ")
         std::cout << mantxt << std::endl;
      else
      {
         std::cout << ".TP" << std::endl << "\\fB"<<manflag<<"\\fR"<<std::endl;
         std::cout << mantxt << std::endl;
      }
   }
   else
   {
      std::size_t start = 0;
      if (mantxt.size() > 0)
         if(mantxt[0] == '\n')
            mantxt = mantxt.substr(1);
      replaceAll(mantxt,"\n","\n\t  ");
      if (flag == " ")
         std::cout << flag << " " << "                " << mantxt.substr(start) << std::endl;
      else
         std::cout << flag << " " << ": " << mantxt.substr(start) << std::endl;
   }
}

void Docs::vim_help()
{
#include "vim_help.inc"
}

void Docs::vim_findent()
{
#include "vim_findent.inc"
}

void Docs::vim_fortran()
{
#include "vim_fortran.inc"
}

void Docs::gedit_help()
{
#include "gedit_help.inc"
}

void Docs::gedit_external()
{
#include "gedit_external.inc"
}

void Docs::gedit_plugin()
{
#include "gedit_plugin.inc"
}

void Docs::gedit_plugin_py()
{
#include "gedit_plugin_py.inc"
}

void Docs::emacs_help()
{
#include "emacs_help.inc"
}

void Docs::emacs_findent()
{
#include "emacs_findent.inc"
}

void Docs::readme()
{
#include "readme.inc"
}

void Docs::makefdeps()
{
#include "makefdeps.inc"
}

void Docs::changelog()
{
#include "changelog.inc"
}
