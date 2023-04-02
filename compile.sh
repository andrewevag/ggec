#!/bin/bash

./ggec -i < "$@" > out.ll
llc-11 out.ll
clang-11 -o out out.s libEdsger.a -lm


