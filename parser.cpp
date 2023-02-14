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
	Token current = tokens.current();
	while (current.get_type() != END){
		if (current.get_type() == NL) {
			tokens.eat(); // eat newline
			current = tokens.current();
			continue;
		}
		Expression* exp = parseLine(tokens);
		std::cout << "Programm as nested Tree:\n";
		std::cout << exp->toTreeString() << std::endl;
		current = tokens.current();
	}
	return Programm();
}

Expression* parseLine(Tokens& tokens) {

	Token current = tokens.current();
	Token next = tokens.next();
	Expression* exp;

	if (current.get_type()==VAR && next.get_type() == EQUAL) {
		exp = parseDeclaration(tokens);
	}
	else if (current.get_type() == KEYWORD && current.value == "if"){
		exp = parseIf(tokens);
	}
	else if (current.get_type() == KEYWORD && current.value == "while") {
		exp = parseWhile(tokens);
	}
	else if (current.get_type() == KEYWORD && current.value == "for") {
		exp = parseFor(tokens);
	}
	else {
		exp = buildStatement(tokens);
	}
	return exp;

}

Expression* parseWhile(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();
	if (!(current.get_type() == KEYWORD && current.value == "while")){
		parse_error("while <condition> <body>", current.value);
	}
	tokens.eat(); // eat while keyword
	Statement* condition = buildStatement(tokens);
	current = tokens.current();
	// add : for one line while
	if (current.get_type() != NL && current.get_type() != COLON){
		parse_error("newline or colon", current.value);
	}
	bool one_liner = false;
	if (current.get_type() == COLON){
		one_liner = true;
	}
	tokens.eat(); // eat newline
	Block* body = parseBody(tokens, one_liner);
	current = tokens.current();
	if (current.get_type()== NL) {
		tokens.eat(); // eat newline after end
	}
	return new While(condition, body);
}

Expression* parseIf(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();
	if (!(current.get_type() == KEYWORD && current.value == "if")){
		parse_error("if <condition> <body>", current.value);
	}
	tokens.eat(); // eat if keyword
	Statement* condition = buildStatement(tokens);
	current = tokens.current();
	// add : for one line if 
	if (current.get_type() != NL && current.get_type() != COLON){
		parse_error("newline or colon", current.value);
	}
	bool one_liner = false;
	if (current.get_type() == COLON){
		one_liner = true;
	}
	tokens.eat(); // eat newline
	Block* body = parseBody(tokens, one_liner);
	current = tokens.current();
	return new If(condition, body);

}

Block* parseBody(Tokens& tokens, bool one_line){
	Token current = tokens.current();
	std::vector<Expression*> expressions;

	if  (one_line){
		expressions.push_back(parseLine(tokens));
		current = tokens.current();
		if (current.get_type() != NL && current.get_type() != END){
			parse_error("new line or EOF", current.value);
		}
	}
	else {

		while(current.value != "end") {
			if (current.get_type() == NL) {
				tokens.eat(); // eat the newline
				current = tokens.current();
				continue;
			}
			expressions.push_back(parseLine(tokens));
			if (tokens.current().get_type() != NL) {
				parse_error("newline", tokens.current().value);
			}
			tokens.eat(); // eat newline
			current = tokens.current();
		}
		tokens.eat(); // eat keyword end
	}
	return new Block(expressions);


}

Expression* parseDeclaration(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();

	if (!(current.get_type()==VAR && next.get_type() == EQUAL)) {
		parse_error("variable=", current.value + next.value);
	}
	tokens.eat(); // eat variable name
	tokens.eat(); // eat equal sign
	Statement* assigned_statement = buildStatement(tokens);
	std::string variable_name = current.value;
	Expression* var_decl = new VariableDeclaration(variable_name, assigned_statement);

	return var_decl;
}

Expression* parseFor(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();

	if (!(current.get_type()==KEYWORD && current.value == "for")) {
		parse_error("for (<init>;<condition>;<changer>;) <body>", current.value);
	}
	if (next.get_type() != LPAREN) {
		parse_error("(", next.value);
	}


	tokens.eat(); // eat for keyword
	tokens.eat(); // left parenthesis
	VariableDeclaration* init = (VariableDeclaration*) parseDeclaration(tokens);
	tokens.eat(); // eat semicolon ;
	Statement* condition = buildStatement(tokens);
	tokens.eat(); // eat semicolon ;
	VariableDeclaration* changer = (VariableDeclaration*) parseDeclaration(tokens);
	tokens.eat(); // eat semicolon ;
	tokens.eat(); // eat left parenthesis

	current = tokens.current();
	if (current.get_type() != NL && current.get_type() != COLON){
		parse_error("newline or colon", current.value);
	}
	bool one_liner = false;
	if (current.get_type() == COLON){
		one_liner = true;
	}
	tokens.eat(); // eat newline
	Block* body = parseBody(tokens, one_liner);
	current = tokens.current();
	if (current.get_type()== NL) {
		tokens.eat(); // eat newline after end
	}
	return new For(init, condition, changer, body);


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
	return stm;
}


// ------------------------------------------------ Class Definitions ---------------------------------------------

Programm::Programm(){

}

Parser::Parser() {
	
}

Expression::Expression(){};

Statement::Statement() {}

VariableDeclaration::VariableDeclaration(std::string var_name, Statement* assigned_statement) {
	this->var_name = var_name;
	this->assigned_stm = assigned_statement;
}

std::string VariableDeclaration::toTreeString() {
	return "VariableDeclaration(" + this->var_name + ";" + this->assigned_stm->toTreeString() + ")";
}

Block::Block(std::vector<Expression*> expressions){
	this->expressions = expressions;
}

std::string Block::toTreeString() {
	std::string ret_string = "Block(";
	for (auto exp: this->expressions){
		ret_string += exp->toTreeString() + ";";
	}
	ret_string.pop_back();
	ret_string += ")";
	return ret_string;
}

If::If(Statement* condition, Block* body){
	this->condition = condition;
	this->body = body;
}

std::string If::toTreeString() {
	return "If(" + this->condition->toTreeString() + ";" + this->body->toTreeString() + ")";
}

While::While(Statement* condition, Block* body) {
	this->condition = condition;
	this->body = body;
}

std::string While::toTreeString() {
	return "While(" + this->condition->toTreeString() + ";" + this->body->toTreeString() + ")";
}

For::For(VariableDeclaration* init, Statement* condition, VariableDeclaration* changer, Block* body) {
	this->init = init;
	this->condition = condition;
	this->changer = changer;
	this->body = body;
}

std::string For::toTreeString() {
	return "For(" + this->init->toTreeString() + ";" + this->condition->toTreeString() + ";" + this->changer->toTreeString() + ";" + this->body->toTreeString() + ")";
}