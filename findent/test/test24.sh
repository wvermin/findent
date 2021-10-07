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

# testing error conditions of relabeling

if test -e prelude ; then
   . ./prelude
else
   . ./debian/tests/prelude
fi
rc=0
cat << eof > prog
subroutine s
goto 10
10 continue
end
   program p
     goto 123 
  end
eof
cat << eof > expect
subroutine s
   goto 1000
1000 continue
end
program p
   goto 123
end
eof
../doit "--relabel" "-ifree" " Test001: undefined label format:free"
rc=`expr $rc + $?`

cat << eof > prog
         subroutine s
         goto 10
         10 continue
         end
      program p
       goto 123 
       end
eof
cat << eof > expect
      subroutine s
         goto 1000
 1000    continue
      end
      program p
         goto 123
      end
eof
../doit "--relabel" "-ifixed" " Test002: undefined label format: fixed"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s
goto 10
10 continue
end
   program p
     goto 123 
  end
eof
cat << eof > expect
!Relabeling error: undefined label: 123 (6);
eof
../doit "--query-relabel=1" "-ifree" " Test003: undefined label format:free"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s;continue
goto 10
10 continue
end
   program p
     goto 123 
123 continue
  end
eof
cat << eof > expect
subroutine s;continue
   goto 10
10 continue
end
program p
   goto 123
123 continue
end
eof
../doit "--relabel" "-ifree" " Test004: subroutine in multistatemen line: free"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s;continue
      goto 10
10     continue
      end
         program p
           goto 123 
123      continue
        end
eof
cat << eof > expect
      subroutine s;continue
         goto 10
   10    continue
      end
      program p
         goto 123
  123    continue
      end
eof
../doit "--relabel" "-ifixed" " Test005: subroutine in multistatemen line: fixed"

cat << eof > prog
      subroutine s;continue
      goto 10
10     continue
      end
         program p
           goto 123 
123      continue
        end
eof
cat << eof > expect
!Relabeling error: subroutine statement part of multi statement line (1);
eof
../doit "--query_relabel=1" "-ifixed" " Test006: subroutine in multistatemen line: fixed"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s
function f(x)
goto 10
10 continue
end
   program p
     goto 123 
123 continue
  end
eof
cat << eof > expect
subroutine s
   function f(x)
      goto 10
10    continue
   end
   program p
      goto 123
123   continue
   end
eof
../doit "--relabel" "-ifree" " Test007: misplaced function definition: free"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s
            goto 10
       continue
         end
         program p
         subroutine f(x)
            goto 123
      continue
         end
eof
cat << eof > expect
      subroutine s
         goto 10
         continue
      end
      program p
         subroutine f(x)
            goto 123
            continue
         end
eof
../doit "--relabel" "-ifixed" " Test008: misplaced subroutine definition: fixed"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s
goto 10
10 continue
end
   program p
   function f(x)
     goto 123 
123 continue
  end
eof
cat << eof > expect
!subroutine s:
!original(def'd )     new (used)
!      10(     3) ->  1000(2)
!Relabeling error: misplaced function statement (6);
eof
../doit "--query-relabel=2" "-ifree" " Test009: misplaced function definition: free"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s
goto (10,20,x),i
10 continue
20 continue
end
   program p
     goto 123 
123 continue
  end
eof
cat << eof > expect
subroutine s
   goto (10,20,x),i
10 continue
20 continue
end
program p
   goto 123
123 continue
end
eof
../doit "--relabel" "-ifree" " Test010: computed goto: free"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s
      goto (10,20,x),i
10       continue
 20      continue
      end
         program p
           goto 123 
123       continue
        end
eof
cat << eof > expect
!Relabeling error: error in computed GOTO list (2);
eof
../doit "--query-relabel=1" "-ifixed" " Test011: computed goto: fixed"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s
      assign 20 to i
      goto i,(10,20,)
10       continue
 20      continue
      end
         program p
           goto 123 
123       continue
        end
eof
cat << eof > expect
      subroutine s
         assign 20 to i
         goto i,(10,20,)
   10    continue
   20    continue
      end
      program p
         goto 123
  123    continue
      end
eof
../doit "--relabel" "-ifixed" " Test012: assigned goto: fixed"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s
      assign 20 to i
      goto i,(10,20,)
10       continue
 20      continue
      end
         program p
           goto 123 
123       continue
        end
eof
cat << eof > expect
!Relabeling error: error in assigned GOTO list (3);
eof
../doit "--query_relabel=1" "-ifixed" " Test013: assigned goto: fixed"
rc=`expr $rc + $?`

cat << eof > prog
subroutine s
goto (10,20),i
10 continue
20 continue
end
   program p
     goto 12&
     &3 
123 continue
  end
eof
cat << eof > expect
subroutine s
   goto (1000,1010),i
1000 continue
1010 continue
end
program p
   goto 12&
   &3
123 continue
end
eof
../doit "--relabel" "-ifree" " Test014: continuation in label: free"
rc=`expr $rc + $?`

cat << eof > prog
         subroutine s
         goto (10,20),i
10          continue
20          continue
         end
            program p
              goto 12
     &3 
123          continue
           end
eof
cat << eof > expect
!subroutine s:
!original(def'd )     new (used)
!      10(     3) ->  1000(2)
!      20(     4) ->  1010(2)
!Relabeling error: label not on one line: 123(8)
eof
../doit "--query-relabel=2" "-ifixed" " Test015: continuation in label: fixed"
rc=`expr $rc + $?`


cat << eof > prog
subroutine s
goto (10,20,30),i
10 continue
20 continue
30 continue
end
   program p
     goto (123,&
#ifdef X
     1)
#else
     2)
#endif
123 continue
1 continue
2 continue
  end
eof
cat << eof > expect
subroutine s
   goto (1000,1010,1020),i
1000 continue
1010 continue
1020 continue
end
program p
   goto (123,&
#ifdef X
      1)
#else
   2)
