/*
 *  Written by    : Nikolaos S. Papaspyrou
 *  Modified by   : Nikoletta Barmpa, Andreas Evangelatos
 */

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <sstream>

#include "general.hpp"
#include "error.hpp"
#include "symbol.hpp"
#include "ast.hpp"


/* ---------------------------------------------------------------------
   ------------- Καθολικές μεταβλητές του πίνακα συμβόλων --------------
   --------------------------------------------------------------------- */

Scope        * currentScope;           /* Τρέχουσα εμβέλεια              */
unsigned int   quadNext;               /* Αριθμός επόμενης τετράδας      */
unsigned int   tempNumber;             /* Αρίθμηση των temporaries       */

std::vector<SymbolEntry*> lastDefFuns;     

static unsigned int   hashTableSize;   /* Μέγεθος πίνακα κατακερματισμού */
static SymbolEntry ** hashTable;       /* Πίνακας κατακερματισμού        */

static struct Type_tag typeConst [] = {
    { Type_tag::TYPE_VOID,    NULL, 0, 0 },
    { Type_tag::TYPE_INTEGER, NULL, 0, 0 },
    { Type_tag::TYPE_BOOLEAN, NULL, 0, 0 },
    { Type_tag::TYPE_CHAR,    NULL, 0, 0 },
    { Type_tag::TYPE_REAL,    NULL, 0, 0 },
    { Type_tag::TYPE_ANY,     NULL, 0, 0 }
};

const Type typeVoid    = &(typeConst[0]);
const Type typeInteger = &(typeConst[1]);
const Type typeBoolean = &(typeConst[2]);
const Type typeChar    = &(typeConst[3]);
const Type typeReal    = &(typeConst[4]);
const Type typeAny     = &(typeConst[5]); // No Longer Used
size_t labelNamingInt = 0;


/* ---------------------------------------------------------------------
   ------- Υλοποίηση βοηθητικών συναρτήσεων του πίνακα συμβόλων --------
   --------------------------------------------------------------------- */

typedef unsigned long int HashType;

static HashType PJW_hash (const char * key)
{
    /*
     *  P.J. Weinberger's hashing function. See also:
     *  Aho A.V., Sethi R. & Ullman J.D, "Compilers: Principles,
     *  Techniques and Tools", Addison Wesley, 1986, pp. 433-437.
     */

    const HashType PJW_OVERFLOW =
        (((HashType) 0xf) << (8 * sizeof(HashType) - 4));
    const int PJW_SHIFT = (8 * (sizeof(HashType) - 1));
    
    HashType h, g;
    
    for (h = 0; *key != '\0'; key++) {
        h = (h << 4) + (*key);
        if ((g = h & PJW_OVERFLOW) != 0) {
            h ^= g >> PJW_SHIFT;
            h ^= g;
        }
    }
    return h;
}

void strAppendChar (char * buffer, RepChar c)
{
    switch (c) {
        case '\n':
            strcat(buffer, "\\n");
            break;
        case '\t':
            strcat(buffer, "\\t");
            break;
        case '\r':
            strcat(buffer, "\\r");
            break;
        case '\0':
            strcat(buffer, "\\0");
            break;
        case '\\':
            strcat(buffer, "\\\\");
            break;
        case '\'':
            strcat(buffer, "\\'");
            break;
        case '\"':
            strcat(buffer, "\\\"");
            break;
        default: {
            char s[] = { '\0', '\0' };
            
            *s = c;
            strcat(buffer, s);
        }
    }
}

void strAppendString (char * buffer, RepString str)
{
    const char * s;
    
    for (s = str; *s != '\0'; s++)
        strAppendChar(buffer, *s);
}


/* ---------------------------------------------------------------------
   ------ Υλοποίηση των συναρτήσεων χειρισμού του πίνακα συμβόλων ------
   --------------------------------------------------------------------- */

