#!/bin/sh
SRCDIR=${SRCDIR:-.}
$SRCDIR/test/test-compile.sh $SRCDIR/test/progfixed-dos.f
rc=$?
exit $rc
