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

%{
#include <iostream>
#include <deque>
#include <string>

#include "debug.h"
#include "lexer.h"
#include "line_prep.h"
#include "parser.hpp"

#define YY_DECL extern "C" int yylex()

#define YY_USER_ACTION line_pos += yyleng;

std::deque<int> start_stack;
std::string     lexer_name, lexer_stlabel, lexeri_number, lexer_rest, lexer_string, lexer_lr;
int             lexer_lrpos;
std::string     lexer_include;
std::string     input_string;
int             lrlevel;
bool            lrreturn;
Line_prep       pline;
int             line_pos;

%}

   /* double [ gives problems in lexer comments */

%s state_identifier state_keyword state_esope state_cidentifier 

%x state_lr state_lrb state_skip state_skipall state_skipnoop state_error
%x state_findformat state_scanfixpre state_scanompfixed state_scanompfree
%x state_incfilename state_char state_labels state_iolist
%x state_startiolist state_calllist state_startcalllist state_skipnos

b [ \t]

  //  following are keywords starting with c
ckey  call|close|common|continue|case|contains|cycle|class|codimension|contiguous|critical|complex|changeteam
%%

<state_error>.  {  D(O("ERROR");Y;); return ERROR; }

<state_lr>{
"("             { D(O("lrplus");O(lrlevel);Y;); 
                   lrlevel++;
		   if (lrlevel > 1)
		      lexer_lr += yytext;
		}

")"             { D(O("lrmin");O(lrlevel);Y;); 
                  lrlevel--;
		  if(lrlevel == 0)
		  {
		     D(O("state:");O(start_stack.back());O(lrreturn););
		     BEGIN(start_stack.back());
		     start_stack.pop_back();
		     if(lrreturn)
		     {
			D(O("LR");Y); return LR;
		     }
		  }
		  else
		  {
		      lexer_lr += yytext;
		  }
		}

.               { D(O("lrbla");O(lrlevel);Y;); 
                    lexer_lr += yytext;
		} 
\n              { D(O("lreol");O(lrlevel);Y;); yyless(0); BEGIN(start_stack.back()); }

}

<state_lrb>{
"["             { D(O("lrb+");O(lrlevel);Y;); 
                  lrlevel++;
		}

"]"             { D(O("lrb-");O(lrlevel);Y;); 
                  lrlevel--;
		  if(lrlevel == 0)
		  {
		     D(O("state:");O(start_stack.back());O(lrreturn););
		     BEGIN(start_stack.back());
		     start_stack.pop_back();
		     if(lrreturn)
		     {
			return LRB;
		     }
		  }
		}

.               { D(O("lrbbla");O(lrlevel);Y;); 
		}

\n              { D(O("lrbeol");O(lrlevel);Y;) 
                  yyless(0);
                  BEGIN(start_stack.back());
		}

}

<state_skip,state_skipall,state_skipnoop,state_skipnos,state_iolist,state_calllist>{
"("          {  D(O("skipLPAREN");Y;);
		start_stack.push_back(YY_START);
		lrlevel  = 1;
		lrreturn = 0;
		lexer_lr = "";
		lexer_lrpos = line_pos; 
		BEGIN(state_lr);
	     }

"["          {  D(O("skipLPAREN");Y;);
		start_stack.push_back(YY_START);
		lrlevel  = 1;
		lrreturn = 0;
		BEGIN(state_lrb);
	     }
}

