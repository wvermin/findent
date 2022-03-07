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

# tests according to "Modern Fortran explaned" ISBN 978-0-19-881188-6
if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
cat << eof > prog
! if statement and if construct. pp 55-57
  program p_if
logical a,b

  if(a) continue

  if(a) then
  continue
  endif

  if (a) then
  continue
  else
  continue
  endif

  if (a) then
  continue
  elseif (b) then
  continue
  else
  continue
  endif

  l1: if(a) then
  continue
  endif l1

  l2: if (a) then
  continue
  else l2
  continue
  endif l2

  l3: if (a) then
  continue
  else if (b) then l3
  continue
  else l3
  continue
  endif l3
  continue
  end
eof
cat << eof > expect
! if statement and if construct. pp 55-57
  program p_if
     logical a,b

     if(a) continue

     if(a) then
          continue
     endif

     if (a) then
          continue
     else
          continue
     endif

     if (a) then
          continue
     elseif (b) then
          continue
     else
          continue
     endif

     l1: if(a) then
          continue
     endif l1

     l2: if (a) then
          continue
     else l2
          continue
     endif l2

     l3: if (a) then
          continue
     else if (b) then l3
          continue
     else l3
          continue
     endif l3
     continue
  end
eof

../doit "-f5 --indent-if=5 --indent_if=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! case construct pp 57-59
    program p_case
    integer i
    select case (i)
    case (3)
    continue
    case (4)
    continue
    case default
    continue
    end select

    l1: select case (i)
    case (3) l1
    continue
    case (4) l1
    continue
    case default l1
    continue
    end select l1
    end
eof
cat << eof > expect
! case construct pp 57-59
    program p_case
       integer i
       select case (i)
          case (3)
            continue
          case (4)
            continue
          case default
            continue
       end select

       l1: select case (i)
          case (3) l1
            continue
          case (4) l1
            continue
          case default l1
            continue
       end select l1
    end
eof

../doit "-s5 --indent-select=5 --indent_select=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! do construct pp 59-63
    program p_do
    do i=1,10
    continue
    enddo

    l1: do i=1,10,2
    continue
    enddo l1

    do , i=1,10
    continue
    end do

    l2: do, i=1,10,2
    continue
    end do l2

continue
    end
eof
cat << eof > expect
! do construct pp 59-63
    program p_do
       do i=1,10
            continue
       enddo

       l1: do i=1,10,2
            continue
       enddo l1

       do , i=1,10
            continue
       end do

       l2: do, i=1,10,2
            continue
       end do l2

       continue
    end
eof

../doit "-d5 --indent-do=5 --indent_do=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Main program pp 70-71
    program p_main
    continue
    end program main
    program p_main1
    continue
    end program
    program p_main2
    continue
    contains
    subroutine mysub
    continue
    end
    end
eof
cat << eof > expect
! Main program pp 70-71
    program p_main
         continue
    end program main
    program p_main1
         continue
    end program
    program p_main2
         continue
      contains
         subroutine mysub
              continue
         end
    end
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! External subprograms pp 72-73
   subroutine p_sub1
   continue
   contains
   subroutine mysub
   continue
   end
   end subroutine p_sub1

   real function myfunc()
   continue
   contains
   subroutine asub(x)
   continue
   end subroutine asub
   end function myfunc
eof
cat << eof > expect
! External subprograms pp 72-73
   subroutine p_sub1
        continue
     contains
        subroutine mysub
             continue
        end
   end subroutine p_sub1

   real function myfunc()
        continue
     contains
        subroutine asub(x)
             continue
        end subroutine asub
   end function myfunc
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Modules pp 73-75
   module p_module
   integer i
   contains
   subroutine mysub
   continue
   end subroutine mysub
   end

   module p_module1
   integer j
   contains
   real function myfunc(x)
   continue
   myfunc=x
   end function
   end module p_module1

   module p_module2
   integer k
   contains
   real function afunc(x)
   continue
   myfunc=x*k
   end function
   end module
eof
cat << eof > expect
! Modules pp 73-75
   module p_module
        integer i
     contains
        subroutine mysub
           continue
        end subroutine mysub
   end

   module p_module1
        integer j
     contains
        real function myfunc(x)
           continue
           myfunc=x
        end function
   end module p_module1

   module p_module2
        integer k
     contains
        real function afunc(x)
           continue
           myfunc=x*k
        end function
   end module
eof

