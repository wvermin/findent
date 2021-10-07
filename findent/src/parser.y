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

#include "debug.h"
#include "lexer.h"
#include "line_prep.h"
#include "prop.h"

struct propstruct properties;
bool parselabeling = false;
int  itemnumber = 0;
%}

%token MODULE SUBFUN ENTRY SUBMODULE
%token MODULESUBROUTINE MODULEFUNCTION
%token END ENDSUBROUTINE ENDFUNCTION ENDPROGRAM ENDMODULE ENDSUBMODULE
%token IF THEN ELSE ELSEIF ENDIF SIMPLEIF NWAYIF
%token WHERE ENDWHERE FORALL ENDFORALL ELSEWHERE
%token DO DOCOMMA DOWHILE DOCONCURRENT ENDDO
%token SELECTCASE SELECTTYPE ENDSELECT CASE CASEDEFAULT TYPEIS CLASSIS CLASSDEFAULT
%token SELECTRANK RANK RANKDEFAULT
%token INTERFACE INTERFACE1 ABSTRACTINTERFACE ENDINTERFACE
%token CONTAINS
%token BLOCK ENDBLOCK
%token BLOCKDATA ENDBLOCKDATA
%token ASSOCIATE ENDASSOCIATE
%token CRITICAL ENDCRITICAL
%token CHANGETEAM ENDTEAM
%token ENUM ENDENUM
%token ASSIGNMENT
%token ASSIGN TO
%token GOTO GOTO_COMPUTED GOTO_ASSIGNED READ PRINT WRITE IO IOLIST STARTIO
%token LABELIS OTHER
%token MODULEPROCEDURE PROCEDURE ENDPROCEDURE
%token TIDENTIFIER
%token BLANK CHAR
%token FINDFORMAT UNKNOWN FREE FIXED UNSURE PROBFREE FINDENTFIX FIXFINDENTFIX
%token P_ON P_OFF
%token SCANFIXPRE
%token CPP_IF CPP_ENDIF CPP_ELSE CPP_ELIF CPP
%token COCO_IF COCO_ENDIF COCO_ELSE COCO_ELIF COCO
%token INCLUDE INCLUDE_CPP INCLUDE_CPP_STD INCLUDE_COCO INCFILENAME
%token USE
%token SEGMENT ENDSEGMENT ESOPE

%token IDENTIFIER CIDENTIFIER SKIP SKIPALL SKIPNOOP SKIPNOS KEYWORD

%token ELEMENTAL IMPURE PURE RECURSIVE NON_RECURSIVE SUBROUTINE FUNCTION PROGRAM
%token EOL NAMED_LABEL
%token STLABEL LABEL LABELS LABELLIST CALL STARTCALL CALLLIST
%token TYPE ENDTYPE CLASS
%token BASICTYPE TYPEC
%token QSTRING HSTRING
%token LR LRB
%token DOTOPERATOR
%token I_NUMBER
%token UNCLASSIFIED
%token ERROR
%token OMP SCANOMPFIXED SCANOMPFREE
%token DEF EXEC ENDDEF NONE
%token LAST_TOKEN

%left '%'

%%

lline:               labels line
     ;
labels:              empty
      |              stlabel
      |              named_label
      |              stlabel  named_label
      ;
