#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfixedfree-dos.f
rc=$?
. ../postlude
exit $rc
