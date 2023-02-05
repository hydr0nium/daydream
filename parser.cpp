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
	return new And();
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
			parse_error(")", "End of File");
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
		if(params_tokens.tokens.size()>0){
			params_tokens.eat(); // eat comma or EOF
		}
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


void parseKeyword(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	std::string keyword = current.value;
	if (keyword=="not" || keyword=="and" || keyword=="or") {
		parseLogicalOperator(tokens, operators, queue);
	}
	else if (keyword == "true" || keyword == "false"){
		parseBool(tokens, queue);
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

void parseMinus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue) {
	Minus* op_minus = new Minus();
	tokens.eat();
	StatementHelper helper("-", op_minus);
	pushOperatorToStack(operatorStack, helper, queue);

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

ReversePolishNotation convertInfixToRPN(Tokens& tokens){

	std::vector<StatementHelper> rpn_queue; // output queue in reverse polish notation
	std::stack<StatementHelper> operatorStack;
	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	while(current.type != END && current.type != NL && current.type != COMMA){
		current = tokens.current();
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, rpn_queue);
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, rpn_queue);
			break;
		case EQUAL:
			parseEquality(tokens, operatorStack, rpn_queue);
			break;
		case TIMES:
			parseMultOrPower(tokens, operatorStack, rpn_queue);
			break;
		case KEYWORD:
			parseKeyword(tokens, operatorStack, rpn_queue);
			break;
		case SLASH:
			parseSlash(tokens, operatorStack, rpn_queue);
			break;
		case LESS:
			parseLess(tokens, operatorStack, rpn_queue);
			break;
		case GREATER:
			parseGreater(tokens, operatorStack, rpn_queue);
			break;
		case PLUS:
			parsePlus(tokens, operatorStack, rpn_queue);
			break;
		case MINUS:
			parseMinus(tokens, operatorStack, rpn_queue);
			break;
		case LPAREN:
			parseLParen(tokens, operatorStack, rpn_queue);
			break;
		case RPAREN:
			parseRParen(tokens, operatorStack, rpn_queue);
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


Statement* buildStatement(Tokens& tokens){
	
	ReversePolishNotation rpn = convertInfixToRPN(tokens);
	std::cout << rpn.toString() << std::endl;
	
	//This is temporary such that I get no errors:
	return new And;
}


// Class Definitions

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

Equality::Equality(){};

std::string Equality::toString(){
	return "==";
}

Power::Power(){};

std::string Power::toString(){
	return "**";
}

Not::Not(){};

std::string Not::toString(){
	return "not";
}

And::And(){};

std::string And::toString(){
	return "and";
}

Or::Or(){};

std::string Or::toString(){
	return  "or";
}

Bool::Bool(std::string truth){
	this->truth = truth;
}

std::string Bool::toString(){
	return this->truth;
}

Plus::Plus(){}

std::string Plus::toString(){
	return "+";
}

FunctionCall::FunctionCall(std::string functionName, Params params){
	this->functionName = functionName;
	this->params = params;
}

std::string FunctionCall::toString(){
	return this->functionName + "( " + this->params.toString() + ")";
}

Multiplication::Multiplication(){}

std::string Multiplication::toString(){
	return "*";
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
	return out;
}

StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

}

Minus::Minus(){}

std::string Minus::toString(){
	return "-";
}

Divide::Divide(){}

std::string Divide::toString(){
	return "/";
}

LParen::LParen(){};

std::string LParen::toString(){
	return "(";
}

RParen::RParen(){};

std::string RParen::toString(){
	return ")";
}

Less::Less(){}

std::string Less::toString(){
	return "<";
}

Greater::Greater(){}

std::string Greater::toString(){
	return ">";
}

LessEqual::LessEqual(){}

std::string LessEqual::toString(){
	return "<=";
}

GreaterEqual::GreaterEqual(){}

std::string GreaterEqual::toString(){
	return ">=";
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