line:
    |                abstractinterface        { properties.kind = ABSTRACTINTERFACE; properties.type = DEF;    }
    |                assign                   { properties.kind = ASSIGN;            properties.type = EXEC;   }
    |                assignment               { properties.kind = ASSIGNMENT;        properties.type = EXEC;   }
    |                associate                { properties.kind = ASSOCIATE;         properties.type = EXEC;   }
    |                blank                    { properties.kind = BLANK;             properties.type = NONE;   }
    |                block                    { properties.kind = BLOCK;             properties.type = EXEC;   }
    |                blockdata                { properties.kind = BLOCKDATA;         properties.type = DEF;    }
    |                call                     { properties.kind = CALL;              properties.type = EXEC;   }
    |                calllist                 { properties.kind = CALLLIST;          properties.type = NONE;   }
    |                case                     { properties.kind = CASE;              properties.type = EXEC;   }
    |                casedefault              { properties.kind = CASEDEFAULT;       properties.type = EXEC;   }
    |                rank                     { properties.kind = RANK;              properties.type = EXEC;   }
    |                rankdefault              { properties.kind = RANKDEFAULT;       properties.type = EXEC;   }
    |                changeteam               { properties.kind = CHANGETEAM;        properties.type = EXEC;   }
    |                classdefault             { properties.kind = CLASSDEFAULT;      properties.type = EXEC;   }
    |                classis                  { properties.kind = CLASSIS;           properties.type = EXEC;   }
    |                contains                 { properties.kind = CONTAINS;          properties.type = DEF;    }
    |                critical                 { properties.kind = CRITICAL;          properties.type = DEF;    }
    |                do_construct             { properties.kind = DO;                properties.type = EXEC;   }
    |                else                     { properties.kind = ELSE;              properties.type = EXEC;   }
    |                elseif                   { properties.kind = ELSEIF;            properties.type = EXEC;   }
    |                elsewhere                { properties.kind = ELSEWHERE;         properties.type = EXEC;   }
    |                endassociate             { properties.kind = ENDASSOCIATE;      properties.type = ENDDEF; }
    |                endblock                 { properties.kind = ENDBLOCK;          properties.type = EXEC;   }
    |                endblockdata             { properties.kind = ENDBLOCKDATA;      properties.type = ENDDEF; }
    |                endcritical              { properties.kind = ENDCRITICAL;       properties.type = ENDDEF; }
    |                enddo                    { properties.kind = ENDDO;             properties.type = EXEC;   }
    |                endenum                  { properties.kind = ENDENUM;           properties.type = DEF;    }
    |                endforall                { properties.kind = ENDFORALL;         properties.type = EXEC;   }
    |                endfunction              { properties.kind = ENDFUNCTION;       properties.type = ENDDEF; }
    |                endif                    { properties.kind = ENDIF;             properties.type = EXEC;   }
    |                endinterface             { properties.kind = ENDINTERFACE;      properties.type = ENDDEF; }
    |                endmodule                { properties.kind = ENDMODULE;         properties.type = ENDDEF; }
    |                endprocedure             { properties.kind = ENDPROCEDURE;      properties.type = ENDDEF; }
    |                endprogram               { properties.kind = ENDPROGRAM;        properties.type = ENDDEF; }
    |                endselect                { properties.kind = ENDSELECT;         properties.type = EXEC;   }
    |                endsubmodule             { properties.kind = ENDSUBMODULE;      properties.type = ENDDEF; }
    |                endsubroutine            { properties.kind = ENDSUBROUTINE;     properties.type = ENDDEF; }
    |                endteam                  { properties.kind = ENDTEAM;           properties.type = EXEC;   }
    |                endtype                  { properties.kind = ENDTYPE;           properties.type = ENDDEF; }
    |                endwhere                 { properties.kind = ENDWHERE;          properties.type = EXEC;   }
    |                entry                    { properties.kind = ENTRY;             properties.type = DEF;    }
    |                enum                     { properties.kind = ENUM;              properties.type = DEF;    }
    |                forall_construct         { properties.kind = FORALL;            properties.type = EXEC;   }
    |                goto                     { properties.kind = GOTO;              properties.type = EXEC;   }
    |                goto_assigned            { properties.kind = GOTO_ASSIGNED;     properties.type = EXEC;   }
    |                goto_computed            { properties.kind = GOTO_COMPUTED;     properties.type = EXEC;   }
    |                if                       { properties.kind = SIMPLEIF;          properties.type = EXEC;   }
    |                if_construct             { properties.kind = IF;                properties.type = EXEC;   }
    |                include                  { properties.kind = INCLUDE;           properties.type = NONE;   }
    |                interface                { properties.kind = INTERFACE;         properties.type = DEF;    }
    |                labellist                { properties.kind = LABELLIST;         properties.type = NONE;   }
    |                module                   { properties.kind = MODULE;            properties.type = DEF;    }
    |                moduleprocedure          { properties.kind = PROCEDURE;         properties.type = DEF;    }
    |                nwayif                   { properties.kind = NWAYIF;            properties.type = EXEC;   }
    |                print                    { properties.kind = PRINT;             properties.type = EXEC;   }
    |                program_stmt             { properties.kind = PROGRAM;           properties.type = DEF;    }
    |                procedure                { properties.kind = PROCEDURE;         properties.type = DEF;    }
    |                read                     { properties.kind = READ;              properties.type = EXEC;   }
    |                io                       { properties.kind = IO;                properties.type = EXEC;   }
    |                iolist                   { properties.kind = IOLIST;            properties.type = NONE;   }
    |                selectcase               { properties.kind = SELECTCASE;        properties.type = EXEC;   }
    |                selectrank               { properties.kind = SELECTRANK;        properties.type = EXEC;   }
    |                selecttype               { properties.kind = SELECTTYPE;        properties.type = EXEC;   }
    |                simple_end               { properties.kind = END;               properties.type = EXEC;   }
    |                subroutine_stmt          { properties.kind = SUBROUTINE;        properties.type = DEF;    }
    |                function_stmt            { properties.kind = FUNCTION;          properties.type = DEF;    }
    |                submodule                { properties.kind = SUBMODULE;         properties.type = DEF;    }
    |                type                     { properties.kind = TYPE;              properties.type = DEF;    }
    |                typeis                   { properties.kind = TYPEIS;            properties.type = EXEC;   }
    |                use                      { properties.kind = USE;               properties.type = DEF;    }
    |                where_construct          { properties.kind = WHERE;             properties.type = EXEC;   }
    |                segment                  { properties.kind = SEGMENT;           properties.type = EXEC;   }
    |                endsegment               { properties.kind = ENDSEGMENT;        properties.type = EXEC;   }
    ;
