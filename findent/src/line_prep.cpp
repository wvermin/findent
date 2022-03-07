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
#include <string>
#include <deque>
#include <utility>
#include <assert.h>
#include "line_prep.h"
#include "debug.h"
#include "findent_types.h"

#define DECIMAL_DIGITS                          \
   '0': case '1': case '2': case '3': case '4': \
   case '5': case '6': case '7': case '8': case '9'

#define HANDLE_END_STLABEL                   \
{                                            \
   sv           += place_holder;             \
   vstruct.type  = is_stlabel;               \
   vstruct.value = v;                        \
   wv.push_back(vstruct);                    \
   if(p) pv.push_back((*p)[start_index]);    \
}


void Line_prep::set_line(const std::string &s, const intpairs_t *p)
{

   enum states { maybe_stlabel=1, in_stlabel, in_code, in_qstring, 
      pre_hollerith, in_hollerith, in_dotop};

   struct whats vstruct,nonestruct;

   contains_hollerith    = false;

   nonestruct.type       = is_none;
   nonestruct.value      = "";
   nonestruct.stringtype = ' ';

   set_place_holder(' ');

   //
   // parsing stops at end of string s or at ';' or at '\n'
   // sl will contain s, space removed, except in strings and holleriths and label
   // sv will contain s, space removed, strings and hollertihs replaced
   //    by place_holder, which can be any character except probably \n or ;
   // wv will contain for each element of sv an element of type 'whats'
   // sc will contain sv, placeholders replaced by SPACEnnnSPACE where nnn is the index in
   //    wv that describes the placeholder
   // rest will contain the not-parsed part of the line
   //
   // optional parameter p contains at input the positions of s in the original lines.
   // if p != 0, then:
   //  pv will contain the positions, with respect to sv
   //  pc will contain the positions, with respect to sc
   //  prest will contain the positions with respect to rest
   // p etc is used while relabelling
   //    
   // examples: (place_holder = ' ')
   //   s:  [123  format(5habcde  ,i5  ,'foo'  'bar')]
   //  sl:  [123 format(5habcde,i5,'foo''bar')]
   //  sv:  [ format( ,i5, )]
   //  sc:  [ 0 format( 8 ,i5, 13 )]
   //       [01234567890123456789012345678901234567890
   //  wv[0]:  type = type_stlabel, value = "123"
   //  wv[8]:  type = type_string, value = "abcde", stringtype = 'h'
   //  wv[13]: type = type_string, value = "foo'bar", stringtype = '\''
   //  The other elements of wv: type = type_none
   //
   //  sl is available via method string get_line_spaces_removed()
   //  sv is available via method string get_line_compressed()
   //  sc is available via method string get_line_encoded()
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
   //    is_invalid
   //    is_none
   //    is_string
   //    is_stlabel
   //    is_dotop
   //     

   line = s;
   sl   = "";
   sv   = "";
   if(p)
      pv.clear();

   states state = maybe_stlabel;

   std::string v;

   char c, prevc, prevstringtype = ' ', quotechar = ' ';
   int prevtype;
   int nhol       = 0;
   int index      = -1;
   bool getnextc  = 1;
   int start_index = -1;

   D(O("in Line_prep:");O(s););
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
      //
      // also end, when encountering a ';' which is not part of a string:
      //
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
		  start_index = index;
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
	       //
	       // a sneak preview, to see if the first non-blank
	       // is a digit. In this case the label is not ended here
	       //
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
	    //
	    // here we found a label, directly followed by a non-blank
	    // which tells us, that this is not a label
	    // correction:
	    //
	    sv += v;
	    for (unsigned int i=0; i<v.size(); i++)
	       wv.push_back(nonestruct);

	    if(p)
	       for(size_t i = start_index; (int)i < index; i++)
		  if (s.at(i) != ' ' && s.at(i) != '\t')
		     pv.push_back((*p)[i]);

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
		  //
		  // this could be an hollerith, if the previous character
		  // is not part of an identifier or the immediately preceding
		  // item was an hollerith
		  //
		  if ((prevc != '_' && !isalnum(prevc)) || 
			(prevtype == is_string && prevstringtype == 'h'))
		  {
		     nhol  = c-'0';  // counting the number of characters for this hollerith
		     v     = c;      // we keep a copy of the number before the 'h' or 'H'
		     state = pre_hollerith;
		     start_index = index;
		     break;
		  }
		  sv += c;
		  wv.push_back(nonestruct);

		  if(p)
		     pv.push_back((*p)[index]);

		  break;
	       case '"': case '\'':    // start of string
		  sl       += c;
		  // but this is not the start of a string if the previous non-blank
		  // character is [a-zA-Z_0-9]. This happens in for example
		  //   FIND(u'n)   ! direct access, DEC, INTEL
		  // or
		  //   FIND(10'5)
		  if (sl.size() == 0)
		     prevc = 0;
		  else
		     prevc = sl[sl.size()-1];

		  if (prevc == '_' || isalnum(prevc))
		     break;

		  quotechar = c;

		  state = in_qstring;
		  start_index = index;

		  if(p)
		     pv.push_back((*p)[index]);

		  //
		  // if there is a string immediately before this one, and the
		  // quotation character was the same, (like: 'foo''bar') we
		  //	 take appropriate action:
		  //
		  {
		     unsigned int nwv = wv.size();
		     if (nwv != 0 && wv[nwv-1].type == is_string && wv[nwv-1].stringtype == c)
		     {
			v = wv[nwv-1].value + c;
			wv.pop_back();
			sv.erase(nwv-1);

			if(p)
			   pv.pop_back();
		     }
		     else
			v = "";
		  }
		  break;
	       case '.':
		  sl   += c;
		  v     = "";
		  state = in_dotop;
		  start_index = index;
		  D(O("start state in_dotop"););
		  break;
	       default:
		  sl   += c;
		  sv   += c;
		  wv.push_back(nonestruct);

		  if(p)
		  {
		     pv.push_back((*p)[index]);
		  }

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
		     contains_hollerith = true;

		     if (nhol > 0)
		     {
			v     = "";
			state = in_hollerith;
		     }
		     else
		     {
			//
			// after all, this was an hollerith with length 0, adapt sv and wv
			//
			sv += v + c;
			D(O(v);O(sv););
			for (unsigned int i=0; i<v.size()+1;i++)
			   wv.push_back(nonestruct);

			if(p)
			   for(size_t i = 0; i<v.size()+1; i++)
			      pv.push_back((*p)[index-v.size()-1+i]);

			state = in_code;
		     }
		     break;
		  default:
		     //
		     // and here it was no hollerith at all, adapt sv and wv
		     //
		     nhol = 0;
		     for (unsigned int i=0; i<v.size()+1; i++)
			wv.push_back(nonestruct);
		     sv   += v + c;

		     // adapt pv. v contains only non-blank characters
		     if(p)
		     {
			for(size_t i = start_index; (int)i <= index; i++)
			   if (s.at(i) != ' ' && s.at(i) != '\t')
			      pv.push_back((*p)[i]);
		     }

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

	       if(p)
		  pv.push_back((*p)[start_index]);

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
	    //
	    // a dot operator can be [a-zA-z][a-zA-Z0-9_]*
	    // check if first char is [a-zA-Z]
	    //
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
		  //
		  // this was no dotop
		  //
		  getnextc = 0;         // re-examine this c
		  sv += '.';

		  if(p)
		     pv.push_back((*p)[start_index]);

		  wv.push_back(nonestruct);
		  D(O("leaving in_dotop:");O(c);O(v););
		  state    = in_code;
		  break;
	       }
	    }
	    if (c == '.')   // end of this dotop
	    {
	       sv           += place_holder;

	       if(p)
		  pv.push_back((*p)[start_index]);

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
	    //
	    // this is no dotop, but we already collected some
	    // characters for it. Correct this:
	    //
	    sv      += '.' + v;

	    if(p)
	       for(size_t i = start_index; (int)i < index; i++)
		  if (s.at(i) != ' ' && s.at(i) != '\t')
		     pv.push_back((*p)[i]);

	    for (unsigned int i = 0; i < v.size()+1; i++)
	       wv.push_back(nonestruct);
	    getnextc = 0;        // re-examine this c
	    state    = in_code;
	    break;
      }
   }

   //
   // depending on the state we are in at the end, we take appropriate action:
   //

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

	 if(p)
	    for(size_t i = start_index+1; (int)i < index; i++)
	       pv.push_back((*p)[i]);

	 for (unsigned int i = 0; i < v.size()+1; i++)
	    wv.push_back(nonestruct);
	 break;
      case pre_hollerith:
	 sv += v;

	 if(p)
	 {
	    for(size_t i = start_index; (int)i < index; i++)
	       if (s.at(i) != ' ' && s.at(i) != '\t')
		  pv.push_back((*p)[i]);
	 }

	 for (unsigned int i=0; i < v.size(); i++)
	    wv.push_back(nonestruct);
	 break;
      case in_hollerith:
	 if (nhol > 0)
	 {
	    sl                += std::string(nhol,' ');
	    v                 += std::string(nhol,' ');
	    sv                += place_holder;

	    if(p)
	       pv.push_back((*p)[index]);

	    vstruct.type       = is_string;
	    vstruct.value      = v;
	    vstruct.stringtype = 'h';
	    wv.push_back(vstruct);
	 }
	 break;
      case in_dotop:
	 sv += '.' + v;

	 if(p)
	    for(size_t i = start_index; (int)i < index; i++)
	       if (s.at(i) != ' ' && s.at(i) != '\t')
		  pv.push_back((*p)[i]);

	 for (unsigned int i = 0; i < v.size()+1; i++)
	    wv.push_back(nonestruct);
	 break;
   }

   sc = "";
   if(p)
      pc.clear();
   for (unsigned int i=0; i<sv.size(); i++)
   {
      switch (wv[i].type)
      {
	 case is_none: case is_invalid:
	    sc += sv[i];
	    if(p)
	       pc.push_back(pv[i]);
	    break;
	 default:
	    std::string n = ' ' + number2string(i) + ' ';
	    sc +=  n;
	    if(p)
	       for (size_t j=0; j<n.length(); j++)
		  pc.push_back(pv[i]);
	    break;
      }
   }

   if (index < (int)line.size())
   {
      rest = line.substr(index+1);
      if(p)
      {
	 prest = *p;
	 prest.erase(prest.begin(),prest.begin()+index+1);
      }
   }
   else
   {
      rest = "";
      if(p)
	 prest.clear();
   }

}