void initSymbolTable (unsigned int size)
{
    unsigned int i;
    
    /* Διάφορες αρχικοποιήσεις */
    
    currentScope = NULL;
    quadNext     = 1;
    tempNumber   = 1;
    
    /* Αρχικοποίηση του πίνακα κατακερματισμού */
    
    hashTableSize = size;
    hashTable = (SymbolEntry **) new SymbolEntry*[size];
    
    for (i = 0; i < size; i++)
        hashTable[i] = NULL;
}

void destroySymbolTable ()
{
    unsigned int i;
    
    /* Καταστροφή του πίνακα κατακερματισμού */
    
    for (i = 0; i < hashTableSize; i++)
        if (hashTable[i] != NULL)
            destroyEntry(hashTable[i]);

    delete [] hashTable;
}

void openScope ()
{
    Scope * newScope = (Scope *) new Scope; 

    newScope->negOffset = START_NEGATIVE_OFFSET;
    newScope->parent    = currentScope;
    newScope->entries   = NULL;
    newScope->varOffset = START_POSITIVE_OFFSET;

    if (currentScope == NULL)
        newScope->nestingLevel = 1;
    else
        newScope->nestingLevel = currentScope->nestingLevel + 1;
    
    currentScope = newScope;
}

void closeScope ()
{
    SymbolEntry * e = currentScope->entries;
    Scope       * t = currentScope;
    
    while (e != NULL) {
        SymbolEntry * next = e->nextInScope;
        
        hashTable[e->hashValue] = e->nextHash;
        destroyEntry(e);
        e = next;
    }
    
    currentScope = currentScope->parent;
    if(lastDefFuns.size() != 0)
        lastDefFuns.pop_back();
    delete(t);
}

static void insertEntry (SymbolEntry * e)
{
    e->nextHash             = hashTable[e->hashValue];
    hashTable[e->hashValue] = e;
    e->nextInScope          = currentScope->entries;
    currentScope->entries   = e;
}

static SymbolEntry * newEntry (const char * name)
{
    SymbolEntry * e;
    
    /* Έλεγχος αν υπάρχει ήδη */
    
    for (e = currentScope->entries; e != NULL; e = e->nextInScope)
        if (strcmp(name, e->id) == 0) {
            error("Duplicate identifier: %s", name);
            return NULL;
        }

    /* Αρχικοποίηση όλων εκτός: entryType και u */

    e = (SymbolEntry *) new SymbolEntry; 
    e->id = (const char *) new char[strlen(name) + 1];

    strcpy((char *) (e->id), name);
    e->hashValue    = PJW_hash(name) % hashTableSize;
    e->nestingLevel = currentScope->nestingLevel;
    insertEntry(e);
    return e;
}

SymbolEntry * newVariable (const char * name, Type type)
{
    SymbolEntry * e = newEntry(name);
    
    if (e != NULL) {
        e->entryType = ENTRY_VARIABLE;
        e->u.eVariable.type = type;
        type->refCount++;
        currentScope->negOffset -= sizeOfType(type);
        e->u.eVariable.offset = currentScope->varOffset;
        currentScope->varOffset += sizeOfType(type);
    }
    return e;
}

