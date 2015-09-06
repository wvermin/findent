AU_ALIAS([AC_PROG_JAR_WORKS], [AX_PROG_JAR_WORKS])
AC_DEFUN([AX_PROG_JAR_WORKS], [
AC_PATH_PROG(UUDECODE, uudecode, [no])
if test x$UUDECODE != xno; then
dnl /**
dnl  * Test.java: used to test if java compiler works.
dnl  */
dnl public class Test
dnl {
dnl
dnl public static void
dnl main( String[] argv )
dnl {
dnl     System.exit (0);
dnl }
dnl
dnl }
cat << \EOF > Test.uue
begin-base64 644 Test.class
yv66vgADAC0AFQcAAgEABFRlc3QHAAQBABBqYXZhL2xhbmcvT2JqZWN0AQAE
bWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARDb2RlAQAPTGluZU51
bWJlclRhYmxlDAAKAAsBAARleGl0AQAEKEkpVgoADQAJBwAOAQAQamF2YS9s
YW5nL1N5c3RlbQEABjxpbml0PgEAAygpVgwADwAQCgADABEBAApTb3VyY2VG
aWxlAQAJVGVzdC5qYXZhACEAAQADAAAAAAACAAkABQAGAAEABwAAACEAAQAB
AAAABQO4AAyxAAAAAQAIAAAACgACAAAACgAEAAsAAQAPABAAAQAHAAAAIQAB
AAEAAAAFKrcAErEAAAABAAgAAAAKAAIAAAAEAAQABAABABMAAAACABQ=
====
EOF
AC_CACHE_CHECK([if uudecode can decode base 64 file], ac_cv_prog_uudecode_base64, [
if $UUDECODE Test.uue; then
        ac_cv_prog_uudecode_base64=yes
else
        echo "configure: __oline__: uudecode had trouble decoding base 64 file 'Test.uue'" >&AS_MESSAGE_LOG_FD
        echo "configure: failed file was:" >&AS_MESSAGE_LOG_FD
        cat Test.uue >&AS_MESSAGE_LOG_FD
        ac_cv_prog_uudecode_base64=no
fi
])
fi

if test x$ac_cv_prog_uudecode_base64 = xyes; then
   rm -f Test.class
   $UUDECODE Test.uue
fi

rm -f Test.uue

if test x$ac_cv_prog_uudecode_base64 != xyes; then
        rm -f Test.class
        AC_MSG_WARN([I have to compile Test.class from scratch])
        if test x$ac_cv_prog_javac_works = xno; then
                AC_MSG_ERROR([Cannot compile java source. $JAVAC does not work properly])
        fi
        if test x$ac_cv_prog_javac_works = x; then
                AX_PROG_JAVAC
        fi
fi
AC_CACHE_CHECK(if $JAR works, ac_cv_prog_jar_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
TEST=Test
JAR_TEST=Test.jar
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
public static void main (String args@<:@@:>@) {
        System.exit (0);
} }
EOF
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) && test -s $CLASS_TEST; then
                :
        else
          echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
          cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
          AC_MSG_ERROR(In jartest The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?))
        fi
fi
if AC_TRY_COMMAND($JAVA -classpath . $JAVAFLAGS $TEST) >/dev/null 2>&1; then
  rm -f $JAR_TEST
  if AC_TRY_COMMAND($JAR cfe $JAR_TEST $TEST $CLASS_TEST) >/dev/null 2>&1 ; then
    ac_cv_prog_jar_works=yes
  else
    echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
    cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
    AC_MSG_ERROR(The jar command $JAR failed (see config.log))
  fi
else
  echo "configure: failed program was:" >&AS_MESSAGE_LOG_FD
  cat $JAVA_TEST >&AS_MESSAGE_LOG_FD
  AC_MSG_ERROR(IN jartest The Java VM $JAVA failed (see config.log, check the CLASSPATH?))
fi
rm -fr $JAVA_TEST $CLASS_TEST Test.uue $JAR_TEST
])
AC_PROVIDE([$0])dnl
]
)
