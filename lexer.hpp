#include <cstdint>


int yylex();

void yyerror(const char *msg);

uint8_t getCharFromString(char* s);