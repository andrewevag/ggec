#!/bin/bash

rm out.ll
./tests/codegen/codegentest -i < $@ > out.ll
llc out.ll
clang -o out out.s libEdsger.a


