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

#ifndef NFINDENT_TYPES_H
#define NFINDENT_TYPES_H

#include <deque>
#include <iostream>
#include <string>

typedef std::deque<int>               ints_t;
typedef std::deque<ints_t>            ints_store_t;
typedef std::deque<int>               indents_t;
typedef std::deque<indents_t>         indent_store_t;
typedef std::deque<bool>              bools_t;
typedef std::deque<std::string>       strings_t;
typedef std::pair<int,int>            intpair_t;
typedef std::deque<intpair_t>         intpairs_t;
typedef std::deque<intpairs_t>        intpairs_store_t;

template <class C> std::ostream& operator <<(std::ostream &os, std::deque<C> &obj)
{
   //
   // to print a deque
   //
   if (obj.empty())
   {
      os << ":empty"<<std::endl;
      return os;
   }

   os << std::endl;

   typename std::deque<C>::iterator it = obj.begin();
   int i = 0;
   while (it != obj.end())
      os << ++i << ":" << *it++ << std::endl;
   return os;
}


#endif
