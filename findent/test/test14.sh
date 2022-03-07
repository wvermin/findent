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

# vim: indentexpr=none
if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
doit=../doit
cat << eof > prog
      program fixed001
      continue
      # if xyz || \
abc
      x=1
      # endif
      end
eof

cat << eof > expect
fixed
eof

$doit "-q --query_fix_free" "" "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
      program free001
 continue
      end
eof

cat << eof > expect
free
eof

$doit "-q --query_fix_free" "" "for free input"
rc=`expr $rc + $?`

cat << eof > prog
      program p001
      do i=1,10
      continue
      enddo
      end
eof

cat << eof > expect
program p001
  do i=1,10
  continue
  enddo
end
eof

$doit "-M2 --max_indent=2" "-ifree -i8" "for free input"
rc=`expr $rc + $?`

cat << eof > expect
      program p001
        do i=1,10
        continue
        enddo
      end
eof

$doit "-M2 --max_indent=2" "-ifixed -i8" "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
program p002
x=x+ &
10+ &
& 20+ &
30 
end
eof

cat << eof > expect
      program p002
         x=x+
     1   10+
     2    20+
     3   30
      end
eof

$doit "--continuation=0" "-ofixed" "--continuation=0 for free input"
rc=`expr $rc + $?`

cat << eof > expect
      program p002
         x=x+
     &   10+
     &    20+
     &   30
      end
eof

$doit "--continuation= " "-ofixed" "--continuation=' ' for free input"
rc=`expr $rc + $?`

cat << eof > expect
      program p002
         x=x+
     +   10+
     +    20+
     +   30
      end
eof

$doit "--continuation=+" "-ofixed" "--continuation=+ for free input"
rc=`expr $rc + $?`

cat << eof > prog
      program p003
       x=9+
     +   20 +
     ! abc
     ! abc
     ! abc
     +  30
      end
eof

cat << eof > expect
      program p003
         x=9+
     +     20 +
         ! abc
         ! abc
         ! abc
     +    30
      end
eof

$doit "--continuation= " "" "--continuation=' ' for fixed input"
rc=`expr $rc + $?`

cat << eof > expect
      program p003
         x=9+
     9     20 +
         ! abc
         ! abc
         ! abc
     9    30
      end
eof

$doit "--continuation=9" "" "--continuation=9 for fixed input"

cat << eof > prog
#define W \
      program
       program p004
	      continue
	      end
eof

cat << eof > expect
#define W \
      program
       program p004
          continue
       end
eof

$doit "-Ia --start-indent=a" "-ifree" "for free input"

$doit "-Ia --start-indent=a" "-ifixed" "for fixed input"

rc=`expr $rc + $?`
. ../postlude
exit $rc