SymbolEntry * newConstant (const char * name, Type type, ...)
{
    SymbolEntry * e;
    va_list ap;

    union {
        RepInteger vInteger;
        RepBoolean vBoolean;
        RepChar    vChar;
        RepReal    vReal;
        RepString  vString;
    } value;
         
    va_start(ap, type);
    switch (type->kind) {
        case Type_tag::TYPE_INTEGER:
            value.vInteger = va_arg(ap, RepInteger);
            break;
        case Type_tag::TYPE_BOOLEAN:
            value.vBoolean = va_arg(ap, int);     /* RepBool is promoted */
            break;
        case Type_tag::TYPE_CHAR:
            value.vChar = va_arg(ap, int);        /* RepChar is promoted */
            break;
        case Type_tag::TYPE_REAL:
            value.vReal = va_arg(ap, RepReal);
            break;
        case Type_tag::TYPE_ARRAY:
            if (equalType(type->refType, typeChar)) {
                RepString str = va_arg(ap, RepString);
                
                value.vString = (const char *) new char[strlen(str) + 1];
                strcpy((char *) (value.vString), str);
                break;
            }
        default:
            internal("Invalid type for constant");
    }
    va_end(ap);

    if (name == NULL) {
        char buffer[256];
        
        switch (type->kind) {
            case Type_tag::TYPE_INTEGER:
                sprintf(buffer, "%d", value.vInteger);
                break;
            case Type_tag::TYPE_BOOLEAN:
                if (value.vBoolean)
                    sprintf(buffer, "true");
                else
                    sprintf(buffer, "false");
                break;
            case Type_tag::TYPE_CHAR:
                strcpy(buffer, "'");
                strAppendChar(buffer, value.vChar);
                strcat(buffer, "'");
                break;
            case Type_tag::TYPE_REAL:
                sprintf(buffer, "%Lg", value.vReal);
                break;
            case Type_tag::TYPE_ARRAY:
                strcpy(buffer, "\"");
                strAppendString(buffer, value.vString);
                strcat(buffer, "\"");
                break;
            default:
                fprintf(stderr, "No such type of constant!!");
                break;     
        }
        e = newEntry(buffer);
    }
    else
        e = newEntry(name);
    
    if (e != NULL) {
        e->entryType = ENTRY_CONSTANT;
        e->u.eConstant.type = type;
        type->refCount++;
        switch (type->kind) {
            case Type_tag::TYPE_INTEGER:
                e->u.eConstant.value.vInteger = value.vInteger;
                break;
            case Type_tag::TYPE_BOOLEAN:
                e->u.eConstant.value.vBoolean = value.vBoolean;
                break;
            case Type_tag::TYPE_CHAR:
                e->u.eConstant.value.vChar = value.vChar;
                break;
            case Type_tag::TYPE_REAL:
                e->u.eConstant.value.vReal = value.vReal;
                break;
            case Type_tag::TYPE_ARRAY:
                e->u.eConstant.value.vString = value.vString;
                break;
            default:
                break;
        }
    }
    return e;
}

SymbolEntry * newFunction (const char * name)
{
    SymbolEntry * e = lookupEntry(name, LOOKUP_CURRENT_SCOPE, false);

    if (e == NULL) {
        e = newEntry(name);
        if (e != NULL) {
            e->entryType = ENTRY_FUNCTION;
            e->u.eFunction.isForward = false;
            e->u.eFunction.pardef = PARDEF_DEFINE;
            e->u.eFunction.firstArgument = e->u.eFunction.lastArgument = NULL;
            e->u.eFunction.resultType = NULL;
            e->u.eFunction.hasHead = false;
            lastDefFuns.push_back(e);
        }
        return e;
    }
    else if (e->entryType == ENTRY_FUNCTION && e->u.eFunction.isForward) {
        e->u.eFunction.isForward = false;
        e->u.eFunction.pardef = PARDEF_CHECK;
        e->u.eFunction.lastArgument = NULL;
        lastDefFuns.push_back(e);
        return e;
    }
    else {
       error("Duplicate identifier: %s", name);
       return NULL;
    }
}

