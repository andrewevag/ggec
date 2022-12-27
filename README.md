# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language
```
                 |
                 |
                ,|.
               ,\|/.
             ,' .V. `.
            / .     . \
           /_`       '_\
          ,' .:     ;, `.
          |@)|  . .  |(@|
     ,-._ `._';  .  :`_,' _,-.
    '--  `-\ /,-===-.\ /-'  --`
   (----  _|  ||___||  |_  ----)
    `._,-'  \ /`-.-' \ /  `-._,'
             /-.___,-'\     ap
          /\/          \/\
```

# Invariant
## Semantic Analysis
- When processing a function first add the function in current scope and open a new one =>> The function whose processing is being done is the last entry of the parent scope if such exists. Since things added in the symbol table are backwards it is the first in the list of entries in parent scope. 
- Scope nesting level of a function body is always > 1. Global scope is with nesting level 1.
- Non Nested Function Declarations can be defined externally by using the same name convention
# Tasks do not forget:
- Fix Symbol offset in symbol table. (Pointers are different size in LLVM)
- grep -rnw TODO to find todo in code.
- NULL

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
- Διπλού βάθους declaration list on functions
  ``` C
	void main(){
		int x, y;
	}
  ```
  └──Program <br />
    └──DeclarationList <br />
        └──FunctionDefinition(main) <br />
            ├──BasicType(void) <br />
            ├──DeclarationList <br />
            │   └──DeclarationList <br />
            │       ├──VariableDeclaration(x) <br />
            │       │   └──BasicType(int) <br />
            │       └──VariableDeclaration(y) <br />
            │           └──BasicType(int) <br />
            └──StatementList <br />
- TypeExpression::fromType handling of TYPE_ANY
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