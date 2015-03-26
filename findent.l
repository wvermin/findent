%{
#include <iostream>
#include <stdlib.h>
#include <stack>
using namespace std;
#include "findent_parser.h"
#include "findent.h"

#define YY_DECL extern "C" int yylex()

#define YY_USER_ACTION { lexer_position += yyleng; }

int lexer_position;

int                    plevel, wftype;
int                    start_state;
stack<int>             lexer_position_stack;
stack<int>             lexer_start_stack;
void quiet_compiler();
void lexer_set_state(int k);


%}
%x cmt rest dolabel wf wf1 ff scanstrcmt untermstr

   // end of line, including ; and !
eol [\n;!]

   // any character, not being end of line or start of string
neol [^\n;!\'\"]

   // blank character
b [ \t]

   // a character that can't be part of a word
nc [ \t\n;!\(\)]

   // an alphanumeric character or % or _ or space or tab or *
bc [[:alnum:]_%\* \t]

   // string starting with "
str1 \"[^\"]*\"

   // string starting with '
str2 \'[^\']*\'

   // unterminated string starting with "
ustr1 \"[^\"]*

   // unterminated string starting with '
ustr2 \'[^\']*

   // blancs or construct name
b_cname {b}*|{b}*[[:alnum:]_]+{b}*:{b}*   


  // following are keywords explicitly not-handled by this code
skk assign|backspace|call|close|common|continue|data|dimension|endfile|equivalence|external|format|goto|implicit|inquire|open|parameter|pause|print|read|return|rewind|rewrite|save|stop|write|allocatable|allocate|cycle|deallocate|exit|include|intent|namelist|nullify|only|operator|optional|pointer|private|procedure|public|result|sequence|target|use|asynchronous|bind|class|deferred|enum|enumerator|extends|final|flush|import|nopass|pass|protected|value|volatile|codimension|contiguous|error{b}*stop|sync{b}*all|sync{b}*images|sync{b}*memory|lock|unlock

  //  following are keywords starting with c
ckey  call|close|common|continue|case|contains|cycle|class|codimension|contiguous|critical|complex
%%

<INITIAL,cmt,dolabel,rest,wf,untermstr>{eol}	{  // end of line
		   D(O("EOL"); Y;)
		   yylval.semi_eol = (yytext[0] == ';');
		   switch (start_state)
		   {
		      case(FREE)      : BEGIN INITIAL;   break;
		      case(FINDFORMAT): BEGIN ff;        break;
		      case(UNTERMSTR) : BEGIN untermstr; break;
		      default         : BEGIN INITIAL;   break;
		   }
		   return EOL;
		}

<cmt>{   // skip to end of line
.  	 {
	    D(O("CMT"); Y;)
	 }
}
<rest>{     // skip to end of line or ;

{str1}  |
{str2}   {
            D(O("RESTSTRING"); Y;)
	 }

{ustr1} |
{ustr2}  {
            D(O("RESTUSTRING"); Y;)
	 }

{neol}*  {
	    D(O("REST"); Y;)
	 }
}

<dolabel>{        // find label after do as in: do 100 i=1,9
{b}*[0-9]+     {
		  D(O("DOLABEL:"); Y;)
		  yylval.val.ival = atoi(yytext);
		  BEGIN rest;
		  return DOLABEL;
	       }
[^;]              { BEGIN rest; }

}

<wf>{         // find rest of expression ending with ) Then change state to wf1

\(             {
		  plevel++;
                  D(O("wf + ");O(plevel);)
	       }

\)             {  plevel--;
                  D(O("wf - ");O(plevel);)
                  if (plevel == 0)
		  {
		     D(O("wf switch to wf1, wftype=");O(wftype);)
		     BEGIN wf1;
		  }
	       }
!.*            {
                  D(O("wf cmt ");Y;O(plevel);)
               }

.              {
                  D(O("wf . ");Y;O(plevel);)
               }

}

<wf1>{

{b}*/[\n;!]     { // if rest of line contains no code, return something meaningful
                    D(O("WFCONSTRUCT");O(wftype);Y;)
                    BEGIN rest;
		    return wftype;
	       }

.              { // code found after ), skip to rest of line
                  D(O("wf1 . ");Y;O(plevel);)
		  BEGIN rest;
	       }
}

<ff>{

^[^dDcC#!* 0-9].*\n {
                  D(O("FREE");Y;)
		  return FREE;
               }

^[ 0-9]{1,4}[[:alpha:]_%].*\n {
                  D(O("FREE");Y;)
		  return FREE;
	       }

^[^!]*!.*\n    {
                  D(O("UNSURE");Y;)
		  return UNSURE;
               }

^[ ]{5}&[^&]*\n {
                  D(O("UNSURE");Y;)
		  return UNSURE;
               }

^.{0,4}&{b}*\n {
                  D(O("FREE");Y;)
		  return FREE;
               }

^.{6,}&{b}*\n  {
                  D(O("FREE");Y;)
		  return FREE;
               }
                  
^[^&]*&{b}*\n  {
                  D(O("FREE");Y;)
		  return FREE;
               }

^[ ]{0,4}&.*\n {
                  D(O("FREE");Y;)
		  return FREE;
               }
^[ ]{6,}&.*\n  {
                  D(O("FREE");Y;)
		  return FREE;
               }
^{ckey}.*\n    {
                  D(O("FREE");Y;)
                  return FREE;
	       }
^c{b}+[[:alnum:]].*\n         {
                  D(O("FIXED");Y;)
		  return FIXED;
	       }
^[^!]{1,}.*!.*\n {
                  D(O("PROBFREE");Y;)
		  return PROBFREE;
	       }
^[ 0123456789]{6}.*\n {
                  D(O("UNSURE");Y;)
		  return UNSURE;
               }
.*\n           {
                  D(O("UNSURE");Y;)
		  return UNSURE;
	       }
}

<scanstrcmt>{
{str1}         {  // string like "foo"
		  D(O("scan STRING:");Y;)
	       }
{str2}         {  // string like 'foo'
		  D(O("scan STRING:");Y;)
	       }
!              {
                  D(O("scan CMT:");Y;O(lexer_position);)
		  return CMT;
	       }

\n             {
                  D(O("scan eol");Y;)
		  return EOL;
	       }
.              {
                  D(O("scan BLA");Y;)
	       }
}

<untermstr>{
{str1}         {
                  D(O("str1");Y;)
	       }

{str2}         {
                  D(O("str2");Y;)
	       }

\"             {
                  D(O("DQUOTE");Y;)
		  return DQUOTE;
	       }

\'             {
                  D(O("SQUOTE");Y;)
		  return SQUOTE;
	       }

