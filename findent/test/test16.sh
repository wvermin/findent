#!/bin/sh
# -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021,2022 Willem Vermin wvermin@gmail.com
#-# 
#-# License: BSD-3-Clause
#-#  Redistribution and use in source and binary forms, with or without
#-#  modification, are permitted provided that the following conditions
#-#  are met:
#-#  1. Redistributions of source code must retain the above copyright
#-#     notice, this list of conditions and the following disclaimer.
#-#  2. Redistributions in binary form must reproduce the above copyright
#-#     notice, this list of conditions and the following disclaimer in the
#-#     documentation and/or other materials provided with the distribution.
#-#  3. Neither the name of the copyright holder nor the names of its
#-#     contributors may be used to endorse or promote products derived
#-#     from this software without specific prior written permission.
#-#   
#-#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#-#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#-#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#-#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE HOLDERS OR
#-#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#-#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#-#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#-#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#-#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#-#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#-#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
doit=../doit
cat << eof > prog
      program main01
      continue
      end
eof
cat << eof > expect
           program main01
              continue
           end
eof

$doit "-I5 --start_indent=5" -i3 "for fixed input"
rc=`expr $rc + $?`

cat << eof > prog
      program main02
 continue
      end
eof
cat << eof > expect
     program main02
        continue
     end
eof
$doit "-I5 --start_indent=5" -i3 "for free input"
rc=`expr $rc + $?`

cat << eof > prog
        program main03
      continue
      end
eof
cat << eof > expect
        program main03
           continue
        end
eof
$doit "-Ia --start_indent=a" "-I0 -i3" ""
rc=`expr $rc + $?`

cat << eof > prog
 program main04
continue
do i=1,10
do j=1,20
   continue
enddo
enddo
end
eof

cat << eof > expect
program main04
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
      program main06
associate( z => sin(theta))
print *,z
end associate
      end
eof

cat << eof > expect
program main06
   associate( z => sin(theta))
        print *,z
   end associate
end
eof

$doit "-a5 --indent_associate=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main07
block
continue
end block
end
eof
cat << eof > expect
program main07
   block
        continue
   end block
end
eof

$doit "-b5 --indent_block=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
        program main08
do i=1,10
do j=1,5
   print *,i,j
enddo
enddo
         end
eof

cat << eof > expect
program main08
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
  program main09
if (i .eq. 7) then
   print *,'foo'
   continue
endif

         continue
         end
eof

cat << eof > expect
program main09
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
  program main10
enum, bind(c)
enumerator :: red = 1, blue black = 5
enumerator yellow
end enum
         end
eof

cat << eof > expect
program main10
   enum, bind(c)
        enumerator :: red = 1, blue black = 5
        enumerator yellow
   end enum
end
eof

$doit "-E5 --indent_enum=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
   program main11
forall(k=1:1000)
x(k) = 23
y(k) = 67
end forall
end
eof

cat << eof > expect
program main11
   forall(k=1:1000)
        x(k) = 23
        y(k) = 67
   end forall
end
eof

$doit "-F5 --indent_forall=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main12
interface myinterface
subroutine mysub(x)
real x
end subroutine
end interface
end
eof

cat << eof > expect
program main12
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
   module mymodule01
integer x
contains
subroutine mysub
continue
end subroutine
end module
eof

cat << eof > expect
module mymodule01
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
   subroutine mysub01
continue
end subroutine
function myfun(x)
real x
end function
eof

cat << eof > expect
subroutine mysub01
     continue
end subroutine
function myfun(x)
     real x
end function
eof

$doit "-r5 --indent_procedure=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main13
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
program main13
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
program main14
type mytype
integer :: i
real x,y
end type mytype
end program
eof
cat << eof > expect
program main14
   type mytype
        integer :: i
        real x,y
   end type mytype
end program
eof
$doit "-t5 --indent_type=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main15
where(x>21)
y=10
z=11
end where
end program
eof
cat << eof > expect
program main15
   where(x>21)
        y=10
        z=11
   end where
end program
eof
$doit "-w5 --indent_where=5" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
program main16
continue
critical
x=10
y=11
end critical
end program
eof
cat << eof > expect
program main16
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
program main17
continue
contains
subroutine mysub
continue
end subroutine mysub
end program
eof
cat << eof > expect
program main17
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
program main18
continue
x = x + &
& 10
y = y + &
5
end program
eof
cat << eof > expect
program main18
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
program main19
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
program main19
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
program main20
continue
contains
function foo(bar)
real bar
continue
end function
end program
eof
cat << eof > expect
program main20
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
subroutine mysub02
continue
entry myentry
continue
end subroutine
eof
cat << eof > expect
subroutine mysub02
      continue
  entry myentry
      continue
end subroutine
eof
$doit "-e4 --indent_entry=4" "-i6 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
subroutine mysub03
do i=1,20
continue
!$ continue
enddo
end subroutine
eof
cat << eof > expect
subroutine mysub03
   do i=1,20
      continue
!$ continue
   enddo
end subroutine
eof
$doit "--openmp=0" "-i3 -I0" ""
rc=`expr $rc + $?`

cat << eof > prog
         subroutine mysub04
         do i=1,20
               continue
!$       continue
            enddo
            end subroutine
eof
cat << eof > expect
      subroutine mysub04
         do i=1,20
            continue
!$       continue
         enddo
      end subroutine
eof
$doit "--openmp=0" "-i3 -I0" ""
rc=`expr $rc + $?`

. ../postlude
exit $rc
