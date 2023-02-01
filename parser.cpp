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

	return new Statement();
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
	Token current;
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
	
	// Todo params and then follow up on here
	// generate function object
	FunctionCall* func = new FunctionCall();
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
		os << "Successfully Parsed: " << helper.type << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack) {
		
		std::stack<StatementHelper> opStack = operatorStack;
    	while(!opStack.empty()){
			os << "Successfully Parsed: " << opStack.top().type << std::endl;
			opStack.pop();
		}
		return os;
}


Statement* buildStatement(Tokens tokens){
	
	std::vector<StatementHelper> queue;
	std::stack<StatementHelper> operatorStack;
	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	while(current.type != END && current.type != NL){
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
	return new Statement;
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

Variable::Variable(std::string name){
	this->name = name;
}

Equality::Equality(){};

Power::Power(){};

Not::Not(){};

And::And(){};

Or::Or(){};

Bool::Bool(std::string truth){
	this->truth = truth;
}

Plus::Plus(){}

FunctionCall::FunctionCall(){}

Multiplication::Multiplication(){}

StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

}

