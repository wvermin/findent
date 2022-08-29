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


#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "debug.h"
#include "flags.h"
#include "parser.hpp"

#define optargcheck if (!optarg || strlen(optarg)==0) break;

const int RELABEL_START     = 1000;
const int RELABEL_INCREMENT = 10;

void Flags::set_defaults(void)
{
   include_left_default = 0;
   default_indent       = 3;
   all_indent           = default_indent;

   align_paren          = 0;
   apply_indent         = 1;
   auto_firstindent     = 0;
   conchar              = ' ';
   deps                 = 0;
   do_shuffle           = 0;
   end_env_found        = 0;
   honour_omp           = 1;
   include_left         = include_left_default;
   indent_ampersand     = 0;
   input_format         = UNKNOWN;
   input_format_gnu     = 0;
   input_line_length    = 0;
   label_left           = 1;
   label_left_set       = 0;
   last_indent_only     = 0;
   last_usable_only     = 0;
   max_indent           = 100;
   only_fix_free        = 0;
   output_format        = 0;
   query_relabel        = 0;
   refactor_end         = 0;
   relabel              = 0;
   relabel_reset        = 1;
   relabel_start        = RELABEL_START;
   relabel_increment    = RELABEL_INCREMENT;
   return_format        = 0;
   safe                 = 0;
   start_indent         = 0;
   upcase_end_type      = 0;
   ws_remred            = 0;

   set_default_indents();
}

void Flags::set_default_indents()
{
   associate_indent    = all_indent;              // -a
   block_indent        = all_indent;              // -b
   case_indent         = all_indent-all_indent/2; // -c
   changeteam_indent   = all_indent;
   cont_indent         = all_indent;              // -k
   contains_indent     = all_indent;              // -C
   critical_indent     = all_indent;              // -x
   do_indent           = all_indent;              // -d
   entry_indent        = all_indent-all_indent/2; // -e
   enum_indent         = all_indent;              // -E
   forall_indent       = all_indent;              // -F
   if_indent           = all_indent;              // -f
   indent_continuation = 1;                       // !-k-
   indent_contain      = 1;                       // !-C-
   interface_indent    = all_indent;              // -j
   module_indent       = all_indent;              // -m
   routine_indent      = all_indent;              // -r
#ifdef USEESOPE
   segment_indent      = all_indent;
#endif
   select_indent       = all_indent;              // -s
   type_indent         = all_indent;              // -w
   where_indent        = all_indent;              // -w
}

