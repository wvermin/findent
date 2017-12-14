#!/bin/sh
rc=0
cat << eof > prog
   program prog
  continue
  !comment
eof
cat << eof > expect
2
eof

$SRCDIR/test/doit "-lastusable --last_usable" "-ifree -Ia" "" 
rc=`expr $rc + $?`

exit $rc