SymbolEntry * newParameter (const char * name, Type type,
                            PassMode mode, SymbolEntry * f)
{
    SymbolEntry * e;
    
    if (f->entryType != ENTRY_FUNCTION)
        internal("Cannot add a parameter to a non-function");
    switch (f->u.eFunction.pardef) {
        case PARDEF_DEFINE:
            e = newEntry(name);
            if (e != NULL) {
                e->entryType = ENTRY_PARAMETER;
                e->u.eParameter.type = type;
                
                type->refCount++;
                e->u.eParameter.mode = mode;
                e->u.eParameter.next = NULL;

                e->u.eParameter.offset = currentScope->varOffset;
                // TODO 
                if(e->u.eParameter.mode == PASS_BY_REFERENCE){
                    auto tt = typePointer(type);
                    currentScope->varOffset += sizeOfType(tt);
                    destroyType(tt);    
                }else
                    currentScope->varOffset += sizeOfType(type);
                

            }
            if (f->u.eFunction.lastArgument == NULL)
                f->u.eFunction.firstArgument = f->u.eFunction.lastArgument = e;
            else {
                f->u.eFunction.lastArgument->u.eParameter.next = e;
                f->u.eFunction.lastArgument = e;
            }
            return e;            
        case PARDEF_CHECK:
            e = f->u.eFunction.lastArgument;
            if (e == NULL)
                e = f->u.eFunction.firstArgument;
            else
                e = e->u.eParameter.next;
            if (e == NULL)
                error("More parameters than expected in redeclaration "
                      "of function %s", f->id);
            else if (!equalType(e->u.eParameter.type, type))
                error("Parameter type mismatch in redeclaration "
                      "of function %s", f->id);
            else if (e->u.eParameter.mode != mode)
                error("Parameter passing mode mismatch in redeclaration "
                      "of function %s", f->id);
            else if (strcmp(e->id, name) != 0)
                error("Parameter name mismatch in redeclaration "
                      "of function %s", f->id);
            else
                insertEntry(e);
            f->u.eFunction.lastArgument = e;
            return e;
        case PARDEF_COMPLETE:
            fatal("Cannot add a parameter to an already defined function");
    }
    return NULL;
}

void forwardFunction (SymbolEntry * f)
{
    if (f->entryType != ENTRY_FUNCTION)
        internal("Cannot make a non-function forward");
    f->u.eFunction.isForward = true;
    // lastDefFuns.pop_back(); getPopped in closeScope();
}

void endFunctionHeader (SymbolEntry * f, Type type)
{
    if (f->entryType != ENTRY_FUNCTION)
        internal("Cannot end parameters in a non-function");
    switch (f->u.eFunction.pardef) {
        case PARDEF_COMPLETE:
            internal("Cannot end parameters in an already defined function");
            break;
        case PARDEF_DEFINE:
            // currentScope->varOffset+=fixOffset(f->u.eFunction.firstArgument);
            f->u.eFunction.resultType = type;
            type->refCount++;
            break;
        case PARDEF_CHECK:
            if ((f->u.eFunction.lastArgument != NULL &&
                 f->u.eFunction.lastArgument->u.eParameter.next != NULL) ||
                (f->u.eFunction.lastArgument == NULL &&
                 f->u.eFunction.firstArgument != NULL))
                error("Fewer parameters than expected in redeclaration "
                      "of function %s", f->id);
            if (!equalType(f->u.eFunction.resultType, type))
                error("Result type mismatch in redeclaration of function %s",
                      f->id);
            break;
    }
    f->u.eFunction.pardef = PARDEF_COMPLETE;
}

SymbolEntry * newTemporary (Type type)
{
    char buffer[10];
    SymbolEntry * e;

    sprintf(buffer, "$%d", tempNumber);
    e = newEntry(buffer);
    
    if (e != NULL) {
        e->entryType = ENTRY_TEMPORARY;
        e->u.eVariable.type = type;
        type->refCount++;
        currentScope->negOffset -= sizeOfType(type);
        e->u.eTemporary.offset = currentScope->negOffset;
        e->u.eTemporary.number = tempNumber++;
    }
    return e;
}

//====================================================================================//
// Function For Adding Label in The Symbol Table                                      //
//====================================================================================//
SymbolEntry * newLabel (const char * name)
{
    SymbolEntry* e;

    e = newEntry(name);
    if(e != NULL){
        e->entryType = ENTRY_LABEL;
        e->u.eLabel = {};
        e->u.eLabel.active = true;
        e->u.eLabel.explicitelyNamed = true;
        e->u.eLabel.unnamedConst = ++labelNamingInt;
        e->u.eLabel.entry = nullptr;
        e->u.eLabel.exit = nullptr;
    }
    return e;
}

