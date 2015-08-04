%{
// $Id: parser.y 71 2015-07-30 11:49:09Z willem_vermin $
#include <iostream>
using namespace std;
#include "findent.h"
#include "line_prep.h"
struct propstruct properties;
%}

%code requires {
}

%token MODULE SUBFUN ENTRY SUBMODULE
%token MODULESUBROUTINE MODULEFUNCTION
%token END ENDSUBROUTINE ENDFUNCTION ENDPROGRAM ENDMODULE ENDSUBMODULE
%token IF THEN ELSE ELSEIF ENDIF
%token WHERE ENDWHERE FORALL ENDFORALL ELSEWHERE
%token DO DOWHILE DOCONCURRENT ENDDO
%token SELECTCASE SELECTTYPE ENDSELECT CASE CASEDEFAULT TYPEIS CLASSIS CLASSDEFAULT
%token INTERFACE ABSTRACTINTERFACE ENDINTERFACE
%token CONTAINS
%token BLOCK ENDBLOCK
%token BLOCKDATA ENDBLOCKDATA
%token ASSOCIATE ENDASSOCIATE
%token CRITICAL ENDCRITICAL
%token ENUM ENDENUM
%token ASSIGNMENT
%token ASSIGN TO
%token MODULEPROCEDURE PROCEDURE ENDPROCEDURE
%token TIDENTIFIER
%token BLANK
%token FINDFORMAT FREE FIXED UNSURE PROBFREE

%token IDENTIFIER SKIP SKIPALL SKIPNOOP KEYWORD

%token ELEMENTAL IMPURE PURE RECURSIVE SUBROUTINE FUNCTION PROGRAM 
%token EOL NAMED_LABEL
%token STLABEL 
%token TYPE ENDTYPE CLASS
%token BASICTYPE TYPEC
%token QSTRING HSTRING 
%token LR LRB 
%token DOTOPERATOR 
%token I_NUMBER 
%token UNCLASSIFIED
%token ERROR
%token LAST_TOKEN

%%

lline:               labels line
     ;
labels:              empty          
      |              stlabel        
      |              named_label    
      |              stlabel  named_label
      ;
line: 
    |                abstractinterface        { properties.kind = ABSTRACTINTERFACE; }
    |                assignment               { properties.kind = ASSIGNMENT;        }
    |                associate                { properties.kind = ASSOCIATE;         }
    |                blank                    { properties.kind = BLANK;             }
    |                block                    { properties.kind = BLOCK;             }
    |                blockdata                { properties.kind = BLOCKDATA;         }
    |                case                     { properties.kind = CASE;              }
    |                casedefault              { properties.kind = CASEDEFAULT;       }
    |                classdefault             { properties.kind = CLASSDEFAULT;      }
    |                classis                  { properties.kind = CLASSIS;           }
    |                contains                 { properties.kind = CONTAINS;          }
    |                critical                 { properties.kind = CRITICAL;          }
    |                do_construct             { properties.kind = DO;                }
    |                else                     { properties.kind = ELSE;              }
    |                elseif                   { properties.kind = ELSEIF;            }
    |                elsewhere                { properties.kind = ELSEWHERE;         }
    |                endassociate             { properties.kind = ENDASSOCIATE;      }
    |                endblock                 { properties.kind = ENDBLOCK;          }
    |                endblockdata             { properties.kind = ENDBLOCKDATA;      }
    |                endcritical              { properties.kind = ENDCRITICAL;       }
    |                enddo                    { properties.kind = ENDDO;             }
    |                endenum                  { properties.kind = ENDENUM;           }
    |                endforall                { properties.kind = ENDFORALL;         }
    |                endfunction              { properties.kind = ENDFUNCTION;       }
    |                endif                    { properties.kind = ENDIF;             }
    |                endinterface             { properties.kind = ENDINTERFACE;      }
    |                endmodule                { properties.kind = ENDMODULE;         }
    |                endprocedure             { properties.kind = ENDPROCEDURE;      }
    |                endprogram               { properties.kind = ENDPROGRAM;        }
    |                endselect                { properties.kind = ENDSELECT;         }
    |                endsubmodule             { properties.kind = ENDSUBMODULE;      }
    |                endsubroutine            { properties.kind = ENDSUBROUTINE;     }
    |                endtype                  { properties.kind = ENDTYPE;           }
    |                endwhere                 { properties.kind = ENDWHERE;          }
    |                entry                    { properties.kind = ENTRY;             }
    |                enum                     { properties.kind = ENUM;              }
    |                forall_construct         { properties.kind = FORALL;            }
    |                if_construct             { properties.kind = IF;                }
    |                interface                { properties.kind = INTERFACE;         }
    |                module                   { properties.kind = MODULE;            }
    |                moduleprocedure          { properties.kind = PROCEDURE;         }
    |                selectcase               { properties.kind = SELECTCASE;        }
    |                selecttype               { properties.kind = SELECTTYPE;        }
    |                simple_end               { properties.kind = END;               }
    |                subfun_stmt_bind         {                                      }
    |                submodule                { properties.kind = SUBMODULE;         }
    |                type                     { properties.kind = TYPE;              }
    |                typeis                   { properties.kind = TYPEIS;            }
    |                where_construct          { properties.kind = WHERE;             }
    ;
blank:               BLANK ;

stlabel:             STLABEL getstlabel ;
named_label:         NAMED_LABEL        ;

module:              MODULE enable_identifier IDENTIFIER getname EOL ;

abstractinterface:   ABSTRACTINTERFACE  EOL     ;
contains:            CONTAINS           EOL     ;
interface:           INTERFACE          skipall ;
moduleprocedure:     MODULEPROCEDURE    enable_identifier IDENTIFIER getname EOL ;


