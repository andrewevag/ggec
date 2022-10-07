# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language


# Tasks do not forget:
- #include
- Do not forget to take a look at dynamically allocated
  objects in the lexer
- https://stackoverflow.com/questions/62115979/how-to-implement-better-error-messages-for-flex-bison
# Known bugs

# Warnings to take into account
- SOS IN EXPRESSION COULD BE TOTAL EXPRESSION AND NOT NO_COMMA_EXPRESSION TRY IT IN C (Tried it works in C and switched it to take into account commas in parenthsis)
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

---
# DO NOT FORGET
- Να καθορίσουμε στο makefile ακριβως ποιες version χρησιμοποιούμε 
- τα paths στα .sh αρχεια είναι για το mac 