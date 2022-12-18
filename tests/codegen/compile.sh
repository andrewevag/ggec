#!/bin/bash

./tests/codegen/codegentest "$@"
llc out.ll
clang -o out out.s libEdsger.a


