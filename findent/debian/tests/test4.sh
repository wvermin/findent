#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfixedfree.f
rc=$?
. ../postlude
exit $rc