std::pair<SymbolEntry*, std::string> newUnnamedLabel ()
{
    SymbolEntry* e;
    std::ostringstream ss;
    ss << "@for" << ++labelNamingInt;
    e = newEntry(ss.str().c_str());
    if(e != NULL){
        e->entryType = ENTRY_LABEL;
        e->u.eLabel = {};
        e->u.eLabel.active = true;
        e->u.eLabel.explicitelyNamed = false;
        e->u.eLabel.unnamedConst = labelNamingInt;
        e->u.eLabel.entry = nullptr;
        e->u.eLabel.exit = nullptr;
    }
    return std::make_pair<>(e, ss.str());
}

//====================================================================================//
//====================================================================================//


void destroyEntry (SymbolEntry * e)
{
    SymbolEntry * args;
    
    switch (e->entryType) {
        case ENTRY_VARIABLE:
            destroyType(e->u.eVariable.type);
            break;
        case ENTRY_CONSTANT:
            if (e->u.eConstant.type->kind == Type_tag::TYPE_ARRAY)
                delete[] (char *) (e->u.eConstant.value.vString);
            destroyType(e->u.eConstant.type);
            break;
        case ENTRY_FUNCTION:
            args = e->u.eFunction.firstArgument;
            while (args != NULL) {
                SymbolEntry * p = args;
                
                destroyType(args->u.eParameter.type);
                delete[] (char *) (args->id);
                args = args->u.eParameter.next;
                delete p;
            }
            destroyType(e->u.eFunction.resultType);
            break;
        case ENTRY_PARAMETER:
            /* Οι παράμετροι καταστρέφονται μαζί με τη συνάρτηση */
            return;
        case ENTRY_TEMPORARY:
            destroyType(e->u.eTemporary.type);
            break;
        default:
            break;
    }
    delete[] (char *)(e->id);
    delete e;        
}

SymbolEntry * lookupEntry (const char * name, LookupType type, bool err)
{
    unsigned int  hashValue = PJW_hash(name) % hashTableSize;
    SymbolEntry * e         = hashTable[hashValue];
    
    switch (type) {
        case LOOKUP_CURRENT_SCOPE:
            while (e != NULL && e->nestingLevel == currentScope->nestingLevel)
                if (strcmp(e->id, name) == 0 && e->entryType != ENTRY_LABEL)
                    return e;
                else
                    e = e->nextHash;
            break;
        case LOOKUP_ALL_SCOPES:
            while (e != NULL)
                if (strcmp(e->id, name) == 0 && e->entryType != ENTRY_LABEL)
                    return e;
                else
                    e = e->nextHash;
            break;
    }
    
    if (err)
        error("Unknown identifier: %s", name);
    return NULL;
}

//====================================================================================//
// Function For Looking Up Labels in the symbol Table                                 //
//====================================================================================//

SymbolEntry * lookupLabel(const char * name, bool explicitelyNamed)
{
    unsigned int  hashValue;
    SymbolEntry * e;
    size_t        maxTilNow = 0;
    if(explicitelyNamed)
    {
        hashValue = PJW_hash(name) % hashTableSize;
        e = hashTable[hashValue];
        while(e != NULL && e->nestingLevel == currentScope->nestingLevel){
            if (strcmp(e->id, name) == 0 && e->entryType == ENTRY_LABEL){
                return e;
            }
        }
        return NULL;
    }
    else{
        e = NULL;
        // Label with max level is what opened last..
        for(SymbolEntry * i = currentScope->entries; i != NULL; i = i->nextInScope)
        {
            if (i->entryType == ENTRY_LABEL){
                if(i->u.eLabel.unnamedConst > maxTilNow){
                    e = i;
                    maxTilNow = i->u.eLabel.unnamedConst;
                }
            }
        }
        return e;
    }
    
}
/**
 * Query The Stack of Functions for the last active one
 * 
 */
