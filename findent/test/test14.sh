#!/bin/sh
rc=0
doit=$SRCDIR/test/doit
cat << eof > prog
      program main
      continue
      end
eof
cat << eof > expect
      program main
         continue
      end
eof

$doit "-iauto --input_format=auto -ifixed --input_format=fixed" "-I0 -i3" "for fixed input"
rc=`expr $rc + $?`

cat << eof > expect
program main
   continue
end
eof

$doit "-ifree --input_format=free" "-I0 -i3" "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
 program main
continue
end
eof

cat << eof > expect
program main
   continue
end
eof

$doit "-iauto --input_format=auto -ifree --input_format=free" "-I0 -i3" "for free input"
rc=`expr $rc + $?`

cat << eof > prog
      program main
         continue
      end
eof
cp prog expect

$doit "-i- --indent=none" "" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
x =                                   10   + 6
end
eof
cat << eof > expect
program main
   x =                                   10
end
eof

$doit "-L42 --input_line_length=42" "-I0 -i3" ""
rc=`expr $rc + $?`

cat << eof > prog
        program main
         continue
         end
eof

cat << eof > expect
program main
   continue
end
eof

$doit "-ofree --output_format=free" "-I0 -i3" ""
rc=`expr $rc + $?`

cat << eof > prog
  program main
         continue
         end
eof

cat << eof > expect
program main
   continue
end
eof

$doit "-osame --output_format=same" "-I0 -i3" "for free input"
rc=`expr $rc + $?`

cat << eof > prog
        program main
         continue
         end
eof

cat << eof > expect
      program main
         continue
      end
eof

$doit "-osame --output_format=same" "-I0 -i3" "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
   program main
continue
contains
subroutine mysub
         continue
end
end
eof

cat << eof > expect
program main
   continue
contains
   subroutine mysub
      continue
   end subroutine mysub
end program main
eof

$doit "-Rr --refactor_procedures" "-I0 -i3" "for free input"
rc=`expr $rc + $?`

cat << eof > expect
program main
   continue
contains
   subroutine mysub
      continue
   end SUBROUTINE mysub
end PROGRAM main
eof

$doit "-RR --refactor_procedures=upcase" "-I0 -i3" "for free input"
rc=`expr $rc + $?`

cat << eof > prog
       program main
         continue
         contains
         subroutine mysub
         continue
         end
         end
eof

cat << eof > expect
      program main
         continue
      contains
         subroutine mysub
            continue
         end subroutine mysub
      end program main
eof

$doit "-Rr --refactor_procedures" "-I0 -i3" "for fixed input"
rc=`expr $rc + $?`

cat << eof > expect
      program main
         continue
      contains
         subroutine mysub
            continue
         end SUBROUTINE mysub
      end PROGRAM main
eof

$doit "-RR --refactor_procedures=upcase" "-I0 -i3" "for fixed input"
rc=`expr $rc + $?`
exit $rc
