#!/bin/sh
export FINDENT_FLAGS="$TEST_FINDENT_FLAGS"
prog="$1"
echo "$0: $prog"
gfortran=gfortran
$gfortran -v > /dev/null 2>&1  || { echo "Cannot compile: gfortran not installed" ; gfortran=: ; }
rc=0
exe=${FINDENT:-../src/findent}
$exe -v
bprog="`basename $prog`"
rm -f "bprog.try.f"
echo -n "$bprog: "
case "$bprog" in
   *fixed*) 
      format="-ffixed-form -ffixed-line-length-none -fd-lines-as-comments"
      ;;
   *) 
      format="-ffree-form -ffree-line-length-none" 
      ;;
esac

$gfortran -fcoarray=single -cpp $format -o prog $prog >/dev/null 2>&1
if [ $? -ne 0 ] ; then
   echo -n " original program does not compile "
   rc=1
fi

parms=`head -n1 < $prog | tr '!' ' '|tr '\r' ' '`
$exe $parms < $prog > $bprog.try.f 2>/dev/null
if [ -f $prog.try.f.ref ]; then
   case "`file $exe`" in
      *"MS Windows"*)
	 case $prog in
	    *dos*)
	       :
	       ;;
	    *)
	       sed -i 's/\r//' $bprog.try.f
	       ;;
	 esac
   esac
   cmp -s $bprog.try.f $prog.try.f.ref >/dev/null 2>&1
   if [ "$?" -eq 0 ]; then
      echo -n "OK"
   else
      echo -n "WRONG"
      rc=1
   fi
   echo
else
   echo "cannot compare"
   rc=1
fi
case "$bprog" in
   *free*) 
      format="-ffree-form -ffree-line-length-none" 
      ;;
   *) 
      format="-ffixed-form -ffixed-line-length-none -fd-lines-as-comments"
      ;;
esac
$gfortran -fcoarray=single -cpp $format -o prog $bprog.try.f >/dev/null 2>&1
if [ $? -ne 0 ] ; then
   echo "        converted program does not compile "
   rc=1
fi

echo "END TESTING FINDENT rc=$rc"
if [ $rc -ne 0 ] ; then
   echo "if you are sure $prog.try.f is correct:"
   echo "cp $prog.try.f  $prog.try.f.ref"
   echo "cp $prog.try.f.ref $prog.try.f.ref.in"
fi
rm -f prog
exit $rc
