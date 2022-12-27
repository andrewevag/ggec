/******************************************************************************
 *  CVS version:
 *     $Id: general.hpp,v 1.1 2004/05/05 22:00:08 nickie Exp $
 ******************************************************************************
 *
 *  C header file : general.hpp
 *  Project       : PCL Compiler
 *  Version       : 1.0 alpha
 *  Written by    : Nikolaos S. Papaspyrou (nickie@softlab.ntua.gr)
 *  Date          : May 5, 2004
 *  Description   : Generic symbol table in C, general.hppeader file
 *
 *  Comments: (in Greek iso-8859-7)
 *  ---------
 *  Εθνικό Μετσόβιο Πολυτεχνείο.
 *  Σχολή Ηλεκτρολόγων Μηχανικών και Μηχανικών Υπολογιστών.
 *  Τομέας Τεχνολογίας Πληροφορικής και Υπολογιστών.
 *  Εργαστήριο Τεχνολογίας Λογισμικού
 */


#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__
#include <string>
#include <set>
/* ---------------------------------------------------------------------
   -------------- Καθολικές μεταβλητές του μεταγλωττιστή ---------------
   --------------------------------------------------------------------- */

extern int linecount;
extern const char * filename;

extern bool optimize_flag;
extern bool _f_flag;
extern bool _i_flag;
extern std::set<std::string> fileset;
extern std::string currentFilename;
extern std::string interMediateOutFileName;
extern std::string assemblyOutFileName;

/**
 * @brief Prints the help message. Gets also called when errors in handle Arguments.
 */
void printHelpMessage();

/**
 * @brief Gets the program's arguments and sets accordingly the following variables of the global state :
 * optimize_flag           (-O),
 * _f_flag                 (-f),
 * _i_flag                 (-i),
 * fileset,
 * currentFilename         (*.eds | stdin),
 * interMediateOutFileName (*.imm | ""),
 * assemblyOutFileName     (*.asm | ""),
 * @param argc the program's argument count
 * @param argv the program's arguments
 */
void handleArguments(int argc, char** argv);

/**
 * @brief Handles output of the code in intermediate or assembly format
 * according to the flags set by handleArguments
 */
void handleOutput();

#endif