.              {
                  D(O("untermstr BLA");Y;)
	       }

}

    /* INITIAL  */

{str1}         {  // string like "foo"
		  D(O("STRING:");Y;)
	       }
{str2}         {  // string like 'foo'
		  D(O("STRING:");Y;)
	       }

!.*            {
                  D(O("start cmt:");Y;)
                  BEGIN cmt;   // comment found, skip rest
	       }


^{b}*{skk}{b}+{bc}*                {   D(O("USE CALL ... ");Y;)
				       BEGIN rest;
				   }

^{b}*end{b}*[=\(\)].*              {   // false end
				       D(O("FALSE END");Y;)
				       BEGIN rest;
				   }
                                  
^{b}*end/{nc}+                     |
^{b}*end{b}*function/{nc}+         |
^{b}*end{b}*program/{nc}+          |
^{b}*end{b}*subroutine/{nc}+       |
^{b}*end{b}*do/{nc}+               |
^{b}*end{b}*if/{nc}+               |
^{b}*end{b}*module/{nc}+           |
^{b}*end{b}*submodule/{nc}+        |
^{b}*end{b}*where/{nc}+            |
^{b}*end{b}*associate/{nc}+        |
^{b}*end{b}*type/{nc}+             |
^{b}*end{b}*block/{nc}+            |
^{b}*end{b}*forall/{nc}+           |
^{b}*end{b}*critical/{nc}+         |
^{b}*end{b}*enum/{nc}+             |
^{b}*end{b}*while/{nc}+             {  // generic end
					  D(O("END");Y;)
					  BEGIN rest;
			  		  return END;
				    }

