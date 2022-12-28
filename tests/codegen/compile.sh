#!/bin/bash

rm out.ll
./tests/codegen/codegentest -i < $@ > out.ll
llc-10 out.ll
clang-10 -o out out.s libEdsger.a


