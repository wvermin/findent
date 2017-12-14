#!/bin/sh
rc=0
SRCDIR=${SRCDIR:-.}
for f in vim_help gedit_help vim_fortran vim_findent gedit_external gedit_plugin gedit_plugin_py ; do
   flag="--$f"
   ../src/findent $flag | head -n 2 | tr -d '\r' > $f.try
   cmp -s $SRCDIR/test/$f.ref $f.try
   r=$?
   if [ $r -eq 0 ] ; then
      echo "$flag : works OK"
   else
      echo "$flag : works NOT OK, compare $f.try and $f.ref"
   fi
   rc=`expr $rc + $r` 
done
exit $rc
