#!/bin/sh
cd debian/tests
. ./prelude
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
