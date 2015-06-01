%{
#include <iostream>
#include <string>
#include <stack>
#include <vector>
using namespace std;
#include "parser.h"
#include "findent.h"
#include "debug.h"
#define YY_DECL extern "C" int yylex()
stack<int>      start_stack;
string          lexer_name, lexer_stlabel, lexeri_number;
string          input_string;
int             lrlevel;
bool            lrreturn;
class line_prep pline;

%}

%s state_identifier state_keyword

%x state_lr state_lrb state_skip state_skipall state_skipnoop state_error
%x state_findformat

b [ \t]

  //  following are keywords starting with c
ckey  call|close|common|continue|case|contains|cycle|class|codimension|contiguous|critical|complex
%%

<state_error>.  {  D(O("ERROR");Y;); return ERROR; }

<state_lr>{
"("             { D(O("lr+");O(lrlevel);Y;); lrlevel++; }

")"             { D(O("lr-");O(lrlevel);Y;); 
                  lrlevel--;
		  if(lrlevel == 0)
		  {
		     D(O("state:");O(start_stack.top());O(lrreturn););
		     BEGIN(start_stack.top());
		     start_stack.pop();
		     if(lrreturn)
		     {
			return LR;
		     }
		  }
		}

.               { D(O("lrbla");O(lrlevel);Y;); } 
\n              { D(O("lreol");O(lrlevel);Y;); yyless(0); BEGIN(start_stack.top()); }

}

