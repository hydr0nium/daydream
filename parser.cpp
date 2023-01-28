#include "util.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>


Ast Parser::parse(Tokens tokens) {
	Token next_token = tokens.next();
	if (next_token.get_type()==VAR){
		
		Assignment assign = parse_assignment(tokens);
		std::cout << "Successfully parsed assignment with: " << assign.name.value << "=" << (assign.value)->get_value() << std::endl;
	}
	return Ast();
}

Assignment parse_assignment(Tokens& tokens) {
	Token next_token = tokens.next();
    Descriptor descriptor(next_token.value);
	Exp* exp;
	tokens.eat();
	eat_whitespace(tokens);	
	next_token = tokens.next();
	if (next_token.get_type() == EQUAL){
		tokens.eat();
		eat_whitespace(tokens);
		exp = parse_expression(tokens);
	}
	eat_whitespace(tokens);
	next_token = tokens.next();
	if(!(next_token.get_type()==NL || next_token.get_type()==END)){
		std::cout << "There was an error parsing!" << std::endl;
	}
	Assignment a(descriptor, exp);
	return a;
}

Exp* parse_expression(Tokens& tokens) {
	Token next_token = tokens.next();
	if (next_token.get_type()==INT){
		Int* num = new Int(next_token.value);
		tokens.eat();
		return num;
	}
	std::cout << "Parsing error expression not found" << std::endl;
	exit(1);
}


// Class Definitions


Ast::Ast(){

}

Descriptor::Descriptor(std::string v) {
	value = v;
}


Assignment::Assignment(Descriptor d, Exp* v) {
	name = d;
	value = v;
}

Int::Int(std::string v) {

	value = v;

}

std::string Int::get_value() {
	return value;
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
