#!/bin/sh
cd debian/tests
. ./prelude
rc=0
prog="  program main
10      continue
      end
   "
expect="program main
      10      continue
end"
exe=$FINDENT
for flag in -l0 --label_left=0 ; do
   a=`echo "$prog" | $exe "$flag" -I0 -i6 | tr -d '\r'`
   if [ "$a" = "$expect" ]; then
      echo "$flag works OK"
   else
      echo "$flag works NOT OK:"
      echo "expected:"
      echo "$expect"
      echo "got     :"
      echo "$a"
      echo "program is:"
      echo "$prog"
      rc=`expr $rc + 1`
   fi
done

expect="program main
10    continue
end"
for flag in -l1 --label_left=1 ; do
   a=`echo "$prog" | $exe "$flag" -I0 -i6 | tr -d '\r'`
   if [ "$a" = "$expect" ]; then
      echo "$flag works OK"
   else
      echo "$flag works NOT OK:"
      echo "expected:"
      echo "$expect"
      echo "got     :"
      echo "$a"
      echo "program is:"
      echo "$prog"
      rc=`expr $rc + 1`
   fi
done

. ../postlude
exit $rc
