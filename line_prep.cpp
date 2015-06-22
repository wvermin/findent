// $Id: line_prep.cpp 65 2015-06-22 10:01:15Z willem_vermin $
#include "line_prep.h"
using namespace std;
#include "debug.h"

#define DECIMAL_DIGITS                          \
   '0': case '1': case '2': case '3': case '4': \
case '5': case '6': case '7': case '8': case '9'

#define HANDLE_END_STLABEL       \
{                                \
   sv           += place_holder; \
   vstruct.type  = is_stlabel;   \
   vstruct.value = v;            \
   wv.push_back(vstruct);        \
}

line_prep::line_prep(const string s)
{
   enum states { maybe_stlabel, in_stlabel, in_code, in_qstring, 
      pre_hollerith, in_hollerith, in_dotop};

   struct whats vstruct,nonestruct;

   nonestruct.type       = is_none;
   nonestruct.value      = "";
   nonestruct.stringtype = ' ';

   set_place_holder(' ');

   // parsing stops at end of string s or at ';' or at '\n'
   // sl will contain s, space removed, except in strings and holleriths and label
   // sv will contain s, space removed, strings and hollertihs replaced
   //    by place_holder, which can be any character except probably \n or ;
   // wv will contain for each element of sv an element of type 'whats'
   // rest will contain the not-parsed part of the line
   //    
   // examples: (place_holder = ' ')
   //   s:  123  format(5habcde  ,i5  ,'foo'  'bar')
   //  sl:  123 format(5habcde,i5,'foo''bar')
   //  sv:   format( ,i5, )
   //       012345678901234
   //  wv[0]:  type = type_stlabel, value = "123"
   //  wv[8]:  type = type_string, value = "abcde", stringtype = 'h'
   //  wv[13]: type = type_string, value = "foo'bar", stringtype = '\''
   //  The other elements of wv: type = type_none
   //
   //  sl is available via method string get_line_spaces_removed()
   //  sv is available via method string get_line_compressed()
   //  rest is available via method string get_line_rest()
   //
   //  Elements of wv[i] are available via
   //     int    get_type(int i)
   //     string get_value(int i)
   //     char   get_stringtype(int i)
   //     string get_type_as_string(int i)
   //
   //  The types are defined as member constants:
   //
   //    type_invalid
   //    type_none
   //    type_string
   //    type_stlabel
   //    type_dotop
   //     

   line = s;
   sl   = "";
   sv   = "";

   sl.reserve(line.size());
   sv.reserve(line.size());
   wv.reserve(line.size());

   states state = maybe_stlabel;

   string v;

   char c, prevc, prevstringtype = ' ', quotechar = ' ';
   int prevtype;
   int nhol       = 0;
   int index      = -1;
   bool getnextc  = 1;

   while(1)
   {
      if (getnextc)
	 index++;
      else
	 getnextc = 1;
      if (index >= (int)line.size())
	 break;

      c = line[index];
      if (c == '\n') 
	 break;
      // also end, when encountering a ';' which is not part of a string:
      if (c == ';' && state != in_qstring && state != in_hollerith)
	 break;

      vstruct = nonestruct;

      switch(state)
      {
	 case maybe_stlabel:
	    D(O("maybe_stlabel:");O(c););
	    if(isblank(c))
	       break;
	    switch(c)
	    {
	       case DECIMAL_DIGITS:
		  sl   += c;
		  v     = c;
		  state = in_stlabel;
		  break;
	       default:
		  getnextc = 0;
		  state = in_code;
		  break;
	    }
	    break;
	 case in_stlabel:
	    D(O("in_stlabel:");O(c););
	    if( c >= '0' && c <= '9')
	    {
	       sl += c;
	       v  += c;
	       break;
	    }
	    if (isblank(c))
	    {
	       // a sneak preview, to see if the first non-blank
	       // is a digit. In this case the label is not ended here
	       bool digitfound = 0;
	       for (unsigned int j=index+1; j<line.size(); j++)
	       {
		  if(!isblank(line[j]))
		  {
		     digitfound = isdigit(line[j]);
		     break;
		  }
	       }
	       if(!digitfound)
	       {
		  HANDLE_END_STLABEL;
		  sl += ' ';
		  state = in_code;
	       }
	       break;
	    }

	    // here we found a label, directly follwed by a non-blank
	    // which tells us, that this is not a label
	    // correction:
	    sv += v;
	    for (unsigned int i=0; i<v.size(); i++)
	       wv.push_back(nonestruct);
	    getnextc = 0;
	    state    = in_code;
	    break;
	 case in_code:
	    D(O("in_code:");O(c););
	    if(isblank(c))
	       break;
	    switch(c)
	    {
	       case DECIMAL_DIGITS:
		  if (sl.size() == 0)
		     prevc = 0;
		  else
		     prevc = sl[sl.size()-1];
		  if (wv.size() == 0)
		     prevtype = -1;
		  else
		  {
		     prevtype = wv[wv.size() - 1].type;
		     prevstringtype = wv[wv.size() - 1].stringtype;
		  }

		  D(O(sl);O(c);O(prevc););
		  sl   += c;
		  // this could be an hollerith, if the previous character
		  // is not part of an identifier or the immediately preceding
		  // item was an hollerith
		  if ((prevc != '_' && !isalnum(prevc)) || 
			(prevtype == is_string && prevstringtype == 'h'))
		  {
		     nhol  = c-'0';  // counting the number of characters for this hollerith
		     v     = c;      // we keep a copy of the number before the 'h' or 'H'
		     state = pre_hollerith;
		     break;
		  }
		  sv += c;
		  wv.push_back(nonestruct);
		  break;
	       case '"': case '\'':
		  sl       += c;
		  quotechar = c;

		  state = in_qstring;

		  /* if there is a string immediately before this one, and the    */
		  /* quotation character was the same, (like: 'foo''bar') we      */
		  /*	 take appropriate action:")                                */
		  {
		     unsigned int nwv = wv.size();
		     if (nwv != 0 && wv[nwv-1].type == is_string && wv[nwv-1].stringtype == c)
		     {
			v = wv[nwv-1].value + c;
			wv.pop_back();
			sv.erase(nwv-1);
		     }
		     else
			v = "";
		  }
		  break;
	       case '.':
		  sl   += c;
		  v     = "";
		  state = in_dotop;
		  D(O("start state in_dotop"););
		  break;
	       default:
		  sl   += c;
		  sv   += c;
		  wv.push_back(nonestruct);
		  break;
	    }
	    break;
	 case pre_hollerith:
	    D(O("pre_hollerith:");O(c););
	    {
	       switch(c)
	       {
		  case ' ': case '\t':
		     break;
		  case DECIMAL_DIGITS:
		     sl   += c;
		     v    += c;
		     nhol  = 10*nhol + c - '0';
		     D(O(nhol););
		     break;
		  case 'h': case 'H':
		     sl   += c;
		     if (nhol > 0)
		     {
			v     = "";
			state = in_hollerith;
		     }
		     else
		     {
			// after all, this was an hollerith with length 0, adapt sv and wv
			sv += v + c;
			D(O(v);O(sv););
			for (unsigned int i=0; i<v.size()+1;i++)
			   wv.push_back(nonestruct);
			state = in_code;
		     }
		     break;
		  default:
		     // and here it was no hollerith at all, adapt sv and wv
		     nhol = 0;
		     for (unsigned int i=0; i<v.size()+1; i++)
			wv.push_back(nonestruct);
		     sv   += v + c;
		     sl   += c;
		     state = in_code;
		     break;
	       }
	    }
	    break;
	 case in_hollerith:
	    D(O("in_hollerith:");O(c););
	    sl += c;
	    v  += c;
	    if(nhol >0)
	       nhol --;
	    if (nhol == 0)
	    {
	       sv                += place_holder;
	       vstruct.type       = is_string;
	       vstruct.value      = v;
	       vstruct.stringtype = 'h';
	       state              = in_code;
	       wv.push_back(vstruct);
	    }
	    break;
	 case in_qstring:
	    D(O("in_q:");O(c););
	    if(c == quotechar)
	    {
	       sl                += c;
	       sv                += place_holder;
	       vstruct.type       = is_string;
	       vstruct.value      = v;
	       vstruct.stringtype = c;
	       state              = in_code;
	       wv.push_back(vstruct);
	       break;
	    }
	    sl   += c;
	    v    += c;
	    break;
	 case in_dotop:
	    D(O("in_dotop:");O(c););
	    if(isblank(c))
	    {
	       break;
	    }
	    // a dot operator can be [a-zA-z][a-zA-Z0-9_]*
	    // check if first char is [a-zA-Z]
	    if (v.size() == 0)
	    {
	       if(isalpha(c))
	       {
		  v  += c;
		  sl += c;
		  D(O("in_dotop:");O(c);O(v););
		  break;
	       }
	       else
	       {
		  // this was no dotop
		  getnextc = 0;         // re-examine this c
		  sv += '.';
		  wv.push_back(nonestruct);
		  D(O("leaving in_dotop:");O(c);O(v););
		  state    = in_code;
		  break;
	       }
	    }
	    if (c == '.')   // end of this dotop
	    {
	       sv           += place_holder;
	       sl           += c;
	       vstruct.type  = is_dotop;
	       vstruct.value = v;
	       state         = in_code;
	       wv.push_back(vstruct);
	       break;
	    }
	    if (isalnum(c) || c == '_')
	    {
	       v  += c;
	       sl += c;
	       break;
	    }
	    // this is no dotop, but we already collected some
	    // characters for it. Correct this:
	    sv      += '.' + v;
	    for (unsigned int i = 0; i < v.size()+1; i++)
	       wv.push_back(nonestruct);
	    getnextc = 0;        // re-examine this c
	    state    = in_code;
	    break;
      }
   }

   // depending on the state we are in at the end, we take appropriate action:

   switch(state)
   {
      case maybe_stlabel:
	 break;
      case in_stlabel:
	 HANDLE_END_STLABEL;
	 break;
      case in_code:
	 break;
      case in_qstring:
	 sv += quotechar + v;
	 for (unsigned int i = 0; i < v.size()+1; i++)
	    wv.push_back(nonestruct);
	 break;
      case pre_hollerith:
	 sv += v;
	 for (unsigned int i=0; i < v.size(); i++)
	    wv.push_back(nonestruct);
	 break;
      case in_hollerith:
	 if (nhol > 0)
	 {
	    sl                += string(nhol,' ');
	    v                 += string(nhol,' ');
	    sv                += place_holder;
	    vstruct.type       = is_string;
	    vstruct.value      = v;
	    vstruct.stringtype = 'h';
	    wv.push_back(vstruct);
	 }
	 break;
      case in_dotop:
	 sv += '.' + v;
	 for (unsigned int i = 0; i < v.size()+1; i++)
	    wv.push_back(nonestruct);
	 break;
   }

   sc = "";
   for (unsigned int i=0; i<sv.size(); i++)
   {
      switch (wv[i].type)
      {
	 case is_none: case is_invalid:
	    sc += sv[i];
	    break;
	 default:
	    sc += ' ' + number2string(i) + ' ';
	    break;
      }
   }

   if (index < (int)line.size())
      rest = line.substr(index+1);
   else
      rest = "";
}