blank:               BLANK
     ;
stlabel:             STLABEL getstlabel
       ;
named_label:         NAMED_LABEL getname
	   ;
module:              MODULE enable_identifier IDENTIFIER getname EOL
      ;
use:                 USE    enable_identifier IDENTIFIER getname enable_skipall SKIPALL
   ;
include:             INCLUDE     QSTRING getstring EOL {D(O("include"););}  /* include "file.inc" */
       ;
identifiers:         IDENTIFIER
	   |         identifiers ',' IDENTIFIER
	   ;
abstractinterface:   ABSTRACTINTERFACE  skipall
		 ;
contains:            CONTAINS           EOL
	;
interface:           INTERFACE skipall    /* interface [name, operator(*), assignment(=) */
	 |           INTERFACE1 enable_identifier IDENTIFIER getname skipall
	 ;

moduleprocedure:     MODULEPROCEDURE    enable_identifier identifiers getname EOL
	       ;
procedure:           PROCEDURE          enable_identifier IDENTIFIER getname EOL
	 ;


/* standard dictates that only                                         */
	/*    PROGRAM program_name                                             */
	/* is allowed, but there is (or was) at least one dialect that allows: */
	/*    PROGRAM                                                          */
	/* and another dialect that allows                                     */
	/*    PROGRAM program_name(p1,...,p8), comment                         */
	/* and                                                                 */
	/*    PROGRAM program_name,p1,...,p8, comment                          */
	/* so we will be a bit sloppy here                                     */

program_stmt:            PROGRAM enable_identifier IDENTIFIER getname enable_skip SKIP
	    |            PROGRAM enable_identifier EOL
	    ;
subroutine_stmt:         subroutineprefix subroutine subroutinename enable_skip SKIP
	       ;
subroutine:              subroutine_spec
	  ;
subroutine_spec:         SUBROUTINE
	       |         MODULESUBROUTINE
	       ;
subroutinename:          enable_identifier IDENTIFIER getname
	      ;
subroutineprefix:        empty
		|        subroutineprefix subroutineprefix_spec
		;
subroutineprefix_spec:   ELEMENTAL
		     |   IMPURE
		     |   PURE
		     |   RECURSIVE
		     |   NON_RECURSIVE
		     |   intrinsic_type_spec
		     |   TYPEC LR
		     |   CLASS LR
		     ;
