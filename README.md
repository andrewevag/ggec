# Gospel's Goatfish Edsger Compiler
A Compiler for the Edsger Programming Language


# Tasks do not forget:
- #include
- Do not forget to take a look at dynamically allocated
  objects in the lexer
- https://stackoverflow.com/questions/62115979/how-to-implement-better-error-messages-for-flex-bison
# Known bugs
- Comma expressions bind wrong!! (FIXED IT NO_COMMA_EXPRESSIONS IN STRONGER BOUNDING EXPRESSIONS)
# Warnings to take into account
- SOS IN EXPRESSION COULD BE TOTAL EXPRESSION AND NOT NO_COMMA_EXPRESSION TRY IT IN C
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