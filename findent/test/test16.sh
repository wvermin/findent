#!/bin/sh
. ./prelude
exe=$FINDENT
rc=0
for f in vim_help gedit_help vim_fortran vim_findent \
   gedit_external gedit_plugin gedit_plugin_py \
   emacs_help emacs_findent readme ; do
   flag="--$f"
   $exe $flag | head -n 2 | tr -d '\r' > $f.try
   cmp -s ../$f.ref $f.try
   r=$?
   if [ $r -eq 0 ] ; then
      echo "$flag : works OK"
   else
      echo "$flag : works NOT OK, compare $f.try and $f.ref"
   fi
   rc=`expr $rc + $r` 
done
exit $rc
