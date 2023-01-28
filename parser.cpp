#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>


Ast Parser::parse(Tokens tokens) {
	
}

Statement* parseStatement(Tokens& tokens){}

Number* parseNum(Tokens& tokens){
	Token current = tokens.current();
	Number* number = new Number(current.value);
	tokens.eat();
	return number;
}

Variable* parseVariable(Tokens& tokens){
	Token current = tokens.current();
	Variable* variable = new Variable(current.value);
	tokens.eat();
	return variable;
}

Multiplication* parseMultiplication(Tokens& tokens){
	Token current = tokens.current();

	// TODO
}

FunctionCall* parseFunctionCall(Tokens& tokens){
	Token current = tokens.current();
	
	// Todo params and then follow up on here

}

Equality* parseEquality(Tokens& tokens){
	tokens.eat();
	tokens.eat();
	return new Equality();
}

Statement* buildStatement(Tokens tokens){
	
	std::vector<StatementHelper> queue;
	std::vector<StatementHelper> operators;
	// Change while condition to be not one of the statement types
	while(!(tokens.current().get_type() == EOF && tokens.current().get_type() == NL)){
		Token current = tokens.current();
		if(current.get_type() == NUM){
			Number* number = parseNum(tokens);
			StatementHelper helper("number", number);
			queue.push_back(helper);
		}
		else if(current.get_type() == VAR){
			Token next = tokens.next();
			if (next.get_type() == LPAREN){
				FunctionCall* functionCall = parseFunctionCall(tokens);
				StatementHelper helper("functionCall", functionCall);
				current = tokens.current();
				if (current.get_type() != RPAREN) {
					parse_error(")", current.value);
				}
				tokens.eat();
				queue.push_back(helper);
			}
			Variable* variable = parseVariable(tokens);
			StatementHelper helper("variable", variable);
			queue.push_back(helper);
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
		else if (current.get_type() == EQUAL){
			Token next = tokens.next();
			if (next.get_type() != EQUAL){
				parse_error("=", current.value);
			}
			Equality* equality = parseEquality(tokens);
			StatementHelper helper("variable", equality);
			operators.push_back(helper);

		}
	}

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
