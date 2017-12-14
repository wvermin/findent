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

$doit "-I5 --start_indent=5" -i3 "for fixed input"
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
$doit "-I5 --start_indent=5" -i3 "for free input"
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
$doit "-Ia --start_indent=a" "-I0 -i3" ""
rc=`expr $rc + $?`

cat << eof > prog
 program main
continue
do i=1,10
do j=1,20
   continue
enddo
enddo
end
eof

cat << eof > expect
program main
     continue
     do i=1,10
          do j=1,20
               continue
          enddo
     enddo
end
eof
$doit "-i5 --indent=5" "-I0" ""
rc=`expr $rc + $?`


cat << eof > prog
      program main
associate( z => sin(theta))
print *,z
end associate
      end
eof

cat << eof > expect
program main
   associate( z => sin(theta))
        print *,z
   end associate
end
eof

$doit "-a5 --indent_associate=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
block
continue
end block
end
eof
cat << eof > expect
program main
   block
        continue
   end block
end
eof

$doit "-b5 --indent_block=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
        program main
do i=1,10
do j=1,5
   print *,i,j
enddo
enddo
         end
eof

cat << eof > expect
program main
   do i=1,10
        do j=1,5
             print *,i,j
        enddo
   enddo
end
eof

$doit "-d5 --indent_do=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
  program main
if (i .eq. 7) then
   print *,'foo'
   continue
endif

         continue
         end
eof

cat << eof > expect
program main
   if (i .eq. 7) then
        print *,'foo'
        continue
   endif

   continue
end
eof

$doit "-f5 --indent_if=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
  program main
enum, bind(c)
enumerator :: red = 1, blue black = 5
enumerator yellow
end enum
         end
eof

cat << eof > expect
program main
   enum, bind(c)
        enumerator :: red = 1, blue black = 5
        enumerator yellow
   end enum
end
eof

$doit "-E5 --indent_enum=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
   program main
forall(k=1:1000)
x(k) = 23
y(k) = 67
end forall
end
eof

cat << eof > expect
program main
   forall(k=1:1000)
        x(k) = 23
        y(k) = 67
   end forall
end
eof

$doit "-F5 --indent_forall=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
interface myinterface
subroutine mysub(x)
real x
end subroutine
end interface
end
eof

cat << eof > expect
program main
   interface myinterface
        subroutine mysub(x)
           real x
        end subroutine
   end interface
end
eof

$doit "-j5 --indent_interface=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
   module mymodule
integer x
contains
subroutine mysub
continue
end subroutine
end module
eof

cat << eof > expect
module mymodule
     integer x
  contains
     subroutine mysub
        continue
     end subroutine
end module
eof

$doit "-m5 --indent_module=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
   subroutine mysub
continue
end subroutine
function myfun(x)
real x
end function
eof

cat << eof > expect
subroutine mysub
     continue
end subroutine
function myfun(x)
     real x
end function
eof

$doit "-r5 --indent_procedure=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
      continue
select case(i)
   case(1)
      x=2
      case(3)
         x=4
      end select
      continue
   end program
eof

cat << eof > expect
program main
   continue
   select case(i)
      case(1)
        x=2
      case(3)
        x=4
   end select
   continue
end program
eof
$doit "-s5 --indent_select=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
type mytype
integer :: i
real x,y
end type mytype
end program
eof
cat << eof > expect
program main
   type mytype
        integer :: i
        real x,y
   end type mytype
end program
eof
$doit "-t5 --indent_type=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
where(x>21)
y=10
z=11
end where
end program
eof
cat << eof > expect
program main
   where(x>21)
        y=10
        z=11
   end where
end program
eof
$doit "-w5 --indent_where=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
continue
critical
x=10
y=11
end critical
end program
eof
cat << eof > expect
program main
   continue
   critical
        x=10
        y=11
   end critical
end program
eof
$doit "-x5 --indent_critical=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
continue
contains
subroutine mysub
continue
end subroutine mysub
end program
eof
cat << eof > expect
program main
   continue
contains
subroutine mysub
   continue
end subroutine mysub
end program
eof
$doit "-C- --indent_contains=restart" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
continue
x = x + &
& 10
y = y + &
5
end program
eof
cat << eof > expect
program main
   continue
   x = x + &
   & 10
   y = y + &
5
end program
eof
$doit "-k- --indent_continuation=none" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
continue
select case(i)
   case(3)
      x=8
      case(5)
         y=2
      end select
      continue
end program
eof
cat << eof > expect
program main
      continue
      select case(i)
        case(3)
            x=8
        case(5)
            y=2
      end select
      continue
end program
eof
$doit "-c4 --indent_case=4" "-i6 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main
continue
contains
function foo(bar)
real bar
continue
end function
end program
eof
cat << eof > expect
program main
      continue
  contains
      function foo(bar)
            real bar
            continue
      end function
end program
eof
$doit "-C4 --indent_contains=4" "-i6 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
subroutine mysub
continue
entry myentry
continue
end subroutine
eof
cat << eof > expect
subroutine mysub
      continue
  entry myentry
      continue
end subroutine
eof
$doit "-e4 --indent_entry=4" "-i6 -I0" ""
rc=`expr $rc + $?`

exit $rc