SymbolEntry * lookupActiveFun    ()
{
    if(lastDefFuns.size() == 0)
        return NULL;
    else return lastDefFuns.back();
}

//====================================================================================//
// Semantic Type Operations                                                           //
//====================================================================================//


Type typeArray (RepInteger size, Type refType)
{
    Type n = (Type) new Type_tag;

    n->kind     = Type_tag::TYPE_ARRAY;
    n->refType  = refType;
    n->size     = size;
    n->refCount = 1;
    
    refType->refCount++;

    return n;
}

Type typeIArray (Type refType)
{
    Type n = (Type) new Type_tag;

    n->kind     = Type_tag::TYPE_IARRAY;
    n->refType  = refType;
    n->refCount = 1;
    
    refType->refCount++;

    return n;
}

Type typePointer (Type refType)
{
    Type n = (Type) new Type_tag;

    n->kind     = Type_tag::TYPE_POINTER;
    n->refType  = refType;
    n->refCount = 1;
    
    refType->refCount++;

    return n;
}

void destroyType (Type type)
{
    switch (type->kind) {
        case Type_tag::TYPE_ARRAY:
        case Type_tag::TYPE_IARRAY:
        case Type_tag::TYPE_POINTER:
            if (--(type->refCount) == 0) {
                destroyType(type->refType);
                delete type;
            }
            break;
        default: break;
    }
}

unsigned int sizeOfType (Type type)
{
    // Get the size of the type for DataLayout to handle alignment.
    if(type->kind == Type_tag::TYPE_ARRAY){
        auto size = type->size * sizeOfType(type->refType);
        return size;
    }else{       
        auto size = AST::TheModule->getDataLayout().getTypeAllocSize(toLLVMType(type));
        return size;
    }

    
    // switch (type->kind) {
    //     case Type_tag::TYPE_VOID:
    //         internal("Type void has no size");
    //         break;
    //     case Type_tag::TYPE_INTEGER:
    //         return 2;
    //     case Type_tag::TYPE_IARRAY:
    //     case Type_tag::TYPE_POINTER:
    //         return 2;
    //     case Type_tag::TYPE_BOOLEAN:
    //     case Type_tag::TYPE_CHAR:
    //         return 1;
    //     case Type_tag::TYPE_REAL:
    //         return 10;
    //     case Type_tag::TYPE_ARRAY:
    //         return type->size * sizeOfType(type->refType);
    //     case Type_tag::TYPE_ANY:
    //         return 0;
    // }
    // return 0;
}

bool equalType (Type type1, Type type2)
{
    if (type1->kind == Type_tag::TYPE_ARRAY && type2->kind == Type_tag::TYPE_POINTER){
        return equalType(type1->refType, type2->refType);
    }
    if (type2->kind == Type_tag::TYPE_ARRAY && type1->kind == Type_tag::TYPE_POINTER){
        return equalType(type1->refType, type2->refType);
    }
    
    if (type1->kind != type2->kind){
        if (type1->kind == Type_tag::TYPE_ANY || type2->kind == Type_tag::TYPE_ANY)
            return true;
        else return false;
    }
    switch (type1->kind) {
        case Type_tag::TYPE_ARRAY:
            return equalType(type1->refType, type2->refType);
        case Type_tag::TYPE_IARRAY:
        case Type_tag::TYPE_POINTER:
            return equalType(type1->refType, type2->refType);
        default: break;

    }
    return true;        
}

