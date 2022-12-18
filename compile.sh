#!/bin/bash

./ggec "$@"
llc out.ll
clang -o out out.s libEdsger.a


