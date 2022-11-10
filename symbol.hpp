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

	void pushScope();
	void popScope();
	void insert(std::string name, SymbolEntry* e);
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