void printType (Type type)
{
    if (type == NULL) {
        fprintf(stderr, "<undefined>");
        return;
    }
    
    switch (type->kind) {
        case Type_tag::TYPE_VOID:
            fprintf(stderr, "void");
            break;
        case Type_tag::TYPE_INTEGER:
            fprintf(stderr, "int");
            break;
        case Type_tag::TYPE_BOOLEAN:
            fprintf(stderr, "bool");
            break;
        case Type_tag::TYPE_CHAR:
            fprintf(stderr, "char");
            break;
        case Type_tag::TYPE_REAL:
            fprintf(stderr, "double");
            break;
        case Type_tag::TYPE_ARRAY:
            fprintf(stderr, "array [%d] of ", type->size);
            printType(type->refType);
            break;
        case Type_tag::TYPE_IARRAY:
            fprintf(stderr, "array of ");
            printType(type->refType);
            break;
        case Type_tag::TYPE_POINTER:
            fprintf(stderr, "*");
            printType(type->refType);
            break;
        case Type_tag::TYPE_ANY:
            fprintf(stderr, "Any");
            break;
    }
}

void printMode (PassMode mode)
{
    if (mode == PASS_BY_REFERENCE)
        printf("var ");
}


#define SHOW_OFFSETS

void printSymbolTable ()
{
    Scope       * scp;
    SymbolEntry * e;
    SymbolEntry * args;
    
    scp = currentScope;
    if (scp == NULL)
        printf("no scope\n");
    else
        while (scp != NULL) {
            printf("scope: ");
            e = scp->entries;
            while (e != NULL) {
                if (e->entryType == ENTRY_TEMPORARY)
                    printf("$%d", e->u.eTemporary.number);
                else
                    printf("%s", e->id);
                switch (e->entryType) {
                    case ENTRY_FUNCTION:
                        printf("(");
                        args = e->u.eFunction.firstArgument;
                        while (args != NULL) {
                            printMode(args->u.eParameter.mode);
                            printf("%s : ", args->id);
                            printType(args->u.eParameter.type);
                            args = args->u.eParameter.next;
                            if (args != NULL)
                                printf("; ");
                        }
                        printf(") : ");
                        printType(e->u.eFunction.resultType);
                        break;
#ifdef SHOW_OFFSETS
                    case ENTRY_VARIABLE:
                        printf("[%d]", e->u.eVariable.offset);
                        break;
                    case ENTRY_PARAMETER:
                        printf("[%d]", e->u.eParameter.offset);
                        break;
                    case ENTRY_TEMPORARY:
                        printf("[%d]", e->u.eTemporary.offset);
                        break;
                    default:
                        break;
#endif
                }
                e = e->nextInScope;
                if (e != NULL)
                    printf(", ");
            }
            scp = scp->parent;
            printf("\n");
        }
    printf("----------------------------------------\n");
    printf("Nesting Funs Size: %ld\n", lastDefFuns.size());
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
}




llvm::Type* toLLVMType(Type t)
{
    switch (t->kind) {
		case Type_tag::TYPE_BOOLEAN:
			return llvm::IntegerType::get(AST::TheContext, 8);
		case  Type_tag::TYPE_INTEGER:
			return llvm::IntegerType::get(AST::TheContext, 16);
		case  Type_tag::TYPE_REAL:
			return llvm::Type::getX86_FP80Ty(AST::TheContext);
		case  Type_tag::TYPE_VOID:
			return llvm::Type::getVoidTy(AST::TheContext);
		case Type_tag::TYPE_POINTER:
			return llvm::PointerType::get(toLLVMType(t->refType),0);
		case Type_tag::TYPE_CHAR:
			return llvm::IntegerType::get(AST::TheContext, 8);
		case Type_tag::TYPE_ARRAY:
            return llvm::PointerType::get(toLLVMType(t->refType),0);
			
            return llvm::ArrayType::get(toLLVMType(t->refType),t->size);
		default:
			fatal("toLLVMType: No Such type");
            return nullptr;
			break;
	}
}

void deleteType(Type t){
    if (t == NULL)
        return;
    switch (t->kind) {
        case Type_tag::TYPE_ARRAY:
            deleteType(t->refType);
            break;
        case Type_tag::TYPE_IARRAY:
            deleteType(t->refType);
            break;
        case Type_tag::TYPE_POINTER:
            deleteType(t->refType);
            break;
        default:
            break;
    }
    delete t;
}

TypedExpression::~TypedExpression(){
    deleteType(_t);
}