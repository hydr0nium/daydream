#include <iostream>
#include "lexer.h"
#include "parser.h"

int main(){
	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "function((test),test2,test3)";
	std::cout << "Using: " << input << std::endl;
	Lexer l;
	std::cout << "[!] Starting Lexing:\n";
	Tokens tokens = l.lex(input);
	std::cout << "[o] Found these Tokens while lexing:\n";
	for (auto token:tokens.tokens){
		std::cout << token << std::endl;
	}
	std::cout << "[!] Starting Parsing:\n";
	Parser p;
	p.parse(tokens);
	return 0;

}