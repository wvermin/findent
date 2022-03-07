#!/bin/sh
# -copyright-
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

if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
exe=$FINDENT
for flag in -h --help ; do
   $exe "$flag" | head -n 1 | tr -d '\r' > help.try
   cmp -s ../help.ref help.try
   r=$?
   if [ $r -eq 0 ] ; then
      echo "$flag : works OK"
   else
      echo "$flag : works NOT OK, compare help.try and help.ref"
   fi
   rc=`expr $rc + $r` 
done

for flag in -H --manpage ; do
   $exe "$flag" | head -n 1 | tr -d '\r' > manpage.try
   cmp -s ../manpage.ref manpage.try
   r=$?
   if [ $r -eq 0 ] ; then
      echo "$flag : OK"
   else
      echo "$flag : NOT OK, compare manpage.try and manpage.ref"
   fi
   rc=`expr $rc + $r` 
done

for flag in -v --version ; do
   $exe $flag > result
   if grep -q "^findent version" result ; then
      echo "$flag: OK"
   else
      echo "$flag : NOT OK"
      echo "expected : findent version ..."
      echo "got:"
      cat result
      rc=`expr $rc + 1`
   fi
done

. ../postlude
exit $rc
