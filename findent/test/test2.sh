#!/bin/sh
. ./prelude
../test-compile.sh ../progfixed.f
rc=$?
exit $rc
