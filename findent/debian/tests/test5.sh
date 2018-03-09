#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfree-dos.f
rc=$?
. ../postlude
exit $rc
