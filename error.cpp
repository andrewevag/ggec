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

#include "general.hpp"
#include "error.hpp"


/* ---------------------------------------------------------------------
   --------- Υλοποίηση των συναρτήσεων του χειριστή σφαλμάτων ----------
   --------------------------------------------------------------------- */

extern int lineno;


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
      fprintf(stderr, "%s:%d: ", filename, lineno);
   fprintf(stderr, "Fatal error, ");
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
   exit(1);
}

void error (const char * fmt, ...)
{
   va_list ap;

   va_start(ap, fmt);
   if (fmt[0] == '\r')
      fmt++;
   else
      fprintf(stderr, "%s:%d: ", filename, lineno);
   fprintf(stderr, "Error, ");
   vfprintf(stderr, fmt, ap);
   fprintf(stderr, "\n");
   va_end(ap);
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
	fprintf(stderr, "Error in line: %d\n", lineno);
	fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}


ErrorInfo::ErrorInfo(){
   this->_lineappeared = lineno;
}

ErrorInfo::~ErrorInfo(){}

int ErrorInfo::getLineAppeared(){
   return this->_lineappeared;
}

void ErrorInfo::Fatal(ErrorInfo* where, std::string msg){   
   fprintf(stderr, "Error @%d : %s", where->getLineAppeared(), msg.c_str());
   exit(EXIT_FAILURE);
}
