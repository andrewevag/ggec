/******************************************************************************
 *  CVS version:
 *     $Id: error.c,v 1.2 2004/05/05 22:00:08 nickie Exp $
 ******************************************************************************
 *
 *  C code file : error.c
 *  Project     : PCL Compiler
 *  Version     : 1.0 alpha
 *  Written by  : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date        : May 14, 2003
 *  Description : Generic symbol table in C, simple error.hppandler
 *
 *  Comments: (in Greek iso-8859-7)
 *  ---------
 *  Εθνικό Μετσόβιο Πολυτεχνείο.
 *  Σχολή Ηλεκτρολόγων Μηχανικών και Μηχανικών Υπολογιστών.
 *  Τομέας Τεχνολογίας Πληροφορικής και Υπολογιστών.
 *  Εργαστήριο Τεχνολογίας Λογισμικού
 */


/* ---------------------------------------------------------------------
   ---------------------------- Header files ---------------------------
   --------------------------------------------------------------------- */

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include "general.hpp"
#include "error.hpp"


/* ---------------------------------------------------------------------
   --------- Υλοποίηση των συναρτήσεων του χειριστή σφαλμάτων ----------
   --------------------------------------------------------------------- */

extern int lineno;
extern int columnno;
extern std::set<std::string> fileset;
extern std::string currentFilename;

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
   auto s = getLine(currentFilename, lineno, columnno);
   printf("%s\n", s.c_str());
   for(int i = 0; i < columnno-1; i++){
		if(s[i] == '\t')
			std::cout << "_______";
		std::cout << "_";
   }
	std::cout << "^\n";   
   va_end(ap);
   exit(1);
}

void error (const char * fmt, ...)
{
   // va_list ap;

   // va_start(ap, fmt);
   // if (fmt[0] == '\r')
   //    fmt++;
   // else
   //    fprintf(stderr, "%s:%d: ", filename, lineno);
   // fprintf(stderr, "Error, ");
   // vfprintf(stderr, fmt, ap);
   // fprintf(stderr, "\n");
   // va_end(ap);
}

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

void yyerror(const char *msg)
{
   fatal(msg);
	// fprintf(stderr, "Error in line: %d\n", lineno);
	// fprintf(stderr, "%s\n", msg);
	// exit(EXIT_FAILURE);
}


ErrorInfo::ErrorInfo(){
   this->_lineappeared = lineno;
   this->_columnappeared = columnno;
   this->_fileappeared = fileset.find(currentFilename);
}

ErrorInfo::~ErrorInfo(){}

int ErrorInfo::getLineAppeared(){
   return this->_lineappeared;
}


void ErrorInfo::fatal(const char * fmt, ...){   
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "Fatal Error @ \"%s\":%d:%d \n", this->_fileappeared->c_str(), this->_lineappeared, this->_columnappeared);
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   auto s = getLine(*this->_fileappeared, this->_lineappeared, this->_columnappeared);
   printf("%s\n", s.c_str());
   for(int i = 0; i < this->_columnappeared-1; i++){
		if(s[i] == '\t')
			std::cout << "_______";
		std::cout << "_";
   }
	std::cout << "^\n";   
   va_end(ap);
   exit(1);   
}

