#include "types.hpp"
#include <vector>
#include <string>
#include <utility>

class SymbolEntry {
public:
	bool _isImmutable;
	Entity* _entity;
	bool _isLVAL;
	int scopingDepth;
	size_t offset;
	
	///=================================///
	///============LLVM STUFF===========///


	///=================================///
};

class SymbolTable {
public:
	SymbolTable() : 
	_tbl(std::vector<std::vector<std::pair<std::string, SymbolEntry*> > >()) {}

	/**
	 * @brief Push A new Empty Scope =>> We expect to have the current function as head of the scope
	 *
	 */
	void pushScope();
	/**
	 * @brief Pop A new scope
	 * 
	 */
	void popScope();

	/**
	 * @brief Insert a new name in the final scope. Fix table offset of variables
	 * 
	 * @param name The name to add
	 * @param e The species of entity this is.
	 */
	void insert(std::string name, SymbolEntry* e);

	/**
	 * @brief Lookup for a name in the Symbol Table. Policy: Last allocated variable 
	 * with the same name.
	 * 
	 * @param name The name to search.
	 * @return SymbolEntry* If not found returns nullptr else returns the non nullptr SymbolEntry.
	 */
	SymbolEntry* lookup(std::string name);
	
	int depth=0;
	size_t offset=0;
private:
	std::vector<std::vector<std::pair<std::string, SymbolEntry*> > > _tbl;
	
};

class State {
public:
	SymbolTable* globalTable;
};