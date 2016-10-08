// $Id: findent.cpp 132 2016-10-02 15:11:46Z willem_vermin $
#include <cstdio>
#include <iostream>
#include <stack>
#include <queue>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include <string.h>
using namespace std;
#include "findent.h"
#include "version.h"
#include "parser.h"
extern "C" FILE *yyin;
string mygetline();
void get_full_statement();
void handle_free(string s,bool &more);
void remove_trailing_comment(string &s);
void handle_fixed(string s, bool &more);
void handle_preproc(string s, bool &more);
void output_line();
int pop_indent();
int top_indent();
void push_indent(int p);
void init_indent();
string whatprop(struct propstruct p);
string stoupper(const string s);
char firstchar(const string s);
char lastchar(const string s);

void push_rprops(struct propstruct p);
void init_rprops();
struct propstruct pop_rprops();
struct propstruct top_rprops();
const  struct propstruct empty_rprop={0,"","",""};

void handle_pre(const string s);

void handle_reading_from_tty();

int pop_dolabel();
int top_dolabel();
void push_dolabel(int l);
void empty_dolabels();
void indent_and_output();

void set_default_indents();
void usage(const bool doman);
void manout(const string flag, const string txt, const bool doman);
string trim(const string& str);
string rtrim(const string& str);
string ltab2sp(const string& s);
string handle_dos(const string s);

int input_format, output_format;
int guess_indent(const string str);
bool reading_from_tty = 0;
int lines_read        = 0;
int input_line_length = 0;
bool input_format_gnu = 0;
int determine_fix_or_free(const bool store);
bool isfixedcmt(const string s);
char fixedmissingquote(const string s);

stack<int> indent;                 // to store indents
stack<stack <int> > indent_stack;  // to store indent stack
stack<bool> pre_stack;             // to note if there is an #else after #if for indenting
stack<bool> prep_stack;            // to note if there is an #else after #if for routines

struct propstruct prev_props;
stack<struct propstruct> rprops;     // to store routines (module, subroutine ...)
stack<stack <struct propstruct> > rprops_stack;
struct propstruct cur_rprop = empty_rprop;

int cur_indent, start_indent;
bool auto_firstindent;
int num_lines;                    // number of lines read sofar
bool non_blank_line_seen;
bool indent_handled;

stack<int> dolabels;          // to store labels, necessary for labelled do
int stlabel;
int labelleng;

string full_statement;        // current statement, including continuation lines, &'s removed
deque <string> lines;         // current line, one continuation line per item
deque <string> olines;        // the original line
queue <string> linebuffer;    // used when determining fixed or free
stack <string> linestack;     // used for fixed format
bool end_of_file;

string endline   = "\n";
bool endline_set = 0;
const int default_indent = 3;
int all_indent;
int associate_indent;
int block_indent;
int cont_indent;
int contains_indent;
int case_indent;
int critical_indent;
int do_indent;
int entry_indent;
int enum_indent;
int forall_indent;
int if_indent;
bool indent_cont;
bool indent_contain;
int interface_indent;
int routine_indent, module_indent;
int select_indent; 
int type_indent;
int where_indent;

bool label_left;            // 1: put statement labels on the start of the line
const bool label_left_default = 1;

bool refactor_routines = 0; // 1: refactor routine-end statements
bool upcase_routine_type = 0; // 1: use 'SUBROUTINE' etc in stead of 'subroutine'

bool simple_end_found  = 0;

bool cur_semi_eol;

int main(int argc, char*argv[])
{
   all_indent          = default_indent;
   start_indent        = 0;                       // -I
   auto_firstindent    = 0;
   set_default_indents();
   num_lines           = 0;
   label_left          = label_left_default;
   non_blank_line_seen = 0;
   input_format        = 0;
   output_format       = 0;

   int c,rc;
   opterr = 0;
   while((c=getopt(argc,argv,"a:b:c:C:d:e:E:f:F:hHi:I:j:k:l:L:m:o:qr:R:s:t:vw:x:"))!=-1)
      switch(c)
         {
	   case 'a' :
	      associate_indent  = atoi(optarg);
	      break;
	   case 'b' :
	      block_indent      = atoi(optarg);
	      break;
	   case 'c' :
	      case_indent       = atoi(optarg);
	      break;
	   case 'C' :
	      if(optarg[0] == '-')
		 indent_contain = 0;
	      else
		 contains_indent   = atoi(optarg);
	      break;
	   case 'd' :
	      do_indent         = atoi(optarg);
	      break;
	   case 'e' :
	      entry_indent      = atoi(optarg);
	      break;
	   case 'E' :
	      enum_indent       = atoi(optarg);
	      break;
	   case 'f' :
	      if_indent         = atoi(optarg);
	      break;
	   case 'F' :   
	      forall_indent     = atoi(optarg);
	      break;
	   case 'h' :
	      usage(0);
	      return 0;
	      break;
	   case 'H':
	      usage(1);
	      return 0;
	      break;
	   case 'i' :
	      if      (string(optarg) == "fixed")
	         input_format = FIXED;
	      else if (string(optarg) == "free")
	         input_format = FREE;
	      else if (string(optarg) == "auto")
	         input_format = determine_fix_or_free(1);
	      else
	      {
		 all_indent        = atoi(optarg);
		 set_default_indents();
	      }
	      D(O("i flag:");O(optarg);O(" format:");
	        if (input_format == FIXED) O("fixed")
		if (input_format == FREE)  O("free")
		)
	      break;
	   case 'I' :
	      if (optarg[0] == 'a')
	         auto_firstindent = 1;
	      else
		 start_indent   = atoi(optarg);
	      break;
	   case 'j' :
	      interface_indent  = atoi(optarg);
	      break;
	   case 'k' :
	      if (optarg[0] == '-')
		 indent_cont = 0;
	      else
		 cont_indent = atoi(optarg);
	      break;
	   case 'l' :
	      label_left        = (atoi(optarg) != 0);
	      break;
	   case 'L' :
	      input_line_length = atoi(optarg);
	      input_format_gnu  = (optarg[strlen(optarg)-1] == 'g');
	   case 'm' :
	      module_indent     = atoi(optarg);
	      break;
	   case 'o' :
	      if(string(optarg) == "free")
	         output_format = FREE;
	      break;
	   case 'q' :
	      D(O("finding fix or free"););
	      handle_reading_from_tty();
	      rc = determine_fix_or_free(0);
	      switch(rc)
	      {
	         case FIXED : cout << "fixed" << endl;
		 break;
		 case FREE : cout << "free" << endl;
		 break;
	      }
	      return 0;
	      break;
	   case 'r' :
	      routine_indent    = atoi(optarg);
	      break;
	   case 'R':
	      switch(optarg[0])
	      {
	         case 'R' :
		    upcase_routine_type = 1;
		 case 'r' :
		    refactor_routines = 1;
		    break;
	      }
	      break;
	   case 's' :
	      select_indent     = atoi(optarg);
	      break;
	   case 't' :
	      type_indent       = atoi(optarg);
	      break;
	   case 'v' :
	      cout << "findent version "<<VERSION<<endl;
	      return 0;
	      break;
	   case 'w' :
	      where_indent      = atoi(optarg);
	      break;
	   case 'x' :
	      critical_indent   = atoi(optarg);
	      break;
	 }

   handle_reading_from_tty();

   if (input_format == 0)
      input_format = determine_fix_or_free(1);

   if(output_format == 0)
      output_format = input_format;

   labelleng        = 0;
   end_of_file      = 0;
   cur_rprop        = empty_rprop;
   get_full_statement();
   cur_semi_eol     = 0;
   cur_indent       = start_indent;
   init_indent();
   indent_and_output();
   if(end_of_file)
      return 0;

   while(1)
   {
      get_full_statement();
      indent_and_output();
      if (end_of_file)
	 return 0;
   }
   return 0;
}

