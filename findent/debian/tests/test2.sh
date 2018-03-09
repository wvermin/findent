#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfixed.f
rc=$?
. ../postlude
exit $rc
