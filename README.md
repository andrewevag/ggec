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

Use #include "headers/stdio.h" for the Standard Library in test.eds


# Features 
1. 10 byte real variables
2. Type casting
3. Optimization with LLVM
4. Function Overloading

# Function Overloading
Τα ονόματα των συναρτήσεων επεκτείνονται κατά τον εσωτερικό χειρισμό από τον compiler με πληροφορίες για τους τύπους των ορισμάτων.
Έτσι για παράδειγμα η 
``` C
bool prime(int n);
```
εσωτερικά παίρνει το όνομα *prime_int*.

Για αυτό το λόγο οι εξωτερικές συναρτήσεις πρέπει να χρησιμοποιούν το ίδιο naming convention. Η βιβλιοθήκη της Edsger έχει γραφεί σε C ακολουθώντας την σύμβαση (στον φάκελο `lib`)



# Type Casting


# Invariant
## Semantic Analysis
- When processing a function first add the function in current scope and open a new one =>> The function whose processing is being done is the last entry of the parent scope if such exists. Since things added in the symbol table are backwards it is the first in the list of entries in parent scope. 
- Scope nesting level of a function body is always > 1. Global scope is with nesting level 1.
- Non Nested Function Declarations can be defined externally by using the same name convention

# Codegen Invariants
1. Δινεις παντα σε κατωτερες nested συναρτησεις το δικο σου local environmnent.
2. Possible offsets when calling functions
    1. +1 local nested function -> call using your own environment
    2. 0  recursion or nested function on same level -> call using the environment
            you were called with. |nestingLevelOfTheCalle - youNestingLevel| + 1.
    3. <0 functions that you are inside of -> get the environment to call climping
            up |nestingLevelOftheCalle - yourNestingLevel| + 1.

**In the symbol table for every function we need it's context.**

# Known bugs
- flex scanner jammed on unexpected #include
# Warnings to take into account
- Function Stack is kept in a global vector in the symbol table
  - This was needed because we couldn't decide which is the last function we're in
    - When newFunction is called it's entry is pushed into the stack
    - When closeScope() is called it's entry is popped from the stack but only if there is an entry to begin with (account for global scope)
Note:
We have the declaration of Functions and each line
ex. int x, y; is it's own declaration List.

---
# DO NOT FORGET
- Να καθορίσουμε στο makefile ακριβως ποιες version χρησιμοποιούμε 
- τα paths στα .sh αρχεια είναι για το mac 


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
- EdsgerProgramGenerator..

# Περιγραφή αρχείων
- `main.cpp`
  
  Ορισμός της main.

- `inc/tree.h`, `src/tree.c`
  
  Ορίζεται class για το τύπωμα δέντρων. Οι κόμβοι του AST την κληρονομούν. Χρησιμοποιείται για την οπτικοποίηση του συντακτικού δέντρου.

- `ast.hpp`

  Ορισμός των κόμβων του AST.

- `ast.cpp`

  Οι destructors των κλάσεων που ορίστηκαν στο `ast.hpp`

- `tojsonstring.cpp`

  Οι υλοποιήσεις των συναρτήσεων toJSONString για τους κόμβους του AST. Αυτή η συνάρτηση χρησιμοποιείται μόνο κατά το testing του parser για την σύγκριση συντακτικών δέντρων.

- `lexer.l`
  
  Ο lexer. Meta-program του flex.
- `lexer.hpp`
  
  Ορισμός των συναρτήσεων του lexer για χειρισμό από άλλα μέρη.

- `parser.y`

  Ο Parser. Meta-program του bison.

- `llvmhead.hpp`
  
  #include τα απαραίτητα για το llvm.

- `error.hpp`, `error.cpp`

  Συναρτήσεις για τον χειρισμό σφαλμάτων και ErrorInfo Class για τους κόμβους του AST.

- `general.hpp`, `general.cpp`

  Ορισμός καθολικών μεταβλητών του state και υλοποιήσεις συναρτήσεων χειρισμού ορισμάτων (flags) και εξόδου (print to stdout, asm, imm).

- `symbol.hpp`, `symbol.cpp`

  Ορισμός και υλοποίηση symbol table και τύπων.

- `semantical.cpp`

  Ορισμός της sem (σημασιολογικής ανάλυσης) για τους κόμβους του AST.

- `codegen.cpp`
  
  Ορισμός της codegen (παραγωγή κώδικα και βοηθητικές) για τους κόμβους του AST.