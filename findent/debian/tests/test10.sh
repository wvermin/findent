#!/bin/sh
cd debian/tests
. ./prelude
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

. ../postlude
exit $rc
