#include <iostream>
#include "ast.hpp"

int main(){
	Program *p = new Program(new DeclarationList());
	std::cout << p->toJSONString() << std::endl;

}
