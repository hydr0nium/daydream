#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>


Ast Parser::parse(Tokens tokens) {
	
}

Statement* parseStatement(Tokens& tokens){}

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
		FunctionCall* functionCall = parseFunctionCall(tokens, queue);
	}
	else {
		Variable* variable = new Variable(current.value);
		tokens.eat();
		StatementHelper helper("variable", variable);
		queue.push_back(helper);
	}
}

Multiplication* parseMultiplication(Tokens& tokens){
	Token current = tokens.current();

	// TODO
}

FunctionCall* parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue){
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

Equality* parseEquality(Tokens& tokens, std::stack<StatementHelper>& operators){
	Token next = tokens.next();
	Token current = tokens.current();
	if (next.get_type() != EQUAL){
		parse_error("=", current.value);
	}
	tokens.eat();
	tokens.eat();
	Equality* equality = new Equality();
	StatementHelper helper("variable", equality);
	operators.push(helper);
}

Power* parsePower(Tokens& tokens, std::stack<StatementHelper>& operators){
	//TODO
	tokens.eat();
	tokens.eat();
	return new Power();
}

Statement* buildStatement(Tokens tokens){
	
	std::vector<StatementHelper> queue;
	std::stack<StatementHelper> operators;
	// Change while condition to be not one of the statement types
	while(!(tokens.current().get_type() == EOF && tokens.current().get_type() == NL)){
		Token current = tokens.current();
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, queue);
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, queue);
			break;
		case EQUAL:
			parseEquality(tokens, operators);
			break;
		case TIMES:
			parsePower(tokens, operators);
			break;
		default:
			break;
		}
			
	}
		// Check for all the operators
		/*
		Highest
		3 ==, !=, <=, >=, <, >
		2 **
		1 *, /
		0 +, -
		Lowest
		*/
		// Highest Operator == is always pushed to the stack
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

StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

}


int main(){

	std::string input = "a  =  5";
	std::cout << "Using: " << input << std::endl;
	Lexer l;
	Tokens tokens = l.lex(input);
	Parser p;
	p.parse(tokens);
	for (auto token:tokens.tokens){
		std::cout << token;
	}

}
