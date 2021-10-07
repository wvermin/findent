#!/bin/sh
# -copyright-
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

# vim: indentexpr=none
if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
cat << eof > prog
  program prog
  continue
eof
cat << eof > expect
5
eof

../doit "-lastindent --last_indent" "-ifree -Ia -i3"
rc=`expr $rc + $?`


cat << eof > prog
     endif
eof
cat << eof > expect
5
eof

../doit "-lastindent --last_indent" "-ifree -Ia -i3"
rc=`expr $rc + $?`

# test 2018 critical(stat = istat)
cat << eof > prog 
program pcritical
critical(stat=istat)
continue
end critical
end
eof

cat << eof > expect
program pcritical
   critical(stat=istat)
       continue
   end critical
end
eof

../doit "--indent-critical=4 -x4" "-ifree"
rc=`expr $rc + $?`

# test 2018 change team(), end team [()]
cat << eof > prog 
program pchangeteam
change team(newteam)
continue
end team
continue
change team(newteam)
continue
end team (stat=istat)
l: change team(newteam)
continue
end team (stat=istat) l
k: change team(newteam)
continue
end team k
end
eof

cat << eof > expect
program pchangeteam
   change team(newteam)
       continue
   end team
   continue
   change team(newteam)
       continue
   end team (stat=istat)
   l: change team(newteam)
       continue
   end team (stat=istat) l
   k: change team(newteam)
       continue
   end team k
end
eof

../doit "--indent-changeteam=4 --indent_changeteam=4" "-ifree"
rc=`expr $rc + $?`

. ../postlude

exit $rc