../doit "-m5 --indent-module=5 --indent_module=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Arguments of procedures pp 76-83
      subroutine s_arguments(x,y)
  integer, intent(inout) :: x,y
  continue
  end

    integer function f_arguments(x,y,z)
    integer x,y,z
    f_arguments = x+y+z
    end
eof
cat << eof > expect
! Arguments of procedures pp 76-83
      subroutine s_arguments(x,y)
           integer, intent(inout) :: x,y
           continue
      end

      integer function f_arguments(x,y,z)
           integer x,y,z
           f_arguments = x+y+z
      end
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Explicit and implicit interfaces pp 83-84
      program p_interface
      interface inter
      subroutine s(x)
      real x
      end
      end interface inter

      interface
      subroutine y(x)
      real x
      end
      end interface
      end program
eof
cat << eof > expect
! Explicit and implicit interfaces pp 83-84
      program p_interface
         interface inter
              subroutine s(x)
                 real x
              end
         end interface inter

         interface
              subroutine y(x)
                 real x
              end
         end interface
      end program
eof

../doit "-j5 --indent-interface=5 --indent_interface=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Direct recursion pp 92-93
     recursive function f_recur(n) result(i)
     integer i,n
     i = n
     end function
eof
cat << eof > expect
! Direct recursion pp 92-93
     recursive function f_recur(n) result(i)
          integer i,n
          i = n
     end function
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Overloading and generic interfaces pp 94-99
  program p_overload
  interface doit
  real function doit1(x)
  real x
  end function doit1
  real function doit2(x,y)
  real x,y
  end function doit2
  end interface doit
  interface operator(*)
  logical function myand(a,b)
  logical, intent(in) :: a,b
  end function myand
  end interface operator(*)
  end
eof
cat << eof > expect
! Overloading and generic interfaces pp 94-99
  program p_overload
     interface doit
          real function doit1(x)
             real x
          end function doit1
          real function doit2(x,y)
             real x,y
          end function doit2
     end interface doit
     interface operator(*)
          logical function myand(a,b)
             logical, intent(in) :: a,b
          end function myand
     end interface operator(*)
  end
eof

../doit "-j5 --indent-interface=5 --indent_interface=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The subroutine and function statements pp 99-101
      program p_subroutine
end
      integer(4), pure elemental function myfunc(x)
      integer, intent(in) ::  x
      myfunc = x
      end function

      pure function pfunc(x) result(y)
      real*8, intent(in) :: x
      real*8 y
      y=x
      end

      elemental subroutine mysub(i)
      integer, intent(inout) :: i
      i = 2*i
      continue
      end subroutine mysub

      pure subroutine psub(x,y)
      real, intent(inout) :: x
      real, intent(in) :: y
      x = x*y
      continue
      end
eof
cat << eof > expect
! The subroutine and function statements pp 99-101
      program p_subroutine
      end
      integer(4), pure elemental function myfunc(x)
      integer, intent(in) ::  x
      myfunc = x
   end function

   pure function pfunc(x) result(y)
        real*8, intent(in) :: x
        real*8 y
        y=x
   end

   elemental subroutine mysub(i)
        integer, intent(inout) :: i
        i = 2*i
        continue
   end subroutine mysub

   pure subroutine psub(x,y)
        real, intent(inout) :: x
        real, intent(in) :: y
        x = x*y
        continue
   end
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The where statement and construct pp 123-126
    program p_where
    real a(10)
    where(a /= 0) a=1/a

    where (a /= 0)
    a=1/a
    end where

       where (a /= 0)
            a=1/a
         elsewhere
            a=9
       end where

    where (a == 0)
    a = 1
    elsewhere (a > -1)
    a=0.5
    elsewhere (a > 4)
    a=3
    end where

    l1: where (a /= 0)
    a=1/a
    end where l1

    l2: where (a == 0)
    a = 1
    elsewhere (a /= 24 )l2
    a=0.5
    elsewhere (a > 4) l2
    a=3
    end where l2 

       l3:where (a /= 0)
            a=1/a
            elsewhere l3
            a=9
         end where l3

    end
eof
cat << eof > expect
! The where statement and construct pp 123-126
    program p_where
       real a(10)
       where(a /= 0) a=1/a

       where (a /= 0)
            a=1/a
       end where

       where (a /= 0)
            a=1/a
       elsewhere
            a=9
       end where

       where (a == 0)
            a = 1
       elsewhere (a > -1)
            a=0.5
       elsewhere (a > 4)
            a=3
       end where

       l1: where (a /= 0)
            a=1/a
       end where l1

       l2: where (a == 0)
            a = 1
       elsewhere (a /= 24 )l2
            a=0.5
       elsewhere (a > 4) l2
            a=3
       end where l2

       l3:where (a /= 0)
            a=1/a
       elsewhere l3
            a=9
       end where l3

    end
