#!/bin/sh
# - makefdeps -
# Creates make-dependencies for fortran sources, based on
# analysis by findent of (sub)modules that are needed or created and
# include files. Include files are not further analyzed.
# Dependencies will be like:
# prog.o: sub1.o sub2.o
# prog.o: file1.inc file2.inc
# Example:
# makefdeps *.f90 > deps
usage()
{
   echo "usage:"
   echo "$0 [-s objectsuffix] fortansources ..."
   echo "objectsuffix: default '.o'"
   echo "depending on your make system, '.lo' can be the correct one"
   exit 1
}
FINDENTPROG=${FINDENT:-findent}

O='.o'
if [ "$1" = "-s" ] ; then
   O="$2"
   shift; shift
fi

while [ "$1" ] ; do
   j=${1%.*}$O
   $FINDENTPROG --deps < $1 | awk -v name="$j" '{ print $0, name}' 
   shift
done | 
awk '/^use /{use[$3] = use[$3] " " $2}
     /^mod |^sub /{ if (NF == 3) {module[$2] = $3}} 
     /^inc |^cpp |^coc |^std / {include[$3] = include[$3] " " $2}
END { 
      for (k in use) 
      {
	  m = ""
	  split(use[k],u)
	  for (l in u) { 
	     m=m " " module[u[l]]
	     }
	  mm = m; gsub(" ","",mm)
	  if (length(mm) != 0) { 
	     while (gsub("  "," ",m)){}
	     sub("^ ","",m)
	     print k ": " m 
	  }
      }
      for (k in include) { 
         while(sub("^ ","",include[k])){}
         print k ": " include[k] 
      }
    }'