#endif
123 continue
1  continue
2  continue
end
eof
../doit "--relabel" "-ifree" " Test016: intervening preprocessor statement: free"
rc=`expr $rc + $?`

cat << eof > prog
      subroutine s
      goto (10,20,30),i
10         continue
20         continue
30         continue
      end
       program p
       goto (123,
#ifdef X
     &1)
#else
     &2)
#endif
123 continue
1 continue
2 continue
  end
eof
cat << eof > expect
!Relabeling error: intervening preprocessor statement (12);
eof
../doit "--query-relabel=1" "-ifixed" " Test017: intervening preprocessor statement: fixed"
rc=`expr $rc + $?`

cat << eof > prog
         subroutine s
         goto (10,20),i
10          continue
20          continue
         end
            program p
              goto 123
123          continue
1          continue
4          continue
5          continue
9          continue
10       continue
           end
eof
cat << eof > expect
      subroutine s
         goto (99990,99992),i
99990    continue
99992    continue
      end
      program p
         goto 123
  123    continue
    1    continue
    4    continue
    5    continue
    9    continue
   10    continue
      end
eof
../doit "--relabel=99990,2" "-ifixed" " Test018: label too large: fixed"
rc=`expr $rc + $?`


cat << eof > prog
 subroutine s
         goto (10,20),i
10          continue
20          continue
         end
            program p
              goto 123
123          continue
1          continue
4          continue
5          continue
9          continue
10         continue
           end
eof
cat << eof > expect
!Relabeling error: label would go out of range: 10 -> 100000 (13);
eof
../doit "--query-relabel=1" "-ifree --relabel=99990,2" " Test019: label too large: free"
rc=`expr $rc + $?`

cat << eof > prog
         subroutine s
         goto (10,20),i
10          continue
20          continue
         end; program p
              goto 123
123          continue
           end
eof
cat << eof > expect
      subroutine s
         goto (10,20),i
   10    continue
   20    continue
      end; program p
         goto 123
  123    continue
      end
eof
../doit "--relabel" "-ifixed" " Test020: end statement in multi statement line: fixed"
rc=`expr $rc + $?`

cat << eof > prog
  subroutine s
         goto (10,20),i
10          continue
20          continue
  end; program p
              goto 123
123          continue
           end
eof
cat << eof > expect
!Relabeling error: END in multi statement line (5);
eof
../doit "--relabel" "-ifree --query-relabel=1" " Test021: end statement in multi statement line: free"
rc=`expr $rc + $?`

