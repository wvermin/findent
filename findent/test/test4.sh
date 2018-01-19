#!/bin/sh
. ./prelude
../test-compile.sh ../progfixedfree.f
rc=$?
exit $rc
