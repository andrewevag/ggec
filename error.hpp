/*
 *  Written by    : Nikolaos S. Papaspyrou
 *  Modified by   : Nikoletta Barmpa, Andreas Evangelatos
 */

#ifndef __ERROR_HPP__
#define __ERROR_HPP__
#include <string>
#include <set>

//====================================================================================//
// Prototyped For Error Handling                       								        //
//====================================================================================//

void internal (const char * fmt, ...);
void fatal    (const char * fmt, ...);
void error    (const char * fmt, ...);
void warning  (const char * fmt, ...);

//====================================================================================//
// Info Carrying Class for Error Messaginh                  								  //
//====================================================================================//


class ErrorInfo {
public:
   // this is inhereted by all AST classes and carry the line and column where each node appeared
   // it also a pointer in the fileSet to indicate the file where it appeared.
   ErrorInfo();
   ~ErrorInfo();
   int getLineAppeared();
   // fatal version for AST nodes to also print line, column and the file of the error
   void fatal(const char * fmt, ...);
private:
   int _lineappeared;
   int _columnappeared;
   std::set<std::string>::iterator _fileappeared;
};

#endif
