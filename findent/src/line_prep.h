// $Id: line_prep.h 136 2016-11-06 11:12:38Z willem_vermin $
#ifndef LINE_PREP_H
#define LINE_PREP_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
  std::string number2string ( T number )
  {
    std::ostringstream ss;
    ss << number;
    return ss.str();
  }

template <typename T>
  T string2number ( const std::string &text )
  {
    std::istringstream ss(text);
    T result;
    return ss >> result ? result : 0;
  }

class line_prep
{
   private:
      std::string line,sl,sv,sc,rest;
   char place_holder;

   enum what { is_invalid, is_none, is_string, is_stlabel, is_dotop };

   struct whats
   {
      what   type;
      std::string value;
      char   stringtype;
   };

   std::vector <whats> wv;

   public:

   line_prep(const std::string s);

   line_prep(){};

   static const int type_invalid      = is_invalid;
   static const int type_none         = is_none;
   static const int type_string       = is_string;
   static const int type_stlabel      = is_stlabel;
   static const int type_dot_operator = is_dotop;

   void set_place_holder(const char c)
   {
      place_holder = c;
   }

   char get_place_holder()
   {
      return place_holder;;
   }

   std::string get_line()
   {
      return line;
   }

   std::string get_line_spaces_removed()
   {
      return sl;
   }

   std::string get_line_compressed()
   {
      return sv;
   }

   std::string get_line_encoded()
   {
      return sc;
   }

   std::string get_line_rest()
   {
      return rest;
   }

   int get_type(const unsigned int i)
   {
      if (i < wv.size())
	 return wv[i].type;
      else
	 return is_invalid;
   }

   int get_type(const std::string &s)
   {
      return get_type(string2number<unsigned int>(s));
   }

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