void indent_and_output()
{
   D(O("entering indent_and_output");O(full_statement););
   string rest=full_statement;
   bool first_time = 1;
   while(1)
   {
      D(O("indent_and_output");O(rest););
      line_prep p(rest);
      propstruct props = parseline(p); 
      D(O("props.kind");O(props.kind);O(props.label););
      labelleng = props.label.size();
      if (labelleng > 0)
	 // if there was a previous labeled do, handle it:
      {
	 int ilabel = string2number<int>(props.label);
	 while ( top_dolabel() == ilabel )
	 {
	    pop_indent();
	    pop_dolabel();
	    cur_indent = top_indent();
	    indent_handled = 1;
	 }
      }
      // if the previous non-blank line was PROCEDURE (module procedure)
      // then determine if this was a procedure with content
      // if so: take delayed action with respect to indenting (same as with SUBROUTINE)
      if (prev_props.kind == PROCEDURE)
      {
	 switch (props.kind)
	 {
	    case ASSIGNMENT:
	    case UNCLASSIFIED:
	    case BLOCK:
	    case CONTAINS:
	    case CRITICAL:
	    case DO:
	    case END:
	    case ENDPROCEDURE:
	    case ENTRY:
	    case ENUM:
	    case FORALL:
	    case IF:
	    case SELECTCASE:
	    case SELECTTYPE:
	    case TYPE:
	    case WHERE:
	       cur_indent = top_indent();
	       D(O("cur_indent:");O(cur_indent););
	       push_indent(cur_indent + routine_indent);
	       empty_dolabels();
	       push_rprops(prev_props);
	       break;
	    default:
	       break;
	 }
      }
      cur_indent = top_indent();
      simple_end_found = 0;
      switch(props.kind)
      {
	 case SUBROUTINE:
	 case FUNCTION:
	 case PROGRAM:
	 case BLOCKDATA:
	    D(O("SUBFUN");O(whatprop(props));O(props.name););
	    cur_indent = top_indent();
	    D(O("cur_indent:");O(cur_indent););
	    push_indent(cur_indent + routine_indent);
	    empty_dolabels();
	    push_rprops(props);
	    break;
	 case MODULE:
	 case SUBMODULE:
	    D(O("MODULESTART");O(whatprop(props));O(props.name););
	    cur_indent = top_indent();
	    D(O("cur_indent:");O(cur_indent););
	    push_indent(cur_indent + module_indent);
	    empty_dolabels();
	    push_rprops(props);
	    break;
	 case BLOCK:
	    D(O("BLOCK"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + block_indent);
	    break;
	 case CRITICAL:
	    D(O("CRITICAL"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + critical_indent);
	    break;
	 case ENUM:
	    D(O("ENUM"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + enum_indent);
	    empty_dolabels();
	    break;
	 case ABSTRACTINTERFACE:
	 case INTERFACE:
	    D(O("INTERFACE"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + interface_indent);
	    empty_dolabels();
	    break;
	 case DO:
	    D(O("DOSTART"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + do_indent);
	    if (props.dolabel != "")
	    {
	       D(O("DOSTART LABEL");O(props.dolabel);O(string2number<int>(props.dolabel)););
	       push_dolabel(string2number<int>(props.dolabel));
	    }
	    break;
	 case SELECTCASE:
	 case SELECTTYPE:
	    D(O("SELECTCASE"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + select_indent);
	    break;
	 case CASE:
	 case CASEDEFAULT:
	 case CLASSDEFAULT:
	 case CLASSIS:
	 case TYPEIS:
	    D(O("CASE"););
	    cur_indent -= case_indent;
	    break;
	 case END:
	    D(O("GENERIC_END"););
	    if (!indent_handled)
	       cur_indent = pop_indent();
	    cur_rprop = top_rprops();
	    simple_end_found = 1;
	    D(O(whatprop(cur_rprop));O(cur_rprop.name););
	    pop_rprops();
	    break;
	 case ENDASSOCIATE:
	 case ENDBLOCK:
	 case ENDCRITICAL:
	 case ENDDO:
	 case ENDENUM:
	 case ENDFORALL:
	 case ENDIF:
	 case ENDINTERFACE:
	 case ENDMODULE:
	 case ENDSELECT:
	 case ENDSUBMODULE:
	 case ENDTYPE:
	 case ENDWHERE:
	    D(O("SPECIFIC_END"););
	    if (!indent_handled)
	       cur_indent = pop_indent();
	    break;
	 case ENDBLOCKDATA:
	 case ENDFUNCTION:
	 case ENDPROCEDURE:
	 case ENDPROGRAM:
	 case ENDSUBROUTINE:
	    D(O("ROUTINE_END"););
	    if (!indent_handled)
	       cur_indent = pop_indent();
	    D(O(whatprop(cur_rprop));O(cur_rprop.name););
	       pop_rprops();
	    break;
	 case PROCEDURE:  // in fact: moduleprocedure
	    D(O("PROCEDURE");O(whatprop(props));O(props.name););
	    // depending on what follows this will be 
	    // recognized as a module procedure with content
	    // or only a moduleprocedure specification
	    break;
	 case CONTAINS:
	    D(O("CONTAINS"););
	    if (indent_contain)
	       cur_indent -= contains_indent;
	    else
	    {
	       cur_indent = start_indent;
	       pop_indent();
	       push_indent(cur_indent);
	    }
	    break;
	 case IF:
	    D(O("IFCONSTRUCT"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + if_indent);
	    break;
	 case ELSE:
	    D(O("ELSECONSTRUCT"););
	    cur_indent -= if_indent;
	    break;
	 case ELSEIF:
	    D(O("ELSEIF"););
	    cur_indent -= if_indent;
	    break;
	 case ELSEWHERE:
	    D(O("ELSECONSTRUCT"););
	    cur_indent -= where_indent;
	    break;
	 case ENTRY:
	    D(O("ENTRY"););
	    cur_indent -= entry_indent;
	    break;
	 case WHERE:
	    D(O("WHERECONSTRUCT"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + where_indent);
	    break;
	 case ASSOCIATE:
	    D(O("ASSOCIATECONSTRUCT"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + associate_indent);
	    break;
	 case TYPE:
	    D(O("TYPECONSTRUCT"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + type_indent);
	    break;
	 case FORALL:
	    D(O("FORALLCONSTRUCT"););
	    cur_indent = top_indent();
	    push_indent(cur_indent + forall_indent);
	    break;
	 default:
	    cur_indent = top_indent();
      }
      switch(props.kind)
      {
	 case BLANK:
	    break;
	 default:
	    prev_props = props;
      }
      if(first_time)
      {
	 D(O("calling output_line"););
	 output_line();
	 first_time = 0;
      }
      rest = p.get_line_rest();
      if (rest == "")
	 break;
   }
}

void handle_reading_from_tty()
{
   reading_from_tty = isatty(fileno(stdin));
   if (reading_from_tty)
   {
      cerr << "! Warning: reading from terminal"                << endl;
      cerr << "! End this session by typing a single dot ('.')" << endl;
      cerr << "!     on a new line"                             << endl;
      cerr << "! "                                              << endl;
      cerr << "! Examples of typical usage:"                    << endl;
      cerr << "!   help:    findent -h"                         << endl;
      cerr << "!   indent:  findent < in.f > out.f"             << endl;
      cerr << "!   convert: findent -ofree < prog.f > prog.f90" << endl;
   }
}

void init_indent()
// fills the indent-stack until indent 0
{
   D(O("init_indent");O(indent.size()););
   while(!indent.empty())
      indent.pop();
   int l=0;
   for (int i=0; l<start_indent; i++)
   {
      indent.push(l);
      l = i*default_indent;
   }
   indent.push(start_indent);
}

string trim(const string& str)
{
    const string whitespace = " \t";
    const size_t strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const size_t strEnd = str.find_last_not_of(whitespace);
    const size_t strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

string rtrim(const string& str)
{
    const string whitespace = " \t";

    const size_t strEnd = str.find_last_not_of(whitespace);
    if (strEnd == string::npos)
        return ""; // no content
    return str.substr(0,strEnd+1);
}

string ltab2sp(const string& s)
{  // converts leading white space and white space after a statement label
   //   to spaces and removes trailing white space
   // if line starts with 0-5 spaces or digits followed by a tab, followed
   //   by 1-9, this is counted as 5 spaces, it appears that many compilers
   //   assume that 
   //   <tab>1  <some code>
   //   is a continuation statement, if the continuation character is 1-9
   //   If the character is not within 1-9, it is assumed that 
   //   this character is the first of a statement, so in this case
   //   this is counted as 6 spaces

   int si         = 0;
   bool ready     = 0;
   const int tabl = 6;
   bool firsttab  = 1;
   string leader  = "";
   int removed    = 0;

   for (unsigned int j=0; j<s.length(); j++)
   {
      switch (s[j])
      {
	 case ' ' :
	    si ++;
	    removed++;
	    leader = leader + " ";
	    break;
	 case '\t' :
	    if (firsttab)
	    {
	       firsttab = 0;
	       if (si < 6)
	          // investigate next char: if 1-9, count this tab as 5 spaces
	          if (s.length() > j+1)
		     if (s[j+1] >= '1' && s[j+1] <= '9')
		     {
		        si = 5;
			removed++;
			leader = string(5,' ');
			break;
		     }
	    }
	    si = (si/tabl)*tabl + tabl;
	    leader = leader + string(si - leader.length(),' ');
	    break;
	    case '0': case '1': case '2': case '3': case '4':
	    case '5': case '6': case '7': case '8': case '9':
	    si++;
	    removed++;
	    leader += s[j];
	    firsttab = 0;
	    break;
	 default:
	    ready = 1;
	    break;
      }
      if(ready)
	 break;
   }
   return leader + trim(s.substr(removed));
}

string mygetline()
{
   // reads next line from cin.
   // side effects:
   //   end_of_file is set if endoffile condition is met
   //   endline is set to \n or \r\n
   //   lines_read is incremented

   string s;

   getline(cin,s);
   D(O("getline:");O(s);O("eofbit");O(cin.eofbit);O(cin.eof()););

   // sometimes, files do not end with (cr)lf, hence the test for s=="":
   end_of_file = (cin.eof() && s == "");

   lines_read ++;

   if (!end_of_file && reading_from_tty)
      end_of_file = (s == ".");

   return handle_dos(s);

}

void get_full_statement()
   // this function collects 'full_statement': a complete
   // fortran line, concatenated from possible continuation lines,
   // comments and preprocessor lines removed
   //
   // full_statement is used to determine the indentation
   //
   // Also, every line is stored in 'lines'
   // 
   // If the input starts with a comment or a preprocessor line,
   // full_statement = "", lines will contain the line, possibly
   // followed by preprocessor continuation lines as in:
   /* #define foo \   */
   //           bar
   //
   // A totally empty line is treated as a comment line,
   //
   // Comment lines and preprocessor lines can be embedded in 
   // fortran continuation lines as in:
   //
   //    subroutine compute( &
   // #ifdef one
   //      x &
   // #else
   //      x,y &
   // #endif
   //     )
   //
   // In this example, full_stataement will contain:
   //
   //    subroutine compute(x x,y)
   // ( this is not correct, of course, but it will not prevent 
   //   correct indenting)
   //
   // and lines will contain:
   //
   // #ifdef one
   //      x &
   // #else
   //      x,y &
   // #endif
   //     )
   //
{
   string s;
   full_statement       = "";
   indent_handled       = 0;
   bool preproc_more    = 0;
   bool fortran_more    = 0;

   while(1)
   {
      if (!linestack.empty())
      {
         s = linestack.top();
	 linestack.pop();
      }
      else if (linebuffer.empty())
      {
         s = mygetline();
      }
      else
      {
         s = linebuffer.front();
	 linebuffer.pop();
	 if (reading_from_tty && s == ".")
	    end_of_file = 1;
      }

      num_lines++;

      D(O(s););

      if (!non_blank_line_seen)
      {
         if (input_format == FREE)
	    non_blank_line_seen = (trim(s) != "");
	 else
	    {
	       string s1;
	       s1 = ltab2sp(s);
	       if (s1.length() > 6)
	          non_blank_line_seen = (s[0] != 'c' && s[0] != 'C' && s[0] != '!' && s[0] != '*' && s[0] != '#');
	    }
	 if (auto_firstindent && non_blank_line_seen)
	 {
	    start_indent = guess_indent(s);
	    cur_indent   = start_indent;
	    init_indent();
	    indent_handled = 1;
	 }
      }

      D(O("get_full_statement s leng:");O(s);O(s.length());O("endline:");O(endline.length()););

      if(preproc_more || (firstchar(s) == '#'))
      {
	 D(O("preproc");O(s);O(preproc_more););
	 handle_preproc(s, preproc_more);
	 if (preproc_more || fortran_more)
	    continue;
	 else
	    break;
      }

      if (input_format == FREE)
      {
	 handle_free(s,fortran_more);
	 if (fortran_more) 
	    continue;
	 else
	    break;
      }
      else
      {
	 handle_fixed(s, fortran_more);
	 if (fortran_more)
	    continue;
	 else
	    break;
      }
   }
   D(O("full_statement:");O(num_lines);O(full_statement););
   D(O("lines:"); for (unsigned int i=0; i<lines.size(); i++) { O(i);O(lines[i]); })
}

void handle_preproc(string s, bool &more)
   // adds preprocessor continuation line s to full statement
   // more = 1: more preprocessor lines are to expected
   //        0: this line is complete
{
   if(end_of_file)
   {
      D(O("end of file"););
      more = 0;
      return;
   }
   string sl = rtrim(s);
   lines.push_back(sl);
   olines.push_back(s);
   if(lastchar(sl) == '\\')
      more = 1;
   else
      more = 0;
}

void handle_free(string s, bool &more)
// adds input line s to full_statement
// more 1: more lines are to expected
//        0: this line is complete

{
   if(end_of_file)
   {
      D(O("end of file"););
      more = 0;
      return;
   }

   if (input_line_length !=0)
      s = s.substr(0,input_line_length);

   string sl     = trim(s);

   if (firstchar(sl) == '&')
   {
      sl.erase(0,1);
      sl = trim(sl);
   }

   //  if this line is pure comment or empty or preprocessor statement:
   //     add it to lines

   if (sl == "" || firstchar(sl) == '!' )
   {
      lines.push_back(trim(s));
      olines.push_back(s);
      return;
   }

   // Investigate if this line wants a continuation.
   // We append the line to full_statement, and look for
   // an unterminated string. 
   // If there is an unterminated
   // string, and the line ends with '&', a continuation
   // line is expected.
   // If there is no unterminated string, we strip of
   // a possible trailing comment, and trim.
   // If the last character = '&', a continuation is expected.

   string cs = full_statement + sl;

   remove_trailing_comment(cs);
   cs = rtrim(cs);

   more = ( lastchar(cs) == '&');
   if (more)            // chop off '&' :
      cs.erase(cs.length()-1);

   full_statement = cs;
   lines.push_back(trim(s));
   olines.push_back(s);
   D(O("full_statement");O(full_statement);O(more););
}

void handle_fixed(string s, bool &more)
{
   if(end_of_file)
   {
      more = 0;
      return;
   }

   string s0 = s;

   D(O("fixed s");O(s););

   if (input_line_length != 0)
   {
      // with tabbed input there is a difference between
      // gfortran and other compilers
      // other compilers simply count the number of characters.
      // gfortran always assumes that the
      // continuation character is in column 6
      // so this needs extra attention:
      if (input_format_gnu) // convert leading tabs to spaces
         s = ltab2sp(s);
      s = s.substr(0,input_line_length);
   }

   if (isfixedcmt(s))
   {  // this is a blank or comment or preprocessor line
      lines.push_back(trim(s));
      olines.push_back(s);
      if (lines.size() ==1)
         more = 0;   // do not expect continuation lines
      else
	 more = 1;      // but here we do
      return;
   }

   // replace leading tabs by spaces
   s = ltab2sp(s);
   D(O("fixed:");O("s");O(s););

   string sl;
   sl = s.substr(0,5);
   if (s.length() >6)
      sl = sl+' '+s.substr(6);

   D(O("sl:");O(sl);O(lines.size()););

   // this is a line with code
   if (lines.empty())
   {  // this is the first line
      lines.push_back(s);
      olines.push_back(s);
      full_statement += trim(sl);
      remove_trailing_comment(full_statement);
      full_statement = rtrim(full_statement);
      D(O("sl:");O(sl);O(full_statement);O(lines.size()););
      more = 1;      // maybe there are continuation lines
      return;
   }

   // this is possibly a continuation line
   if (s.length() < 6 || s[5] == ' ' || s[5] == '0')
   {  // this is not a continuation line
      // push it back, we will see it later
      // we push back the line in it's original state
      // to prevent double line truncation
      linestack.push(s0);
      more = 0;          // do not look for further continuation lines
      return;
   }
   // this is a continuation line
   lines.push_back(s);
   olines.push_back(s);
   full_statement += rtrim((rtrim(sl)+"      ").substr(6));
   remove_trailing_comment(full_statement);
   full_statement = rtrim(full_statement);
   D(O(full_statement);O(rtrim(sl));O(sl););
   more = 1;   // look for more continuation lines
   return;
}

void remove_trailing_comment(string &s)
{
   // removes trailing comment

   bool instring = 0;
   char q        = ' ';
   for (unsigned int i=0; i<s.size(); i++)
   {
      if (instring)
      {
	 if(s[i] == q)
	    instring = 0;
      }
      else
      {
	 switch(s[i])
	 {
	    case '"': case '\'':
	       instring = 1;
	       q = s[i];
	       break;
	    case '!':
	       s.erase(i,string::npos);
	       return;
	 }
      }
   }
   return;
}

int guess_indent(const string s)
{ // count spaces at start of line, correct for tabs and & and label
   int si         = 0;
   bool ready     = 0;
   const int tabl = 8;

   D(O("guess_indent entered"););

   if (input_format == FIXED)
   {
      string s1 = ltab2sp(s);
      si        = s1.find_first_not_of(' ') -6;
      if (si <0)
         si = 0;
      return si;
   }

   for (unsigned int j=0; j<s.length(); j++)
   {
      switch (s[j])
      {
	 case ' ' : case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9' :
	    si ++;
	    break;
	 case '&' :
	    si++;
	    ready = 1;
	    break;
	 case '\t' :
	    si = (si/tabl)*tabl + tabl;
	    break;
	 default:
	    ready = 1;
	    break;
      }
      if(ready)
	 break;
   }
   return si;
}

string stoupper(const string s)
{
   string sl = s;
   int l     = sl.size();
   for (int i=0; i<l; i++)
      sl[i] = toupper(sl[i]);
   return sl;
}

void output_line()
{
   D(O("output_line, nr of lines:");O(lines.size());O("cur_indent");O(cur_indent);O("already done:"););
   if (lines.empty())
      return;
   int lineno                     = 0;
   string needamp                 = "";
   char prevquote                 = ' ';
   char preprevquote              = ' ';
   string outputline;

   D(O("refactor");O(refactor_routines);O(simple_end_found);O(lines.size());O(cur_rprop.kind););
   if (refactor_routines && simple_end_found)
   {
      if (cur_rprop.kind != 0)
      {
	// modify first line to match the corrsponding module, subroutine ... line  
	// find the location of 'end'
	string s = stoupper(lines[0]);
	size_t p = s.find("END");
	D(O("p");O(p););
	if ( p != string::npos)
	{
	   string routinetype = whatprop(cur_rprop);
	   if (upcase_routine_type)
	      routinetype = stoupper(routinetype);
	   lines[0] = lines[0].substr(0,p+3)+
	   " "+routinetype+" "+cur_rprop.name + lines[0].substr(p+3);
	   D(O(s);O(lines[0]););
	}
      }
   }

   if (input_format == FREE)
   {
      string firstline = lines.front();
      string ofirstline = olines.front();
      char ofc = firstchar(ofirstline);
      D(O("firstline");O(firstline););
      lines.pop_front();
      olines.pop_front();
      if(firstchar(firstline) == '#')
      {
	 handle_pre(firstline);
      }
      else
      {
	 int l;
	 if (firstline != "" || lines.size() > 1)
	 {
	    if (label_left && labelleng > 0)
	    {  // put label at start of line
	       string label = firstline.substr(0,labelleng);
	       firstline    = trim(firstline.substr(labelleng));
	       cout << label;
	       l = cur_indent - labelleng;
	       if ( l <= 0 )
		  l=1;
	    }
	    else
	       l = cur_indent;

	    // If first character of original line is '!', do not indent
	    // Do not use start_indent: on the next run the first char could
	    // be space and not '!'
	    if(ofc == '!')
	       l=0;

	    // if this is a comment line, and the original line did not start
	    // with '!', and cur_indent == 0: put a space before this line, 
	    // to make sure that re-indenting will give the same result
	    //
	    if (ofc != '!' && firstchar(firstline) == '!' && cur_indent == 0)
	       l=1;

	    if (l<0)
	       l=0;
	    cout << string(l,' '); 
	    D(O("output indent before firstline");O(string(l,' ')););
	 }
	 cout << firstline << endline;
	 D(O("output firstline");O(firstline););
      }
      while (!lines.empty())
      {
	 // sometimes, there are preprocessor statements within a continuation ...
	 string s = lines.front();
	 string os = olines.front();
	 char fc  = firstchar(s);
	 char ofc  = firstchar(os);
	 lines.pop_front();
	 olines.pop_front();
	 if(fc == '#')
	 {
	    handle_pre(s);
	 }
	 else
	 {
	    if (indent_cont || fc == '&')
	    {
	       int l = 0;
	       // if first character of original line == '!', do not indent
	       // Do not use start_indent: on the next run the first char could
	       // be space and not '!'
	       if (ofc == '!')
		  l = 0;
	       else
	       {
		  if (fc == '&')
		  {
		     // if continuation starts with '&', use current indentation
		     // else use current indentation + cont_indent 
		     l = max(cur_indent,0);
		  }
		  else
		  {
		     l = max(cur_indent+cont_indent,0);
		  }
	       }
	       // if this is a comment line, and the original line did not start
	       // with '!', and cur_indent == 0: put a at least one space before this line, 
	       // to make sure that re-indenting will give the same result
	       //
	       if (ofc != '!' && fc == '!' && cur_indent == 0)
		  l=max(1,cont_indent);
	       cout << string(l,' ');
	       cout << s <<endline;
	    }
	    else
	    {
	       cout << os << endline;
	    }
	 }
      }
   }
   else   // input_format = FIXED, output can be FREE or FIXED
   {
      unsigned int old_indent = 0;
      unsigned int first_indent = 0;
      while(!lines.empty())
      {
         lineno++;
         string s = lines.front();
	 string os = olines.front();
	 char ofc  = firstchar(os);
	 lines.pop_front();
	 olines.pop_front();
	 if (firstchar(s) == '#')
	 {
	    handle_pre(s);
	    continue;
	 }
	 if(isfixedcmt(s))
	 {  // this is an empty line or comment line or a preprocessing line
	    if (output_format == FIXED)
	       cout << trim(s) << endline;
	    else  // output_format = FREE
	    {
	       if (trim(s) == "")
	          cout << endline;
	       else
	       {
	          switch (firstchar(s))
		  {
	             // special hack for lines starting with 'd' or 'D'
	             case 'd' :
		     case 'D' :
		        cout << "!" + trim(s) << endline;
			break;
	             default:
			cout << string(max(cur_indent,0),' ') << "!" << trim(s.substr(1)) << endline;
		  }
	       }
	    }
	 }
	 else
	 {
	    if (output_format == FIXED)
	    {
	       cout << s.substr(0,6);
	       
	       if(s.length() > 6)
	       {
		  bool iscont = (s[5] != ' ' && s[5] != '0');

		  // try to honor current indentation
		  // if this is a continuation line, count the number
		  // of spaces after column 6.
		  if (iscont)
		  {
		     string s6 = ltab2sp(s.substr(6))+'x';
		     old_indent = s6.find_first_not_of(' ');
		  }
		  else
		  {  // count the number of spaces after column 6 of the first line
		     string s6 = ltab2sp(s.substr(6))+'x';
		     first_indent = s6.find_first_not_of(' ');
		  }
		  int adjust_indent = old_indent - first_indent;
		  if (adjust_indent < 0)
		     adjust_indent = 0;

		  switch(prevquote)
		  {
		     case ' ' :   // no dangling strings, output with indent
			cout << string(max(adjust_indent+cur_indent,0),' ') << trim(s.substr(6));
			break;
		     default:  // dangling string, output asis
			cout << s.substr(6);
			break;
		  }
		  // investigate if this line terminates a string

		  prevquote = fixedmissingquote(prevquote + s);
	       }
		        
	       cout << endline;
	    }
	    else  // output_format == FREE
	    {
	       outputline = needamp;
	       if (lineno == 1)
	       {
		  D(O("s");O(s);O(labelleng););
		  int l;
		  if (s.length()>6)
		  {
		     string s6 = ltab2sp(s.substr(6))+'x';
		     first_indent = s6.find_first_not_of(' ');
		  }
		  if (s != "" || lines.size() > 1)
		  {
		     if (label_left && labelleng > 0)
		     {  // put label at start of line
		        s = trim(s);
			string label = s.substr(0,labelleng);
			s    = trim(s.substr(labelleng));
			cout << label;
			l = cur_indent - labelleng;
			if ( l <= 0 )
			   l=1;
		     }
		     else
			l = cur_indent;

		     cout <<string(max(l,0),' '); 
		  }
		  outputline += trim(s);
		  // we need the value of prevquote later on:
		  preprevquote = prevquote;
		  prevquote    = fixedmissingquote(prevquote + s);
	       }
	       else
	       {
	          if (s.length() >6)
		  {
		     cout << string(max(cur_indent,0),' ');
		     // try to honor current indentation
		     // if this is a continuation line, count the number
		     // of spaces after column 6.
		     if (s[5] != ' ' && s[5] != '0')
		     {
			string s6 = ltab2sp(s.substr(6))+'x';
			old_indent = s6.find_first_not_of(' ');
			D(O(s6);O(old_indent););
		     }
		     D(O(old_indent);O(first_indent););
		     int adjust_indent = old_indent - first_indent;
		     if (adjust_indent < 0 || prevquote!=' ')
			adjust_indent = 0;

		     string s1 = s.substr(6);
		     if (prevquote == ' ')
			s1 = trim(s1);
		     if (s1 != "")
			outputline += string(adjust_indent,' ');
		     outputline += s1;
		     // we need the value of prevquote later on:
		     preprevquote = prevquote;
		     prevquote = fixedmissingquote(prevquote + s1);
		  }
	       }
	       // check for continuation lines
	       // there is a continuation line if there is a non-comment in 
	       // the lines deque, in that case a trailing & is needed
	       // and the non-comment line needs a leading &

	       needamp="";
	       deque<string>::iterator it= lines.begin();
               char prevlchar = 0;
               bool inpreproc = 0;
	       while(it != lines.end())
	       {
                  // if previous lastchar was '\\', do not consider this 
                  // line, if we are in a preprocessor statement
                  D(O(*it);O(firstchar(*it));O(lastchar(*it));O(prevlchar);O(inpreproc););
                  if (inpreproc && prevlchar == '\\')
                  {
                     D(O("skipit"););
                     prevlchar = lastchar(*it);
                     *it++;
                     continue;
                  }
                  prevlchar = lastchar(*it);
                  inpreproc = (firstchar(*it) == '#');
		  if (!isfixedcmt(*it++))
		  {
                     D(O("!isfixedcmt"););
		     needamp =  '&';
		     // we have to put an '&' at the end of the line,
		     // but what if the line ends in a ! comment, as in
		     //   x = x *  ! comment
		     // Then we have to put the '&' like this:
		     //   x = x * & ! comment
		     // but in this case:
		     //  x = ' a string ! no comment
		     // we have to place the '&' like this:
		     //  x = ' a string ! no comment&

                     // if there is an unterminated string, then:
		     if (prevquote != ' ')
		     {
		        outputline = outputline + '&';
			break;
		     }

                     // the line does not contain an unterminated string,
		     // is there a !comment outside a string?
		     // here we need the value of preprevquote:

		     string work = preprevquote + outputline;
		     int p0 = work.size();
		     remove_trailing_comment(work);
		     int p = work.size();
		     if (p0 != p)
		     {
			// there is a comment outside strings,
			// the number of characters before the comment
			// has to be decreased by one, because
			// of the prepending of preprevquote
			p = max(p-1,0);
			outputline = outputline.substr(0, p) + '&' + outputline.substr(p);
			break;
		     }
		     else
		     {
		        // no !comment, simply put '&' at the end
			outputline += '&';
			break;
		     }
		  }
	       }
	       cout << outputline << endline;
	    }
	 }
      }
   }
}

int pop_indent()
{
   if (indent.empty())
      return 0;
   indent.pop();
   return top_indent();
}

int top_indent()
{
   if (indent.empty())
      return 0;
   return indent.top();
}

void push_indent(int p)
{
   indent.push(p);
}

void push_rprops(struct propstruct p)
{
   rprops.push(p);
}

void init_rprops()
// empties the rprops-stack
{
   D(O("rprops");O(rprops.size()););
   while(!rprops.empty())
      rprops.pop();
}

struct propstruct pop_rprops()
{
   if (rprops.empty())
      return empty_rprop;
   rprops.pop();
   return top_rprops();
}

struct propstruct top_rprops()
{
   if (rprops.empty())
      return empty_rprop;
   return rprops.top();
}

string whatprop(struct propstruct p)
{
    switch(p.kind)
    { 
       case SUBROUTINE:
       case MODULESUBROUTINE: return("subroutine");
       case PROGRAM:          return("program");
       case BLOCKDATA:        return("blockdata");
       case FUNCTION:
       case MODULEFUNCTION:   return("function");
       case MODULE:           return("module");
       case SUBMODULE:        return("submodule");
       case PROCEDURE:        return("procedure");
       default:               return("UNKNOWN");
    }
 }

int pop_dolabel()
{
   if (dolabels.empty())
      return -1;
   dolabels.pop();
   return top_dolabel();
}

int top_dolabel()
{
   if (dolabels.empty())
      return -1;
   return dolabels.top();
}

void push_dolabel(int p)
{
   dolabels.push(p);
}

void empty_dolabels()
{
   while(!dolabels.empty())
      dolabels.pop();
}

void set_default_indents()
{
   associate_indent = all_indent;              // -a
   block_indent     = all_indent;              // -b
   case_indent      = all_indent-all_indent/2; // -c
   contains_indent  = all_indent;              // -C
   indent_cont      = 1;                       // !-k-
   indent_contain   = 1;                       // !-C-
   do_indent        = all_indent;              // -d
   entry_indent     = all_indent-all_indent/2; // -e
   enum_indent      = all_indent;              // -E
   if_indent        = all_indent;              // -f
   forall_indent    = all_indent;              // -F
   interface_indent = all_indent;              // -j
   cont_indent      = all_indent;              // -k
   module_indent    = all_indent;              // -m
   routine_indent   = all_indent;              // -r
   select_indent    = all_indent;              // -s
   type_indent      = all_indent;              // -w
   where_indent     = all_indent;              // -w
   critical_indent  = all_indent;              // -x
}

void usage(const bool doman)
{
   if (doman)
   {
      cout << ".\\\" DO NOT MODIFY THIS FILE! It was created by findent -H"                << endl;
      cout << ".TH FINDENT \"1\" \"2015\" \"findent-" << VERSION << "\" \"User Commands\"" << endl;
      cout << ".SH NAME"                                                                   << endl;
      cout << "findent \\- Indents and optionally converts Fortran program source"         << endl;
      cout << ".SH SYNOPSIS"                                                               << endl;
      cout << ".B findent"                                                                 << endl;
      cout << "[\\fIOPTION\\fR]..."                                                        << endl;
      cout << ".PP"<<endl<< "findent reads from stdin and writes to stdout."               << endl;
      cout << ".SH DESCRIPTION"                                                            << endl;
      cout << "Findent indents a Fortran source. Findent uses various kinds of"            << endl;
      cout << "indentations, see OPTIONS. Findent can convert from fixed form to"          << endl;
      cout << "free form, and can supplement single END statements, see 'Refactor' below." << endl;
      cout << "Errors in OPTIONS are silently ignored."                                    << endl;
      cout << ".PP" << endl << ".SS \"General options:"                                    << endl;
   }
   else
   {
      cout << "findent [options]"                       << endl;
      cout << "   format fortran program"               << endl;
      cout << "   reads from STDIN, writes to STDOUT"   << endl;
      cout << "   comment lines with '!' in column one are not indented" << endl;
      cout << "options: (errors are silently ignored)"  << endl;
      cout << "  general:"                              << endl;
   }

   manout("-h","print this text",                                         doman);
   manout("-H","print man page",                                          doman);
   manout("-v","prints findent version",                                  doman);
   manout("-q","guess free or fixed, prints 'fixed' or 'free' and exits", doman);
   manout("-l","(0/1) 1: statement labels to start of line (default:1)",  doman);
   manout(" ","      (only for free format)",                            doman);
   manout("-iauto","determine automatically input format (free or fixed)",doman);
   manout("-ifixed","force input format fixed",                           doman);
   manout(" ","(default: auto)",                                          doman);
   manout("-ifree","force input format free",                             doman);
   manout(" ","(default: auto)",                                          doman);
   manout("-Lnnn","use only first nnn characters of each line",           doman);
   manout(" ","default=0: take whole lines",                              doman);
   manout("-Lnnng","same as above, but use gfortran convention",          doman);
   manout(" ","for counting the characters with tabbed lines",            doman);
   manout("-ofree","force free format output",                            doman);
   manout("-Rr","refactor blocks: a single 'end'",                      doman);
   manout(" "," is, if possible, replaced by",                              doman);
   manout(" "," 'end subroutine <name>' or",                              doman);
   manout(" "," 'end function <name>' or",                                doman);
   manout(" "," 'end program <name>' or",                                 doman);
   manout(" "," 'end blockdata <name>' or",                               doman);
   manout(" "," 'end module <name>'",                                     doman);
   manout(" "," where <name> is the name of the appropriate block",       doman);
   manout("-RR","same as -Rr, but 'SUBROUTINE'",                          doman);
   manout(" ","in stead of 'subroutine' etc",                             doman);
   if(doman)
   {
      cout << ".PP" << endl << ".SS \"Indenting options:" << endl;
   }
   else
   {
      cout << "  indents:"                              << endl;
   }
   manout("-In","starting  indent (default:0)",                                                 doman);
   manout("-Ia","determine starting indent from first line",                                    doman);
   manout("-in","all       indents except I,c,C,e (default: "+number2string(default_indent)+")",doman);
   manout("-an","ASSOCIATE  indent",                 doman);
   manout("-bn","BLOCK      indent",                 doman);
   manout("-dn","DO         indent",                 doman);
   manout("-fn","IF         indent",                 doman);
   manout("-En","ENUM       indent",                 doman);
   manout("-Fn","FORALL     indent",                 doman);
   manout("-jn","INTERFACE  indent",                 doman);
   manout("-mn","MODULE     indent",                 doman);
   manout("-rn","SUBROUTINE indent",                 doman);
   manout("-rn","FUNCTION   indent",                 doman);
   manout("-sn","SELECT     indent",                 doman);
   manout("-tn","TYPE       indent",                 doman);
   manout("-wn","WHERE      indent",                 doman);
   manout("-xn","CRITICAL   indent",                 doman);
   manout("-C-","restart indent after CONTAINS",     doman);
   manout("-kn","continuation indent except   ",     doman);
   manout(" ","  for lines starting with '&'",       doman);
   manout(" ","     free to free only",              doman);
   manout("-k-","continuation lines not preceded",   doman);
   manout(" ","  by '&' are untouched",              doman);
   manout(" ","     free to free only",              doman);
   manout("  ","next defaults are: all - all/2",     doman);
   manout("-cn","CASE      negative indent",         doman);
   manout("-Cn","CONTAINS  negative indent",         doman);
   manout("-en","ENTRY     negative indent",         doman);
   manout(" "," ",                                   doman);
   if(doman)
   {
      cout << ".PP" << endl << ".SS" << endl;
   }
   cout << "Examples:"                                         << endl;
   manout(" ","indent: findent < in.f > out.f",                doman);
   manout(" ","        findent -i2 -r0 < in.f > out.f",        doman);
   manout(" ","convert: findent -ofree < prog.f > prog.f90",   doman);
   manout(" ","refactor 'end': findent -Rr < in.f90 > out.f90",doman);
   if(doman)
   {
      cout << ".SH COPYRIGHT" << endl;
      cout << ".br"           << endl;
   }
   else
   {
      cout << "COPYRIGHT" << endl;
   }
   cout << "This is free software; see the source for copying conditions.  There is NO"  <<endl;
   cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." <<endl;
}

// doman == 1: output in man page format
// flag: " ": normal continuation line
//       otherwize : skip to new paragraph and use bold format
// txt: Line to be output
//
void manout(const string flag, const string txt, const bool doman)
{

   if (doman)
   {
      if (flag == " ")
	 cout << txt << endl;
      else
      {
	 cout << ".TP" << endl << "\\fB\\"<<flag<<"\\fR"<<endl;
	 cout << txt << endl;
      }
   }
   else
   {
      cout << flag << "\t" << ": " << txt << endl;
   }
}

int guess_fixedfree(const string s)
{
   D(O("entering guess_fixedfree"););
   // sometimes, program sources contain carriage control characters
   // such as ^L
   // I cannot get the lexer to respond to [:cntrl:]
   // so I handle that here:
   if (firstchar(s) != '\t')
      if(firstchar(s) < 32)
         return UNSURE;

   lexer_set(ltab2sp(s));
   int rc = yylex();
   return rc;
}

int determine_fix_or_free(const bool store)
{
   int rc;
   string s;
   int n = 0;
   const int nmax = 4000;
   while ( n < nmax)
   {
      n++;
      s = mygetline();
      if (end_of_file)
      {
         // to avoid to have to type twice a dot to
	 // end input from terminal:

         if(store && reading_from_tty)
	    linebuffer.push(s);       // s == "."
         break;
      }

      if (store)
	 linebuffer.push(s);

      rc = guess_fixedfree(s);
      switch(rc)
      {
         case UNSURE : 
	    D(O(s);O("UNSURE"););
	    break;
	 case PROBFREE : 
	    D(O(s);O("PROBFREE"););
	    break;
	 case FREE : 
	    D(O(s);O("FREE"););
	    return FREE;
	 case FIXED : 
	    D(O(s);O("FIXED"););
	    return FIXED;
      }
   }
   return FIXED;
}

string handle_dos(const string s)
//  determine if the input is dos format:
// side effect: sets endline if not already been set
{
   string sl;
   sl = s;
   if (!endline_set)
   {
      D(O("setting endline");O(sl));
      if (sl != "" && lastchar(sl) == '\r')
      {
	 endline = "\r\n";
         D(O(endline.length()););
      }
      endline_set = 1;
   }
   if (sl != "" && lastchar(sl) =='\r')
      sl.erase(sl.length()-1);
   return sl;
}
 
bool isfixedcmt(const string s)
{
// returns 1 if this is a fixed empty line or fixed comment line or preprocessor line
//                                         or debug line ('d' or 'D' in column 1)
   if (s == "" || trim(s) == "")
      return 1;
   char c = firstchar(s);
   return (c == 'C' || c == 'c' || c == '!' || c == '*' || c == '#' || c == 'd' || c == 'D'); 
}

char fixedmissingquote(const string s)
{
   // investigate if this line terminates a string
   // returns ' ' if there is no unterminated string
   // returns '"' if this is an unterminated string, a '"' is missing
   // returns '\'' if this is an unterminated string, a '\'' is missing
   bool instring = 0;
   char q = ' ';
   for(unsigned int i=0; i<s.size(); i++)
   {
      if (instring)
      {
	 if (s[i] == q)
	    instring = 0;
      }
      else
      {
	 switch(s[i])
	 {
	    case '"': case '\'':
	       instring = 1;
	       q = s[i];
	 }
      }
   }
   if (instring)
      return q;
   return ' ';
}

// #if, #ifdef, #ifndef, #else, #elif and #endif

// after an #if{,def,ndef} the indent stack is pushed
// on indent_stack, and the current indent stack is used.
// after an #elif the indent stack is made equal to indent_stack.top()
// after an #else, the indent stack is popped from indent_stack.
// after an #endif:
//     if the index stack was not already popped from index_stack (after
//        #else), it is popped
// the stack pre_stack is used to note if the index stack was already
//      popped by #else
//
// So, after #endif, the indentation is taken from the code after #else
//     or, if there is no #else, from the code before the #if{,def,ndef}

// something like this has been done for the rprops stack

void handle_pre(const string s)
{
   D(O("pre before");O(cur_indent);O(top_indent());O(s););

   if (s == "" || firstchar(s) != '#')
      return;

   if (s.find("#if") == 0)   // covers #if #ifdef #ifndef
   {
      indent_stack.push(indent);
      pre_stack.push(0);
      rprops_stack.push(rprops);
      prep_stack.push(0);
   }
   else if (s.find("#elif") == 0)
   {
      if (!indent_stack.empty())
	 indent = indent_stack.top();
      if (!rprops_stack.empty())
	 rprops = rprops_stack.top();
   }
   else if (s.find("#else") == 0)
   {
      if (!indent_stack.empty())
      {
	 indent = indent_stack.top();
	 indent_stack.pop();
	 pre_stack.pop();
	 pre_stack.push(1);
      }
      if (!rprops_stack.empty())
      {
	 rprops = rprops_stack.top();
	 rprops_stack.pop();
	 prep_stack.pop();
	 prep_stack.push(1);
      }
   }
   else if (s.find("#endif") == 0)
   {
      if (!indent_stack.empty())
      {
	 if (!pre_stack.top())
	 {
	    indent = indent_stack.top();
	    indent_stack.pop();
	 }
	 pre_stack.pop();
      }
      if (!rprops_stack.empty())
      {
	 if (!prep_stack.top())
	 {
	    rprops = rprops_stack.top();
	    rprops_stack.pop();
	 }
	 prep_stack.pop();
      }
   }
   D(O("pre after");O(cur_indent);O(top_indent()););
   cout << s << endline;
   char lchar = lastchar(s);
   while (!lines.empty())
   {
      if (lchar != '\\')
	 break;
      cout <<lines.front() << endline;
      lchar = lastchar(lines.front());
      lines.pop_front();
      olines.pop_front();
   }
}

char firstchar(const string s)
{
   size_t l = s.length();
   if (l == 0)
      return 0;
   return s[0];
}

char lastchar(const string s)
{
   size_t l = s.length();
   if (l == 0)
      return 0;
   return s[l-1];
}