cp ../progrelabelfree.f prog
cat << eof > expect
!subroutine mysub:
!original(def'd )     new (used)
!subroutine mysubomp:
!original(def'd )     new (used)
!      10(     9) ->  1000(6,7)
!subroutine mysub1:
!original(def'd )     new (used)
!       1(    13) ->  1000()
!subroutine mysub2:
!original(def'd )     new (used)
!function myfun:
!original(def'd )     new (used)
!      10(    23) ->  1000(21)
!      20(    25) ->  1010(21)
!      30(    26) ->  1020(21)
!      50(    31) ->  1030(23,25,26,28)
!program p:
!original(def'd )     new (used)
!       1(    44) ->  1080(41)
!    1000(    62) ->  1140(61)
!    1001(    62) ->  1150(60)
!      11(    40) ->  1030()
!    1101(    94) ->  1260(84,87,88,89)
!    1102(    97) ->  1270(87,92)
!     111(    39) ->  1000()
!     112(    39) ->  1010()
!     115(    39) ->  1020()
!     120(    65) ->  1160(64)
!    1200(    93) ->  1250(94,95,96,97,98,100,102)
!   12345(    53) ->  1110(51)
!     130(    66) ->  1170(64,87)
!       2(    44) ->  1070(41)
!    2010(   147) ->  1280(92,100,102,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,129,129,129,130,130,130,131,132,136,136,136,138,138,143,143)
!    2020(   148) ->  1290(107,108,126,129,133,136,139,140,141,144,145,146)
!       3(    43) ->  1060(41)
!    3010(   165) ->  1300(149,151,155,156,157,161,162,163)
!    3020(   166) ->  1310(149,151,155,156,157,161,162,163)
!    3030(   167) ->  1320(149,151,155,156,157,161,162,163)
!      31(    40) ->  1040()
!       5(    50) ->  1100(50)
!      51(    40) ->  1050()
!     511(    48) ->  1090(45,46,47)
!       8(    56) ->  1120(54)
!       9(    59) ->  1130(57)
!      91(    78) ->  1220(68,68,72,72,75,84,87,89,92)
!      92(    79) ->  1230(68,68,72,72,75,84,87,89,92)
!      93(    80) ->  1240(68,68,72,72,84,87,89,92)
!      95(    68) ->  1180()
!     951(    72) ->  1200()
!      96(    68) ->  1190()
!     961(    72) ->  1210()
!function f:
!original(def'd )     new (used)
!       1(   170) ->  1000()
!subroutine subf:
!original(def'd )     new (used)
!       1(   174) ->  1000()
!subroutine subx:
!original(def'd )     new (used)
!     100(   225) ->  1060(194,195,196,197,198,199,201,202,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224)
!     101(   185) ->  1010(184)
!     102(   187) ->  1020(186)
!     103(   189) ->  1030(188)
!     104(   191) ->  1040(190)
!     105(   193) ->  1050(192)
!     110(   225) ->  1070(182,183,197,199,200,201,203,206,207,208,214,217,220,222,224)
!     120(   225) ->  1080(182,183,197,206,207,208,214,217,220,222,224)
!     130(   225) ->  1090(182,214,217)
!      99(   181) ->  1000()
!subroutine subx1:
!original(def'd )     new (used)
!     100(   262) ->  1000(230,231,232,233,234,235,237,238,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260)
!     110(   262) ->  1010(233,235,236,237,239,242,243,244,250,253,256,258,260)
!     120(   262) ->  1020(233,242,243,244,250,253,256,258,260)
!     130(   262) ->  1030(250,253)
!subroutine suby:
!original(def'd )     new (used)
!     100(   311) ->  1060(280,281,282,283,284,285,287,288,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310)
!     101(   271) ->  1010(270)
!     102(   273) ->  1020(272)
!     103(   275) ->  1030(274)
!     104(   277) ->  1040(276)
!     105(   279) ->  1050(278)
!     110(   311) ->  1070(268,269,283,285,286,287,289,292,293,294,300,303,306,308,310)
!     120(   311) ->  1080(268,269,283,292,293,294,300,303,306,308,310)
!     130(   311) ->  1090(268,300,303)
!      99(   267) ->  1000()
!subroutine suby1:
!original(def'd )     new (used)
!     100(   348) ->  1000(316,317,318,319,320,321,323,324,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346)
!     110(   348) ->  1010(319,321,322,323,325,328,329,330,336,339,342,344,346)
!     120(   348) ->  1020(319,328,329,330,336,339,342,344,346)
!     130(   348) ->  1030(336,339)
!subroutine old_dec:
!original(def'd )     new (used)
!      10(   374) ->  1000(357,362,367)
!      20(   375) ->  1010(369,370)
!      30(   376) ->  1020(372)
!      40(   377) ->  1030(373)
!      50(   378) ->  1040(358,359,360,361,362,363,364,365,365,372)
!      60(   379) ->  1050(365,373)
!Relabeling error: intervening preprocessor statement (387);
eof
../doit "--query-relabel=2" "" " Test022: query-relabel of larger program: free"
rc=`expr $rc + $?`

cp ../progfixedrelabelfixed.f prog
cat << eof > expect
!subroutine mysub:
!original(def'd )     new (used)
!subroutine mysubomp:
!original(def'd )     new (used)
!      10(     9) ->  1000(6,7)
!subroutine mysub1:
!original(def'd )     new (used)
!       1(    13) ->  1000()
!subroutine mysub2:
!original(def'd )     new (used)
!function myfun:
!original(def'd )     new (used)
!      10(    23) ->  1000(21)
!      20(    25) ->  1010(21)
!      30(    26) ->  1020(21)
!      50(    30) ->  1030(23,25,26,28)
!program p:
!original(def'd )     new (used)
!       1(    44) ->  1040(40)
!    1000(    63) ->  1100(62)
!    1001(    64) ->  1110(61)
!      11(    39) ->  1010()
!    1101(   100) ->  1220(90,93,94,95)
!    1102(   104) ->  1230(93,98)
!     111(    38) ->  1000()
!     120(    67) ->  1120(66)
!    1200(    99) ->  1210(100,101,102,103,104,105,107,109)
!   12345(    54) ->  1070(52)
!     130(    68) ->  1130(66,93)
!       2(    43) ->  1030(40)
!    2010(   155) ->  1240(98,107,109,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,136,136,136,137,137,138,139,140,144,144,144,146,146,151,151)
!    2020(   156) ->  1250(114,115,133,136,141,144,147,148,149,152,153,154)
!       3(    42) ->  1020(40)
!    3010(   173) ->  1260(157,159,163,164,165,169,170,171)
!    3020(   174) ->  1270(157,159,163,164,165,169,170,171)
!    3030(   175) ->  1280(157,159,163,164,165,169,170,171)
!       5(    51) ->  1060(50)
!     511(    48) ->  1050(45,46,47)
!       8(    57) ->  1080(55)
!       9(    60) ->  1090(58)
!      91(    84) ->  1180(70,71,76,77,81,90,93,95,98)
!      92(    85) ->  1190(70,71,76,77,81,90,93,95,98)
!      93(    86) ->  1200(70,72,76,78,90,93,95,98)
!      95(    71) ->  1140()
!     951(    77) ->  1160()
!      96(    72) ->  1150()
!     961(    78) ->  1170()
!function f:
!original(def'd )     new (used)
!      28(   179) ->  1000(178)
!subroutine subx:
!original(def'd )     new (used)
!      29(   184) ->  1000(183)
!subroutine subx1:
!original(def'd )     new (used)
!     100(   236) ->  1060(204,205,206,207,208,209,211,212,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234)
!     101(   195) ->  1010(194)
!     102(   197) ->  1020(196)
!     103(   199) ->  1030(198)
!     104(   201) ->  1040(200)
!     105(   203) ->  1050(202)
!     110(   237) ->  1070(192,193,207,209,210,211,213,216,217,218,224,227,230,232,234)
!     120(   238) ->  1080(192,193,207,216,217,218,224,227,230,232,234)
!     130(   239) ->  1090(192,224,227)
!      99(   191) ->  1000()
!subroutine subx2:
!original(def'd )     new (used)
!     100(   277) ->  1000(245,246,247,248,249,250,252,253,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275)
!     110(   278) ->  1010(248,250,251,252,254,257,258,259,265,268,271,273,275)
!     120(   279) ->  1020(248,257,258,259,265,268,271,273,275)
!     130(   280) ->  1030(265,268)
!subroutine suby1:
!original(def'd )     new (used)
!     100(   331) ->  1060(299,300,301,302,303,304,306,307,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329)
!     101(   290) ->  1010(289)
!     102(   292) ->  1020(291)
!     103(   294) ->  1030(293)
!     104(   296) ->  1040(295)
!     105(   298) ->  1050(297)
!     110(   332) ->  1070(287,288,302,304,305,306,308,311,312,313,319,322,325,327,329)
!     120(   333) ->  1080(287,288,302,311,312,313,319,322,325,327,329)
!     130(   334) ->  1090(287,319,322)
!      99(   286) ->  1000()
!subroutine suby2:
!original(def'd )     new (used)
!     100(   372) ->  1000(339,340,341,342,343,344,346,347,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369)
!     110(   373) ->  1010(342,344,345,346,348,351,352,353,359,362,365,367,369)
!     120(   374) ->  1020(342,351,352,353,359,362,365,367,369)
!     130(   375) ->  1030(359,362)
!subroutine old_dec:
!original(def'd )     new (used)
!      10(   400) ->  1000(385,391,393)
!      20(   401) ->  1010(389,390,395,396)
!      30(   402) ->  1020(398)
!      40(   403) ->  1030(399)
!      50(   404) ->  1040(381,382,383,384,385,386,387,388,388,398)
!      60(   405) ->  1050(388,399)
!Relabeling error: intervening preprocessor statement (415);
eof
../doit "--query-relabel=2" "" " Test023: query-relabel of larger program: fixed"
rc=`expr $rc + $?`

cat << eof > prog
  subroutine s
         goto (10,20),i
10          continue
20          continue
  end
  program p
              goto 123
123          continue
continue;contains
real function f(x)
20 continue
end function f
           end program p
eof
cat << eof > expect
!Relabeling error: CONTAINS in multi statement line (9);
eof
../doit "--relabel" "-ifree --query-relabel=1" " Test024: contains statement in multi statement line: free"
rc=`expr $rc + $?`

cat << eof > prog
         subroutine s
         goto (10,20),i
10          continue
20          continue
               end
            program p
              goto 123
123          continue
         contains; real function f(x)
20         continue
          f=10*x
          end function f
           end program p
eof
cat << eof > expect
!subroutine s:
!original(def'd )     new (used)
!      10(     3) ->  1000(2)
!      20(     4) ->  1010(2)
!Relabeling error: CONTAINS in multi statement line (9);
eof
../doit "--relabel" "-ifixed --query-relabel=2" " Test025: contains statement in multi statement line: fixed"
rc=`expr $rc + $?`

cat << eof > prog
subroutine gnikit
 x = p + fun (a,&
  b,&
  c)
10 call sub1(a,&
  bcd,&
   fun(3.0,&
   4.0,&
      5.0),&
  [6, &
  7, &
  8] &
   )
 call sub1(a,&
  bcd,&
   fun(3.0,&
! comment
   4.0,&
   ! comment
      5.0),&
  [6, &
  7, &
  8] &
   )
  end
eof
cat << eof > expect
        subroutine gnikit
           x = p + fun (a,&
                        b,&
                        c)
10         call sub1(a,&
                     bcd,&
                     fun(3.0,&
                         4.0,&
                         5.0),&
                     [6, &
                      7, &
                      8] &
                     )
           call sub1(a,&
                     bcd,&
                     fun(3.0,&
! comment
                         4.0,&
           ! comment
                         5.0),&
                     [6, &
                      7, &
                      8] &
                     )
        end
eof
../doit "--align_paren=1 --align-paren=1" "-ifree -I8" " Test026: --align_paren: free"
rc=`expr $rc + $?`

cat << eof > expect
        subroutine gnikit
           x = p + fun (a,&
                        b,&
                        c)
           10 call sub1(a,&
                        bcd,&
                        fun(3.0,&
                            4.0,&
                            5.0),&
                        [6, &
                         7, &
                         8] &
                        )
           call sub1(a,&
                     bcd,&
                     fun(3.0,&
! comment
                         4.0,&
           ! comment
                         5.0),&
                     [6, &
                      7, &
                      8] &
                     )
        end
eof
../doit "--align_paren=1 --align-paren=1 " "-ifree -I8 --label_left=0" " Test027: --align_paren --label_left=0: free"
rc=`expr $rc + $?`

cat << eof > expect
        subroutine gnikit
           x = p + fun (a,&
              b,&
              c)
10         call sub1(a,&
              bcd,&
              fun(3.0,&
              4.0,&
              5.0),&
              [6, &
              7, &
              8] &
              )
           call sub1(a,&
              bcd,&
              fun(3.0,&
! comment
              4.0,&
           ! comment
              5.0),&
              [6, &
              7, &
              8] &
              )
        end
eof
../doit "-kd" "-ifree -I8 -k-" " Test028: -k- -kd: free"
rc=`expr $rc + $?`

cat << eof > expect
        subroutine gnikit
           x = p + fun (a,&
              b,&
              c)
10         call sub1(a,&
              bcd,&
              fun(3.0,&
              4.0,&
              5.0),&
              [6, &
              7, &
              8] &
              )
           call sub1(a,&
              bcd,&
              fun(3.0,&
! comment
              4.0,&
           ! comment
              5.0),&
              [6, &
              7, &
              8] &
              )
        end
eof
../doit "--indent_continuation=default --indent-continuation=default" "-I8 -ifree  -k- " " Test029: -k- --indent_continuation=default: free"
rc=`expr $rc + $?`

cat << eof > expect
subroutine gnikit
   x = p + fun (a,&
      b,&
      c)
10 call sub1(a,&
      bcd,&
      fun(3.0,&
      4.0,&
      5.0),&
      [6, &
      7, &
      8] &
      )
   call sub1(a,&
      bcd,&
      fun(3.0,&
! comment
      4.0,&
   ! comment
      5.0),&
      [6, &
      7, &
      8] &
      )
end
eof
../doit "-q" "--safe" " Test030: -k- --indent_continuation=default: free"
rc=`expr $rc + $?`

. ../postlude
exit $rc
# vim: indentexpr=none
