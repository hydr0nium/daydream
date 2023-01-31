#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>


Ast Parser::parse(Tokens tokens) {
	buildStatement(tokens);
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
		parseFunctionCall(tokens, queue);
	}
	else {
		Variable* variable = new Variable(current.value);
		tokens.eat();
		StatementHelper helper("variable", variable);
		queue.push_back(helper);
	}
}

void parseMultiplication(Tokens& tokens){
	Token current = tokens.current();

	// TODO
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

void parsePower(Tokens& tokens, std::stack<StatementHelper>& operators){
	//TODO
	tokens.eat();
	tokens.eat();
	
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

std::ostream& operator<<(std::ostream& os, const std::vector<StatementHelper> queue) {
	for (auto helper: queue){
		os << helper.type << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack) {

    	os << operatorStack.top().type << std::endl;
		return os;
}


Statement* buildStatement(Tokens tokens){
	
	std::vector<StatementHelper> queue;
	std::stack<StatementHelper> operators;
	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	while(current.type != END && current.type != NL){
		current = tokens.current();
		std::cout << "Parsing " << enum_to_string(current.type) << " with value " << current.value << std::endl;
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, queue);
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, queue);
			break;
		case EQUAL:
			parseEquality(tokens, operators, queue);
			break;
		case TIMES:
			// Change this name
			parsePower(tokens, operators);
			break;
		case KEYWORD:
			parseKeyword(tokens, operators, queue);
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
	std::cout << queue;
	std::cout << operators;
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

FunctionCall::FunctionCall(){}

StatementHelper::StatementHelper(std::string type, Statement* statement){
	this->type = type;
	this->statement = statement;

}


int main(){

	std::string input = "5 var ==";
	std::cout << "Using: " << input << std::endl;
	Lexer l;
	Tokens tokens = l.lex(input);
	Parser p;
	p.parse(tokens);
	for (auto token:tokens.tokens){
		std::cout << token;
	}

}