<state_skip>"="             { D(O("NOSKIP");Y;);     return yytext[yyleng-1]; }
<state_skipnoop>[,|~!#$]    { D(O("NOSKIPNOOP");Y;); return yytext[yyleng-1]; }
<state_skipnos>":"          { D(O("NOSKIPNOS");Y;);  return yytext[yyleng-1]; }

<state_skip,state_skipall,state_skipnoop,state_skipnos>{
\n           { D(O("endSKIP");Y;);
               switch(YY_START)
	       {
                  case state_skip:
		     return SKIP;
	          case state_skipall:
		     return SKIPALL;
		  case state_skipnoop:
		     return SKIPNOOP;
		  case state_skipnos:
		     return SKIPNOS;
	       }
	     }

.            { D(O("SKIP");Y;); }
}

<state_identifier>{
[[:alpha:]][[:alnum:]_$]*  { D(O("IDENTIFIER");Y;);  lexer_name = yytext; return IDENTIFIER;  }
[[:alpha:]][[:alnum:]_$%]* { D(O("TIDENTIFIER");Y;); lexer_name = yytext; return TIDENTIFIER; }
}

<state_cidentifier>{
[[:alpha:]][[:alnum:]_$]*,{0,1}   { D(O("IDENTIFIER");Y;);   lexer_name = yytext; return IDENTIFIER;  }
[[:alpha:]][[:alnum:]_$%]*,{0,1}  { D(O("TIDENTIFIER");Y;);  lexer_name = yytext; return TIDENTIFIER; }
}

<state_incfilename>{  /* to be used in '#include <file>' */
[^>]+                      { D(O("INCFILENAME");Y;); lexer_string = yytext; 
                             BEGIN state_char; 
			     return INCFILENAME; }
}

<state_char>{
\n                            {  D(O("EOL"););   return EOL;        }
.                             {  D(O("BLA");Y;); return yytext[0];  }
}

<state_esope,state_keyword>{
abstractinterface       { D(O("ABSTRACTINTERFACE");Y;); return ABSTRACTINTERFACE; }
associate               { D(O("ASSOCIATE");Y;);         return ASSOCIATE;         }
block                   { D(O("BLOCK");Y;);             return BLOCK;             }
blockdata               { D(O("BLOCKDATA");Y;);         return BLOCKDATA;         }
case                    { D(O("CASE");Y);               return CASE;              }
casedefault             { D(O("CASEDEFAULT");Y);        return CASEDEFAULT;       }
character               { D(O("CHARACTER");Y;);         return BASICTYPE;         }
changeteam              { D(O("CHANGETEAM");Y);         return CHANGETEAM;        }
class                   { D(O("CLASS");Y;);             return CLASS;             }
classdefault            { D(O("CLASSDEFAULT");Y);       return CLASSDEFAULT;      }
	/* see comment at TYPEIS */
classis/\(              { D(O("CLASSIS");Y);            return CLASSIS;           }
complex                 { D(O("COMPLEX");Y;);           return BASICTYPE;         }
contains                { D(O("CONTAINS");Y);           return CONTAINS;          }
critical                { D(O("CRITICAL");Y;);          return CRITICAL;          }
do                      { D(O("DO");Y;);                return DO;                }
do,                     { D(O("DO,");Y;);               return DOCOMMA;           }
doconcurrent            { D(O("DOCONCURRENT");Y;);      return DOCONCURRENT;      }
do,concurrent           { D(O("DO,CONCURRENT");Y;);     return DOCONCURRENT;      }
dowhile                 { D(O("DOWHILE");Y;);           return DOWHILE;           }
do,while                { D(O("DO,WHILE");Y;);          return DOWHILE;           }
doublecomplex           { D(O("DOUBLECOMPLEX");Y;);     return BASICTYPE;         }
doubleprecision         { D(O("DOUBLEPRECISION");Y;);   return BASICTYPE;         }
elemental               { D(O("ELEMENTAL");Y;);         return ELEMENTAL;         }
else                    { D(O("ELSE");Y);               return ELSE;              }
elseif                  { D(O("ELSEIF");Y);             return ELSEIF;            }
elsewhere               { D(O("ELSEWHERE");Y);          return ELSEWHERE;         }
end                     { D(O("END");Y;);               return END;               }
endassociate            { D(O("ENDASSOCIATE");Y;);      return ENDASSOCIATE;      }
endblock                { D(O("ENDBLOCK");Y;);          return ENDBLOCK;          }
endblockdata            { D(O("ENDBLOCKDATA");Y;);      return ENDBLOCKDATA;      }
endcritical             { D(O("ENDCRITICAL");Y;);       return ENDCRITICAL;       }
enddo                   { D(O("ENDDO");Y;);             return ENDDO;             }
endenum                 { D(O("ENDENUM");Y;);           return ENDENUM;           }
endforall               { D(O("ENDFORALL");Y;);         return ENDFORALL;         }
endfunction             { D(O("ENDFUNCTION");Y;);       return ENDFUNCTION;       }
endif                   { D(O("ENDIF");Y);              return ENDIF;             }
endinterface            { D(O("ENDINTERFACE");Y;);      return ENDINTERFACE;      }
endmodule               { D(O("ENDMODULE");Y;);         return ENDMODULE;         }
endprocedure            { D(O("ENDPROCEDURE");Y;);      return ENDPROCEDURE;      }
endprogram              { D(O("ENDPROGRAM");Y;);        return ENDPROGRAM;        }
endselect               { D(O("ENDSELECT");Y);          return ENDSELECT;         }
endsubroutine           { D(O("ENDSUBROUTINE");Y;);     return ENDSUBROUTINE;     }
endsubmodule            { D(O("ENDSUBMODULE");Y;);      return ENDSUBMODULE;      }
endteam                 { D(O("ENDTEAM");Y);            return ENDTEAM;           }
endtype                 { D(O("ENDTYPE");Y;);           return ENDTYPE;           }
endwhere                { D(O("ENDWHERE");Y;);          return ENDWHERE;          }
entry                   { D(O("ENTRY");Y);              return ENTRY;             }
enum                    { D(O("ENUM");Y;);              return ENUM;              }
forall                  { D(O("FORALL");Y;);            return FORALL;            }
function                { D(O("FUNCTION");Y;);          return FUNCTION;          }
if                      { D(O("IF");Y;);                return IF;                }
impure                  { D(O("IMPURE");Y;);            return IMPURE;            }
include                 { D(O("INCLUDE");Y;);           return INCLUDE;           }
integer                 { D(O("INTEGER");Y;);           return BASICTYPE;         }
interface               { D(O("INTERFACE");Y;);         return INTERFACE;         }
interface/[[:alpha:]_$] { D(O("INTERFACE1");Y;);        return INTERFACE1;        }
logical                 { D(O("LOGICAL");Y;);           return BASICTYPE;         }
module                  { D(O("MODULE");Y);             return MODULE;            }
moduleprocedure         { D(O("MODULEPROCEDURE");Y);    return MODULEPROCEDURE;   }
modulesubroutine        { D(O("MODULESUBROUTINE");Y);   return MODULESUBROUTINE;  }
modulefunction          { D(O("MODULEFUNCTION");Y);     return MODULEFUNCTION;    }
procedure               { D(O("PROCEDURE");Y);          return PROCEDURE;         }
program                 { D(O("PROGRAM");Y;);           return PROGRAM;           }
pure                    { D(O("PURE");Y;);              return PURE;              }
rank                    { D(O("RANK");Y);               return RANK;              }
rankdefault             { D(O("RANKDEFAULT");Y);        return RANKDEFAULT;       }
real                    { D(O("REAL");Y;);              return BASICTYPE;         }
recursive               { D(O("RECURSIVE");Y;);         return RECURSIVE;         } 
non_recursive           { D(O("NON_RECURSIVE");Y;);     return NON_RECURSIVE;     } 
selectcase              { D(O("SELECTCASE");Y);         return SELECTCASE;        }
selectrank              { D(O("SELECTRANK");Y);         return SELECTRANK;        }
selecttype              { D(O("SELECTTYPE");Y);         return SELECTTYPE;        }
submodule               { D(O("SUBMODULE");Y;);         return SUBMODULE;         }
subroutine              { D(O("SUBROUTINE");Y;);        return SUBROUTINE;        }
then                    { D(O("THEN");Y;);              return THEN;              }
type                    { D(O("TYPE");Y;);              return TYPE;              }
type/\(                 { D(O("TYPEC");Y;);             return TYPEC;             }
	/* To disambiguate between 'TYPE ISMYTYPE' and TYPE IS(MYTYPE), the lexer */
	/* recognizes 'TYPEIS(' and returns "TYPEIS" */
	/* Note that the '(' stays in the input buffer */
typeis/\(               { D(O("TYPEIS");Y);             return TYPEIS;            }
use                     { D(O("USE");Y);                return USE;               }
where                   { D(O("WHERE");Y;);             return WHERE;             }

      /* following are only used while relabeling */

assign                  { D(O("ASSIGN");Y;);            return ASSIGN;            }
call                    { D(O("CALL");Y;);              return CALL;              }
goto                    { D(O("GOTO");Y;);              return GOTO;              }
print                   { D(O("PRINT");Y;);             return PRINT;             }
read                    { D(O("READ");Y;);              return READ;              }
reread                  { D(O("REREAD");Y;);            return READ;              }      /* DEC fortran */
to                      { D(O("TO");Y;);                return TO;                }
write                   { D(O("WRITE");Y;);             return WRITE;             }
backspace/\(            { D(O("IO");Y;);                return IO;                }
close/\(                { D(O("IO");Y;);                return IO;                }
endfile/\(              { D(O("IO");Y;);                return IO;                }
flush/\(                { D(O("IO");Y;);                return IO;                }
inquire/\(              { D(O("IO");Y;);                return IO;                }
open/\(                 { D(O("IO");Y;);                return IO;                }
read/\(                 { D(O("IO");Y;);                return IO;                }
reread/\(               { D(O("IO");Y;);                return IO;                }      /* DEC fortran */
rewind/\(               { D(O("IO");Y;);                return IO;                }
wait/\(                 { D(O("IO");Y;);                return IO;                }
write/\(                { D(O("IO");Y;);                return IO;                }
decode/\(               { D(O("IO");Y;);                return IO;                }      /* DEC fortran */
encode/\(               { D(O("IO");Y;);                return IO;                }      /* DEC fortran */
delete/\(               { D(O("IO");Y;);                return IO;                }      /* INTEL fortran */
find/\(                 { D(O("IO");Y;);                return IO;                }      /* INTEL fortran */
rewrite/\(              { D(O("IO");Y;);                return IO;                }      /* INTEL fortran */
   /* ACCEPT and TYPE, followed by a label are dec-extensions, and are disguised 
      as READ and PRINT respectively */
accept/[0-9]            { D(O("ACCEPT");Y;);            return READ;              }      /* DEC fortran */
type/[0-9]              { D(O("TYPE0-9");Y);            return PRINT;             }      /* DEC fortran */
}

<state_esope>{
segment                 { D(O("SEGMENT");Y;);           return SEGMENT;           }
segment,                { D(O("SEGMENT,");Y;);          return SEGMENT;           }
endsegment              { D(O("ENDSEGMENT");Y;);        return ENDSEGMENT;        }
}

<state_findformat>{
^\?\?.*\n                     { D(O("UNSURE");Y;);   return UNSURE;   }
^[^dDcC#!* 0-9].*\n           { D(O("FREE");Y;);     return FREE;     }
^[ 0-9]{1,4}[[:alpha:]_%].*\n { D(O("FREE");Y;);     return FREE;     }
^[^!]*!.*\n                   { D(O("UNSURE");Y;);   return UNSURE;   }
^[ ]{5}&[^&]*\n               { D(O("UNSURE");Y;);   return UNSURE;   }
^.{0,4}&{b}*\n                { D(O("FREE");Y;);     return FREE;     }
^.{6,}&{b}*\n                 { D(O("FREE");Y;);     return FREE;     }
^[^&]*&{b}*\n                 { D(O("FREE");Y;);     return FREE;     }
^[ ]{0,4}&.*\n                { D(O("FREE");Y;);     return FREE;     }
^[ ]{6,}&.*\n                 { D(O("FREE");Y;);     return FREE;     }
^{ckey}.*\n                   { D(O("FREE");Y;);     return FREE;     }
^c{b}+[[:alnum:]].*\n         { D(O("FIXED");Y;);    return FIXED;    }
^[ 0123456789]{6}.*\n         { D(O("UNSURE");Y;);   return UNSURE;   }
.*\n                          { D(O("UNSURE");Y;);   return UNSURE;   }
}

<state_scanompfixed>{
^[c!\*]\$[ 0-9]{3}[ 0]{0,1} |          /* a first line */
^[c!\*]\$[ ]{3}[^ ^0]       |          /* a continuation line  */
^[c!\*]\${b}*                 { D(O("FIXOMP");Y;);
                                return OMP;
                              }
\n                            {  D(O("EOL"););   return EOL;        }
.                             {  D(O("BLA");Y;); return yytext[0];  }
}
<state_scanompfree>{
^{b}*!\$$ |
^{b}*!\${b}                   { D(O("OMP");Y;);
				return OMP;
                              }
\n                            {  D(O("EOL"););   return EOL;        }
.                             {  D(O("BLA");Y;); return yytext[0];  }
}

<state_labels>{

[0-9][0-9]*                   {  D(O("LABEL");Y;);
			         lexeri_number = yytext;
                                 return LABEL;
                              }
\n                            {  D(O("EOL"););   return EOL;        }
.                             {  D(O("BLA");Y;); return yytext[0];  }
}

<state_iolist>{
[0-9][0-9]*                   {  D(O("LABEL");Y;);
			         lexeri_number = yytext;
                                 return LABEL;
                              }
err=/[0-9]                    {  D(O("LABELIS");Y);  return LABELIS;    } 
fmt=/[0-9]                    {  D(O("LABELIS");Y);  return LABELIS;    } 
end=/[0-9]                    {  D(O("LABELIS");Y);  return LABELIS;    } 
eor=/[0-9]                    {  D(O("LABELIS");Y);  return LABELIS;    } 
[ ][0-9]*[ ]                  {  D(O("OTHER");Y);    return OTHER;      }  /* e.g: '(a80)' generated by Line_prep() */
[0-9a-z_]{1,}[#'][^,]*        {  D(O("OTHER");Y);    return OTHER;      }  /* e.g: read(5#n,x) dec extension */
\n                            {  D(O("EOL"););       return EOL;        }
,                             {  D(O("COMMA");Y;);   return yytext[0];  }
.                             {  D(O("OTHER");Y;);   return OTHER;      }
}

<state_startiolist>{
.                              {  D(O("STARTIO");Y;); 
				  BEGIN(state_iolist); 
                                  line_pos-=strlen(yytext); 
				  yyless(0);
				  return STARTIO; 
			       }
}

<state_calllist>{
,[*&$][0-9]{1,}/[,\n]         {  D(O("LABEL_CALLLIST");Y;);    /* DEC allows & and $ next to * */
			         lexeri_number = yytext+2;
                                 return LABEL;
                              }

\n                            {  D(O("EOL_CALLLIST"););       return EOL;        }
.                             {  D(O("OTHER_CALLLIST");Y;);   return OTHER;      }
}

<state_startcalllist>{
.                              {  D(O("STARTCALL");Y;); 
				  BEGIN(state_calllist); 
                                  line_pos-=strlen(yytext); 
				  yyless(0);
				  return STARTCALL; 
			       }
}

<state_scanfixpre>{
{b}*\!{b}*findentfix:p-on   { D(O("FINDENTFIX P-ON");Y;); 
				return P_ON;
		              }
^[c\*]{b}*findentfix:p-on   { D(O("FIXFINDENTFIX P-ON");Y;);
				return P_ON;
		              }
{b}*\!{b}*findentfix:p-off  { D(O("FINDENTFIX P-OFF");Y;); 
				return P_OFF;
		              }
^[c\*]{b}*findentfix:p-off  { D(O("FIXFINDENTFIX P-OFF");Y;);
				return P_OFF;
		              }
{b}*\!{b}*findentfix:         { D(O("FINDENTFIX");Y;); 
                                size_t l = input_string.length()-yyleng-1;
                                lexer_rest = input_string.substr(yyleng,l);
				return FINDENTFIX;
		              }
^[c\*]{b}*findentfix:         { D(O("FIXFINDENTFIX");Y;);
                                size_t l = input_string.length()-yyleng-1;
                                lexer_rest = input_string.substr(yyleng,l);
				return FIXFINDENTFIX;
		              }
^{b}*#                        { D(O("CPP");Y;);
                                return CPP;
                              }
^{b}*#{b}*if                  { D(O("CPP_IF");Y;);
                                return CPP_IF;
			      }
^{b}*#{b}*endif               { D(O("CPP_ENDIF");Y;);
                                return CPP_ENDIF;
			      }
^{b}*#{b}*else                { D(O("CPP_ELSE");Y;);
                                return CPP_ELSE;
			      }
^{b}*#{b}*elif                { D(O("CPP_ELIF");Y;);
                                return CPP_ELIF;
			      }
^{b}*#{b}*include{b}*/\"[^\n]*\"{b}*\n  { D(O("INCLUDE_CPP");Y;);
                                extract_include();
                                return INCLUDE_CPP;
		              }
^{b}*#{b}*include{b}*/<[^\n]*>{b}*\n  { D(O("INCLUDE_CPP_STD");Y;);
                                extract_include();
                                return INCLUDE_CPP_STD;
		              }


^{b}*\?\?                     { D(O("COCO");Y;);
                                return COCO;
                              }
^{b}*\?\?{b}*i{b}*f           { D(O("COCO_IF");Y;);
                                return COCO_IF;
			      }
^{b}*\?\?{b}*e{b}*n{b}*d{b}*i{b}*f { D(O("COCO_ENDIF");Y;);
                                return COCO_ENDIF;
			      }
^{b}*\?\?{b}*e{b}*l{b}*s{b}*e { D(O("COCO_ELSE");Y;);
                                return COCO_ELSE;
			      }
^{b}*\?\?{b}*e{b}*l{b}*s{b}*e{b}*i{b}*f { D(O("COCO_ELIF");Y;);
                                return COCO_ELIF;
			      }
^{b}*\?\?{b}*i{b}*n{b}*c{b}*l{b}*u{b}*d{b}*e{b}*/\'[^\n]+\'{b}*\n          { D(O("INCLUDE_COCO");Y;);
                              /* coco statements must start with '??' in column 1.
			         However, for this code that is somewhat inconvenient,
				 so we allow white space before '??' */
                                extract_include();
                                return INCLUDE_COCO;
		              }
^{b}*\?\?{b}*i{b}*n{b}*c{b}*l{b}*u{b}*d{b}*e{b}*/\"[^\n]+\"{b}*\n          { D(O("INCLUDE_COCO");Y;);
                              /* coco statements must start with '??' in column 1.
			         However, for this code that is somewhat inconvenient,
				 so we allow white space before '??' */
                                extract_include();
                                return INCLUDE_COCO;
		              }


\n                       {  D(O("EOL"););   return EOL;        }
.                        {  D(O("BLA");Y;); return yytext[0];  }
}

" "[0-9]*" "    {
		   int type = pline.get_type(std::string(yytext));
                   D(O("found_special");O(type);O(pline.get_type_as_string(std::string(yytext)));Y;);
		   D(O("encoded line");O(pline.get_line_encoded()););
		    if (type == pline.is_string)
		    {
		       lexer_string = pline.get_value(std::string(yytext));
		       D(O("QSTRING");O(lexer_string);Y;);
		       return QSTRING;
		    }
		    if (type == pline.is_stlabel)
		    {
		       lexer_stlabel = pline.get_value(std::string(yytext));
		       D(O("STLABEL");O(lexer_stlabel);Y;);
		       return STLABEL;
		    }
		    if (type == pline.is_dotop)
		    {
		       D(O("DOTOPERATOR"););
		       return DOTOPERATOR;
		    }
		    D(O("This should not happen"););
                }


[[:alpha:]][[:alnum:]_]*:/[^:] {  D(O("NAMED_LABEL");Y;);
                            lexer_name = yytext;
			    lexer_name.erase(lexer_name.size()-1);
                            return NAMED_LABEL;
			 }

[0-9][0-9]*              {  D(O("I_NUMBER");Y;);
                            lexer_name = yytext;
			    lexeri_number = yytext;
                            return I_NUMBER;
                         }


"("                      {  D(O("LPAREN");Y;);
                            start_stack.push_back(YY_START);
			    lrlevel  = 1;
			    lrreturn = 1;
			    lexer_lr = "";
		            lexer_lrpos = line_pos; 
			    BEGIN(state_lr);
			 }

"["                      {  D(O("LPARENB");Y;);
                            start_stack.push_back(YY_START);
			    lrlevel  = 1;
			    lrreturn = 1;
			    BEGIN(state_lrb);
			 }

^\n                      {  D(O("BLANK");); return BLANK;      }

\n                       {  D(O("EOL"););   return EOL;        }
.                        {  D(O("BLA");Y;); return yytext[0];  }

%%

void lexer_set(Line_prep p, const int state)
{
   input_string  = p.get_line_encoded()+'\n';
   D(O("lexer_set");O(input_string);O(p.get_type_as_string(0)););
   pline         = p;
   yy_delete_buffer(YY_CURRENT_BUFFER);
   BEGIN(INITIAL);
   yy_scan_string(input_string.c_str());
   lexer_enable(state);
   line_pos = 0;
}

void lexer_set(const std::string &s, const int state)
{
   input_string  = s+'\n';
   D(O("lexer_set");O(input_string););
   yy_delete_buffer(YY_CURRENT_BUFFER);
   yy_scan_string(input_string.c_str());
   lexer_enable(state);
   line_pos = 0;
}

void lexer_enable(const int k)
{
   switch(k)
   {
      case CALLLIST:      BEGIN(state_startcalllist);   break;  /* prepare for parsing parameters of call to find *100 */
      case CHAR:          BEGIN(state_char);            break;  /* internal use */
      case CIDENTIFIER:   BEGIN(state_cidentifier);     break;  /* parse identifier possibly followed by , */
      case IDENTIFIER:    BEGIN(state_identifier);      break;  /* parse identifier */
      case INCFILENAME:   BEGIN(state_incfilename);     break;  /* parse include file name */
      case LABELS:        BEGIN(state_labels);          break;  /* parse label */
      case SKIPALL:       BEGIN(state_skipall);         break;  /* skip everything */
      case SKIP:          BEGIN(state_skip);            break;  /* skip everything except = */
      case SKIPNOOP:      BEGIN(state_skipnoop);        break;  /* skip everything except [,|~!#$] */
      case KEYWORD:       BEGIN(state_keyword);         break;  /* parse keywords */
      case ESOPE:         BEGIN(state_esope);           break;  /* allow esope keywords */
      case FINDFORMAT:    BEGIN(state_findformat);      break;  /* used to determine fixed or free */
      case IOLIST:        BEGIN(state_startiolist);     break;  /* prepare for parsing parameters of read() etc */
      case SCANFIXPRE:    BEGIN(state_scanfixpre);      break;  /* parse preprocessor and findentfix */
      case SCANOMPFIXED:  BEGIN(state_scanompfixed);    break;  /* parse omp, fixed format */
      case SCANOMPFREE:   BEGIN(state_scanompfree);     break;  /* parse omp, free format */
      case SKIPNOS:       BEGIN(state_skipnos);         break;  /* skip everything except : */
      default:
	 D(O("invalid parameter in lexer_enable");O(k););
	 BEGIN(state_error);
	 break;
   }
   D(O("enabled");O(k);O(YY_START););
}

std::string lexer_getname()
{
   return lexer_name;
}
std::string lexer_getstlabel()
{
   return lexer_stlabel;
}
std::string lexer_geti_number()
{
   return lexeri_number;
}
int lexer_get_pos()
{
   return line_pos;
}
int lexer_get_len()
{
   return yyleng;
}
std::string lexer_getrest()
{
   return lexer_rest;
}
std::string lexer_getstring()
{
   D(O("lexer_string");O(lexer_string););
   return lexer_string;
}
std::string lexer_getlr()
{
   D(O("lexer_lr");O(lexer_lr););
   return lexer_lr;
}
int lexer_getlrpos()
{
   D(O("lexer_lrpos");O(lexer_lrpos););
   return lexer_lrpos;
}

std::string lexer_getinclude()
{
   D(O("lexer_getinclude");O(lexer_include););
   return lexer_include;
}

void extract_include()
{
// removes first and last character of rest of input line
// example: 
// #include "file.inc"
// after scanning of '#include ', the string "file.inc",
// without quotes is put in lexer_include

   size_t l = input_string.length()-yyleng-1;
   lexer_include = input_string.substr(yyleng,l);
   if (lexer_include.size() > 1)
   {
      lexer_include.erase(lexer_include.end()-1);
      lexer_include.erase(lexer_include.begin());
   }
   D(O("lexer_include");O(lexer_include););
}

#if 0
// maybe we need this in the future:
// not sure if this works OK
std::deque<std::string> input_string_stack;
void lexer_push(std::string s, const int state)
{
   // https://sourceforge.net/p/flex/mailman/message/30542280/

   input_string_stack.push_back(s);
   input_string = s+'\n';
   yypush_buffer_state(YY_CURRENT_BUFFER);
   yy_scan_string(input_string.c_str());
   lexer_enable(state);
}
void lexer_pop()
{
   yypop_buffer_state();
   input_string=input_string_stack.back();
   input_string_stack.pop_back();
}
#endif

void quiet_compiler()                   // to prevent warning about unused unput
{
   (void)yyunput;
}

