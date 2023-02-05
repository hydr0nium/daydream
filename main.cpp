#include <iostream>
#include "lexer.h"
#include "parser.h"

int main(){
	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "3 + 4 *  5 / 25 - 4 == false <= true >= 3 and 4 or 5 and not false";
	std::cout << "Using: " << input << std::endl;
	Lexer lexr;
	std::cout << "[!] Starting Lexing\n";
	Tokens tokens = lexr.lex(input);
	//lexr.print(tokens);
	//std::cout << "[o] Found these Tokens while lexing:\n";

	std::cout << "[!] Starting Parsing\n";
	Parser p;
	p.parse(tokens);
	return 0;

}