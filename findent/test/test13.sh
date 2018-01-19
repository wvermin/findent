#!/bin/sh
. ./prelude
rc=0
doit=../doit
cat << eof > prog
      program fixed
      continue
      end
eof

cat << eof > expect
fixed
eof

$doit "-q --query_fix_free" "" "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
      program free
 continue
      end
eof

cat << eof > expect
free
eof

$doit "-q --query_fix_free" "" "for free input"
rc=`expr $rc + $?`

exit $rc