eof

../doit "-w5 --indent-where=5 --indent_where=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Impure elemental procedures pp 128-129
   program p_impure
   contains
   impure elemental integer function myfunc(x)
   integer, intent(in) :: x
   myfunc = x+1
   end function
   end program
eof
cat << eof > expect
! Impure elemental procedures pp 128-129
   program p_impure
     contains
        impure elemental integer function myfunc(x)
             integer, intent(in) :: x
             myfunc = x+1
        end function
   end program
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The do concurrent construct
       program p_doconcurrent
       real:: a(10)
       do concurrent(i=1:10)
       a(i) = 8
       enddo

       l1: do concurrent(i=1:10)
       a(i) = 8
       enddo l1

       do, concurrent(i=1:10)
       a(i) = 8
       enddo

       l2: do, concurrent(i=1:10)
       a(i) = 8
       enddo l2
       end
eof
cat << eof > expect
! The do concurrent construct
       program p_doconcurrent
          real:: a(10)
          do concurrent(i=1:10)
               a(i) = 8
          enddo

          l1: do concurrent(i=1:10)
               a(i) = 8
          enddo l1

          do, concurrent(i=1:10)
               a(i) = 8
          enddo

          l2: do, concurrent(i=1:10)
               a(i) = 8
          enddo l2
       end
eof

../doit "-d5 --indent-do=5 --indent_do=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The block construct  pp 168-170
        program p_block

	block
	continue
	end block

	l1:block
	continue
	end block l1

	end
eof
cat << eof > expect
! The block construct  pp 168-170
        program p_block

           block
                continue
           end block

           l1:block
                continue
           end block l1

        end
eof

../doit "-b5 --indent-block=5 --indent_block=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Derived-type definitions  pp 173-177

     module p_type

     type,public :: mytype
     integer a(10)
     end type mytype

     type :: atype
     integer a(10)
     end type 

     type :: btype
     integer a(10)
     end type btype

        type anothertype
     integer x(10)
        end type

     end
eof
cat << eof > expect
! Derived-type definitions  pp 173-177

     module p_type

        type,public :: mytype
             integer a(10)
        end type mytype

        type :: atype
             integer a(10)
        end type

        type :: btype
             integer a(10)
        end type btype

        type anothertype
             integer x(10)
        end type

     end
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Advanced type parameter features
     program p_advancedtype

     type mytype(k,l)
     integer, kind :: k
     integer, len  :: l
     real(k) :: a(l)
     end type mytype

     type(mytype(selected_real_kind(6),10)) :: x
     x%a(2) = 9

     end
eof
cat << eof > expect
! Advanced type parameter features
     program p_advancedtype

        type mytype(k,l)
             integer, kind :: k
             integer, len  :: l
             real(k) :: a(l)
        end type mytype

        type(mytype(selected_real_kind(6),10)) :: x
        x%a(2) = 9

     end
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Abstract interfaces  pp 285-287

      program p_abstractinterfaces

      abstract interface
      integer function f(x,y)
      integer x,y
      end function
      end interface

end
eof
cat << eof > expect
! Abstract interfaces  pp 285-287

      program p_abstractinterfaces

         abstract interface
              integer function f(x,y)
                 integer x,y
              end function
         end interface

      end
eof

../doit "-j5 --indent-interface=5 --indent_interface=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Type extension  pp 291-293

    program p_typeextension

    type x
    integer a
    end type x

    type, extends(x) :: y
    integer b
    end type y

    end
eof
cat << eof > expect
! Type extension  pp 291-293

    program p_typeextension

       type x
            integer a
       end type x

       type, extends(x) :: y
            integer b
       end type y

    end
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The associate construct  pp 300-301
    program p_associate
    implicit none

    type atype
    real x
    end type atype

    type(atype) :: a

    associate(x => a%x)
    x=10
    end associate

    l1: associate(x => a%x)
    x=10
    end associate l1

    end
eof
cat << eof > expect
! The associate construct  pp 300-301
    program p_associate
       implicit none

       type atype
          real x
       end type atype

       type(atype) :: a

       associate(x => a%x)
            x=10
       end associate

       l1: associate(x => a%x)
            x=10
       end associate l1

    end
eof

