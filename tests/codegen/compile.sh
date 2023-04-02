#!/bin/bash

rm out.ll
./tests/codegen/codegentest -i < $@ > out.ll
llc-11 out.ll
clang-11 -o out out.s libEdsger.a


