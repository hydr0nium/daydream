#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>

Ast Parser::parse(Tokens tokens) {
	buildStatement(tokens);
	return Ast();
}

Statement* parseStatement(Tokens& tokens){

	// This is temporary such that I get no error
	return new Or();
}

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
			parse_error(")", "End of File in params");
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
	Tokens params_tokens(token_list);
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
		parse_error(")", current.value);
	}
	tokens.eat();
	queue.push_back(helper);

}

void parseEquality(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue){
	Token next = tokens.next();
	Token current = tokens.current();
	if (next.get_type() != EQUAL){
		parse_error("=", current.value);
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
		parse_error("'not', 'and', 'or', 'true' or 'false'", current.value);
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
		tokens.eat();
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
			parse_error("(", tokens.current().value);
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

std::ostream& operator<<(std::ostream& os, const std::vector<StatementHelper> queue) {
	for (auto helper: queue){
		os << "Successfully Parsed: " << helper.statement->toString() << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack) {
		
		std::stack<StatementHelper> opStack = operatorStack;
    	while(!opStack.empty()){
			os << "Successfully Parsed: " << opStack.top().statement->toString() << std::endl;
			opStack.pop();
		}
		return os;
}

std::ostream& operator<<(std::ostream& os, const std::stack<Statement*> stm) {
		
		std::stack<Statement*> stm_copy = stm;
    	while(!stm_copy.empty()){
			os << stm_copy.top()->toString() << std::endl;
			stm_copy.pop();
		}
		return os;
}

ReversePolishNotation convertInfixToRPN(Tokens& tokens){

	std::vector<StatementHelper> rpn_queue; // output queue in reverse polish notation
	std::stack<StatementHelper> operatorStack;
	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	bool was_operator = false; // this is a dirty way to deal with negative numbers but it works so I will leave it for now
	while(current.type != END && current.type != NL && current.type != COMMA){
		current = tokens.current();
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, rpn_queue);
			was_operator = false;
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, rpn_queue);
			was_operator = false;
			break;
		case EQUAL:
			parseEquality(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case TIMES:
			parseMultOrPower(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case KEYWORD:
			parseKeyword(tokens, operatorStack, rpn_queue, was_operator);
			break;
		case SLASH:
			parseSlash(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case LESS:
			parseLess(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case GREATER:
			parseGreater(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case PLUS:
			parsePlus(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case MINUS:
			parseMinus(tokens, operatorStack, rpn_queue, was_operator);
			break;
		case LPAREN:
			parseLParen(tokens, operatorStack, rpn_queue);
			// Parenthesis don't change if there was a operator or not
			break;
		case RPAREN:
			parseRParen(tokens, operatorStack, rpn_queue);
			// Parenthesis don't change if there was a operator or not
			break;
		default:
			// Temporary Solution
			parse_error("IDK", "stuff");
		}
		current = tokens.current();
			
	}
	// Pop all operators that are still on the operatorStack to the queue
	while(!operatorStack.empty()){
		StatementHelper helper = operatorStack.top();
		operatorStack.pop();
		rpn_queue.push_back(helper);
	}

	// Construct RPN object
	ReversePolishNotation rpn(rpn_queue);
	return rpn;

}

Statement* parseRPN(ReversePolishNotation rpn){
	std::stack<Statement*> statement_stack;
	for(auto helper: rpn.getRpnVector()){
		std::string statement_type = helper.type;
		Statement* stm = helper.statement;
		if (statement_type == "number") {
			statement_stack.push(stm);
		}
		else if (statement_type == "variable"){
			statement_stack.push(stm);
		}
		else if (statement_type == "bool") {
			statement_stack.push(stm);
		}
		else if (statement_type == "functionCall") {
			statement_stack.push(stm);
		}
		else if (statement_type == "+") {
			if( !(statement_stack.size()>=2) ) {
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form an addition object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Plus* plus_stm = new Plus(LHS, RHS);
			statement_stack.push(plus_stm);
			delete stm;
		}
		else if (statement_type == "*") { // Build Multiplication Object
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a multiplication object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Multiplication* mult_stm = new Multiplication(LHS, RHS);
			statement_stack.push(mult_stm);
			delete stm;
		}
		else if (statement_type == "-") { // Build Minus Object
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a subtraction object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Minus* minus_stm = new Minus(LHS, RHS);
			statement_stack.push(minus_stm);
			delete stm;
		}
		else if (statement_type == "/") { // Build Division
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a division object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Divide* div_stm = new Divide(LHS, RHS);
			statement_stack.push(div_stm);
			delete stm;
		}
		else if (statement_type == "**") { // Build Power Object
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a power object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Power* pow_stm = new Power(LHS, RHS);
			statement_stack.push(pow_stm);
			delete stm;
		}
		else if (statement_type == "and") { // Build logical and
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical and object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			And* and_stm = new And(LHS, RHS);
			statement_stack.push(and_stm);
			delete stm;
		}
		else if (statement_type == "not") { // Build logical not
			if ( !(statement_stack.size()>=1) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical not object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Not* not_stm = new Not(RHS);
			statement_stack.push(not_stm);
			delete stm;
		}
		else if (statement_type == "or") { // Build logical or
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical or object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Or* or_stm = new Or(LHS, RHS);
			statement_stack.push(or_stm);
			delete stm;
		}
		else if (statement_type == "<") { // Build less than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a less than object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Less* less_stm = new Less(LHS, RHS);
			statement_stack.push(less_stm);
			delete stm;
		}
		else if (statement_type == "<=") { // Build less equal than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a less or equal than object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			LessEqual* lessEq_stm = new LessEqual(LHS, RHS);
			statement_stack.push(lessEq_stm);
			delete stm;
		}
		else if (statement_type == ">") { // Build greater than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a greater than object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Greater* greater_stm = new Greater(LHS, RHS);
			statement_stack.push(greater_stm);
			delete stm;
		}
		else if (statement_type == ">=") { // Build greater equal than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a greater or equal than object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			GreaterEqual* greaterEq_stm = new GreaterEqual(LHS, RHS);
			statement_stack.push(greaterEq_stm);
			delete stm;
		}
		else if (statement_type == "==") { // Build equality
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a equality object");
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Equality* eq_stm = new Equality(LHS, RHS);
			statement_stack.push(eq_stm);
			delete stm;
		}
		
		//std::cout << "Statement Stack:" << std::endl;
		//std::cout << statement_stack << std::endl;


	}
	if (!(statement_stack.size()==1)){
		parse_error("Could not form AST from RPN. Statement Stack had more than one element. Expected was one.");
	}
	Statement* return_stm = statement_stack.top();
	statement_stack.pop();
	return return_stm;
}

Statement* buildStatement(Tokens& tokens){
	
	ReversePolishNotation rpn = convertInfixToRPN(tokens);
	Statement* stm = parseRPN(rpn);
	std::cout << "In Function Notation:\n";
	std::cout << stm->toTreeString() << std::endl;
	
	//This is temporary such that I get no errors:
	return stm;
}


// ------------------------------------------------ Class Definitions ---------------------------------------------

Ast::Ast(){

}

Parser::Parser() {
	
}

Statement::Statement() {

}

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
	this->varType = UNKNOWN;
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
	for (auto statement: this->params){
		out += statement->toString() + ",";
	}
	out.pop_back();
	return out;
}

std::string Params::toTreeString(){
	std::string param_string = "";
	for (auto statement: this->params){
		param_string += statement->toTreeString() + ",";
	}
	param_string.pop_back();
	return "Params(" + param_string + ")";
}


StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

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

ReversePolishNotation::ReversePolishNotation(std::vector<StatementHelper> rpn){
	this->rpn = rpn;
}

std::string ReversePolishNotation::toString(){
	std::string out = "";
	for(auto helper: this->rpn){
		out += helper.statement->toString() + " ";
	}
	return out;
}

std::vector<StatementHelper> ReversePolishNotation::getRpnVector(){
	return this->rpn;
}
