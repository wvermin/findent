! -I2 -i5 -a1 -b2 -d3 -f4 -m5 -Rr
! ticket 1

block data pblockdata
   common /xcomm/ p
   end 

block data qblockdata
   common /qcomm/ p
   end block data qblockdata
block data rblockdata
   common /rcomm/ p
   end block data

   ! submodule and 'module subroutine' not recognized by gfortran 4.8.2
#if 0
MODULE ancestor              
  INTEGER :: i

  INTERFACE
    MODULE SUBROUTINE sub1(arg1) 
      INTEGER :: arg1
    END SUBROUTINE
    
    MODULE SUBROUTINE sub2(arg2)
      INTEGER :: arg2
    END SUBROUTINE
  END INTERFACE
END MODULE

SUBMODULE (ancestor) descendant 
  INTEGER :: j           

  CONTAINS              
    MODULE SUBROUTINE sub1(arg1) 

      INTEGER :: arg 1
      arg1 = 1
      i = 2                     
      j = 3                    
    END SUBROUTINE

    MODULE PROCEDURE sub2     
      arg2 = 1
    END 
END

#endif

module some_module

   type :: onetype
      integer i
   end type onetype
   type name
        character(22) :: n
    end type

complex ccc
interface myinterface
   subroutine tx(a,b)
      real a,b
   end 
end interface
interface anotherinterface
   module procedure funccc
!
!  trata
   module procedure funcccc
end interface
abstract interface
     character*8 recursive function func(x)
       real :: x
     end function func
  end interface
contains

     character*(8)  impure function funcc(x)
       real :: x
       funcc = 'a'
     end function funcc
     integer(kind=4) pure function funccc(x)
        real, intent(in) :: x
        funccc = 2
     end function
     type(onetype) function funcccc(x)
        type(onetype) :: x
        funcccc = x
     end function
     integer elemental function funcx(x)
        integer, intent(in) :: x
        funcx = x
     end function
#define GCC_VERSION (__GNUC__ * 10000  + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)
          /* Test for GCC >= 4.8.0 */
#if GCC_VERSION >= 40800
             subroutine handleP(p)
             class(*), intent(in) :: p

            select type(p)
            type is (character(len=*))
                write(*,*) len(p), ': ', p
            class is (name)
                write(*,*) len(p%n), ': ', p%n
            class default
                write(*,*) 'Unknown type'
            end select
        end subroutine
#endif
        double complex function compx(x)
       double complex x
       compx = x
    end function
        complex recursive function compxx(x)
       complex x
       compxx = x
    end function
real function alpha_integral_function(x)
implicit none
real:: x
alpha_integral_function = 0
end

end ! end of this module
! /ticket 1

module m1
   integer k
   enum, bind(c)

   enumerator :: red =1, blue, black =5
   enumerator yellow
   enumerator gold, silver, bronze
   enumerator :: purple
   enumerator :: pink, lavender

endenum
   type, public :: mytypea
      integer i
   end type mytypea
   integer, parameter, public :: i8=selected_int_kind(16)
contains
   function f(x)
      print *,'abc &
 def'
 print *,"abcd&
       & efgh"
 return
 end function
 integer(kind=i8) function func1(a, b)
      integer(kind=i8), intent(in) :: a, b
      func1 = a + b
   end function func1
 integer(kind=i8)function funcx(a, b)
      integer(kind=i8), intent(in) :: a, b
      funcx = a + b
   end function funcx
#ifdef usempi
       function f1(x,m)
            real x,m
#elif defined(useopenmp)
       function f1(x,m,n)
            real x,m,n
#else
       function f1(x)
            real x
#endif
            continue
       end function f1
 end module

program progfree
   type mytype  ! mytype
      integer i
      real x
   end type mytype
#ifdef abcde
   real klm(10)
#endif
   integer x(100)
   type(mytype)  xyz
   real do(1000)
   continue
   loop: do i=1,20
   continue
   enddo loop
   loop1 : do i=1,20
   continue
   enddo loop1
   end = 10
   do while(i.eq.0)
   continue
   enddo
   do concurrent(i=1:10)
   x(i) = 0
   enddo

   if ( a.eq.b .and. &
         c.eq.r) then
      n=n+1
   endif

   iff: if (a.eq.b) then
      continue
   endif iff

   lsele: select case(i)
   case(10)
      continue
   case(11)
      continue
   case default
      continue
   end select lsele

   lblock: block
      continue
      end block lblock
    
    x = 3.0*a + 4*b +   &
    &   2  *c +   y
 assign 90 to i
   do i=1,20
      block
         do 90 k=1,90
            do 90 k1=1,4
            call one(a)
            do 7 j=1,20
               call two(a)
             7  x=x+j
            continue
            90 continue
            continue
            end block
         enddo
               do i=1,10; do j=3,5; continue; enddo; enddo 
               do i=1,7; do j=2,2
               continue
               enddo
               continue
               enddo
               do ; enddo
         critical
         x=y
         do(3) = 123
      endcritical
         critical
         x=y
      endcritical
         critical
         x=y
      end critical
            if (.false.) goto 88
            if (.false.) goto 99999
  88            if ('x' .eq. "abc''d") l=1
99999            continue
      if ('x' .eq. "abc''d") l=1
      continue
      if ('x' .eq. "abc''d")then
         continue
      endif
         ASSOCIATE ( Z => EXP(-(a**2+Y**2)) * COS(THETA))
PRINT *, A+Z, A-Z
END ASSOCIATE

      forall(i=1:4) x(i)=5
      continue
      forall(i=1:4)  ! forall
         x(i)=6
      end forall
      hop:forall(i=1:4)  ! forall
         x(i)=6
      end forall hop

      where(x.eq.0) x=5 ! where
      continue
      where(x.eq.0)
         x=10
      endwhere
      continue

      where(x.eq.0)
         x=10
      elsewhere
         x=11
      endwhere

      select case(ia)
      case(1)
         print *,3
         do i=1,9
            continue
         enddo
      case(2)
         print*,6
      case default
         print *,0
      endselect

      end program

      subroutine one(a)
         continue
         entry myentry
         continue
         return
      end 
      subroutine two(a)
         continue
         a = 3 + &! comment1
               4 + & ! comment 2
               5

         a = 3 + & ! test preprocessor within continuation
               4 + &
#ifdef abc
               this would cause a compilation error &
#endif
               5
         return
      endsubroutine
      subroutine three(a)
         do 10 i=1,4
         do 5 j=1,2
         continue
      5 enddo
      10 enddo
      typecode = 9
      continue
         return
      end
      subroutine four(a)
         continue
         return
      endsubroutine four
      subrou&
            &tine five
      continue
      return 
   end
   subroutine seven
           a = 1&
              &+ 2   &! comment 1
              &+ 3   &! comment 2
        &+ 4&
        &+ 5&
              &+ 6
        a = 1&
        &+ 2   &! comment 1
        &+ 3   &! comment 2
#ifdef klm
           &+ 4 syntax error&
        &+ 5 syntax error&
#endif
              &+ 6
                                                   end
