#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "reversePolish.h"
#include "statement.h"

Programm Parser::parse(Tokens tokens) {
	buildStatement(tokens);
	return Programm();
}

Statement* parseStatement(Tokens& tokens){

	// This is temporary such that I get no error
	return new Or();
}


Statement* parseDeclaration(Tokens& tokens){



	return new And();
}

std::ostream& operator<<(std::ostream& os, const std::stack<Statement*> stm) {
		
		std::stack<Statement*> stm_copy = stm;
    	while(!stm_copy.empty()){
			os << stm_copy.top()->toString() << std::endl;
			stm_copy.pop();
		}
		return os;
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

Programm::Programm(){

}

Parser::Parser() {
	
}

Expression::Expression(){};

Statement::Statement() {

}
