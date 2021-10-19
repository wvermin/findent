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
#ifndef LINE_PREP_H
#define LINE_PREP_H

#include <iostream>
#include <string>
#include <deque>

#include "functions.h"
#include "findent_types.h"

class Line_prep
{
   private:
      std::string line,sl,sv,sc,rest;  // todo should be private
      intpairs_t pv;
      intpairs_t pc;
      intpairs_t prest;

      char place_holder;

      struct whats
      {
	 int         type;
	 std::string value;
	 char        stringtype;
      };

      std::deque <whats> wv;

   public:

      enum { is_invalid=1, is_none, is_string, is_stlabel, is_dotop };

      bool contains_hollerith;

      Line_prep(const std::string &s, const intpairs_t *p = 0) { set_line(s, p); }

      Line_prep()                           {}

      void set_line(const std::string &s, const intpairs_t *p = 0);

      void set_place_holder(const char c)       { place_holder = c; }

      char get_place_holder(void)               { return place_holder; }
      
      std::string get_line(void)                { return line; }

      std::string get_line_spaces_removed(void) { return sl; }

      std::string get_line_compressed(void)     { return sv; }

      std::string get_line_encoded(void)        { return sc; }

      std::string get_line_rest(void)           { return rest; }

      intpairs_t  get_pos_compressed(void)      { return pv; }

      intpairs_t  get_pos_encoded(void)         { return pc; }

      intpairs_t  get_pos_rest(void)            { return prest; }

      int get_type(const unsigned int i)
      {
	 if (i < wv.size())
	    return wv[i].type;
	 else
	    return is_invalid;
      }

      int get_type(const std::string &s) { return get_type(string2number<unsigned int>(s)); }

      std::string get_type_as_string(const unsigned int i)
      {
	 int k = get_type(i);

	 switch(k)
	 {
	    case is_invalid:
	       return ("invalid");
	    case is_none:
	       return ("none");
	    case is_string:
	       return ("string");
	    case is_stlabel:
	       return ("label");
	    case is_dotop:
	       return ("dotop");
	    default:
	       return ("impossible");
	 }
      }

      std::string get_type_as_string(const std::string &s)
      {
	 return get_type_as_string(string2number<unsigned int>(s));
      }

      std::string get_value(const unsigned i)
      {
	 if (i < wv.size())
	    return wv[i].value;
	 else
	    return "";
      }

      std::string get_value(const std::string &s)
      {
	 return get_value(string2number<unsigned int>(s));
      }

      char get_stringtype(const unsigned i)
      {
	 if (i < wv.size())
	    return wv[i].stringtype;
	 else
	    return ' ';
      }

      char get_stringtype(const std::string &s)
      {
	 return get_stringtype(string2number<unsigned int>(s));
      }

};

#endif