function_stmt:           subroutineprefix function functionname LR enable_skip SKIP
	     ;
function:                function_spec
	;
function_spec:           FUNCTION
	     |           MODULEFUNCTION
	     ;
functionname:            enable_identifier IDENTIFIER getname
	    ;
submodule:               SUBMODULE LR getlr enable_identifier IDENTIFIER getname EOL
	 ;
intrinsic_type_spec:     BASICTYPE
		   |     BASICTYPE kind_selector
		   ;
kind_selector:           '*' I_NUMBER      /* extension */
	     |           '*' LR            /* extension */
	     |           LR
	     ;
entry:                   ENTRY enable_identifier IDENTIFIER skipall
     ;
endassociate:            ENDASSOCIATE  construct_name EOL
	    ;
endblock:                ENDBLOCK      construct_name EOL
	;
endblockdata:            ENDBLOCKDATA  construct_name EOL
	    ;
endcritical:             ENDCRITICAL   construct_name EOL
	   ;
enddo:                   ENDDO         construct_name EOL
     ;
endenum:                 ENDENUM       EOL
       ;
endforall:               ENDFORALL     construct_name EOL
	 ;
endfunction:             ENDFUNCTION   construct_name EOL
	   |             ENDFUNCTION   construct_name LR EOL  /* to accommodate preprocessors who recognize END FUNCTION MYFUNC (SOMETHING) */
	   ;
endif:                   ENDIF         construct_name EOL
     ;
endinterface:            ENDINTERFACE  skipall               /* end interface [name, operator(*), assignment(=)] */
	    ;
endmodule:               ENDMODULE     construct_name EOL
	 ;
endprocedure:            ENDPROCEDURE  construct_name EOL
	    |            ENDPROCEDURE  construct_name LR EOL  /* to accommodate preprocessors who recognize END PROCEDURE MYPROC (SOMETHING) */
	    ;
endprogram:              ENDPROGRAM    construct_name EOL
	  |              ENDPROGRAM    construct_name LR EOL  /* to accommodate preprocessors who recognize END PROGRAM MYPROG (SOMETHING) */
	  ;
endselect:               ENDSELECT     construct_name EOL
	 ;
endsubmodule:            ENDSUBMODULE  construct_name EOL
	    ;
endsubroutine:           ENDSUBROUTINE construct_name EOL
	     |           ENDSUBROUTINE construct_name LR EOL  /* to accommodate preprocessors who recognize END SUBROUTINE MYSUB (SOMETHING) */
	     ;
endteam:                 ENDTEAM       lr_construct_name EOL
       ;
endtype:                 ENDTYPE       construct_name EOL
       ;
endsegment:              ENDSEGMENT    EOL
	  ;
endwhere:                ENDWHERE      construct_name EOL
	;
simple_end:              END                          EOL
	  ;
gidentifier:             IDENTIFIER
	   |             TIDENTIFIER
	   ;
assignment:              lvalue '=' skipnoop  /* this includes '=>' */
	  ;
assign:                  ASSIGN I_NUMBER getlabel TO enable_identifier gidentifier EOL
      |                  if assign
      ;

else:                    ELSE           construct_name EOL
    ;
elseif:                  ELSEIF LR THEN construct_name EOL
      ;
elsewhere:               ELSEWHERE      lr_construct_name EOL
	 ;

if_construct:            IF     LR THEN EOL                             /* if (bla) then */
	    ;

if          :            IF     LR                                      /* if (bla) */
	    ;

nwayif    :              IF     LR I_NUMBER getlabel ',' I_NUMBER getlabel                        EOL /* if(bla) 100,200 ! CDC extension */ 
	  |              IF     LR I_NUMBER getlabel ',' I_NUMBER getlabel ',' I_NUMBER getlabel  EOL /* if(bla) 100,200,300 */
	  ;

where_construct:         WHERE  LR EOL
	       ;

forall_construct:        FORALL LR EOL
		;

