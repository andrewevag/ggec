# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language


# Tasks do not forget:
- #include

# Known bugs

# Warnings to take into account
- parser.y: 227 see if it better to return an empty parameter list instead of nullptr
- Διπλού βάθους declaration list on functions
  ``` C
	void main(){
		int x, y;
	}
  ```
  └──Program
    └──DeclarationList
        └──FunctionDefinition(main)
            ├──BasicType(void)
            ├──DeclarationList
            │   └──DeclarationList
            │       ├──VariableDeclaration(x)
            │       │   └──BasicType(int)
            │       └──VariableDeclaration(y)
            │           └──BasicType(int)
            └──StatementList



# Destructors under condition at:
-new
-delete
-for
-label


# We should check ifs and ternary conditions at nullptr