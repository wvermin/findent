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

#ifndef PROP_H
#define PROP_H

#include <deque>
#include <string>
#include "findent_types.h"
#include "parser.hpp"

#include "line_prep.h"

struct propstruct
{
   int         kind;    // SUBROUTINE, DO, ...
   int         type;    // EXEC: executable stmnt; DEF: definition; ENDDEF: end definition; NONE
   strings_t   labels;
   ints_t      label_pos;
   std::string stlabel;
   std::string name;
   std::string stringvalue;
   std::string lrvalue;
   int         lrpos;
} ;

const struct propstruct empty_rprop={0,NONE,strings_t(),ints_t(),"","","","",0};

propstruct parseline(const Line_prep &p);

propstruct parselabels(const std::string &s);

propstruct parseio(const std::string &s);

propstruct parsecall(const std::string &s);

std::string whatrprop(struct propstruct &p);

typedef std::deque<struct propstruct> rprops_t;

typedef std::deque<rprops_t>          rprops_store_t;

typedef struct propstruct             prop_t;

#endif
