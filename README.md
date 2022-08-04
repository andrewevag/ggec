# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language


# Tasks do not forget:
- #include
- Do not forget to take a look at dynamically allocated
  objects in the lexer
# Known bugs
- New and Delete Bind stronger than postfix unass which is wrong
# Warnings to take into account
- parser.y: 227 see if it better to return an empty parameter list instead of nullptr
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