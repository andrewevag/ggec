#!/bin/bash

rm out.s
./tests/codegen/codegentest -f < $@ > out.s
# llc-10 out.ll
clang-10 -o out out.s libEdsger.a


