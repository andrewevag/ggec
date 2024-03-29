/*
 *  Written by    : Nikolaos S. Papaspyrou
 *  Modified by   : Nikoletta Barmpa, Andreas Evangelatos
 */


#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <vector>
#include <utility>
#include <string>
#include "llvmhead.hpp"
#include <stdbool.h>


/* ---------------------------------------------------------------------
   ------------ Ορισμός σταθερών του πίνακα συμβόλων -------------------
   --------------------------------------------------------------------- */

#define START_POSITIVE_OFFSET 8     /* Αρχικό θετικό offset στο Ε.Δ.   */
#define START_NEGATIVE_OFFSET 0     /* Αρχικό αρνητικό offset στο Ε.Δ. */

#define GLOBAL_SCOPE          1     /* Nesting Level of Global Scope */


/* ---------------------------------------------------------------------
   --------------- Ορισμός τύπων του πίνακα συμβόλων -------------------
   --------------------------------------------------------------------- */

/* Τύποι δεδομένων για την υλοποίηση των σταθερών */

typedef int           RepInteger;         /* Ακέραιες                  */
typedef unsigned char RepBoolean;         /* Λογικές τιμές             */
typedef char          RepChar;            /* Χαρακτήρες                */
typedef long double   RepReal;            /* Πραγματικές               */
typedef const char *  RepString;          /* Συμβολοσειρές             */


/* Τύποι δεδομένων και αποτελέσματος συναρτήσεων */

typedef struct Type_tag * Type;

struct Type_tag {
    enum {                               /***** Το είδος του τύπου ****/
       TYPE_VOID,                        /* Κενός τύπος αποτελέσματος */
       TYPE_INTEGER,                     /* Ακέραιοι                  */
       TYPE_BOOLEAN,                     /* Λογικές τιμές             */
       TYPE_CHAR,                        /* Χαρακτήρες                */
       TYPE_REAL,                        /* Πραγματικοί               */
       TYPE_ARRAY,                       /* Πίνακες γνωστού μεγέθους  */
       TYPE_IARRAY,                      /* Πίνακες άγνωστου μεγέθους */
       TYPE_POINTER,                     /* Δείκτες                   */
       TYPE_ANY                          /* Τύπος για όλα (NULL*)     */             
                                         /* Ίσο με όλα                */
    } kind;
    Type           refType;              /* Τύπος αναφοράς            */
    RepInteger     size;                 /* Μέγεθος, αν είναι πίνακας */
    unsigned int   refCount;             /* Μετρητής αναφορών         */
};


/* Τύποι εγγραφών του πίνακα συμβόλων */

typedef enum {            
   ENTRY_VARIABLE,                       /* Μεταβλητές                 */
   ENTRY_CONSTANT,                       /* Σταθερές                   */
   ENTRY_FUNCTION,                       /* Συναρτήσεις                */
   ENTRY_PARAMETER,                      /* Παράμετροι συναρτήσεων     */
   ENTRY_TEMPORARY,                      /* Προσωρινές μεταβλητές      */
   ENTRY_LABEL                           /* Ετικέτες για for-s         */
} EntryType;


/* Τύποι περάσματος παραμετρων */

typedef enum {            
   PASS_BY_VALUE,                        /* Κατ' αξία                  */
   PASS_BY_REFERENCE                     /* Κατ' αναφορά               */
} PassMode;

typedef enum {                         /* Κατάσταση παραμέτρων  */
   PARDEF_COMPLETE,                    /* Πλήρης ορισμός     */
   PARDEF_DEFINE,                      /* Εν μέσω ορισμού    */
   PARDEF_CHECK                        /* Εν μέσω ελέγχου    */
} PARDEF;

/* Τύπος εγγραφής στον πίνακα συμβόλων */

typedef struct SymbolEntry_tag SymbolEntry;

struct SymbolEntry_tag {
   const char   * id;                 /* Ονομα αναγνωριστικού          */
   EntryType      entryType;          /* Τύπος της εγγραφής            */
   unsigned int   nestingLevel;       /* Βάθος φωλιάσματος             */
   unsigned int   hashValue;          /* Τιμή κατακερματισμού          */
   SymbolEntry  * nextHash;           /* Επόμενη εγγραφή στον Π.Κ.     */
   SymbolEntry  * nextInScope;        /* Επόμενη εγγραφή στην εμβέλεια */

   union {                            /* Ανάλογα με τον τύπο εγγραφής: */

      struct {                                /******* Μεταβλητή *******/
         Type          type;                  /* Τύπος                 */
         int           offset;                /* Offset στο Ε.Δ.       */
         llvm::Value * llvmVal;               /* Null if not global    */  
      } eVariable;

      struct {                                /******** Σταθερά ********/
         Type          type;                  /* Τύπος                 */
         union {                              /* Τιμή                  */
            RepInteger vInteger;              /*    ακέραια            */
            RepBoolean vBoolean;              /*    λογική             */
            RepChar    vChar;                 /*    χαρακτήρας         */
            RepReal    vReal;                 /*    πραγματική         */
            RepString  vString;               /*    συμβολοσειρά       */
         } value;
      } eConstant;

      struct {                                /******* Συνάρτηση *******/
         bool            isForward;           /* Δήλωση forward        */
         SymbolEntry   * firstArgument;       /* Λίστα παραμέτρων      */
         SymbolEntry   * lastArgument;        /* Τελευταία παράμετρος  */
         Type            resultType;          /* Τύπος αποτελέσματος   */
         PARDEF          pardef;              /* Κατάσταση παραμέτρων  */
         
