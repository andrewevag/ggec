#!/bin/bash

./ggec -i < "$@" > out.ll
llc out.ll
clang -o out out.s libEdsger.a


