#include "ast.hpp"
#include <sstream>
#include <iomanip>
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
	"{ \"" name "\"" " : \"" + attribute + "\"}" +

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
		newvalue
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

std::string SingleExpression::toJSONString(){
	return
	obj_head("SingleExpression")
		+ this->_expr->toJSONString() +
	obj_tail;
}

std::string IfStatement::toJSONString(){
	return
	obj_head("IfStatement")
		+ this->_condition->toJSONString() +
		newvalue
		+ this->_ifbody->toJSONString() +
	obj_tail;
}

std::string IfElseStatement::toJSONString(){
	return
	obj_head("IfElseStatement")
		+ this->_condition->toJSONString() +
		newvalue
		+ this->_ifbody->toJSONString() +
		newvalue
		+ this->_elsebody->toJSONString() +
	obj_tail;
}

std::string ForStatement::toJSONString(){
	return
	obj_head("ForStatement")
		+ (this->_label == nullptr ? "" : this->_label->toJSONString() + newvalue)
		+ (this->_first == nullptr ? "" : this->_first->toJSONString() + newvalue)
		+ (this->_second == nullptr ? "" : this->_second->toJSONString() + newvalue)
		+ (this->_third == nullptr ? "" : this->_third->toJSONString() + newvalue) 
		+ this->_body->toJSONString() + 
	obj_tail;
}

std::string ContinueStatement::toJSONString(){
	std::string res = obj_head("ContinueStatement");
	if(this->_target != "")
	   res+= immediate_attribute("Target", this->_target) "";
	res += obj_tail;
	return res;

}

std::string BreakStatement::toJSONString(){
	std::string res = obj_head("BreakStatement");
	if(this->_target != "")
	   res+= immediate_attribute("Target", this->_target) "";
	res += obj_tail;
	return res;

}

std::string ReturnStatement::toJSONString(){
	return 
	obj_head("ReturnStatement")
	+ (this->_expr == nullptr ? std::string() : this->_expr->toJSONString()) +
	obj_tail;
}

std::string Id::toJSONString(){
	return
	obj_head("Id")
		immediate_attribute("name", this->_name)
	obj_tail;
}

std::string reverseFixString(std::string s){
	std::string revFixed;
	for(auto &c : s){
		if(c == '\n'){
			revFixed += "\\n";
		}
		else if(c == '\t'){
			revFixed += "\\t";
		}
		else{
			revFixed.push_back(c);
		}
	}
	return revFixed;
}

std::string Constant::toJSONString(){
	std::ostringstream ss;
	switch (this->_ct){
	case Bool: ss << open_object << "\"" "BoolConstant(" << (this->_bool ? "true" : "false") << ")" "\" : " open_array close_array close_object; break;
	case Null: ss << open_object << "\"" "NULL" "\" : " open_array close_array close_object; break;
	case Int: ss << open_object << "\"" "IntConstant(" << this->_int << ")" "\" : " open_array close_array close_object; break;
	case Char : ss << open_object << "\"" "CharConstant(" << (uint)this->_char << ")" "\" : " open_array close_array close_object; break;
	case Double : ss << open_object << "\"" "DoubleConstant(" << std::fixed << std::setprecision(4) << this->_double << ")" "\" : " open_array close_array close_object; break;
	case String : ss << open_object << "\"" "StringConstant(" << reverseFixString(this->_string) << ")" "\" : " open_array close_array close_object; break; 
	default: break;
	}
	std::string res = ss.str();
	return res;
}

std::string FunctionCall::toJSONString(){
	return 
	obj_head("FunctionCall")
		immediate_attribute("name", this->_functionName)
		newvalue
		+ this->_arguments->toJSONString() +
	obj_tail;
}

std::string BracketedIndex::toJSONString(){
	return
	obj_head("BracketedIndex")
		+ this->_out->toJSONString() +
		newvalue
		+ this->_in->toJSONString() +
	obj_tail;
}