do_construct:            do       I_NUMBER       getlabel skipall                   /* do 100[,] ...          */
	    |            do       gidentifier             skipall                   /* do i=1,10              */
	    |            docomma  gidentifier             skipall                   /* do, i=1,10             */
	    |            do                                       EOL               /* do                     */
	    |            DOWHILE      LR                          EOL               /* do while (i<10)        */
	    |            DOCONCURRENT LR                          EOL               /* do concurrent (i=1:20) */
	    ;
/* note: do, 100 i=1,10 is not valid fortran */

do:                      DO      enable_identifier
  ;
docomma:                 DOCOMMA enable_identifier
       ;

goto:                    goto_prefix  I_NUMBER    getlabel skipall          /* goto 100 */
    |                    if goto                    /* covers also: if(bla) if(bla) goto 100  which is not fortran */
    ;

goto_computed:           goto_prefix               LR   getlr skipall    /* goto (10,20,30,40)[,] i */
	     |           if goto_computed
	     ;
goto_assigned:           goto_prefix  gidentifier  LR getlr skipall       /* goto i[,] (10,20,30,40) */
	     |           if goto_assigned
	     ;
goto_prefix:             enable_cidentifier GOTO
	   ;

read:                    READ I_NUMBER getlabel skipall
    |                    if read
    ;

print:                   PRINT I_NUMBER getlabel skipall
     |                   if print
     ;

io:               IO  LR getlr skipall
  |               if io
  ;
iolist:           STARTIO itemlist
      ;
itemlist:                item
	|                itemlist ',' item
	|                itemlist ','
	|                itemlist EOL
	;
item:                    LABEL   {itemnumber++;} getlabel1        /* write(*,100)       */
    |                    LABELIS {itemnumber=123456;} LABEL getlabel /* fmt=10, err=10 ... */
    |                    LABELIS {itemnumber=123456;} otheritem      /* fmt="(a80)"        */
    |                    otheritem {itemnumber++;}
    ;
otheritem:               OTHER
	 |               OTHER otheritem
	 ;

labellist:               LABEL getlabel                   /* used in parselabels */
	 |               labellist ',' LABEL getlabel
	 |               labellist EOL
         ;

selectcase:              SELECTCASE LR EOL
	  ;
selectrank:              SELECTRANK LR EOL
	  ;
selecttype:              SELECTTYPE LR EOL
	  ;

call:                    CALL enable_identifier IDENTIFIER LR getlr skipall
    |                    CALL enable_identifier IDENTIFIER    getlr skipall
    |                    if call
    ;
calllist:                STARTCALL clist                 /* used in parsecall */
	;
clist:                   empty
     |                   clist citem
     |                   clist EOL
     ;
citem:                   LABEL getlabel2                               /* call sub(*100) */
     |                   OTHER
     ;

case:                    CASE           LR construct_name EOL        /* case (3) [name]   */
    ;
casedefault:             CASEDEFAULT       construct_name EOL        /* case default [name] */
	   ;

rank:                    RANK           LR  construct_name EOL       /* rank (3) [name]     */
    ;
rankdefault:             RANKDEFAULT       construct_name EOL        /* rank default [name] */
	   ;

classdefault:            CLASSDEFAULT      construct_name EOL
	    ;

classis:                 CLASSIS       LR  construct_name EOL
       ;
typeis:                  TYPEIS        LR  construct_name EOL
      ;

changeteam:              CHANGETEAM    LR  EOL
	  ;
block:                   BLOCK                    EOL
     ;
blockdata:               BLOCKDATA construct_name EOL
	 ;
associate:               ASSOCIATE LR             EOL
	 ;
critical:                CRITICAL                 EOL
	|                CRITICAL  LR             EOL
	;
enum:                    ENUM ','             skipall
    ;

type:                    type1                                       IDENTIFIER getname skipall
    |                    type1             ':' ':'                   IDENTIFIER getname skipall
    |                    type1 ',' skipnos ':' ':' enable_identifier IDENTIFIER getname skipall
type1:                   TYPE enable_identifier
     ;

segment:                 SEGMENT enable_identifier IDENTIFIER EOL
       ;