<state_lrb>{
"["             { D(O("lrb+");O(lrlevel);Y;); 
                  lrlevel++;
		}

"]"             { D(O("lrb-");O(lrlevel);Y;); 
                  lrlevel--;
		  if(lrlevel == 0)
		  {
		     D(O("state:");O(start_stack.top());O(lrreturn););
		     BEGIN(start_stack.top());
		     start_stack.pop();
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
                  BEGIN(start_stack.top());
		}

}

<state_skip,state_skipall,state_skipnoop>{
"("          {  D(O("skipLPAREN");Y;);
		start_stack.push(YY_START);
		lrlevel  = 1;
		lrreturn = 0;
		BEGIN(state_lr);
	     }

"["          {  D(O("skipLPAREN");Y;);
		start_stack.push(YY_START);
		lrlevel  = 1;
		lrreturn = 0;
		BEGIN(state_lrb);
	     }
}

<state_skip>"="             { D(O("NOSKIP");Y;);     return yytext[yyleng-1]; }
<state_skipnoop>[,|~!#$]    { D(O("NOSKIPNOOP");Y;); return yytext[yyleng-1]; }

<state_skip,state_skipall,state_skipnoop>{
\n           { D(O("endSKIP");Y;);
               switch(YY_START)
	       {
                  case state_skip:
		     return SKIP;
	          case state_skipall:
		     return SKIPALL;
		  case state_skipnoop:
		     return SKIPNOOP;
	       }
	     }

.            { D(O("SKIP");Y;); }
}

<state_identifier>{
[[:alpha:]][[:alnum:]_]*  { D(O("IDENTIFIER");Y;);  lexer_name = yytext; return IDENTIFIER;  }
[[:alpha:]][[:alnum:]_%]* { D(O("TIDENTIFIER");Y;); lexer_name = yytext; return TIDENTIFIER; }
}

<state_keyword>{
abstractinterface { D(O("ABSTRACTINTERFACE");Y;); return ABSTRACTINTERFACE; }
associate         { D(O("ASSOCIATE");Y;);         return ASSOCIATE;         }
assign            { D(O("ASSIGN");Y;);            return ASSIGN;            }
block             { D(O("BLOCK");Y;);             return BLOCK;             }
blockdata         { D(O("BLOCKDATA");Y;);         return BLOCKDATA;         }
casedefault       { D(O("CASEDEFAULT");Y);        return CASEDEFAULT;       }
case              { D(O("CASE");Y);               return CASE;              }
character         { D(O("CHARACTER");Y;);         return BASICTYPE;         }
classdefault      { D(O("CLASSDEFAULT");Y);       return CLASSDEFAULT;      }
class             { D(O("CLASS");Y;);             return CLASS;             }
classis           { D(O("CLASSIS");Y);            return CLASSIS;           }
complex           { D(O("COMPLEX");Y;);           return BASICTYPE;         }
contains          { D(O("CONTAINS");Y);           return CONTAINS;          }
critical          { D(O("CRITICAL");Y;);          return CRITICAL;          }
do                { D(O("DO");Y;);                return DO;                }
doublecomplex     { D(O("DOUBLECOMPLEX");Y;);     return BASICTYPE;         }
doubleprecision   { D(O("DOUBLEPRECISION");Y;);   return BASICTYPE;         }
elemental         { D(O("ELEMENTAL");Y;);         return ELEMENTAL;         }
else              { D(O("ELSE");Y);               return ELSE;              }
elseif            { D(O("ELSEIF");Y);             return ELSEIF;            }
elsewhere         { D(O("ELSEWHERE");Y);          return ELSEWHERE;         }
endassociate      { D(O("ENDASSOCIATE");Y;);      return ENDASSOCIATE;      }
endblock          { D(O("ENDBLOCK");Y;);          return ENDBLOCK;          }
endblockdata      { D(O("ENDBLOCKDATA");Y;);      return ENDBLOCKDATA;      }
endcritical       { D(O("ENDCRITICAL");Y;);       return ENDCRITICAL;       }
enddo             { D(O("ENDDO");Y;);             return ENDDO;             }
end               { D(O("END");Y;);               return END;               }
endenum           { D(O("ENDENUM");Y;);           return ENDENUM;           }
endforall         { D(O("ENDFORALL");Y;);         return ENDFORALL;         }
endfunction       { D(O("ENDFUNCTION");Y;);       return ENDFUNCTION;       }
endif             { D(O("ENDIF");Y);              return ENDIF;             }
endinterface      { D(O("ENDINTERFACE");Y;);      return ENDINTERFACE;      }
endmodule         { D(O("ENDMODULE");Y;);         return ENDMODULE;         }
endprocedure      { D(O("ENDPROCEDURE");Y;);      return ENDPROCEDURE;      }
endprogram        { D(O("ENDPROGRAM");Y;);        return ENDPROGRAM;        }
endselect         { D(O("ENDSELECT");Y);          return ENDSELECT;         }
endsubroutine     { D(O("ENDSUBROUTINE");Y;);     return ENDSUBROUTINE;     }
endsubmodule      { D(O("ENDSUBMODULE");Y;);      return ENDSUBMODULE;      }
endtype           { D(O("ENDTYPE");Y;);           return ENDTYPE;           }
endwhere          { D(O("ENDWHERE");Y;);          return ENDWHERE;          }
entry             { D(O("ENTRY");Y);              return ENTRY;             }
enum              { D(O("ENUM");Y;);              return ENUM;              }
forall            { D(O("FORALL");Y;);            return FORALL;            }
function          { D(O("FUNCTION");Y;);          return FUNCTION;          }
if                { D(O("IF");Y;);                return IF;                }
impure            { D(O("IMPURE");Y;);            return IMPURE;            }
integer           { D(O("INTEGER");Y;);           return BASICTYPE;         }
interface         { D(O("INTERFACE");Y;);         return INTERFACE;         }
logical           { D(O("LOGICAL");Y;);           return BASICTYPE;         }
module            { D(O("MODULE");Y);             return MODULE;            }
moduleprocedure   { D(O("MODULEPROCEDURE");Y);    return MODULEPROCEDURE;   }
modulesubroutine  { D(O("MODULESUBROUTINE");Y);   return MODULESUBROUTINE;  }
modulefunction    { D(O("MODULEFUNCTION");Y);     return MODULEFUNCTION;    }
program           { D(O("PROGRAM");Y;);           return PROGRAM;           }
pure              { D(O("PURE");Y;);              return PURE;              }
real              { D(O("REAL");Y;);              return BASICTYPE;         }
recursive         { D(O("RECURSIVE");Y;);         return RECURSIVE;         } 
selectcase        { D(O("SELECTCASE");Y);         return SELECTCASE;        }
selecttype        { D(O("SELECTTYPE");Y);         return SELECTTYPE;        }
submodule         { D(O("SUBMODULE");Y;);         return SUBMODULE;         }
subroutine        { D(O("SUBROUTINE");Y;);        return SUBROUTINE;        }
to                { D(O("TO");Y;);                return TO;                }
then              { D(O("THEN");Y;);              return THEN;              }
type              { D(O("TYPE");Y;);              return TYPE;              }
type/\(           { D(O("TYPEC");Y;);             return TYPEC;             }
typeis            { D(O("TYPEIS");Y);             return TYPEIS;            }
where             { D(O("WHERE");Y;);             return WHERE;             }
}

<state_findformat>{
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

" "[0-9]*" "    {
		   int type = pline.get_type(string(yytext));
                   D(O("found_special");O(type);O(pline.get_type_as_string(string(yytext)));Y;);
		   D(O("encoded line");O(pline.get_line_encoded()););
		    if (type == pline.type_string)
		    {
		       D(O("QSTRING"););
		       return QSTRING;
		    }
		    if (type == pline.type_stlabel)
		    {
		       lexer_stlabel = pline.get_value(string(yytext));
		       D(O("STLABEL");O(lexer_stlabel);Y;);
		       return STLABEL;
		    }
		    if (type == pline.type_dot_operator)
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
                            start_stack.push(YY_START);
			    lrlevel  = 1;
			    lrreturn = 1;
			    BEGIN(state_lr);
			 }

"["                      {  D(O("LPARENB");Y;);
                            start_stack.push(YY_START);
			    lrlevel  = 1;
			    lrreturn = 1;
			    BEGIN(state_lrb);
			 }

^\n                      {  D(O("BLANK");); return BLANK;      }

\n                       {  D(O("EOL"););   return EOL;        }
.                        {  D(O("BLA");Y;); return yytext[0];  }

%%

void lexer_set(class line_prep p)
{
   input_string  = p.get_line_encoded()+'\n';
   pline         = p;
   yy_delete_buffer(YY_CURRENT_BUFFER);
   BEGIN(INITIAL);
   yy_scan_string(input_string.c_str());
   D(O("lexer_set");O(input_string);O(p.get_type_as_string(0)););
}

void lexer_set(string s)
{
   input_string  = s+'\n';
   D(O("lexer_set");O(input_string););
   yy_delete_buffer(YY_CURRENT_BUFFER);
   yy_scan_string(input_string.c_str());
   BEGIN(state_findformat);
}

void lexer_enable(int k)
{
   switch(k)
   {
      case IDENTIFIER:  BEGIN(state_identifier);   break;
      case SKIPALL:     BEGIN(state_skipall);      break;
      case SKIP:        BEGIN(state_skip);         break;
      case SKIPNOOP:    BEGIN(state_skipnoop);     break;
      case KEYWORD:     BEGIN(state_keyword);      break;
      case FINDFORMAT:  BEGIN(state_findformat);   break;
      default:
	 D(O("invalid parameter in lexer_enable");O(k););
	 BEGIN(state_error);
	 break;
   }
   D(O("enabled");O(k);O(YY_START););
}

string lexer_getname()
{
   return lexer_name;
}

string lexer_getstlabel()
{
   return lexer_stlabel;
}
string lexer_geti_number()
{
   return lexeri_number;
}

void quiet_compiler()                   // to prevent warning about unused unput
{
   char *a=0;
   yyunput(1,a);
}

