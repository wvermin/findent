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
../doit "--query_relabel --query-relabel=2" "-ifixed" " Test015: continuation in label: fixed"
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
!function point_dist:
!original(def'd )     new (used)
!procedure fun:
!original(def'd )     new (used)
!function fun:
!original(def'd )     new (used)
!subroutine x:
!original(def'd )     new (used)
!       1(    25) ->  1000(24)
!procedure point_dist:
!original(def'd )     new (used)
!      10(    31) ->  1000(30)
!subroutine mysub:
!original(def'd )     new (used)
!subroutine mysubomp:
!original(def'd )     new (used)
!      10(    45) ->  1000(42,43)
!subroutine mysub1:
!original(def'd )     new (used)
!       1(    49) ->  1000()
!subroutine mysub2:
!original(def'd )     new (used)
!function myfun:
!original(def'd )     new (used)
!      10(    59) ->  1000(57)
!      20(    61) ->  1010(57)
!      30(    62) ->  1020(57)
!      50(    67) ->  1030(59,61,62,64)
!program p:
!original(def'd )     new (used)
!       1(    80) ->  1080(77)
!    1000(    98) ->  1140(97)
!    1001(    98) ->  1150(96)
!      11(    76) ->  1030()
!    1101(   130) ->  1260(120,123,124,125)
!    1102(   133) ->  1270(123,128)
!     111(    75) ->  1000()
!     112(    75) ->  1010()
!     115(    75) ->  1020()
!     120(   101) ->  1160(100)
!    1200(   129) ->  1250(130,131,132,133,134,136,138)
!   12345(    89) ->  1110(87)
!     130(   102) ->  1170(100,123)
!       2(    80) ->  1070(77)
!    2010(   183) ->  1280(128,136,138,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,165,165,165,166,166,166,167,168,172,172,172,174,174,179,179)
!    2020(   184) ->  1290(143,144,162,165,169,172,175,176,177,180,181,182)
!       3(    79) ->  1060(77)
!    3010(   201) ->  1300(185,187,191,192,193,197,198,199)
!    3020(   202) ->  1310(185,187,191,192,193,197,198,199)
!    3030(   203) ->  1320(185,187,191,192,193,197,198,199)
!      31(    76) ->  1040()
!       5(    86) ->  1100(86)
!      51(    76) ->  1050()
!     511(    84) ->  1090(81,82,83)
!       8(    92) ->  1120(90)
!       9(    95) ->  1130(93)
!      91(   114) ->  1220(104,104,108,108,111,120,123,125,128)
!      92(   115) ->  1230(104,104,108,108,111,120,123,125,128)
!      93(   116) ->  1240(104,104,108,108,120,123,125,128)
!      95(   104) ->  1180()
!     951(   108) ->  1200()
!      96(   104) ->  1190()
!     961(   108) ->  1210()
!function f:
!original(def'd )     new (used)
!       1(   206) ->  1000()
!subroutine subf:
!original(def'd )     new (used)
!       1(   210) ->  1000()
!subroutine subx:
!original(def'd )     new (used)
!     100(   261) ->  1060(230,231,232,233,234,235,237,238,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260)
!     101(   221) ->  1010(220)
!     102(   223) ->  1020(222)
!     103(   225) ->  1030(224)
!     104(   227) ->  1040(226)
!     105(   229) ->  1050(228)
!     110(   261) ->  1070(218,219,233,235,236,237,239,242,243,244,250,253,256,258,260)
!     120(   261) ->  1080(218,219,233,242,243,244,250,253,256,258,260)
!     130(   261) ->  1090(218,250,253)
!      99(   217) ->  1000()
!subroutine subx1:
!original(def'd )     new (used)
!     100(   298) ->  1000(266,267,268,269,270,271,273,274,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296)
!     110(   298) ->  1010(269,271,272,273,275,278,279,280,286,289,292,294,296)
!     120(   298) ->  1020(269,278,279,280,286,289,292,294,296)
!     130(   298) ->  1030(286,289)
!subroutine suby:
!original(def'd )     new (used)
!     100(   347) ->  1060(316,317,318,319,320,321,323,324,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346)
!     101(   307) ->  1010(306)
!     102(   309) ->  1020(308)
!     103(   311) ->  1030(310)
!     104(   313) ->  1040(312)
!     105(   315) ->  1050(314)
!     110(   347) ->  1070(304,305,319,321,322,323,325,328,329,330,336,339,342,344,346)
!     120(   347) ->  1080(304,305,319,328,329,330,336,339,342,344,346)
!     130(   347) ->  1090(304,336,339)
!      99(   303) ->  1000()
!subroutine suby1:
!original(def'd )     new (used)
!     100(   384) ->  1000(352,353,354,355,356,357,359,360,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382)
!     110(   384) ->  1010(355,357,358,359,361,364,365,366,372,375,378,380,382)
!     120(   384) ->  1020(355,364,365,366,372,375,378,380,382)
!     130(   384) ->  1030(372,375)
!subroutine old_dec:
!original(def'd )     new (used)
!      10(   410) ->  1000(393,398,403)
!      20(   411) ->  1010(405,406)
!      30(   412) ->  1020(408)
!      40(   413) ->  1030(409)
!      50(   414) ->  1040(394,395,396,397,398,399,400,401,401,408)
!      60(   415) ->  1050(401,409)
!Relabeling error: intervening preprocessor statement (423);
eof
../doit "--query_relabel --query-relabel=2" "" " Test022: query-relabel of larger program: free"
rc=`expr $rc + $?`

cp ../progfixedrelabelfixed.f prog
cat << eof > expect
!function point_dist:
!original(def'd )     new (used)
!procedure fun:
!original(def'd )     new (used)
!function fun:
!original(def'd )     new (used)
!subroutine x:
!original(def'd )     new (used)
!       1(    25) ->  1000(24)
!procedure point_dist:
!original(def'd )     new (used)
!      10(    31) ->  1000(30)
!subroutine mysub:
!original(def'd )     new (used)
!subroutine mysubomp:
!original(def'd )     new (used)
!      10(    44) ->  1000(41,42)
!subroutine mysub1:
!original(def'd )     new (used)
!       1(    48) ->  1000()
!subroutine mysub2:
!original(def'd )     new (used)
!function myfun:
!original(def'd )     new (used)
!      10(    58) ->  1000(56)
!      20(    60) ->  1010(56)
!      30(    61) ->  1020(56)
!      50(    65) ->  1030(58,60,61,63)
!program p:
!original(def'd )     new (used)
!       1(    79) ->  1040(75)
!    1000(    98) ->  1100(97)
!    1001(    99) ->  1110(96)
!      11(    74) ->  1010()
!    1101(   135) ->  1220(125,128,129,130)
!    1102(   139) ->  1230(128,133)
!     111(    73) ->  1000()
!     120(   102) ->  1120(101)
!    1200(   134) ->  1210(135,136,137,138,139,140,142,144)
!   12345(    89) ->  1070(87)
!     130(   103) ->  1130(101,128)
!       2(    78) ->  1030(75)
!    2010(   190) ->  1240(133,142,144,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,171,171,171,172,172,173,174,175,179,179,179,181,181,186,186)
!    2020(   191) ->  1250(149,150,168,171,176,179,182,183,184,187,188,189)
!       3(    77) ->  1020(75)
!    3010(   208) ->  1260(192,194,198,199,200,204,205,206)
!    3020(   209) ->  1270(192,194,198,199,200,204,205,206)
!    3030(   210) ->  1280(192,194,198,199,200,204,205,206)
!       5(    86) ->  1060(85)
!     511(    83) ->  1050(80,81,82)
!       8(    92) ->  1080(90)
!       9(    95) ->  1090(93)
!      91(   119) ->  1180(105,106,111,112,116,125,128,130,133)
!      92(   120) ->  1190(105,106,111,112,116,125,128,130,133)
!      93(   121) ->  1200(105,107,111,113,125,128,130,133)
!      95(   106) ->  1140()
!     951(   112) ->  1160()
!      96(   107) ->  1150()
!     961(   113) ->  1170()
!function f:
!original(def'd )     new (used)
!      28(   214) ->  1000(213)
!subroutine subx:
!original(def'd )     new (used)
!      29(   219) ->  1000(218)
!subroutine subx1:
!original(def'd )     new (used)
!     100(   271) ->  1060(239,240,241,242,243,244,246,247,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269)
!     101(   230) ->  1010(229)
!     102(   232) ->  1020(231)
!     103(   234) ->  1030(233)
!     104(   236) ->  1040(235)
!     105(   238) ->  1050(237)
!     110(   272) ->  1070(227,228,242,244,245,246,248,251,252,253,259,262,265,267,269)
!     120(   273) ->  1080(227,228,242,251,252,253,259,262,265,267,269)
!     130(   274) ->  1090(227,259,262)
!      99(   226) ->  1000()
!subroutine subx2:
!original(def'd )     new (used)
!     100(   312) ->  1000(280,281,282,283,284,285,287,288,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310)
!     110(   313) ->  1010(283,285,286,287,289,292,293,294,300,303,306,308,310)
!     120(   314) ->  1020(283,292,293,294,300,303,306,308,310)
!     130(   315) ->  1030(300,303)
!subroutine suby1:
!original(def'd )     new (used)
!     100(   366) ->  1060(334,335,336,337,338,339,341,342,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364)
!     101(   325) ->  1010(324)
!     102(   327) ->  1020(326)
!     103(   329) ->  1030(328)
!     104(   331) ->  1040(330)
!     105(   333) ->  1050(332)
!     110(   367) ->  1070(322,323,337,339,340,341,343,346,347,348,354,357,360,362,364)
!     120(   368) ->  1080(322,323,337,346,347,348,354,357,360,362,364)
!     130(   369) ->  1090(322,354,357)
!      99(   321) ->  1000()
!subroutine suby2:
!original(def'd )     new (used)
!     100(   407) ->  1000(374,375,376,377,378,379,381,382,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404)
!     110(   408) ->  1010(377,379,380,381,383,386,387,388,394,397,400,402,404)
!     120(   409) ->  1020(377,386,387,388,394,397,400,402,404)
!     130(   410) ->  1030(394,397)
!subroutine old_dec:
!original(def'd )     new (used)
!      10(   435) ->  1000(420,426,428)
!      20(   436) ->  1010(424,425,430,431)
!      30(   437) ->  1020(433)
!      40(   438) ->  1030(434)
!      50(   439) ->  1040(416,417,418,419,420,421,422,423,423,433)
!      60(   440) ->  1050(423,434)
!Relabeling error: intervening preprocessor statement (450);
eof
../doit "--query_relabel --query-relabel=2" "" " Test023: query-relabel of larger program: fixed"
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
program paren
   x = y + &
      fun(a, &
      abcd ,&
      z)
   x = y +  &
              z + &
      a; call sub (a, &
!comment
      b, &
      c)
   x = y +  a; call sub (a, &
      b, &
      c)

   print *,'abc'; call sub(a,&
      b,&
      c)
#if 0
   write(10,'20) x; call sub(a,&
      b,&
      c)
   '
   write(10           '   20) x; call sub(a,&
      b,&
      c)
   write(10"20) x; call sub(a,&
      b,&
      c)
   "
#endif
contains
   subroutine sub(a,b,c)
   end
   function fun(a,b,c)
      fun=a
   end
end program
subroutine gnikit
 x = p + fun (a,&
  b,&
  c)
100 format(4ha(cd, &
       4hx(bc,&
       i5)
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
contains
 subroutine sub1(a,b,c,d)
    integer d(3)
   end
   function fun(a,b,c)
      fun=a
   end
  end
eof
cat << eof > expect
        program paren
           x = y + &
              fun(a, &
                  abcd ,&
                  z)
           x = y +  &
              z + &
              a; call sub (a, &
!comment
                           b, &
                           c)
           x = y +  a; call sub (a, &
                                 b, &
                                 c)

           print *,'abc'; call sub(a,&
                                   b,&
                                   c)
#if 0
           write(10,'20) x; call sub(a,&
                 b,&
                 c)
           '
           write(10           '   20) x; call sub(a,&
                                                  b,&
                                                  c)
           write(10"20) x; call sub(a,&
                 b,&
                 c)
           "
#endif
        contains
           subroutine sub(a,b,c)
           end
           function fun(a,b,c)
              fun=a
           end
        end program
        subroutine gnikit
           x = p + fun (a,&
                        b,&
                        c)
100        format(4ha(cd, &
              4hx(bc,&
              i5)
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
        contains
           subroutine sub1(a,b,c,d)
              integer d(3)
           end
           function fun(a,b,c)
              fun=a
           end
        end
eof
../doit "--align_paren --align-paren --align_paren=1 --align-paren=1" "-ifree -I8" " Test026: --align_paren: free"
rc=`expr $rc + $?`

cat << eof > expect
        program paren
           x = y + &
              fun(a, &
                  abcd ,&
                  z)
           x = y +  &
              z + &
              a; call sub (a, &
!comment
                           b, &
                           c)
           x = y +  a; call sub (a, &
                                 b, &
                                 c)

           print *,'abc'; call sub(a,&
                                   b,&
                                   c)
#if 0
           write(10,'20) x; call sub(a,&
                 b,&
                 c)
           '
           write(10           '   20) x; call sub(a,&
                                                  b,&
                                                  c)
           write(10"20) x; call sub(a,&
                 b,&
                 c)
           "
#endif
        contains
           subroutine sub(a,b,c)
           end
           function fun(a,b,c)
              fun=a
           end
        end program
        subroutine gnikit
           x = p + fun (a,&
                        b,&
                        c)
           100 format(4ha(cd, &
              4hx(bc,&
              i5)
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
        contains
           subroutine sub1(a,b,c,d)
              integer d(3)
           end
           function fun(a,b,c)
              fun=a
           end
        end
eof
../doit "--align_paren --align-paren --align_paren=1 --align-paren=1 " "-ifree -I8 --label_left=0" " Test027: --align_paren --label_left=0: free"
rc=`expr $rc + $?`

cat << eof > expect
        program paren
           x = y + &
              fun(a, &
              abcd ,&
              z)
           x = y +  &
              z + &
              a; call sub (a, &
!comment
              b, &
              c)
           x = y +  a; call sub (a, &
              b, &
              c)

           print *,'abc'; call sub(a,&
              b,&
              c)
#if 0
           write(10,'20) x; call sub(a,&
              b,&
              c)
           '
           write(10           '   20) x; call sub(a,&
              b,&
              c)
           write(10"20) x; call sub(a,&
              b,&
              c)
           "
#endif
        contains
           subroutine sub(a,b,c)
           end
           function fun(a,b,c)
              fun=a
           end
        end program
        subroutine gnikit
           x = p + fun (a,&
              b,&
              c)
100        format(4ha(cd, &
              4hx(bc,&
              i5)
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
        contains
           subroutine sub1(a,b,c,d)
              integer d(3)
           end
           function fun(a,b,c)
              fun=a
           end
        end
eof
../doit "-kd" "-ifree -I8 -k-" " Test028: -k- -kd: free"
rc=`expr $rc + $?`

cat << eof > expect
        program paren
           x = y + &
              fun(a, &
              abcd ,&
              z)
           x = y +  &
              z + &
              a; call sub (a, &
!comment
              b, &
              c)
           x = y +  a; call sub (a, &
              b, &
              c)

           print *,'abc'; call sub(a,&
              b,&
              c)
#if 0
           write(10,'20) x; call sub(a,&
              b,&
              c)
           '
           write(10           '   20) x; call sub(a,&
              b,&
              c)
           write(10"20) x; call sub(a,&
              b,&
              c)
           "
#endif
        contains
           subroutine sub(a,b,c)
           end
           function fun(a,b,c)
              fun=a
           end
        end program
        subroutine gnikit
           x = p + fun (a,&
              b,&
              c)
100        format(4ha(cd, &
              4hx(bc,&
              i5)
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
        contains
           subroutine sub1(a,b,c,d)
              integer d(3)
           end
           function fun(a,b,c)
              fun=a
           end
        end
eof
../doit "--indent_continuation=default --indent-continuation=default" "-I8 -ifree  -k- " " Test029: -k- --indent_continuation=default: free"
rc=`expr $rc + $?`

cat << eof > expect
program paren
   x = y + &
      fun(a, &
      abcd ,&
      z)
   x = y +  &
      z + &
      a; call sub (a, &
!comment
      b, &
      c)
   x = y +  a; call sub (a, &
      b, &
      c)

   print *,'abc'; call sub(a,&
      b,&
      c)
#if 0
   write(10,'20) x; call sub(a,&
      b,&
      c)
   '
   write(10           '   20) x; call sub(a,&
      b,&
      c)
   write(10"20) x; call sub(a,&
      b,&
      c)
   "
#endif
contains
   subroutine sub(a,b,c)
   end
   function fun(a,b,c)
      fun=a
   end
end program
subroutine gnikit
   x = p + fun (a,&
      b,&
      c)
100 format(4ha(cd, &
      4hx(bc,&
      i5)
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
contains
   subroutine sub1(a,b,c,d)
      integer d(3)
   end
   function fun(a,b,c)
      fun=a
   end
end
eof
../doit "-q" "--safe" " Test030: -k- --indent_continuation=default: free"
rc=`expr $rc + $?`

cat << eof > prog
subroutine remred
 x       = p       +fun        (a,           &
  b,&
  c)
100 format(4habcd             , &
       4hx(bc,&
       i5)

101 format("abcd      "             , &
       'pqr  st    '    ,                  &
       i5)

      call      sub1(a,   "hello              &
   world  "     ,  &
   10)
!$      call      sub1(a,   "hello              &
!$   world  "     ,  &
!$   10)
      call      sub1(a,   'hello              &
   world  '     ,  &
   10)
#if 0
   write(10   '  6)       '  a    b    c  '
   write(10   "  6)       ",  a,    b,    c  ! this    is    not    fortran
#endif
  end
eof
cat << eof > expect
        subroutine remred
           x = p +fun (a, &
              b,&
              c)
100        format(4habcd             , &
              4hx(bc,&
              i5)

101        format("abcd      " , &
              'pqr  st    ' , &
              i5)

           call sub1(a, "hello              &
              world  " , &
              10)
!$         call sub1(a, "hello              &
!$            world  " , &
!$            10)
           call sub1(a, 'hello              &
              world  ' , &
              10)
#if 0
           write(10 ' 6) '  a    b    c  '
           write(10 "  6)       ", a, b, c ! this    is    not    fortran
#endif
        end
eof
../doit "--ws-remred --ws_remred" "-ifree -I8" " Test031: --ws-remred free"
rc=`expr $rc + $?`

cat << eof > prog
       subroutine remred
        x       = p       +fun        (a,           
     1     b,
     2            c)
100   format(4habcd             , 
     1 4hx(bc,
     2 i5)

101    format("abcd      "             , 
     1 'pqr  st    '    ,                  
     2 i5)

       call      sub1(a,   "hello              
     1  world  "     ,
     2  10)
c$       call      sub1(a,   "hello              
!$   1  world  "     ,
c$   2  10)
      call      sub1(a,   'hello              
     1 world  '     ,
     2 10)
#if 0
       write(10   '  6)       '  a    b    c  '
      write(10   "  6)       ",  a,    b,    c  ! this    is    not    fortran
#endif
      end
eof
cat << eof > expect
          subroutine remred
             x = p +fun (a,
     1          b,
     2                 c)
  100        format(4habcd             ,
     1        4hx(bc,
     2        i5)

  101        format("abcd      " ,
     1       'pqr  st    ' ,
     2       i5)

             call sub1(a, "hello
     1  world  " ,
     2        10)
c$           call sub1(a, "hello
!$   1  world  " ,
c$   2       10)
             call sub1(a, 'hello
     1 world  ' ,
     2        10)
#if 0
             write(10 ' 6) '  a    b    c  '
             write(10 "  6)       ", a, b, c ! this    is    not    fortran
#endif
          end
eof
../doit "--ws-remred --ws_remred" "-ifixed -I4" " Test032: --ws-remred fixed"
rc=`expr $rc + $?`

. ../postlude
exit $rc
# vim: indentexpr=none
