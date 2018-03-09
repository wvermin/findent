#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfixed1.f
rc=$?
. ../postlude
exit $rc
