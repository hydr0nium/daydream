#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "reversePolish.h"
#include "statement.h"

// Parses a the Tokens line by line. For each line a expression is returned which can be added to the 
// Programm object.
Programm Parser::parse(Tokens tokens) {

	std::vector<Expression*> expressions;

	Token current = tokens.current();
	while (current.get_type() != END){
		Expression* exp = parseLine(tokens);
		expressions.push_back(exp);
		current = tokens.current();
	}
	return Programm(expressions);
}


// Parses a single line of code and removed newline if its the first thing that will be encountered.
Expression* parseLine(Tokens& tokens) {

	Token current = tokens.current();
	Expression* exp;
	Token third;
	// If the first thing in the line is a newline. Eat it and try again
	while (current.get_type() == NL) {
		tokens.eat(); // eat the newline
		current = tokens.current(); // Get next token
	}
	// Get the next two values after the current token
	Token next = tokens.next();

	// This is also a dirty way of solving the problem of having only a single variable / statement in the programm
	if (tokens.tokens.size()<=2) {
		third = Token(END, "EOF");
	}
	else {
		Token third = tokens.next(2);
	}
 	if (current.get_type()==VAR && next.get_type() == EQUAL && third.get_type() != EQUAL) {
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
	else if (current.get_type() == KEYWORD && current.get_value() == "def") {
		exp = parseFunctionDefinition(tokens);
	}
	else if (current.get_type() == KEYWORD && current.get_value() == "return") {
		exp = parseReturn(tokens);
	}
	else if (current.get_type() == KEYWORD && current.get_value() == "break") {
		exp = parseBreak(tokens);
	}
	else if (current.get_type() == KEYWORD && current.get_value() == "debug") {
		exp = parseDebug(tokens);
	}
	else {
		exp = buildStatement(tokens);
	}
	// Eat the last thing at the end of the line. Either this will be at the end of a statement or at the end of if, while, etc. But it will always be a newline
	// In case of if, while, etc it will eat the newline not at the end of the line with the if but after the end that ends the if (if not one liner if) otherwise
	// it will eat the newline after the if.
	current = tokens.current();
	if (!(current.get_type() == NL || current.get_type() == END)) {
		parse_error("newline or end of file", current.get_value(), tokens);
	}
	if (current.get_type() == NL) {
		tokens.eat(); // eat newline
	}
	return exp;

}

Expression* parseDebug(Tokens& tokens) {
	tokens.eat(); // eat debug keyword
	return new Debug();
}

Expression* parseReturn(Tokens& tokens) {
	Token current = tokens.current();
	
	// We check if we really got a return expression
	if (!(current.get_type() == KEYWORD && current.get_value() == "return")){
		parse_error("return <statement>", current.get_value(), tokens);
	}
	tokens.eat(); // eat return keyword

	Statement* return_value = buildStatement(tokens);
	return new Return(return_value);

}

Expression* parseBreak(Tokens& tokens) {
	Token current = tokens.current();
	if (!(current.get_type() == KEYWORD && current.get_value() == "break")){
		parse_error("break", current.get_value(), tokens);
	}
	tokens.eat(); // eat break keyword
	return new Break();
}

Expression* parseFunctionDefinition(Tokens& tokens) {
	Token current = tokens.current();
	Token next = tokens.next();

	// We check if we get a function definition
	if (!(current.get_type() == KEYWORD && current.get_value() == "def")){
		parse_error("def <function name>(<params>): <body>", current.get_value(), tokens);
	}
	tokens.eat(); // eat def keyword
	current = tokens.current();
	
	// Check for the function name after def
	if (current.get_type() != VAR) {
		parse_error("<function name>", current.get_value(), tokens);
	}
	tokens.eat(); // eat function name
	std::string function_name = current.get_value();
	current = tokens.current();

	// Check if we get a opening parenthesis
	if (current.get_type() != LPAREN) {
		parse_error("(", current.get_value(), tokens);
	}
	tokens.eat(); // eat left parenthesis

	std::vector<std::string> param_names = parseFunctionDefinitionParams(tokens);
	current = tokens.current();

	// Check if we get a closing parenthesis
	if (current.get_type() != RPAREN) {
		parse_error(")", current.get_value(), tokens);
	}
	tokens.eat(); // eat right parenthesis
	current = tokens.current();

	// Check if we get a colon
	if (current.get_type() != COLON) {
		parse_error(":", current.get_value(), tokens);
	}
	tokens.eat(); // eat colon
	current = tokens.current();

	bool one_line = false;
	if (current.get_type() != NL) {
		one_line = true;
	}
	else {
		tokens.eat(); // eat newline
	}

	// Parse body of function definiton
	Block* body = parseBody(tokens, one_line);
	body->setBlockType(FUNC);
	return new FunctionDefinition(function_name, param_names, body);
}


std::vector<std::string> parseFunctionDefinitionParams(Tokens& tokens) {
	Token current = tokens.current();
	Token next = tokens.next();
	std::vector<std::string> param_names;
	while (true) {
		if(current.get_type() != VAR) {
			parse_error("<variable name>", current.get_value(), tokens);
		}
		param_names.push_back(current.get_value()); // push var name to param names
		tokens.eat();
		if (next.get_type() != COMMA){
			break;
		}
		tokens.eat();
		current = tokens.current();
		next = tokens.next();

	}
	return param_names;



}

Expression* parseWhile(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();


	// We check if we get a while loop
	if (!(current.get_type() == KEYWORD && current.value == "while")){
		parse_error("while <condition>: <body>", current.value, tokens);
	}
	tokens.eat(); // eat while keyword

	// Get the conditional Statement of the while loop
	Statement* condition = buildStatement(tokens);
	current = tokens.current();

	// Check that a colon follows after the while condition
	if (current.get_type() != COLON){
		parse_error(":", current.value, tokens);
	}
	tokens.eat(); // Eat :
	current = tokens.current();


	bool one_line = false;
	// if there is no newline then this will be a oneline while loop
	if (current.get_type() != NL){
		one_line = true;
	}
	else {
		tokens.eat(); // eat newline after the while loop
	}

	// Parse body for while loop
	Block* body = parseBody(tokens, one_line);
	body->setBlockType(WHILE);
	return new While(condition, body);
}

Expression* parseIf(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();

	// Check if it is really a if statement
	if (!(current.get_type() == KEYWORD && current.value == "if")){
		parse_error("if <condition>: <body>", current.value, tokens);
	}
	tokens.eat(); // eat if keyword

	// Get the conditional Statement of the if
	Statement* condition = buildStatement(tokens);
	current = tokens.current();

	// Check if a colon is after the if
	if (current.get_type() != COLON){
		parse_error(":", current.value, tokens);
	}
	tokens.eat(); // eat the :
	current = tokens.current();

	// This checks if the if statement is a oneline or mutliline (by checking if there is a \n after the if)
	bool one_line = false;
	if (current.get_type() != NL){
		one_line = true;
	}
	else {
		tokens.eat(); // eat newline after the if
	}

	// Start parsing the body of the if 
	Block* body = parseBody(tokens, one_line);
	body->setBlockType(IF);
	return new If(condition, body);

}

Block* parseBody(Tokens& tokens, bool one_line){

	Token current = tokens.current();
	std::vector<Expression*> expressions;
	// The body of the object will be one line. E.g. if, while, for in oneline
	if  (one_line){
		// parse the "line" after the if
		expressions.push_back(parseLine(tokens));
		return new Block(expressions);
	}
	// if we get to here the if is not oneline and the newline after the if was eaten. Meaning we are in a line beneath the if
	// Parse new lines until we reach the end keyword to end the if block
	while(current.value != "end") {

		if (current.get_type() == END) {
			parse_error("end keyword", "EOF", tokens);
		}
		expressions.push_back(parseLine(tokens));
		current = tokens.current();
		

	}
	tokens.eat(); // eat end keyword
	return new Block(expressions);
	
}

Expression* parseDeclaration(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();
	Token after_next;

	if (tokens.tokens.size()<=2) {
		after_next = Token(END, "EOF");
	}
	else {
		after_next = tokens.next(2);
	}


	// Check if we get an assignment meaning we need to have a variable name followed by exactly one equals.
	if (!(current.get_type()==VAR && next.get_type() == EQUAL && after_next.get_type() != EQUAL)) {
		parse_error("<variable_name> = <statement>", current.value + next.value, tokens);
	}
	// if we have a if then we can eat the name and the equal sign
	tokens.eat(); // eat variable name
	tokens.eat(); // eat equal sign

	// Now we build the statement that the variable will be assigned to
	Statement* assigned_statement = buildStatement(tokens);
	std::string variable_name = current.value; // This will be the variable name


	Expression* var_decl = new VariableDeclaration(variable_name, assigned_statement);
	return var_decl;
}


// This is a changed version of the VariableDeclaration for the for loop because it needs for now a hacky way to work properly
Expression* parseForChangerDeclaration(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();
	Token after_next = tokens.next(2);

	// Check if we get an assignment meaning we need to have a variable name followed by exactly one equals.
	if (!(current.get_type()==VAR && next.get_type() == EQUAL && after_next.get_type() != EQUAL)) {
		parse_error("<variable_name> = <statement>", current.value + next.value, tokens);
	}
	// if we have a if then we can eat the name and the equal sign
	tokens.eat(); // eat variable name
	tokens.eat(); // eat equal sign

	// Now we build the statement that the variable will be assigned to
	// Here is where is the difference between normal declaration
	Token* lparen = new Token(LPAREN, "(");
	Statement* assigned_statement = buildStatement(tokens, lparen);
	std::string variable_name = current.value; // This will be the variable name


	Expression* var_decl = new VariableDeclaration(variable_name, assigned_statement);
	return var_decl;
}

Expression* parseFor(Tokens& tokens){
	Token current = tokens.current();
	Token next = tokens.next();

	// Check if we get a for
	if (!(current.get_type()==KEYWORD && current.value == "for")) {
		parse_error("for (<init>;<condition>;<changer>;) <body>", current.value, tokens);
	}

	// Because for loop "params" can not be parsed as one statement we will split into 3 things. First it will start with a left parenthesis
	if (next.get_type() != LPAREN) {
		parse_error("(", next.value, tokens);
	}
	tokens.eat(); // eat for keyword
	tokens.eat(); // eat left parenthesis

	// We first get the variable declaration for the init "statement" in the for
	VariableDeclaration* init = (VariableDeclaration*) parseDeclaration(tokens);

	// The variable declaration will end with a semicolon
	current = tokens.current();
	if (current.get_type() != SEMICOLON) {
		parse_error(";", current.value, tokens);
	}
	tokens.eat(); // eat semicolon

	// Build the conditional statement for the for loop
	Statement* condition = buildStatement(tokens);
	// The conditional will end with a semicolon
	current = tokens.current();
	if (current.get_type() != SEMICOLON) {
		parse_error(";", current.value, tokens);
	}
	tokens.eat(); // eat semicolon

	// Build the changer statement
	VariableDeclaration* changer = (VariableDeclaration*) parseForChangerDeclaration(tokens);
	// The changer will end with ): but the ) is eaten in the variable declaration.
	current = tokens.current();
	if (current.get_type() != COLON) {
		parse_error(":", current.value, tokens);
	}
	tokens.eat(); // eat the colon
	current = tokens.current();
	
	bool one_line = false;
	if (current.get_type() != NL){
		one_line = true;
	}
	else {
		tokens.eat(); // eat newline
	}

	Block* body = parseBody(tokens, one_line);
	body->setBlockType(FOR);
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

Statement* buildStatement(Tokens& tokens, Token* optional_beginning_token){
	
	ReversePolishNotation rpn = convertInfixToRPN(tokens, optional_beginning_token);
	Statement* stm = parseRPN(rpn, tokens);
	return stm;
}


// ------------------------------------------------ Class Definitions ---------------------------------------------


Programm::Programm(std::vector<Expression*> expressions){
	this->expressions = expressions;
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

void Block::setBlockType(BlockType type) {
	this->type = type;
}

BlockType Block::getBlockType() {
	return this->type;
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

std::string Programm::toTreeString() {

	std::string ret = "Programm(";
	for (auto expression: this->expressions){
		ret = ret + expression->toTreeString() + ";";
	}
	// Remove last semicolon
	ret.pop_back();
	ret += ")";
	return ret;

}

FunctionDefinition::FunctionDefinition(std::string function_name, std::vector<std::string> param_names, Block* body) {
	this->name = function_name;
	this->param_names = param_names;
	this->body = body;
}

std::string FunctionDefinition::toTreeString() {
	std::string ret = "FunctionDefinition(" + this->name + ";Params(";
	for (auto name: param_names) {
		ret = ret + name + ";";
	}
	ret.pop_back();
	ret += ");";
	ret += this->body->toTreeString();
	ret += ")";
	return ret;

}

Return::Return(Statement* ret_value) {
	this->return_value = ret_value;
}

std::string Return::toTreeString() {
	return "Return(" + this->return_value->toTreeString() + ")";
}

Break::Break() {};

std::string Break::toTreeString() {
	return "Break()"; 
}

std::vector<Expression*> Block::getExpressions() {
	return this->expressions;
}

Debug::Debug() {};

std::string Debug::toTreeString() {
	return "Debug()";
}
