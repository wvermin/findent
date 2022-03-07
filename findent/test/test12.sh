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
cat << eof > prog
      program prog

      continue
!  comment
eof
cat << eof > expect
3
eof

../doit "-lastusable --last_usable" "-ifree -Ia" "" 
rc=`expr $rc + $?`

../doit "-lastusable --last_usable" "-ifixed -Ia" "" 
rc=`expr $rc + $?`

cat << eof > prog
program prog
include "a.inc"
include 'b.inc'
?? include 'c.inc'
??include "d.inc"
#include "e.inc"
# include <f.inc>
    end
eof
cat << eof > expect
inc a.inc
inc b.inc
cpp e.inc
std f.inc
coc c.inc
coc d.inc
eof

../doit "--deps" "-ifree" ""
rc=`expr $rc + $?`

cat << eof > prog
       program prog
       include "a.inc"
       include 'b.inc'
?? include 'c.inc'
??include "d.inc"
#include "e.inc"
# include <f.inc>
 end
eof
cat << eof > expect
inc a.inc
inc b.inc
cpp e.inc
std f.inc
coc c.inc
coc d.inc
eof

../doit "--deps" "-ifixed" ""
rc=`expr $rc + $?`

cat << eof > main.f90
module mm
integer i
end
program main
use mymod
use mm
include "file.inc"
#include "one.inc"
continue
end
eof
cat << eof > sub.f90
module mymod
integer x
contains
subroutine sub
continue
end subroutine
end module
module mymod1
interface
real module function f(a)
real a
end function
end interface
end module
eof
cat << eof > sub1.f90
submodule (mymod1) mymod2
contains
module procedure f
f = 10.0
end procedure
end submodule
eof
cat << eof |sort >  expect
sub1.lo: sub.lo
main.lo: sub.lo
main.lo: file.inc one.inc
eof
touch file.inc one.inc
# remove '\r' to facilitate dos executable
$FINDENT --makefdeps | tr -d '\r' > makefdeps.sh
chmod +x makefdeps.sh
./makefdeps.sh -s .lo main.f90 sub.f90 sub1.f90 | tr -d '\r' | sort > result
if cmp -s expect result ; then
   echo "--makefdeps : OK"
else
   echo "--makefdeps : NOT OK"
   echo "expected:"
   cat expect
   echo "got:"
   cat result
   rc=`expr $rc + 1`
fi

. ../postlude
exit $rc
