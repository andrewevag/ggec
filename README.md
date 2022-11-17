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
- #include
- https://stackoverflow.com/questions/62115979/how-to-implement-better-error-messages-for-flex-bison
  - global state to keep line and column
  - have each syntactic node keep track of the state where we found it
  - everyone call call error-abort by giving a state.
- Split sem() into parts of before and after codegen..  
- Fix Symbol offset in symbol table.
- grep -rnw TODO to find todo in code.
- Types in TypedExpression Objects need to be explicitely deeply destroyed.
- Memory Cleaning
# Known bugs

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
Note:
We have the declaration of Functions and each line
ex. int x, y; is it's own declaration List.

---
# DO NOT FORGET
- Να καθορίσουμε στο makefile ακριβως ποιες version χρησιμοποιούμε 
- τα paths στα .sh αρχεια είναι για το mac 