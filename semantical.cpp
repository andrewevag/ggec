#include "ast.hpp"



void Program::sem() {
	for(auto &decl : this->_decls->_decls)
		decl->sem();
}

void VariableDeclaration::sem() {
	auto symbolTable = globalState.Table;
	auto myType = this->_typeExpr->getType();
	auto mySymbolEntry = new SymbolEntry(
		false,
		myType->copy(),
		true
	);
	symbolTable->insert(this->_name, mySymbolEntry);
}

void ArrayDeclaration::sem(){
	auto symbolTable = globalState.Table;
	auto myType = new Pointert(this->_typeExpr->getType());
	auto mySymbolEntry = new SymbolEntry(
		true, 
		myType->copy(),
		false
	);
	// Check that the inside expression is constant
	// Size of array...??? Maybe Array as type...
	symbolTable->insert(this->_name, mySymbolEntry);
}


//===================================
// Type Stuff
//===================================

Type* Program::getType() {
	return nullptr;
};

Type* BasicType::getType() {
	if(this->_name == "void")
		return new Voidt();
	else if(this->_name == "int")
		return new Intt();
	else if(this->_name == "char")
		return new Chart();
	else if (this->_name == "bool")
		return new Boolt();
	else if (this->_name == "double")
		return new Doublet();
	else{
		//should not get here
		fprintf(stderr, "Internal Error Unknown Type");
		exit(1);
		return nullptr;
	}
}

	
Type* Pointer::getType() {
	return new Pointert(this->_inner->getType());
}

Type* VariableDeclaration::getType() {
	return nullptr; //no type in the declaration itself.
}

Type* ArrayDeclaration::getType() {
	return nullptr; //no type in the declaration itself.
}

//===================================