# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language

# Tasks do not forget:
- #include
- https://stackoverflow.com/questions/62115979/how-to-implement-better-error-messages-for-flex-bison
  - global state to keep line and column
  - have each syntactic node keep track of the state where we found it
  - everyone call call error-abort by giving a state.
- function overloading
- deep equality on types
- destructors on types and symbols
- to implement overloading -> lookup with type in SymbolTable (that is why equality of types is needed) 

# Known bugs

# Assumptions

## Function Overloading
How it works:
1. Functions with the same name and different parameter list can exist
   1. Difference in parameter lists is defined only with different types and not with different pass specifications
2. Nested Functions can be redefined with the same parameter list as a Non-nested one, being the active one from down under (in statical scope).
3. Overloading by return type is not allowed.

3. Thus defining the name_type1_parameter1_type2_parameter2_... is enough.


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

Note:
We have the declaration of Functions and each line
ex. int x, y; is it's own declaration List.

---
# DO NOT FORGET
- Να καθορίσουμε στο makefile ακριβως ποιες version χρησιμοποιούμε 
- τα paths στα .sh αρχεια είναι για το mac 
