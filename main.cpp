#include <iostream>
#include "lexer.h"
#include "parser.h"

int main(){
	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "test_func(5+5==10, true)";
	// Input will turn into:
	// 3 4 5 * 25 / + 4 - false == true <= 3 4 and 5 false not and or >=
	std::cout << "Using: " << input << std::endl;
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