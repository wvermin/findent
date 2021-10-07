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

#include <string>
#include "pre_analyzer.h"
#include "functions.h"
#include "parser.hpp"
//
// Depending on the value of pretype:
//   the ifelse_stack is manipulated as appropriate; 
//   returns PRE_IF, PRE_ELIF, PRE_ELSE, PRE_ENDIF, PRE_ENDIFE, PRE_NONE as appropiate
//
int Pre_analyzer::analyze(const int pretype)
{
   switch(pretype)
   {
      case CPP_IF:  case CPP_ENDIF:  case CPP_ELSE:  case CPP_ELIF:
      case COCO_IF: case COCO_ENDIF: case COCO_ELSE: case COCO_ELIF: 
      case INCLUDE_CPP: case INCLUDE_COCO: break;
      default: return this->PRE_NONE;
   }

   int r;
   switch(pretype)
   {
      case CPP_IF: case COCO_IF:
	 this->ifelse_stack.push_back(0);
	 return this->PRE_IF;
      case CPP_ELIF: case COCO_ELIF:
	 return this->PRE_ELIF;
      case CPP_ELSE: case COCO_ELSE:
	 if (!this->ifelse_stack.empty())
	 {
	    ifelse_stack.pop_back();
	    ifelse_stack.push_back(1);
	 }
	 return this->PRE_ELSE;
      case CPP_ENDIF: case COCO_ENDIF:
	 r = this->PRE_ENDIF; 
	 if (!ifelse_stack.empty())
	 {
	    if (ifelse_stack.back())
	       r = this->PRE_ENDIFE;
	    ifelse_stack.pop_back();
	 }
	 return r;
    default:
      return this->PRE_NONE;
   }
}
