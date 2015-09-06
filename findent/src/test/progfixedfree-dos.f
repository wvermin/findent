! -Ia -i5 -a1 -b2 -d3 -f4 -m5 -ofree
         module m1
         integer k
         enum, bind(c)
      
         enumerator :: red =1, blue, black =5
         enumerator yellow
         enumerator gold, silver, bronze
         enumerator :: purple
         enumerator :: pink, lavender
      
      endenum
      contains
         function f(x)
            print *,'abc 
     9      def'
       print *,"abcd
     8        efgh"
       return
       end function
       end module
      
      program progfixed
         type mytype  ! mytype
            integer i
            real x
         end type mytype
#ifdef abcde
         real klm@@@(10)
#endif
         integer x(100)
         type(mytype)  xyz
         character*1000 astring
         do i=1,
c a comment         
c a comment         
     x   1
c a comment         
     x   0
         continue
         enddo
         continue
         astring = " ! this
     x is
     x no comment!"
         print *,a,b,
     x           c,d
     x          ,e,f
         do i=1,20
            block
               do 90 k=1,90
                  do 90 k1=1,4
                  call one(a)
                  do 7 j=1,20
                     call two(a)
  7                     x=x+j
                  continue
  90                 continue
                  continue
                  end block
               enddo
               critical
               x=y
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
            end subroutine
            subroutine two(a)
               continue
               return
            endsubroutine
            subroutine three(a)
               continue
               return
            end
            subroutine four(a)
               continue
               return
            endsubroutine four
            subrou
     xtine five
            continue
            return 
         end