std::string UnaryOp::toJSONString(){
	std::string res;
	res += open_object;
	res += "\"UnOp(" + UnaryOp::unaryOpTypeToString(this->_UnOp) +  ")\" : ";
	res += open_array;
	res += this->_operand->toJSONString();
	res += obj_tail;
	return res;
}

std::string BinaryOp::toJSONString(){
	std::string res;
	res += open_object;
	res += "\"BinaryOp(" + BinaryOp::binaryOpTypeToString(this->_BinOp) +  ")\" : ";
	res += open_array;
	res += this->_leftOperand->toJSONString();
	res += newvalue;
	res += this->_rightOperand->toJSONString();
	res += obj_tail;
	return res;
}

std::string PrefixUnAss::toJSONString(){
	std::string res;
	res += open_object;
	res += "\"PrefixUnAss(" + UnAss::UnAssTypeToString(this->_Unass) +  ")\" : ";
	res += open_array;
	res += this->_operand->toJSONString();
	res += obj_tail;
	return res;
}

std::string PostfixUnAss::toJSONString(){
	std::string res;
	res += open_object;
	res += "\"PostfixUnAss(" + UnAss::UnAssTypeToString(this->_Unass) +  ")\" : ";
	res += open_array;
	res += this->_operand->toJSONString();
	res += obj_tail;
	return res;
}

std::string BinaryAss::toJSONString(){
	std::string res;
	res += open_object;
	res += "\"BinaryAss(" + BinaryAss::binaryAssTypeToString(this->_BinAss) +  ")\" : ";
	res += open_array;
	res += this->_leftOperand->toJSONString();
	res += newvalue;
	res += this->_rightOperand->toJSONString();
	res += obj_tail;
	return res;
}

std::string TypeCast::toJSONString(){
	return
	obj_head("TypeCast")
		+ this->_type->toJSONString() +
		newvalue
		+ this->_expr->toJSONString() +
	obj_tail;
}

std::string TernaryOp::toJSONString(){
	return
	obj_head("TernaryOp")
		+ this->_condition->toJSONString() +
		newvalue
		+ this->_ifBody->toJSONString() +
		newvalue
		+ this->_elseBody->toJSONString() +
	obj_tail;
}

std::string New::toJSONString(){
	return
	obj_head("New")
		+ this->_type->toJSONString()
		+ (this->_size == nullptr ? std::string() : newvalue + this->_size->toJSONString()) +
	obj_tail;
}

std::string Delete::toJSONString(){
	return
	obj_head("Delete")
		+ this->_expr->toJSONString() +
	obj_tail;
}

std::string CommaExpr::toJSONString(){
	return
	obj_head("CommaExpr")
		+ this->_left->toJSONString() +
		newvalue
		+ this->_right->toJSONString() +
	obj_tail;
}

std::string Label::toJSONString(){
	return
	obj_head("Label")
		immediate_attribute("name", this->_lblname)
	obj_tail;
}

std::string StatementList::toJSONString(){
	std::string res = obj_head("StatementList");
	for(size_t i = 0; i < this->_stmts.size(); i++){
		res += this->_stmts[i]->toJSONString();
		if(i != this->_stmts.size()-1){
			res += newvalue;
		}
	}
	res += obj_tail;
	return res;
}

std::string ParameterList::toJSONString(){
	std::string res = obj_head("ParameterList");
	for(size_t i = 0; i < this->_parameters.size(); i++){
		res += this->_parameters[i]->toJSONString();
		if(i != this->_parameters.size()-1){
			res += newvalue;
		}
	}
	res += obj_tail;
	return res;
}

std::string ExpressionList::toJSONString(){
	std::string res = obj_head("ExpressionList");
	for(size_t i = 0; i < this->_expressions.size(); i++){
		res += this->_expressions[i]->toJSONString();
		if(i != this->_expressions.size()-1){
			res += newvalue;
		}
	}
	res += obj_tail;
	return res;
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