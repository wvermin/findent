#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfree.f
rc=$?
. ../postlude
exit $rc
