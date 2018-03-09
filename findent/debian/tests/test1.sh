#!/bin/sh
cd debian/tests
. ./prelude
../test-compile.sh ../progfixed-dos.f
rc=$?
. ../postlude
exit $rc
