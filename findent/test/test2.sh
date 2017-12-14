#!/bin/sh
SRCDIR=${SRCDIR:-.}
$SRCDIR/test/test-compile.sh $SRCDIR/test/progfixed.f
rc=$?
exit $rc