int Flags::get_flags(int argc, char *argv[])
{
   char *envopts = getenv("FINDENT_FLAGS");
   char **allflags;
   char *envflags = strdup("");
   int nflags = 1;
   if (envopts != 0)
   {
      //
      // collect flags from environment:
      //
      envflags = strdup(envopts);
      //
      // malloc enough space for all flags:
      //
      allflags    = (char**) malloc(sizeof(char*)*(strlen(envflags)+argc+1));
      allflags[0] = argv[0];
      char *a  = strtok(envflags," \t:");
      while (a != 0)
      {
	 allflags[nflags++] = a;
	 a = strtok(0," \t:");
      }
   }
   else
   {
      allflags    = (char**) malloc(sizeof(char*)*(argc+1));
      allflags[0] = argv[0];
   }

   // add sentinel to separate environment flags frol command line flags

   allflags[nflags++] = (char*)"--end_of_env";

   for (int i = 1; i<argc; i++)
   {
      //
      // collect flags from command line:
      //
      allflags[nflags++] = argv[i];
   }

   struct option longopts[] =
   {
      {"indent"             , required_argument, 0, DO_INDENT            },

      {"continuation"       , required_argument, 0, DO_CONCHAR           },

      {"indent_associate"   , required_argument, 0, 'a'                  },
      {"indent-associate"   , required_argument, 0, 'a'                  },

      {"indent_block"       , required_argument, 0, 'b'                  },
      {"indent-block"       , required_argument, 0, 'b'                  },

      {"indent_case"        , required_argument, 0, 'c'                  },
      {"indent-case"        , required_argument, 0, 'c'                  },

      {"indent_contains"    , required_argument, 0, DO_INDENT_CONTAINS   },
      {"indent-contains"    , required_argument, 0, DO_INDENT_CONTAINS   },

      {"indent_do"          , required_argument, 0, 'd'                  },
      {"indent-do"          , required_argument, 0, 'd'                  },

      {"indent_entry"       , required_argument, 0, 'e'                  },
      {"indent-entry"       , required_argument, 0, 'e'                  },

      {"indent_enum"        , required_argument, 0, 'E'                  },
      {"indent-enum"        , required_argument, 0, 'E'                  },

      {"indent_if"          , required_argument, 0, 'f'                  },
      {"indent-if"          , required_argument, 0, 'f'                  },

      {"indent_forall"      , required_argument, 0, 'F'                  },
      {"indent-forall"      , required_argument, 0, 'F'                  },

      {"help"               , no_argument      , 0, 'h'                  },

      {"manpage"            , no_argument      , 0, 'H'                  },

      {"input_format"       , required_argument, 0, DO_INPUT_FORMAT      },
      {"input-format"       , required_argument, 0, DO_INPUT_FORMAT      },

      {"start_indent"       , required_argument, 0, 'I'                  },
      {"start-indent"       , required_argument, 0, 'I'                  },

      {"include_left"       , required_argument, 0, DO_INCLUDE_LEFT      },
      {"include-left"       , required_argument, 0, DO_INCLUDE_LEFT      },

      {"indent_interface"   , required_argument, 0, 'j'                  },
      {"indent-interface"   , required_argument, 0, 'j'                  },

      {"indent_continuation", required_argument, 0, 'k'                  },
      {"indent-continuation", required_argument, 0, 'k'                  },

      {"indent_ampersand"   , no_argument,       0, 'K'                  },
      {"indent-ampersand"   , no_argument,       0, 'K'                  },

      {"align_paren"        , optional_argument, 0, DO_ALIGN_PAREN       },
      {"align-paren"        , optional_argument, 0, DO_ALIGN_PAREN       },

      {"last_indent"        , no_argument      , 0, DO_LAST_INDENT       },
      {"last-indent"        , no_argument      , 0, DO_LAST_INDENT       },

      {"last_usable"        , no_argument      , 0, DO_LAST_USABLE       },
      {"last-usable"        , no_argument      , 0, DO_LAST_USABLE       },

      {"label_left"         , required_argument, 0, DO_LABEL_LEFT        },
      {"label-left"         , required_argument, 0, DO_LABEL_LEFT        },

      {"input_line_length"  , required_argument, 0, 'L'                  },
      {"input-line-length"  , required_argument, 0, 'L'                  },

      {"indent_module"      , required_argument, 0, 'm'                  },
      {"indent-module"      , required_argument, 0, 'm'                  },

      {"max_indent"         , required_argument, 0, 'M'                  },
      {"max-indent"         , required_argument, 0, 'M'                  },

      {"output_format"      , required_argument, 0, 'o'                  },
      {"output-format"      , required_argument, 0, 'o'                  },

      {"query_fix_free"     , no_argument      , 0, 'q'                  },
      {"query-fix-free"     , no_argument      , 0, 'q'                  },

      {"indent_procedure"   , required_argument, 0, 'r'                  },
      {"indent-procedure"   , required_argument, 0, 'r'                  },

      {"refactor_end"       , optional_argument, 0, DO_REFACTOR_END      },
      {"refactor-end"       , optional_argument, 0, DO_REFACTOR_END      },

      {"refactor_procedures", optional_argument, 0, DO_REFACTOR_END      }, // deprecated
      {"refactor-procedures", optional_argument, 0, DO_REFACTOR_END      }, // deprecated

      {"relabel"            , optional_argument, 0, DO_RELABEL           },

      {"relabel_reset"      , required_argument, 0, DO_RELABEL_RESET     },
      {"relabel-reset"      , required_argument, 0, DO_RELABEL_RESET     },

      {"query_relabel"      , optional_argument, 0, DO_QUERY_RELABEL     },
      {"query-relabel"      , optional_argument, 0, DO_QUERY_RELABEL     },

#ifdef USEESOPE
      {"indent_segment"     , required_argument, 0, DO_SEGMENT           },
      {"indent-segment"     , required_argument, 0, DO_SEGMENT           },
#endif
      {"indent_select"      , required_argument, 0, 's'                  },
      {"indent-select"      , required_argument, 0, 's'                  },

      {"indent_type"        , required_argument, 0, 't'                  },
      {"indent-type"        , required_argument, 0, 't'                  },

      {"version"            , no_argument      , 0, 'v'                  },

      {"indent_where"       , required_argument, 0, 'w'                  },
      {"indent-where"       , required_argument, 0, 'w'                  },

      {"indent_critical"    , required_argument, 0, 'x'                  },
      {"indent-critical"    , required_argument, 0, 'x'                  },

      {"indent_changeteam"  , required_argument, 0, DO_CHANGETEAM        },
      {"indent-changeteam"  , required_argument, 0, DO_CHANGETEAM        },

      {"vim_help"           , no_argument      , 0, DO_VIM_HELP          },
      {"vim-help"           , no_argument      , 0, DO_VIM_HELP          },

      {"gedit_help"         , no_argument      , 0, DO_GEDIT_HELP        },
      {"gedit-help"         , no_argument      , 0, DO_GEDIT_HELP        },

      {"vim_fortran"        , no_argument      , 0, DO_VIM_FORTRAN       },
      {"vim-fortran"        , no_argument      , 0, DO_VIM_FORTRAN       },

      {"vim_findent"        , no_argument      , 0, DO_VIM_FINDENT       },
      {"vim-findent"        , no_argument      , 0, DO_VIM_FINDENT       },

      {"gedit_external"     , no_argument      , 0, DO_GEDIT_EXTERNAL    },
      {"gedit-external"     , no_argument      , 0, DO_GEDIT_EXTERNAL    },

      {"gedit_plugin"       , no_argument      , 0, DO_GEDIT_PLUGIN      },
      {"gedit-plugin"       , no_argument      , 0, DO_GEDIT_PLUGIN      },

      {"gedit_plugin_py"    , no_argument      , 0, DO_GEDIT_PLUGIN_PY   },
      {"gedit-plugin-py"    , no_argument      , 0, DO_GEDIT_PLUGIN_PY   },

      {"emacs_help"         , no_argument      , 0, DO_EMACS_HELP        },
      {"emacs-help"         , no_argument      , 0, DO_EMACS_HELP        },

      {"emacs_findent"      , no_argument      , 0, DO_EMACS_FINDENT     },
      {"emacs-findent"      , no_argument      , 0, DO_EMACS_FINDENT     },

      {"ws_remred"          , optional_argument, 0, DO_WS_REMRED         },
      {"ws-remred"          , optional_argument, 0, DO_WS_REMRED         },

      {"readme"             , no_argument      , 0, DO_README            },

      {"safe"               , no_argument      , 0, DO_SAFE              },

      {"changelog"          , no_argument      , 0, DO_CHANGELOG         },

      {"makefdeps"          , no_argument      , 0, DO_MAKEFDEPS         },

      {"openmp"             , required_argument, 0, DO_OMP               },

      {"deps"               , no_argument,       0, DO_DEPS              },

      {"end_of_env"         , no_argument,       0, DO_END_ENV           },

      {0,0,0,0}
   };

   std::string option_name;
   int c;
   int retval       = DO_NOTHING;
   int option_index = 0;
   opterr           = 0;              // see man getopt_long
   const std::string allowed_conchars =
      " 0123456789"                   // ' ' and '0' have special meaning for findent
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "`~!@#$%^&*()-_=+[{]};:'\"|,<.>/?";

   while((c=getopt_long(nflags,allflags,
	       "a:b:c:C:d:e:E:f:F:hHi:I:j:k:Kl:L:m:M:o:qQr:R:s:t:vw:x:",
	       longopts, &option_index))!=-1)
   {
      switch(c)
      {
	 case 'a' :                            // --indent_associate=nn
	    optargcheck;
	    associate_indent  = atoi(optarg);
	    break;
	 case 'b' :
	    optargcheck;
	    block_indent      = atoi(optarg);  // --indent_block=nn
	    break;
	 case 'c' :
	    optargcheck;
	    case_indent       = atoi(optarg);  // --indent_case=nn
	    break;
	 case 'C' :                            // --indent_contains=nn/none
	    optargcheck;
	    if(strlen(optarg)>0 && optarg[0] == '-')
	       indent_contain = 0;
	    else
	       contains_indent   = atoi(optarg);
	    break;
	 case 'd' :                            // --indent_do=nn
	    optargcheck;
	    do_indent         = atoi(optarg);
	    break;
	 case 'e' :
	    optargcheck;
	    entry_indent      = atoi(optarg);  // --indent_entry=nn
	    break;
	 case 'E' :
	    optargcheck;
	    enum_indent       = atoi(optarg);  // --indent_enum=nn
	    break;
	 case 'f' :
	    optargcheck;
	    if_indent         = atoi(optarg);  // --indent_if=nn
	    break;
	 case 'F' :   
	    optargcheck;
	    forall_indent     = atoi(optarg);  // --indent_forall=nn
	    break;
	 case 'h' :                            // --help
	    if(end_env_found)
	       retval = DO_USAGE;
	    break;
	 case 'H':                             // --manpage
	    if(end_env_found)
	       retval = DO_MANPAGE;
	    break;
	 case 'i' :                            // --input_format=fixed/free/auto
	    optargcheck;
	    if      (std::string(optarg) == "fixed")
	       input_format = FIXED;
	    else if (std::string(optarg) == "free")
	       input_format = FREE;
	    else if (std::string(optarg) == "auto")
	       input_format = UNKNOWN;
	    else if (strlen(optarg)>0 && optarg[0] == '-')        // --indent=no/nn
	       apply_indent = 0;
	    else
	    {
	       all_indent = atoi(optarg);
	       set_default_indents();
	    }
	    break;
	 case 'I' :                          // --start_indent=nn/auto
	    optargcheck;
	    if (strlen(optarg)>0 && optarg[0] == 'a')
	       auto_firstindent = 1;
	    else
	    {
	       start_indent     = atoi(optarg);
	       auto_firstindent = 0;
	    }
	    break;
	 case 'j' :
	    optargcheck;
	    interface_indent  = atoi(optarg);  // --indent_interface=nn
	    break;
	 case 'k' :                           // --indent_continuation=nn/no
	    optargcheck;
	    if (strlen(optarg)>0 && (optarg[0] == 'd' || !strcmp(optarg,"default")))
	       indent_continuation = 1;
	    else if (strlen(optarg)>0 && (optarg[0] == '-' || !strcmp(optarg,"none")))
	       indent_continuation = 0;
	    else
	       cont_indent = atoi(optarg);
	    break;
	 case 'K': // --indent_ampersand
	    if (end_env_found)
	       indent_ampersand = 1;
	    break;
	 case 'l' :
	    optargcheck;
	    if(std::string(optarg) == "astindent")       // --last_indent
	    {
	       if(end_env_found)
		  last_indent_only = 1;
	    }
	    else if(std::string(optarg) == "astusable")  // --last_usable
	    {
	       if(end_env_found)
		  last_usable_only = 1;
	    }
	    else
	    {
	       label_left     = (atoi(optarg) != 0);     // --label_left=0/1
	       label_left_set = true;
	    }
	    break;
	 case 'L' :
	    optargcheck;
	    if (strlen(optarg)>0)
	    {
	       input_line_length = atoi(optarg);            // --input_line_length=nn
	       input_format_gnu  = (optarg[strlen(optarg)-1] == 'g');
	    }
	    break;
	 case 'm' :
	    optargcheck;
	    module_indent     = atoi(optarg);           // --indent_module=nn
	    break;
	 case 'M' :
	    optargcheck;
	    max_indent        = atoi(optarg);           // --max-indent=nn
	    break;
	 case 'o' :
	    optargcheck;
	    if(std::string(optarg) == "free")           // --output_format=free
	    {
	       output_format = FREE;
	       break;
	    }
	    if(std::string(optarg) == "fixed")           // --output_format=fixed
	    {
	       output_format = FIXED;
	       break;
	    }
	    if(std::string(optarg) == "same")            // --output_format=same
	    {
	       output_format = 0;
	       break;
	    }
	    break;
	 case 'q' :                                    // --query_fix_free
	    if (end_env_found)
	       only_fix_free = 1;
	    break;
	 case 'Q':
	    //
	    // return 2 if free, 4 if fixed
	    // not documented, maybe useful in the future
	    //
	    return_format = 1;
	    break;
	 case 'r' :                                    // --indent_procedure
	    optargcheck;
	    routine_indent    = atoi(optarg);
	    break;
	 case 'R':                                     // --refactor_end[=upcase]
	    optargcheck;
	    switch(optarg[0])
	    {
	       case 'R' :
		  upcase_end_type = 1;
		  // fall through
	       case 'r' :
		  refactor_end = 1;
		  break;
	    }
	    break;
	 case 's' :                                 // --indent_select=nn
	    optargcheck;
	    select_indent     = atoi(optarg);
	    break;
	 case 't' :
	    optargcheck;
	    type_indent       = atoi(optarg);       // --indent_type=nn
	    break;
	 case 'v' :
	    if(end_env_found)
	       retval = DO_VERSION;
	    break;
	 case 'w' :
	    optargcheck;
	    where_indent      = atoi(optarg);       // --indent_where=nn
	    break;
	 case 'x' :
	    optargcheck;
	    critical_indent   = atoi(optarg);       // --indent_critical=nn
	    break;
	 case DO_CHANGETEAM:
	    optargcheck;
	    changeteam_indent = atoi(optarg);       // --indent_changeteam=nn
	    break;
	 case DO_CONCHAR:
	    optargcheck;
	    if (strlen(optarg) > 0)
	       conchar=optarg[0];
	    if (allowed_conchars.find(conchar) == std::string::npos)
	       conchar = '&';
	    break;
	 case DO_DEPS:
	    if(end_env_found)
	       deps = 1;
	    break;
	 case DO_INCLUDE_LEFT:
	    optargcheck;
	    include_left       = (atoi(optarg) != 0);     // --include_left=0/1
	    break;
	 case DO_ALIGN_PAREN:
	    align_paren = 1;
	    if(optarg != 0 && strlen(optarg) > 0)
	    {
	       align_paren = (atoi(optarg) != 0);
	    }
	    break;
	 case DO_INDENT_CONTAINS:
	    optargcheck;
	    if (!strcmp(optarg,"restart"))
	       indent_contain = 0;
	    else
	       contains_indent = atoi(optarg);
	    break;
	 case DO_INPUT_FORMAT:
	    optargcheck;
	    if (!strcmp(optarg,"fixed"))
	       input_format = FIXED;
	    else if (!strcmp(optarg,"free"))
	       input_format = FREE;
	    else if (!strcmp(optarg,"auto"))
	       input_format = UNKNOWN;
	    break;
	 case DO_INDENT:
	    optargcheck;
	    if (!strcmp(optarg,"none"))
	       apply_indent = 0;
	    else
	    {
	       all_indent = atoi(optarg);
	       set_default_indents();
	    }
	    break;
	 case DO_LAST_INDENT:
	    if(end_env_found)
	       last_indent_only = 1;
	    break;
	 case DO_LAST_USABLE:
	    if(end_env_found)
	       last_usable_only = 1;
	    break;
	 case DO_LABEL_LEFT:
	    optargcheck;
	    label_left     = (atoi(optarg) != 0);     // --label_left=0/1
	    label_left_set = true;
	    break;
	 case DO_QUERY_RELABEL:
	    if(end_env_found)
	    {
	       query_relabel = 2;
	       relabel       = 1;
	       if(optarg != 0 && strlen(optarg) > 0)
	       {
		  query_relabel = atoi(optarg);
		  if(query_relabel < 0 || query_relabel > 2)
		     query_relabel = 0;
		  if(query_relabel == 0)
		     relabel = 0;
		  else
		     relabel = 1;
	       }
	    }
	    break;
	 case DO_REFACTOR_END:
	    refactor_end     = 1;
	    upcase_end_type  = 0;
	    if (optarg != 0 && strlen(optarg) > 0)
	    {
	       if (!strcmp(optarg,"upcase"))
		  upcase_end_type = 1;
	    }
	    break;
	 case DO_RELABEL:
	    relabel = 1;
	    if (optarg != 0 && strlen(optarg) > 0)
	    {
	       if(!strcmp(optarg,"shuffle"))
	       {
		  do_shuffle = 1;
	       }
	       else
	       {
		  do_shuffle = 0;
		  relabel_start = atoi(optarg);
		  char *p = strchr(optarg,',');
		  if (p)
		     relabel_increment = atoi(p+1);
		  if (relabel_start <= 0 || relabel_increment <= 0)
		     relabel = 0;
	       }
	    }
	    break;
	 case DO_WS_REMRED:
	    ws_remred = 1;
	    if(optarg != 0 && strlen(optarg) > 0)
	    {
	       ws_remred = atoi(optarg) != 0;
	    }
	    break;
	 case DO_RELABEL_RESET:
	    optargcheck;
	    relabel_reset = (atoi(optarg) != 0);
	    break;
	 case DO_SAFE:
	    safe          = 1;
	    end_env_found = 0;  // this is a hack: if we encounter --safe
	    //                     we enter the state as if FINDENT_FLAGS
	    //                     has not been parsed. 
	    break;
#ifdef USEESOPE
	 case DO_SEGMENT:
	    optargcheck;
	    segment_indent      = atoi(optarg);       // --indent_segment=nn
	    break;
#endif
	 case DO_OMP:
	    optargcheck;
	    if (atoi(optarg) == 0)
	       honour_omp = 0;
	    else
	       honour_omp = 1;
	    break;
	 case DO_END_ENV:
	    end_env_found = 1;
	    break;
	 case DO_CHANGELOG:
	 case DO_EMACS_FINDENT:
	 case DO_EMACS_HELP:
	 case DO_GEDIT_EXTERNAL:
	 case DO_GEDIT_HELP:
	 case DO_GEDIT_PLUGIN:
	 case DO_GEDIT_PLUGIN_PY:
	 case DO_MAKEFDEPS:
	 case DO_README:
	 case DO_VIM_FINDENT:
	 case DO_VIM_FORTRAN:
	 case DO_VIM_HELP:
	    if(end_env_found)
	       retval = c;
	    break;
      }
      if (retval != DO_NOTHING)
	 break;
   }

   free(allflags);
   free(envflags);
   return retval;
}
