#!/bin/sh
# -copyright-
#-# Copyright: 2015,2016,2017,2018,2019,2020,2021 Willem Vermin wvermin@gmail.com
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
        module points
           type :: point
              real :: x,y
           end type point
           interface
              real module function point_dist(a,b)
                 type(point), intent(in) :: a,b
              end function point_dist
           end interface
        end module

        submodule (points) points_a
        contains
           module procedure point_dist
           write(*,*) "hoppa"
           point_dist = sqrt((a%x-b%x)**2+(a%y-b%y)**2)
        end procedure
     end submodule
   program main
continue
contains
subroutine mysub
         continue
end
end
eof

cat << eof > expect
        module points
           type :: point
              real :: x,y
           end type point
           interface
              real module function point_dist(a,b)
                 type(point), intent(in) :: a,b
              end function point_dist
           end interface
        end module points

        submodule (points) points_a
        contains
           module procedure point_dist
              write(*,*) "hoppa"
              point_dist = sqrt((a%x-b%x)**2+(a%y-b%y)**2)
           end procedure point_dist
        end submodule points_a
        program main
           continue
        contains
           subroutine mysub
              continue
           end subroutine mysub
        end program main
eof

$doit "-Rr --refactor_procedures --refactor-end" "-Ia -i3" "for free input"
rc=`expr $rc + $?`

cat << eof > expect
        module points
           type :: point
              real :: x,y
           END TYPE point
           interface
              real module function point_dist(a,b)
                 type(point), intent(in) :: a,b
              END FUNCTION point_dist
           END INTERFACE
        END MODULE points

        submodule (points) points_a
        contains
           module procedure point_dist
              write(*,*) "hoppa"
              point_dist = sqrt((a%x-b%x)**2+(a%y-b%y)**2)
           END PROCEDURE point_dist
        END SUBMODULE points_a
        program main
           continue
        contains
           subroutine mysub
              continue
           END SUBROUTINE mysub
        END PROGRAM main
eof

$doit "-RR --refactor_procedures=upcase --refactor_end=upcase" "-Ia -i3" "for free input"
rc=`expr $rc + $?`

. ../postlude
exit $rc
