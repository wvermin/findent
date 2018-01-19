#!/bin/sh
. ./prelude
../test-compile.sh ../progfixed-dos.f
rc=$?
exit $rc