../doit "-a5 --indent-associate=5 --indent_associate=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The select type construct

    module mymod
   implicit none
  type t
  real x
  end type

  type u
  integer i
  end type

  type, extends(u) :: w
  integer j
  end type

  end module mymod

  program p_selecttype
   use mymod
   implicit none

  type(t), target :: a
  type(u), target :: b
  type(w), target :: c
  class(*), pointer :: h

a%x = 11
b%i = -10
c%i = 120
  h => a
  call doit(h)
  h => b
  call doit(h)
  h => c
  call doit(h)

contains 
subroutine doit(h)
class(*), pointer :: h
  select type(h)
  type is (t)
  print *,h%x
  type is(u)
  print *,h%i
  class is (w)
  print *,h%i
  class default
  print *,'ai!'
  end select

  l1: select type(h)
  type is (t) l1
  print *,h%x
  type is(u) l1
  print *,h%i
  class is (w) l1
  print *,h%i
  class default l1
  print *,'ai!'
  end select l1
  end

  end
eof
cat << eof > expect
! The select type construct

    module mymod
       implicit none
       type t
          real x
       end type

       type u
          integer i
       end type

       type, extends(u) :: w
          integer j
       end type

    end module mymod

    program p_selecttype
       use mymod
       implicit none

       type(t), target :: a
       type(u), target :: b
       type(w), target :: c
       class(*), pointer :: h

       a%x = 11
       b%i = -10
       c%i = 120
       h => a
       call doit(h)
       h => b
       call doit(h)
       h => c
       call doit(h)

    contains
       subroutine doit(h)
          class(*), pointer :: h
          select type(h)
             type is (t)
               print *,h%x
             type is(u)
               print *,h%i
             class is (w)
               print *,h%i
             class default
               print *,'ai!'
          end select

          l1: select type(h)
             type is (t) l1
               print *,h%x
             type is(u) l1
               print *,h%i
             class is (w) l1
               print *,h%i
             class default l1
               print *,'ai!'
          end select l1
       end

    end
eof

../doit "-s5 --indent-select=5 --indent_select=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! select rank p 442
#define CANRANK
#ifdef __GNUC__
#if __GNUC__ < 10
#undef CANRANK
#endif
#endif
program selrank
implicit none
real, dimension(3,4) :: y
print *,xfunc(y)
contains
integer function xfunc(a)
   implicit none
   real, intent(in) :: a(..)
   integer k
   k = -2
#ifdef CANRANK 
select rank(a)
rank(*)
   k = -1
  rank(0)
   k = 0
  rank(1)
   k = 1
  rank(2)
   k = 2
  rank default
   k = 100
end select
#endif
xfunc = k
end function
end program selrank
eof
cat << eof > expect
! select rank p 442
#define CANRANK
#ifdef __GNUC__
#if __GNUC__ < 10
#undef CANRANK
#endif
#endif
program selrank
   implicit none
   real, dimension(3,4) :: y
   print *,xfunc(y)
contains
   integer function xfunc(a)
      implicit none
      real, intent(in) :: a(..)
      integer k
      k = -2
#ifdef CANRANK
      select rank(a)
         rank(*)
           k = -1
         rank(0)
           k = 0
         rank(1)
           k = 1
         rank(2)
           k = 2
         rank default
           k = 100
      end select
#endif
      xfunc = k
   end function
end program selrank
eof

../doit "-s5 --indent-select=5 --indent_select=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Type and contains  p 316
      module mymod
      type mytype
      integer i
      contains
	 procedure sub
      end type mytype
   contains
      subroutine sub(this)
	 class(mytype) :: this
	 continue
      end
   end module mymod
   program p_typecontains
      use mymod
      type(mytype) t
   end
eof
cat << eof > expect
! Type and contains  p 316
      module mymod
         type mytype
              integer i
          contains
              procedure sub
         end type mytype
     contains
         subroutine sub(this)
            class(mytype) :: this
            continue
         end
      end module mymod
      program p_typecontains
         use mymod
         type(mytype) t
      end
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-C4 -ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Deferred bindings and abstract types  pp 312-313
    module mymod
     integer i
     type, abstract :: mytype
     contains
     procedure (sub), deferred, pass :: x
     end type mytype
     abstract interface
     subroutine sub(p)
     import mytype
     class(mytype) :: p
     end
     end interface
     type, abstract :: mt
     contains
     procedure(op), deferred :: plus
     generic :: operator(+) => plus
     end type mt
     abstract interface
     function op(a,b) result(r)
     import mt
     class(mt), intent(in) :: a,b
     class (mt), allocatable :: r
     end function
     end interface
     end module
