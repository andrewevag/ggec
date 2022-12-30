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

# Build & Install
Read Documentation.md or for just building run
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
it automatically does the linking with the -lm. and -lc.

Use #include "headers/stdio.h" for the Standard Library in test.eds. Results is in 


# Known bugs
./ggec -f < program.eds > out.s (Causes llvm buffers to break). 

Use ./ggec program.eds instead


# Requirements for Building
- clang++-10
- clang-10
- llvm-10
- flex 2.6.4
- bison 3.5.1
  
# Requirements for Testing
- clang++-10
- clang-10
- llvm-10
- flex 2.6.4
- bison 3.5.1
- llc-10
- python3 with (subprocess, os, deepdiff)
- erl
- erlang proper
- (haskell) stack
- EdsgerProgramGenerator.

