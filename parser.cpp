#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>


Ast Parser::parse(Tokens tokens) {
	
}

Statement* parseStatement(Tokens& tokens){}

Statement* parseNum(Tokens& tokens){
	Token current = tokens.current();
	Statement* number = new Number(current.value);
	tokens.eat();
	return number;
}

Statement* parseVariable(Tokens& tokens){
	Token current = tokens.current();
	Statement* variable = new Variable(current.value);
	tokens.eat();
	return variable;
}

Statement* parseMultiplication(Tokens& tokens){
	Token current = tokens.current();
}

Statement* parseFunctionCall(Tokens& tokens){
	Token current = tokens.current();
	
	// Todo params and then follow up on here

}

Statement* buildStatementInline(Tokens tokens){
	
	std::vector<Statement*> queue;
	std::vector<Statement*> operators;
	while(!(tokens.current().get_type() == EOF && tokens.current().get_type() == NL)){
		Token current = tokens.current();
		if(current.get_type() == NUM){
			Number* number = (Number*) parseNum(tokens);
			queue.push_back(number);
		}
		else if(current.get_type() == VAR){
			Token next = tokens.next();
			if (next.get_type() == LPAREN){
				FunctionCall* functionCall = (FunctionCall*)parseFunctionCall(tokens);
				current = tokens.current();
				if (current.get_type() != RPAREN) {
					parse_error(")", current.value);
				}
				tokens.eat();
				queue.push_back(functionCall);
			}
			Variable* variable = (Variable*) parseVariable(tokens);
			queue.push_back(variable);
		}
		// Check for all the operators
		/*
		Highest
		==, !=, <=, >=, <, >
		*, /
		+, -
		Lowest
		*/
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
