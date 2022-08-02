#include "ast.hpp"


Program::~Program(){
	delete _decls;
}