lvalue:                  gidentifier
      |                  gidentifier LR
      |                  lvalue '%' lvalue
      ;
construct_name:          enable_identifier empty                             /*           */
	      |          enable_identifier IDENTIFIER getname                /* name      */
	      ;
lr_construct_name:       enable_identifier empty                             /*           */
		 |       enable_identifier LR                                /* (..)      */
		 |       enable_identifier IDENTIFIER getname                /* name      */
		 |       enable_identifier LR IDENTIFIER getname             /* (..) name */
		 ;

skipall:                 enable_skipall SKIPALL
       ;
skipnoop:                enable_skipnoop SKIPNOOP
	;
skipnos:                 enable_skipnos 
       ;
enable_identifier:       {lexer_enable(IDENTIFIER);}
		 ;
enable_cidentifier:      {lexer_enable(CIDENTIFIER);}
		 ;
enable_skip:             {lexer_enable(SKIP);}
	   ;
enable_skipall:          {lexer_enable(SKIPALL);}
	      ;
enable_skipnoop:         {lexer_enable(SKIPNOOP);}
	       ;
enable_skipnos:          {lexer_enable(SKIPNOS);}
	      ;
getname:                 {properties.name=lexer_getname();}
       ;
getstlabel:              {properties.stlabel=lexer_getstlabel();}
	  ;
getlabel:                {properties.labels.push_back(lexer_geti_number());
	                  properties.label_pos.push_back(lexer_get_pos()-lexer_get_len());}
	;
getlabel1:               {if (itemnumber == 2)
	                    {properties.labels.push_back(lexer_geti_number());
	                     properties.label_pos.push_back(lexer_get_pos()-lexer_get_len());
			     }
			  }
	;
getlabel2:                {properties.labels.push_back(lexer_geti_number());
	                   /* minus one for the extra comma, plus two for preceding ',*' */
	                  properties.label_pos.push_back(lexer_get_pos()-lexer_get_len()-1+2);}
	 ;
getstring:               {properties.stringvalue=lexer_getstring();}
	 ;
getlr:                   {properties.lrvalue=lexer_getlr();
                          properties.lrpos=lexer_getlrpos();}
     ;
empty:               /* empty */
     ;
%%

void resetprop(void)
{
   properties.kind              = UNCLASSIFIED;
   properties.type              = EXEC;
   properties.labels.clear();
   properties.label_pos.clear();
   properties.stlabel           = "";
   properties.name              = "";
   properties.stringvalue       = "";
   properties.lrvalue           = "";
   properties.lrpos             = -123;
}

propstruct parseline(const Line_prep &p)
{
   D(O("entering parseline"););
   lexer_set(p,IDENTIFIER);
   resetprop();
   yyparse();
   if (properties.kind != UNCLASSIFIED)
      return properties;
#ifdef USEESOPE
   lexer_set(p,ESOPE);    // enables KEYWORD+ESOPE (SEGMENT, ENDSEGMENT)
#else
   lexer_set(p,KEYWORD);  // enables KEYWORD
#endif
   yyparse();
   return properties;
}

propstruct parselabels(const std::string &s)
{
   D(O("entering parselabels"););
   lexer_set(s,LABELS);
   resetprop();
   parselabeling = true;
   yyparse();
   return properties;
}

propstruct parseio(const std::string &s)
{
   D(O("entering parseio"););
   lexer_set(s,IOLIST);
   resetprop();
   parselabeling = true;
   itemnumber = 0;
   yyparse();
   return properties;
}

propstruct parsecall(const std::string &s)
{
   D(O("entering parsecall"););
   lexer_set(','+s,CALLLIST);    // ',' to assist lexer to recognise first label
   resetprop();
   parselabeling = true;
   yyparse();
   return properties;
}

void yyerror(const char *c)
{
    (void)c;
    D(O("ERROR");O(c););
    if(parselabeling)
    {
       properties.kind = UNCLASSIFIED;
       properties.type = EXEC;
       }
}

extern "C" int yywrap()
{
  D(O("yywrap"););
  return 1;
}
