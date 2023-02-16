
#include "statement.h"
#include "util.h"
#include <iostream>


void parseNum(Tokens& tokens, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	Number* number = new Number(current.value);
	tokens.eat();
	StatementHelper helper("number", number);
	queue.push_back(helper);
}

void parseVariableOrFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue){
	Token next = tokens.next();
	Token current = tokens.current();
	if (next.get_type() == LPAREN){
		parseFunctionCall(tokens, queue);
	}
	else {

		Variable* variable = new Variable(current.value);
		tokens.eat();
		StatementHelper helper("variable", variable);
		queue.push_back(helper);
	}
}

void parseMultiplication(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue) {
	Token current = tokens.current();
	Multiplication* mult = new Multiplication();
	tokens.eat();
	StatementHelper helper("*", mult);
	pushOperatorToStack(operatorStack, helper, queue);
}

Params parseParams(Tokens& tokens){
	// This is really bad code but I have no idea on how to do this differently (Its O(2n) which should still be ok)
	int left_paren = 1;
	int right_paren = 0;
	int index = 0;
	std::vector<Token> token_list;
	while(!(left_paren==right_paren)){
		if (index>tokens.tokens.size()-1){
			parse_error(")", "End of File in params", tokens);
		}
		Token token = tokens.next(index);
		token_list.push_back(token);
		if (token.get_type() == LPAREN){
			left_paren++;
		}
		else if (token.get_type() == RPAREN){
			right_paren++;
		}
		index++;
	}
	token_list.pop_back(); // remove right parenthesis because that is needed for checking the end
	token_list.push_back(Token(END, "EOF")); // Add EOF for parsing purposes
	// index will now point to the next element after the function call
	// Construct Params object for function call object
	Tokens params_tokens(token_list, "");
	std::vector<Statement*> params_list;
	while(!params_tokens.tokens.empty()){
		Statement* param = buildStatement(params_tokens);
		params_list.push_back(param);
		params_tokens.eat(); // eat comma or EOF
	}

	// Remove all tokens related to the function call
	for (int i=0; i < index-1; i++){
		tokens.eat();
	}

	Params params(params_list);
	return params;
}

void parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	Token next = tokens.next();
	std::string functionName = current.value;
	tokens.eat(); // eating function name
	tokens.eat(); // eating left parenthesis
	Params params = parseParams(tokens);
	// Construct functionCall object and eat right parenthesis
	FunctionCall* func = new FunctionCall(functionName, params);
	StatementHelper helper("functionCall", func);
	current = tokens.current();
	if (current.get_type() != RPAREN) {
		parse_error(")", current.value, tokens);
	}
	tokens.eat();
	queue.push_back(helper);

}

void parseEquality(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue){
	Token next = tokens.next();
	Token current = tokens.current();

	if (next.get_type() != EQUAL){
		parse_error("=", next.value, tokens);
	}
	tokens.eat();
	tokens.eat();
	Equality* equality = new Equality();
	StatementHelper helper("==", equality);
	pushOperatorToStack(operators, helper, queue);
}

void parsePower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue) {
	Power* pow = new Power();
	tokens.eat();
	tokens.eat();
	StatementHelper helper("**", pow);
	pushOperatorToStack(operatorStack, helper, queue);
}



void parseMultOrPower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	Token next = tokens.next();
	if (next.get_type() == TIMES){
		parsePower(tokens, operatorStack, queue);
	}
	else {
		parseMultiplication(tokens, operatorStack, queue);
	}
}

void parseLogicalOperator(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	std::string keyword = current.value;
	if (keyword=="not") {
		Not* not_op = new Not();
		StatementHelper helper("not", not_op);
		tokens.eat();
		pushOperatorToStack(operators, helper, queue);
	}
	else if (keyword=="and") {
		And* and_op = new And();
		StatementHelper helper("and", and_op);
		tokens.eat();
		pushOperatorToStack(operators, helper, queue);
	}
	else if (keyword=="or") {
		Or* or_op = new Or();
		StatementHelper helper("or", or_op);
		tokens.eat();
		pushOperatorToStack(operators, helper, queue);
	}

}

void parseBool(Tokens& tokens, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	Bool* b = new Bool(current.value);
	tokens.eat();
	StatementHelper helper("bool", b);
	queue.push_back(helper);
}


