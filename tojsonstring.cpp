#include "ast.hpp"
#define open_object "{"
#define close_object "}"
#define open_array "["
#define close_array "]"
#define newvalue ", "
#define enclosed(str) "\"" str  "\"" 
#define obj_head(str) \
	"{" \
		"\"" str "\""  " : " \
		"[" \

#define obj_tail \
		"]"\
	"}"

#define immediate_attribute(name, attribute) \
	"{ \"" name "\"" " : " + attribute + "}" +

std::string Program::toJSONString() {
	return 
	obj_head("Program")
		+ this->_decls->toJSONString() +
	obj_tail;
}

std::string VariableDeclaration::toJSONString(){
	return
	obj_head("VariableDeclaration")
		immediate_attribute("name", this->_name)
		newvalue
		+ this->_typeExpr->toJSONString() +
	obj_tail;
}


std::string ArrayDeclaration::toJSONString(){
	return
	obj_head("ArrayDeclaration")
		immediate_attribute("name", this->_name)
		newvalue
		+ this->_typeExpr->toJSONString() +
		newvalue
		+ this->_expr->toJSONString() +
	obj_tail;
}

std::string FunctionDeclaration::toJSONString(){
	return 
	obj_head("FunctionDeclaration")
		+ this->_resultType->toJSONString() +
		newvalue
		immediate_attribute("name", this->_name)
		newvalue
		+ this->_parameters->toJSONString() +
	obj_tail;
}

std::string FunctionDefinition::toJSONString(){
	return 
	obj_head("FunctionDefinition")
		+ this->_resultType->toJSONString() +
		immediate_attribute("name", this->_name)
		newvalue
		+ this->_parameters->toJSONString() +
		newvalue
		+ this->_decls->toJSONString() +
		newvalue
		+ this->_statements->toJSONString() +
	obj_tail;
}

std::string Parameter::toJSONString(){
	return
	obj_head("Parameter")
		immediate_attribute("PassingWay", passingWayToString(this->_pw))
		newvalue
		immediate_attribute("name", this->_name)
		newvalue
		+ this->_type->toJSONString() +
	obj_tail;
}

std::string BasicType::toJSONString(){
	return
	obj_head("BasicType")
		immediate_attribute("name", this->_name)
	obj_tail;
}

std::string Pointer::toJSONString(){
	return
	obj_head("Pointer")
		+ this->_inner->toJSONString() + 
	obj_tail;
}


std::string EmptyStatement::toJSONString(){
	return 
	obj_head("EmptyStatement")
	obj_tail;
}



std::string DeclarationList::toJSONString(){
	std::string res = obj_head("DeclarationList");
	for(size_t i = 0; i < this->_decls.size(); i++){
		res += this->_decls[i]->toJSONString();
		if(i != this->_decls.size()-1)
			res+= newvalue;
	}
	res += obj_tail;
	return res;
}