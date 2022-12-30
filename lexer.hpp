#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include <cstdint>
#include <string>

int yylex();

void yyerror(const char *msg);

uint8_t getCharFromString(const char* s);
std::string fixChars(std::string s);

#endif