void parseKeyword(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue, bool& was_operator){
	Token current = tokens.current();
	std::string keyword = current.value;
	if (keyword=="not" || keyword=="and" || keyword=="or") {
		parseLogicalOperator(tokens, operators, queue);
		was_operator = true;
	}
	else if (keyword == "true" || keyword == "false"){
		parseBool(tokens, queue);
		was_operator = false;
	}
	else {
		parse_error("'not', 'and', 'or', 'true' or 'false'", current.value, tokens);
	}

}

void parsePlus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue) {
	Plus* op_plus = new Plus();
	tokens.eat();
	StatementHelper helper("+", op_plus);
	pushOperatorToStack(operatorStack, helper, queue);

}

void parseMinus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue, bool& was_operator) {
	
	if (!was_operator){		
		Minus* op_minus = new Minus();
		tokens.eat();
		StatementHelper helper("-", op_minus);
		pushOperatorToStack(operatorStack, helper, queue);
	}
	else {
		tokens.eat(); // eat negative sign
		Token current = tokens.current();
		Number* number = new Number("-" + current.value);
		tokens.eat(); // eat number
		StatementHelper helper("number", number);
		queue.push_back(helper);
	}

}

void parseSlash(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	Divide* div = new Divide();
	tokens.eat();
	StatementHelper helper("/", div);
	pushOperatorToStack(operatorStack, helper, queue);
}

void parseLParen(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	LParen* lpar = new LParen();
	tokens.eat();
	StatementHelper helper("(", lpar);
	operatorStack.push(helper);
}

void parseRParen(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	tokens.eat();
	while(true){
		if(operatorStack.empty()){
			parse_error("(", tokens.current().value, tokens);
		}
		StatementHelper helper = operatorStack.top();
		operatorStack.pop();
		if (helper.type == "(") {
			break;
		}
		queue.push_back(helper);
	}
}

void parseLess(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){

	Token next = tokens.next();
	if (next.get_type()==EQUAL){
		parseLessEqual(tokens, operatorStack, queue);
	}
	else {
		tokens.eat(); // eat <
		Less* less = new Less();
		StatementHelper helper("<", less);
		pushOperatorToStack(operatorStack, helper, queue);
	}


}

void parseLessEqual(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	tokens.eat(); // eat <
	tokens.eat(); // eat =
	LessEqual* lessEqual = new LessEqual();
	StatementHelper helper("<=", lessEqual);
	pushOperatorToStack(operatorStack, helper, queue);
}

void parseGreater(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	Token next = tokens.next();
	if (next.get_type()==EQUAL){
		parseGreaterEqual(tokens, operatorStack, queue);
	}
	else {
		tokens.eat(); // eat >
		Greater* greater = new Greater();
		StatementHelper helper(">", greater);
		pushOperatorToStack(operatorStack, helper, queue);
	}
}

void parseGreaterEqual(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue){
	tokens.eat(); // eat >
	tokens.eat(); // eat =
	GreaterEqual* greaterEqual = new GreaterEqual();
	StatementHelper helper(">=", greaterEqual);
	pushOperatorToStack(operatorStack, helper, queue);
}

void parseString(Tokens& tokens, std::vector<StatementHelper>& queue) {
    Token current = tokens.current();
    String* str = new String(current.get_value());
    tokens.eat(); // eat string token;
    StatementHelper helper("string", str);
    queue.push_back(helper);

}


