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

void parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue){
	Token current = tokens.current();
	std::cout << "Current Token is: " << current << std::endl;
	Token next = tokens.next();
	std::cout << "Next Token is: " << next << std::endl;
	tokens.eat(); // eating function name
	tokens.eat(); // eating left parenthesis

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
	// index will now point to the next element after the function call

	// Construct Params object for function call object
	Tokens params_tokens(token_list);
	std::vector<Statement*> params_list;
	while(!params_tokens.tokens.empty()){
		Statement* param = buildStatement(params_tokens);
		params_list.push_back(param);
		if(params_tokens.tokens.size()>0){
			params_tokens.eat(); // eat comma 
		}
	}
	Params params(params_list);

	// Remove all tokens related to the function call
	for (int i=0; i < index-1; i++){
		tokens.eat();
	}

	// Construct functionCall object and eat right parenthesis
	FunctionCall* func = new FunctionCall(params);
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


Statement* buildStatement(Tokens tokens){
	
	std::vector<StatementHelper> queue;
	std::stack<StatementHelper> operatorStack;
	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	while(current.type != END && current.type != NL && current.type != COMMA){
		current = tokens.current();
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, queue);
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, queue);
			break;
		case EQUAL:
			parseEquality(tokens, operatorStack, queue);
			break;
		case TIMES:
			parseMultOrPower(tokens, operatorStack, queue);
			break;
		case KEYWORD:
			parseKeyword(tokens, operatorStack, queue);
		case SLASH:
			break;
		case LESS:
			break;
		case GREATER:
			break;
		case PLUS:
			break;
		case MINUS:
			break;
		case LPAREN:
			break;
		case RPAREN:
			break;
		default:
			// Temporary Solution
			parse_error("IDK", "stuff");
		}
		current = tokens.current();
			
	}
	std::cout << "Output queue:\n";
	std::cout << queue;
	std::cout << "Operator Stack:\n";
	std::cout << operatorStack;
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
	return "Number(" + this->value + ")";
}

Variable::Variable(std::string name){
	this->name = name;
}

std::string Variable::toString(){
	return "Variable(" + this->name + ")";
}

Equality::Equality(){};

std::string Equality::toString(){
	return "Equality()";
}

Power::Power(){};

std::string Power::toString(){
	return "Power()";
}

Not::Not(){};

std::string Not::toString(){
	return "Not()";
}

And::And(){};

std::string And::toString(){
	return "And()";
}

Or::Or(){};

std::string Or::toString(){
	return  "Or()";
}

Bool::Bool(std::string truth){
	this->truth = truth;
}

std::string Bool::toString(){
	return "Bool(" + this->truth + ")";
}

Plus::Plus(){}

std::string Plus::toString(){
	return "Plus()";
}

FunctionCall::FunctionCall(Params params){
	this->params = params;
}

std::string FunctionCall::toString(){
	return "FunctionCall()";
}

Multiplication::Multiplication(){}

std::string Multiplication::toString(){
	return "Multiplication()";
}

Params::Params(std::vector<Statement*> params){
	this->params = params;
}

Params::Params(){};

std::string Params::toString(){
	std::string out = "Params( ";
	for (auto statement: this->params){
		out += statement->toString() + ",";
	}
	out += " )";
	return out;
}

StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

}

