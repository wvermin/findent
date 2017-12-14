#!/bin/sh
rc=0
cat << eof > prog
  program prog
  continue
eof
cat << eof > expect
5
eof

$SRCDIR/test/doit "-lastindent --last_indent" "-ifree -Ia -i3"
rc=`expr $rc + $?`

exit $rc
