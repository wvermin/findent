#ifndef SIMPLEOSTREAM_H
#define SIMPLEOSTREAM_H
#include <iostream>
#include <sstream>
class simpleostream
{
   private:

      std::stringstream line;
      bool output;
   public:
      simpleostream(void){ output = 1; }
      void reset(void){ line.str(""); }
      template< typename T >
	 simpleostream& operator<<( T val )
	 { 
	    if (output)
	       std::cout << val; 
	    else
	       line << val;
	    return *this; 
	 }
      void setoutput(const bool b){ output = b; }
      bool getoutput(void){ return output; }
      template< typename T>
	 void set( T s ){ reset(); line << s; }
      std::string get(void){ return line.str(); }
}
;
#endif
