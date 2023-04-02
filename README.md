# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language
```
     _____________________________________   _____________________________________
.-/|                                     \ /                                     |\-.
||||                                      |                                      ||||
||||                                      |                  |                   ||||
||||                                      |                  |                   ||||
||||                                      |                 ,|                   ||||
||||     _____  _____ ______ _____        |                ,\|/.                 ||||
||||    / ____|/ ____|  ____/ ____|       |              ,' .V. `.               ||||
||||   | |  __| |  __| |__ | |            |             / .     . \              ||||
||||   | | |_ | | |_ |  __|| |            |            /_`       '_\             ||||
||||   | |__| | |__| | |___| |____        |           ,' .:     ;, `.            ||||
||||    \_____|\_____|______\_____|       |           |@)|  . .  |(@|            ||||
||||                                      |      ,-._ `._';  .  :`_,' _,-.       ||||
||||                                      |     '--  `-\ /,-===-.\ /-'  --`      ||||
||||                                      |    (----  _|  ||___||  |_  ----)     ||||
||||                                      |     `._,-'  \ /`-.-' \ /  `-._,'     ||||
||||                                      |              /-.___,-'\     ap       ||||
||||                                      |           /\/          \/\           ||||
||||_____________________________________ | _____________________________________||||
||/======================================\|/======================================\||
`---------------------------------------~___~--------------------------------------''
```

Should run on Linux on any target supported by LLVM-11. Run make test to see that everything is running as it should be.

# Build & Install
Read **Documentation.pdf/md** or for just building run
```
make help
```

# For Building Executables
Run:
```
make
make library
```
and use 
```
./compile.sh test.eds
```
it automatically does the linking with the libEdsger libm. and libc needed by libEdsger.

Use #include "headers/stdio.h" for the **Standard Library** or the complete path from where the compiler is called to `ggec/headers`


# Known bugs
./ggec -f < program.eds > out.s (Causes llvm buffers to break). 

Use ./ggec program.eds instead


# Requirements for Building
- clang++-11
- clang-11
- llvm-11
- flex 2.6.4
- bison 3.5.1
  
# Requirements for Testing
- clang++-11
- clang-11
- llvm-11
- flex 2.6.4
- bison 3.5.1
- llc-11
- python3 with (subprocess, os, deepdiff)
- erl
- erlang proper
- (haskell) stack
- EdsgerProgramGenerator.