^{bc}*subroutine{b}+               |
^{b}*program{b}*/[;\n![:alpha:]]   |
^[^\'\"!]*{b}+function{b}          |
^[^\'\"!]*\)function{b}            |
^{b}*function{b}                   |
^{b}*block{b}*data/{nc}             {   // generic routine
				       D(O("ROUTINESTART");Y;)
				       BEGIN rest;
				       return ROUTINESTART;
				    }

^{bc}*enum/({b}*,)                  {   // enum
                                        D(O("ENUM");Y;)
					BEGIN rest;
					return ENUM;
				    }
^{b}*do{b}*\(                       {   // false do
                                        D(O("FALSEDO");Y;)
					BEGIN rest;
				    }
                                     

^{b_cname}do{b}*while/{nc}          |
^{b_cname}do/{nc}                   |
^{b_cname}do{b}*concurrent/{nc}     {  D(O("DOSTART");Y;)
				       BEGIN dolabel;
				       return DOSTART;
				    }

^{b_cname}select{b}*case/{b}*\(     {  // select case
				       D(O("DOSTART");Y;)
				       BEGIN rest;
				       return SELECTSTART;
				    }


^{b}*end{b}*select/{nc}             {  // endselect
				       D(O("ENDSELECT");Y;)
				       return ENDSELECT;
				    }
^{b}*module{b}*procedure/{nc}+      {
                                       BEGIN rest;
				    }

^{b}*module/{nc}                 |
^{b}*sub{b}*module/{nc}             {  // module
				       D(O("MODULESTART");Y;)
				       BEGIN rest;
				       return MODULESTART;
				    }
^{b}*abstract{b}+interface/{nc}    |
^{b}*interface/{nc}                 {  // module
				       D(O("INTERFACE");Y;)
				       BEGIN rest;
				       return INTERFACE;
				    }

^{b}*[0-9]+/{b}+[^0-9]              {  // statement label
				       D(O("STLABEL");Y;O(stlabel_found);)
				       if (stlabel_found)
				       {  // a statementlabel has already been found, do not return
				          BEGIN rest;
				       }
				       else
				       {
					  yylval.val.ival = atoi(yytext);
					  yylval.val.ilen = yyleng;
					  return STLABEL;
				       }
				    }

^{b}*contains/{b}*{eol}             {  // contains
				       D(O("CONTAINS");Y;)
				       BEGIN rest;
				       return CONTAINS;
				    }
^{b_cname}*block/{b}*{eol}                {  // block
				       D(O("BLOCK");Y;)
				       BEGIN rest;
				       return BLOCK;
				    }
^{b}*critical/{b}*{eol}             {  // critical
				       D(O("CRITCAL");Y;)
				       BEGIN rest;
				       return CRITICAL;
				    }

^{b_cname}if{b}*\(.*\){b}*then/{b}*{eol} { // if construct (if (...) then )
				       D(O("IFCONSTRUCT");Y;)
				       BEGIN rest;
				       return IFCONSTRUCT;
				    }

^{b}*where{b}*\(                     {
				       // where
				       D(O("WHERE");Y;)
				       plevel = 1;
				       wftype = WHERECONSTRUCT;
				       BEGIN wf;
				    }

^{b}*for{b}*all{b}*\(                {
				       // forall
				       D(O("FORALL");Y;)
				       plevel = 1;
				       wftype = FORALLCONSTRUCT;
				       BEGIN wf;
				    }

^{b}*associate{b}*\(                {
				       // associate
				       D(O("ASSOCIATE");Y;)
				       plevel = 1;
				       wftype = ASSOCIATECONSTRUCT;
				       BEGIN wf;
				    }

^{b}*case{b}*\(                     {
				       // case 
				       D(O("CASE");Y;)
				       plevel = 1;
				       wftype = CASE;
				       BEGIN wf;
				    }

^{b}*type[[:alnum:]_].*             {  // false type
                                       D(O("FALSETYPE");Y;)
				       BEGIN rest;
				    }
                           
^{b}*abstract{b}+type{b}*[,[:alpha:]:]            |
^{b}*type{b}*[,[:alpha:]:]            {
				       // type 
				       D(O("TYPECONSTRUCT");Y;)
				       BEGIN rest;
				       return TYPECONSTRUCT;
				    }

^{b}*case{b}*default/{nc}           {  // case default
				       D(O("CASE default");Y;)
				       BEGIN rest;
				       return CASE;
				    }

^{b}*else/{nc}                      {  // else
				       D(O("ELSE");Y;)
				       BEGIN rest;
				       return ELSE;
				    }

^{b}*else{b}*if/{nc}                {  // elseif
				       D(O("ELSEIF");Y;)
				       BEGIN rest;
				       return ELSEIF;
				    }

^{b}*else{b}*where/{nc}             {  // elsewhere
				       D(O("ELSEWHERE");Y;)
				       BEGIN rest;
				       return ELSEWHERE;
				    }

^{b}*entry/{nc}                     {  // entry
				       D(O("ENTRY");Y;)
				       BEGIN rest;
				       return ENTRY;
				    }

.	                            {  // not recognized, skip rest
				       D(O("BLA");Y;)
				       BEGIN rest;
				    }

%%

void lexer_set_state(int k)
{
   start_state = k;
   switch(k)
   {
      case(FREE): 
         BEGIN INITIAL;
         D(O("BEGIN INITIAL");O(YY_START);)
	 return;
      case(FINDFORMAT):
         BEGIN ff;
         D(O("BEGIN ff");O(YY_START);)
	 return;
      case(SCANSTRCMT):
         BEGIN scanstrcmt;
         D(O("BEGIN scanstrcmt");O(YY_START);)
	 return;
      case(UNTERMSTR):
         BEGIN untermstr;
         D(O("BEGIN untermstr");O(YY_START);)
	 return;
   }
}

void quiet_compiler()                   // to prevent warning about unused unput
{
   char *a=0;
   yyunput(1,a);
}

// the following functions are for use by the parser

void lexer_init()
{
   lexer_position = 0;
   lexer_set("\n",INITIAL);
}

void lexer_set(const string s, int start)
// sets new lexer state, next reading will be from 's', with start state 'start'
{
   D(O("before set");O(YY_START);O(lexer_position);)
   yy_delete_buffer(YY_CURRENT_BUFFER);
   lexer_set_state(start);
   yy_scan_string(s.c_str());
   lexer_position = 0;
   D(O("after set");O(YY_START);O(lexer_position);)
}

void lexer_push(const string s, int start)
// pushes new lexer state, next reading will be from 's', with start state 'start'
{
   D(O("before push");O(YY_START);O(lexer_position);)
   yypush_buffer_state(YY_CURRENT_BUFFER);
   lexer_start_stack.push(YY_START);
   lexer_set_state(start);
   yy_scan_string(s.c_str());
   lexer_position_stack.push(lexer_position);
   lexer_position = 0;
   D(O("after push");O(YY_START);O(lexer_position);)
}

void lexer_pop()
{
// pops lexer state off the stack
   D(O("before pop");O(YY_START);O(lexer_position);)
   yypop_buffer_state();
   start_state = lexer_start_stack.top();
   BEGIN start_state;
   lexer_start_stack.pop();
   lexer_position = lexer_position_stack.top();
   lexer_position_stack.pop();
   D(O("after pop");O(YY_START);O(lexer_position);)
}

