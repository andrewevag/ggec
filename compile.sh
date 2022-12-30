#!/bin/bash

./ggec -i < "$@" > out.ll
llc-10 out.ll
clang-10 -o out out.s libEdsger.a -lm