eof
cat << eof > expect
! Deferred bindings and abstract types  pp 312-313
    module mymod
       integer i
       type, abstract :: mytype
         contains
            procedure (sub), deferred, pass :: x
       end type mytype
       abstract interface
          subroutine sub(p)
             import mytype
             class(mytype) :: p
          end
       end interface
       type, abstract :: mt
         contains
            procedure(op), deferred :: plus
            generic :: operator(+) => plus
       end type mt
       abstract interface
          function op(a,b) result(r)
             import mt
             class(mt), intent(in) :: a,b
             class (mt), allocatable :: r
          end function
       end interface
    end module
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Finalization pp 313-314
	 module mymod
	 type mytype
	 integer i
	 contains
	 final :: doit
	 end type mytype
	 contains
	 subroutine doit(x)
	 type(mytype) :: x
	 end subroutine doit
	 end module
eof
cat << eof > expect
! Finalization pp 313-314
         module mymod
            type mytype
                 integer i
              contains
                 final :: doit
            end type mytype
         contains
            subroutine doit(x)
               type(mytype) :: x
            end subroutine doit
         end module
eof

../doit "-t5 --indent-type=5 --indent_type=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Submodules pp 322-324
            module base
  integer i
  interface
  integer module function square(i)
  integer i
  end function square
  end interface
  end module base
  submodule (base) deriv
  contains
  module procedure square
  square = i*i
  end procedure square
  end submodule deriv
eof
cat << eof > expect
! Submodules pp 322-324
            module base
                 integer i
                 interface
                    integer module function square(i)
                       integer i
                    end function square
                 end interface
            end module base
            submodule (base) deriv
              contains
                 module procedure square
                    square = i*i
                 end procedure square
            end submodule deriv
eof

../doit "-m5 --indent-module=5 --indent_module=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Critical sections pp 340-341
    program p_critical
    critical
    continue
    end critical
    l1: critical
    continue
    end critical l1
    end
eof
cat << eof > expect
! Critical sections pp 340-341
    program p_critical
       critical
            continue
       end critical
       l1: critical
            continue
       end critical l1
    end
eof

../doit "-x5 --indent-critical=5 --indent_critical=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Interoperability with C pp 367-381
       module mymod
       type, bind(c) :: mytype
       integer i
       end type
       enum, bind(c)
       enumerator a=1, b=2
       enumerator c
       end enum
       contains
       function f() bind(c)
       continue
       end function f
       su broutine sub bind(c)
       continue
       end
       subroutine sub1() bind(c)
       continue
       end
       end module
eof
cat << eof > expect
! Interoperability with C pp 367-381
       module mymod
            type, bind(c) :: mytype
               integer i
            end type
            enum, bind(c)
               enumerator a=1, b=2
               enumerator c
            end enum
         contains
            function f() bind(c)
               continue
            end function f
            su broutine sub bind(c)
               continue
            end
            subroutine sub1() bind(c)
               continue
            end
       end module
eof

../doit "-m5 --indent-module=5 --indent_module=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Change team construct pp 385-388
! not compiled
   program p_team
   use iso_fortran_env
   type(team_type) myteam
   change team(myteam)
   continue
   end team
   l1: change team(myteam)
   continue
   end team (stat=i)l1
   critical
   continue
   end critical
   critical (stat=i)
   continue
   end critical
   l2:critical (stat=i)
   continue
   end critical l2
   end
eof
cat << eof > expect
! Change team construct pp 385-388
! not compiled
   program p_team
        use iso_fortran_env
        type(team_type) myteam
        change team(myteam)
           continue
        end team
        l1: change team(myteam)
           continue
        end team (stat=i)l1
        critical
           continue
        end critical
        critical (stat=i)
           continue
        end critical
        l2:critical (stat=i)
           continue
        end critical l2
   end
eof

../doit "-r5--indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The include line p 447
   program p_include
   do i=1,10
   include 'file.inc'
   include "file2.inc"
   continue
   enddo
   end
eof
cat << eof > expect
! The include line p 447
   program p_include
      do i=1,10
         include 'file.inc'
         include "file2.inc"
         continue
      enddo
   end
eof

../doit "-ifree" "-ifree -Ia"
rc=`expr $rc + $?`

../doit "--include_left=0 --include-left=0" "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > expect
! The include line p 447
   program p_include
      do i=1,10
   include 'file.inc'
   include "file2.inc"
         continue
      enddo
   end
eof

