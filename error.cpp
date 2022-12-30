/*
 *  Written by    : Nikolaos S. Papaspyrou
 *  Modified by   : Nikoletta Barmpa, Andreas Evangelatos
 */

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include "general.hpp"
#include "error.hpp"

/**
 * Global variables for error handling
 * 
 */
extern int lineno;
extern int columnno;
extern std::set<std::string> fileset;
extern std::string currentFilename;

//====================================================================================//
// Functions For Error Handling                       								        //
//====================================================================================//

/**
 * @brief Open The file and get the line where an error appeared
 *
 */
std::string getLine(std::string file, int lineno, int columnno){
   std::ifstream inputfile;
   inputfile.open(file);
   for(int i =0 ; i < lineno-1; i++){
      inputfile.ignore(10000, '\n');
   }
   char lex[columnno+20];
   inputfile.getline(lex, columnno+20);
   inputfile.close();
   return std::string(lex);
}

/**
 * @brief Used only for the symbol table
 * 
 */
void internal (const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "%s:%d: ", filename, linecount);
   fprintf(stderr, "Internal error, ");
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   exit(1);
}

/**
 * @brief Static version of ErrorInfo::fatal. Implements panic error messaging.
 * Prints the column, line, and file where the error appeared.
 *
 */
void fatal (const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "Fatal Error @ \"%s\":%d:%d \n", currentFilename.c_str(), lineno, columnno);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   if (currentFilename != "stdin"){
      auto s = getLine(currentFilename, lineno, columnno);
      fprintf(stderr, "%s\n", s.c_str());
      for(int i = 0; i < columnno-1; i++){
         if(s[i] == '\t')
            fprintf(stderr, "_______");
         fprintf(stderr, "_");
      }
      fprintf(stderr, "^\n");   
   }
   va_end(ap);
   exit(1);
}

/*
 * Not used
 */
void error (const char * fmt, ...){}
/*
 * Not used
 */
void warning (const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "%s:%d: ", filename, linecount);
   fprintf(stderr, "Warning, ");
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
}

/**
 * @brief A Way for the parser to call fatal
 * 
 */
void yyerror(const char *msg)
{
   fatal(msg);
}

/**
 * @brief Construct a new Error Info:: Error Info object
 * Save the state for current line, column and filename.
 * 
 */
ErrorInfo::ErrorInfo(){
   this->_lineappeared = lineno;
   this->_columnappeared = columnno;
   this->_fileappeared = fileset.find(currentFilename);
}

ErrorInfo::~ErrorInfo(){}

int ErrorInfo::getLineAppeared(){
   return this->_lineappeared;
}

/**
 * @brief Main function to implement panic from Nodes in AST
 * 
 */
void ErrorInfo::fatal(const char * fmt, ...){   
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "Fatal Error @ \"%s\":%d:%d \n", this->_fileappeared->c_str(), this->_lineappeared, this->_columnappeared);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   if (*this->_fileappeared != "stdin"){
      auto s = getLine(*this->_fileappeared, this->_lineappeared, this->_columnappeared);
      fprintf(stderr, "%s\n", s.c_str());
      for(int i = 0; i < this->_columnappeared-1; i++){
         if(s[i] == '\t')
            fprintf(stderr, "_______");
         fprintf(stderr, "_");
      }
      fprintf(stderr, "^\n");
   }
   va_end(ap);
   exit(1);   
}