subfun_stmt_bind:    subfun_stmt SKIP ;
subfun_stmt:         subfunprefix subfun subfunname ;
subfun:              subfun_spec enable_identifier  ;
subfun_spec:         SUBROUTINE       { properties.kind = SUBROUTINE;}
           |         MODULESUBROUTINE { properties.kind = SUBROUTINE;}
	   |         FUNCTION         { properties.kind = FUNCTION;  }
	   |         MODULEFUNCTION   { properties.kind = FUNCTION;  }
	   |         PROGRAM          { properties.kind = PROGRAM;   }
	   ;
subfunname:          IDENTIFIER enable_skip getname ;
subfunprefix:        empty
            |        subfunprefix subfunprefix_spec ;
subfunprefix_spec:   ELEMENTAL   
		 |   IMPURE      
		 |   PURE        
		 |   RECURSIVE   
		 |   intrinsic_type_spec
		 |   TYPEC LR
		 |   CLASS LR ;
submodule:           SUBMODULE LR enable_identifier IDENTIFIER getname EOL ;
intrinsic_type_spec: BASICTYPE
		   | BASICTYPE kind_selector ;
kind_selector:       '*' I_NUMBER      /* extension */
	     |       '*' LR            /* extension */
	     |       LR ;

entry:               ENTRY enable_identifier IDENTIFIER skipall ;

endassociate:        ENDASSOCIATE  construct_name EOL ;
endblock:            ENDBLOCK      construct_name EOL ;
endblockdata:        ENDBLOCKDATA  construct_name EOL ;
endcritical:         ENDCRITICAL   construct_name EOL ;
enddo:               ENDDO         construct_name EOL ;
endenum:             ENDENUM       EOL                ;
endforall:           ENDFORALL     construct_name EOL ;
endfunction:         ENDFUNCTION   construct_name EOL ;
endif:               ENDIF         construct_name EOL ;
endinterface:        ENDINTERFACE  skipall            ;
endmodule:           ENDMODULE     construct_name EOL ;
endprocedure:        ENDPROCEDURE  construct_name EOL ;
endprogram:          ENDPROGRAM    construct_name EOL ;
endselect:           ENDSELECT     skipall            ;
endsubmodule:        ENDSUBMODULE  construct_name EOL ;
endsubroutine:       ENDSUBROUTINE construct_name EOL ;
endtype:             ENDTYPE       construct_name EOL ;
endwhere:            ENDWHERE      construct_name EOL ;
simple_end:          END                          EOL ;

assignment:          lvalue '=' skipnoop
	  |          ASSIGN I_NUMBER TO enable_identifier IDENTIFIER EOL
	  |          ASSIGN I_NUMBER TO enable_identifier TIDENTIFIER EOL ;

else:                ELSE           construct_name EOL ;
elseif:              ELSEIF LR THEN construct_name EOL ;
elsewhere:           ELSEWHERE LR   construct_name EOL
	 |           ELSEWHERE      construct_name EOL ;

if_construct:        IF     LR THEN EOL ;
where_construct:     WHERE  LR EOL ;
forall_construct:    FORALL LR EOL ;
do_construct:        do I_NUMBER     getdolabel skipall 
            |        do IDENTIFIER              skipall
            |        do TIDENTIFIER             skipall
	    |        do ','                     skipall
	    |        do                         EOL ;
do:                  DO enable_identifier ;

selectcase:          SELECTCASE LR EOL     ;
selecttype:          SELECTTYPE LR skipall ;

case:                CASE          LR  skipall ;
casedefault:         CASEDEFAULT       skipall ;
classdefault:        CLASSDEFAULT      skipall ;
classis:             CLASSIS       LR  skipall ;
typeis:              TYPEIS        LR  skipall ;


block:               BLOCK                    EOL ;
blockdata:           BLOCKDATA construct_name EOL ;
associate:           ASSOCIATE LR             EOL ;
critical:            CRITICAL                 EOL ;
enum:                ENUM ','             skipall ;

type:                type1 ','  skipall
    |                type1 ':'  skipall
    |                type1 IDENTIFIER  skipall ;
type1:               TYPE enable_identifier ;

lvalue:              IDENTIFIER
      |              IDENTIFIER LR
      |              TIDENTIFIER
      |              TIDENTIFIER LR
      ;
construct_name:      enable_identifier empty
	      |      enable_identifier IDENTIFIER getname
	      ;
skipall:             enable_skipall SKIPALL
       ;
skipnoop:            enable_skipnoop SKIPNOOP
        ;
enable_identifier:   {lexer_enable(IDENTIFIER);}
	         ;
enable_skip:         {lexer_enable(SKIP);}
	   ;
enable_skipall:      {lexer_enable(SKIPALL);}
	      ;
enable_skipnoop:     {lexer_enable(SKIPNOOP);}
	       ;
getname:             {properties.name=lexer_getname();}
       ;
getstlabel:          {properties.label=lexer_getstlabel();}
          ;
getdolabel:          {properties.dolabel=lexer_geti_number();}
          ;
empty:               
     ;
%%

void resetprop()
{
   properties.kind    = UNCLASSIFIED;
   properties.name    = "";
   properties.label   = "";
   properties.dolabel = "";
}

propstruct parseline(class line_prep p)
{
   D(O("entering parseline"););
   lexer_set(p);
   resetprop();
   lexer_enable(IDENTIFIER);
   yyparse();
   if (properties.kind != UNCLASSIFIED)
      return properties;
   lexer_set(p);
   lexer_enable(KEYWORD);
   yyparse();
   return properties;
}

void yyerror(const char *c)
{
    D(O("ERROR");O(c););
}

extern "C" int yywrap()
{
  D(O("yywrap"););
  return 1;
}