         llvm::Value   * env;                 /* Current Environament  */
         llvm::Function* fun;                 /* Handle for the func   */
         bool            hasHead;             /* If the head for the   */
                                              /* is generated or not   */
         int             firstQuad;           /* Αρχική τετράδα        */
      } eFunction;

      struct {                                /****** Παράμετρος *******/
         Type          type;                  /* Τύπος                 */
         int           offset;                /* Offset στο Ε.Δ.       */
         PassMode      mode;                  /* Τρόπος περάσματος     */
         SymbolEntry * next;                  /* Επόμενη παράμετρος    */
      } eParameter;

      struct {                                /** Προσωρινή μεταβλητή **/
         Type          type;                  /* Τύπος                 */
         int           offset;                /* Offset στο Ε.Δ.       */
         int           number;
      } eTemporary;

      struct {                                /******* Ετικέτα *********/
         bool         explicitelyNamed;       /* Όνομα απο χρήστη ή οχι*/
         bool         active;                 /* in loop or not        */
         size_t       unnamedConst;           /* Int used to generate  */
         llvm::BasicBlock * entry;            
         llvm::BasicBlock * exit;             /* Used for Break */
         llvm::BasicBlock * loopAfter;        /* Used for Continue */
                                              /* its name              */
      } eLabel;
   } u;                               /* Τέλος του union               */
};


/* Τύπος συνόλου εγγραφών που βρίσκονται στην ίδια εμβέλεια */

typedef struct Scope_tag Scope;

struct Scope_tag {
    unsigned int   nestingLevel;             /* Βάθος φωλιάσματος      */
    unsigned int   negOffset;                /* Τρέχον αρνητικό offset */
    unsigned int   varOffset;                /* Για τις μεταβλητές & παραμέτρους */
    Scope        * parent;                   /* Περιβάλλουσα εμβέλεια  */
    SymbolEntry  * entries;                  /* Σύμβολα της εμβέλειας  */
};


/* Τύπος αναζήτησης στον πίνακα συμβόλων */


typedef enum {
    LOOKUP_CURRENT_SCOPE,
    LOOKUP_ALL_SCOPES
} LookupType;


/* ---------------------------------------------------------------------
   ------------- Καθολικές μεταβλητές του πίνακα συμβόλων --------------
   --------------------------------------------------------------------- */

extern Scope        * currentScope;            /* Τρέχουσα εμβέλεια         */
extern unsigned int   quadNext;                /* Αριθμός επόμενης τετράδας */
extern unsigned int   tempNumber;              /* Αρίθμηση των temporaries  */
extern std::vector<SymbolEntry*> lastDefFuns;  /* function we're processing */

extern const Type typeVoid;
extern const Type typeInteger;
extern const Type typeBoolean;
extern const Type typeChar;
extern const Type typeReal;
extern const Type typeAny;


/**
 * @brief Transform semantic type to llvm type
 *
 */
llvm::Type* toLLVMType(Type t);

/**
 * @brief Class for carrying type information for nodes in the AST.
 * (Also if it is an lval or not)
 * 
 */
class TypedExpression {
public:
   TypedExpression(){
      this->_t = NULL;
      this->_isLval = false;
   }
   virtual ~TypedExpression();

   Type getType() { return this->_t; }
   
   bool isLval() { return this->_isLval; }
   
   bool isPtrType() {
      if (this->_t != NULL){
         if(this->_t->kind == Type_tag::TYPE_POINTER)
            return true;
         else if(this->_t->kind == Type_tag::TYPE_ARRAY)
            return true;
      }
      return false;
   }
   
   static Type copyType(Type _t){
      if( _t->kind == Type_tag::TYPE_ARRAY ||
          _t->kind == Type_tag::TYPE_IARRAY ||
          _t->kind == Type_tag::TYPE_POINTER
       )
       {
         Type t = new Type_tag;
         t->kind = _t->kind;
         t->refType = copyType(_t->refType);
         return t;
       }
       else{
         Type t = new Type_tag;
         t->kind = _t->kind;
         t->refType = NULL;
         return t;
       }
   }
   
protected:
   Type _t;
   bool _isLval;
};


/* ---------------------------------------------------------------------
   ------ Πρωτότυπα των συναρτήσεων χειρισμού του πίνακα συμβολών ------
   --------------------------------------------------------------------- */

void          initSymbolTable    (unsigned int size);
void          destroySymbolTable (void);

void          openScope          (void);
void          closeScope         (void);

SymbolEntry * newVariable        (const char * name, Type type);
SymbolEntry * newConstant        (const char * name, Type type, ...);
SymbolEntry * newFunction        (const char * name);
SymbolEntry * newParameter       (const char * name, Type type,
                                  PassMode mode, SymbolEntry * f);
SymbolEntry * newTemporary       (Type type);
SymbolEntry * newLabel           (const char * name);

std::pair<SymbolEntry*, std::string>
              newUnnamedLabel    ();

void          forwardFunction    (SymbolEntry * f);
void          endFunctionHeader  (SymbolEntry * f, Type type);
void          destroyEntry       (SymbolEntry * e);
SymbolEntry * lookupEntry        (const char * name, LookupType type,
                                  bool err);
SymbolEntry * lookupLabel        (const char * name, bool explicitelyNamed);
SymbolEntry * lookupActiveFun    (); 

Type          typeArray          (RepInteger size, Type refType);
Type          typeIArray         (Type refType);
Type          typePointer        (Type refType);
void          destroyType        (Type type);
unsigned int  sizeOfType         (Type type);
bool          equalType          (Type type1, Type type2);
void          printType          (Type type);
void          printMode          (PassMode mode);
void          printSymbolTable   ();

#endif