../doit "--include_left=1 --include-left=1" "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The include line p 447
      program p_include
      do i=1,10
      include 'file.inc'
      include "file2.inc"
      continue
      enddo
      end
eof
cat << eof > expect
! The include line p 447
      program p_include
         do i=1,10
            include 'file.inc'
            include "file2.inc"
            continue
         enddo
      end
eof

../doit "-ifixed" "-ifixed -Ia"
rc=`expr $rc + $?`

../doit "--include_left=0 --include-left=0" "-ifixed -Ia"
rc=`expr $rc + $?`

cat << eof > expect
! The include line p 447
      program p_include
         do i=1,10
      include 'file.inc'
      include "file2.inc"
            continue
         enddo
      end
eof

../doit "--include_left=1 --include-left=1" "-ifixed -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The do while statement pp 447-448
    program p_dowhile
    logical l
    do while (l)
    continue
    end do
    l1: do while(l)
    continue
    enddo l1
    do 10 while(l)
    continue
    10 continue
    do 20, while(l)
    continue
    20 continue
    end
eof
cat << eof > expect
! The do while statement pp 447-448
    program p_dowhile
         logical l
         do while (l)
            continue
         end do
         l1: do while(l)
            continue
         enddo l1
         do 10 while(l)
            continue
10       continue
         do 20, while(l)
            continue
20       continue
    end
eof

../doit "-r5--indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Character length specification with * p 458
  character*10 function f()
  continue
f='abc'
end
eof
cat << eof > expect
! Character length specification with * p 458
  character*10 function f()
       continue
       f='abc'
  end
eof

../doit "-r5--indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Entry statement pp 462-463
   subroutine sub
  integer i
  continue
  entry sub1
  continue
  end
eof
cat << eof > expect
! Entry statement pp 462-463
   subroutine sub
      integer i
      continue
 entry sub1
      continue
   end
eof

../doit "-e5 --indent-entry=5 --indent_entry=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Forall statement and construct pp 463-466
 program p_forall
 integer a(20)
 forall(i=1:20) a(i) = 3*i ! statement
 forall(i=1:20)  ! construct
 a(i) = 4*i
 end forall
 end
eof
cat << eof > expect
! Forall statement and construct pp 463-466
 program p_forall
    integer a(20)
    forall(i=1:20) a(i) = 3*i ! statement
    forall(i=1:20)  ! construct
         a(i) = 4*i
    end forall
 end
eof

../doit "-F5 --indent-forall=5 --indent_forall=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The block data program unit
 block data
common // i
data i /1/
end block data
eof
cat << eof > expect
! The block data program unit
 block data
      common // i
      data i /1/
 end block data
eof

../doit "-r5 --indent-procedure=5 --indent_procedure=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! The labelled do construct
    program p_dolabel
    do 10 i=1,5
    continue
    10 enddo
    do 20 i=1,5
    continue
    do 15 j=1,5
    continue
    15 continue
    20 continue
    do 50 i=1,5
    do 50 j=1,5
    continue
    50 continue
    end
eof
cat << eof > expect
! The labelled do construct
    program p_dolabel
       do 10 i=1,5
            continue
10     enddo
       do 20 i=1,5
            continue
            do 15 j=1,5
                 continue
15          continue
20     continue
       do 50 i=1,5
            do 50 j=1,5
                 continue
50     continue
    end
eof

../doit "-d5 --indent-do=5 --indent_do=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Arithmetic if statement
  program p_aritif
  i = 2
  if(i) 10,20,30
  10 continue
  20 continue
  30 continue
  end
eof
cat << eof > expect
! Arithmetic if statement
  program p_aritif
     i = 2
     if(i) 10,20,30
10   continue
20   continue
30   continue
  end
eof

../doit "-d5 --indent-do=5 --indent_do=5 " "-ifree -Ia"
rc=`expr $rc + $?`

cat << eof > prog
! Shared do-loop termination
   program p_shdolote
   do 20, i=1,8
   do 20 ,j=1,6
   continue
   20 print *,i*j
   do 30, i=1,8
   do 30, j=1,6
   continue
   30 continue
   end
eof
cat << eof > expect
! Shared do-loop termination
   program p_shdolote
      do 20, i=1,8
           do 20 ,j=1,6
                continue
20    print *,i*j
      do 30, i=1,8
           do 30, j=1,6
                continue
30    continue
   end
eof

../doit "-d5 --indent-do=5 --indent_do=5 " "-ifree -Ia"
rc=`expr $rc + $?`
. ../postlude

exit $rc
# vim: indentexpr=none