std::ostream& operator<<(std::ostream& os, const std::vector<StatementHelper> queue) {
	for (auto helper: queue){
		os << "Successfully Parsed: " << helper.stm->toString() << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack) {
		
		std::stack<StatementHelper> opStack = operatorStack;
    	while(!opStack.empty()){
			os << "Successfully Parsed: " << opStack.top().stm->toString() << std::endl;
			opStack.pop();
		}
		return os;
}

// Class Definitons and Class Functions

Number::Number(std::string value) {
	this->value = value;
}

std::string Number::toString(){
	return this->value;
}

std::string Number::toTreeString() {
	return "Number(" + this->value + ")";
}

Variable::Variable(std::string name, VariableType varType){
	this->varType = varType;
	this->name = name;
}

Variable::Variable(std::string name) {
	this->varType = UNKNOWN_TYPE;
	this->name = name;
}

std::string Variable::toString(){
	return this->name;
}

std::string Variable::toTreeString(){
	return "Variable(" + this->name + ")";
}

Equality::Equality(){}

Equality::Equality(Statement* LHS, Statement* RHS) {
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Equality::toString(){
	return "==";
}

std::string Equality::toTreeString(){
	return "Equality(" + LHS->toTreeString() + "," + RHS->toTreeString() + ")";
}

Power::Power(){};

Power::Power(Statement* LHS, Statement* RHS) {
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Power::toString(){
	return "**";
}

std::string Power::toTreeString() {
	return "Power(" + LHS->toTreeString() + "," + RHS->toTreeString() + ")";
}

Not::Not(){};

Not::Not(Statement* RHS){
	this->RHS = RHS;
}

std::string Not::toString(){
	return "not";
}

std::string Not::toTreeString() {
	return "Not(" + this->RHS->toTreeString() + ")";
}

And::And(){};

And::And(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;

}

std::string And::toString(){
	return "and";
}

std::string And::toTreeString() {
	return "And("+ this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

Or::Or(){};

Or::Or(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Or::toString(){
	return  "or";
}

std::string Or::toTreeString() {
	return "Or(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

Bool::Bool(std::string truth){
	this->truth = truth;
}

std::string Bool::toString(){
	return this->truth;
}

std::string Bool::toTreeString() {
	return "Bool(" + this->truth + ")";
}

Plus::Plus(){}

Plus::Plus(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Plus::toString(){
	return "+";
}

std::string Plus::toTreeString() {
	return "Plus(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

FunctionCall::FunctionCall(std::string functionName, Params params){
	this->functionName = functionName;
	this->params = params;
}

std::string FunctionCall::toString(){
	return this->functionName + "(" + this->params.toString() + ")";
}

std::string FunctionCall::toTreeString() {
	return "FunctionCall(" + this->functionName + ";" + this->params.toTreeString() + ")";
}

Multiplication::Multiplication(){}

Multiplication::Multiplication(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Multiplication::toString(){
	return "*";
}

std::string Multiplication::toTreeString() {
	return "Multiplication(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

Params::Params(std::vector<Statement*> params){
	this->params = params;
}

Params::Params(){};

std::string Params::toString(){
	std::string out = "";
	for (auto stm: this->params){
		out += stm->toString() + ",";
	}
	out.pop_back();
	return out;
}

std::string Params::toTreeString(){
	std::string param_string = "";
	for (auto stm: this->params){
		param_string += stm->toTreeString() + ",";
	}
	param_string.pop_back();
	return "Params(" + param_string + ")";
}


StatementHelper::StatementHelper(std::string type, Statement* stm){
	this->type = type;
	this->stm = stm;

}

Minus::Minus(){}

Minus::Minus(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Minus::toString(){
	return "-";
}

std::string Minus::toTreeString() {
	return "Minus(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

Divide::Divide(){}

Divide::Divide(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Divide::toString(){
	return "/";
}

std::string Divide::toTreeString() {
	return "Divide(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

LParen::LParen(){};

std::string LParen::toString(){
	return "(";
}

std::string LParen::toTreeString() {
	return "";
}

RParen::RParen(){};

std::string RParen::toString(){
	return ")";
}

std::string RParen::toTreeString() {
	return "";
}

Less::Less(){}

Less::Less(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Less::toString(){
	return "<";
}

std::string Less::toTreeString() {
	return "Less(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

Greater::Greater(){}

Greater::Greater(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string Greater::toString(){
	return ">";
}

std::string Greater::toTreeString() {
	return "Greater(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

LessEqual::LessEqual(){}

LessEqual::LessEqual(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string LessEqual::toString(){
	return "<=";
}

std::string LessEqual::toTreeString() {
	return "LessEqual(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

GreaterEqual::GreaterEqual(){}

GreaterEqual::GreaterEqual(Statement* LHS, Statement* RHS){
	this->LHS = LHS;
	this->RHS = RHS;
}

std::string GreaterEqual::toString(){
	return ">=";
}

std::string GreaterEqual::toTreeString(){
	return "GreaterEqual(" + this->LHS->toTreeString() + "," + this->RHS->toTreeString() + ")";
}

String::String(std::string value) {
    this->value = value;
}

std::string String::toString() {
    return this->value;
}

std::string String::toTreeString() {
    return "String(" + this->value + ")";
}
