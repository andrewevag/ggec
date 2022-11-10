#include "symbol.hpp"


void SymbolTable::pushScope(){
	this->_tbl.push_back(std::vector<std::pair<std::string, SymbolEntry*> >());
	
	this->depth++;
	this->offset=0;
}
void SymbolTable::popScope(){
	this->_tbl.pop_back();
	if(this->_tbl.back().size() == 0){
		offset = 0;
	}else{
		auto LastScope = this->_tbl.back();
		auto LastElem = LastScope.back();
		auto LastOffset = LastElem.second->offset;
		this->offset = LastOffset;
		// offset = this->_tbl.back().back().second->offset;
		// !!!!!!!! POSSIBLE SEGFAULT !!!!!!!
		// NOT CHECKING IF THE SYMBOL ENTRY IS NULL
	}
	this->depth--;
}
void SymbolTable::insert(std::string name, SymbolEntry* e){
	auto LastScope = this->_tbl.back();
	LastScope.push_back(std::make_pair(name, e));
	e->scopingDepth = this->depth;
	e->offset = this->offset + e->_entity->getSizeInBytes();
	this->offset = e->offset;
}

SymbolEntry* SymbolTable::lookup(std::string name) {
	return nullptr;
}