#include <iostream>
#include "lexer.h"
#include "parser.h"
\
int main(){
	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "s = 'hello world'";
	std::cout << "Using:\n" << input << std::endl;
	Lexer lexr;
	std::cout << "[!] Starting Lexing\n";
	Tokens tokens = lexr.lex(input);
	//std::cout << "[o] Found these Tokens while lexing:\n";
	//lexr.print(tokens);

	std::cout << "[!] Starting Parsing\n";
	Parser p;
	p.parse(tokens);
	return 0;

}