#include <iostream>
#include "lexer.h"
#include "parser.h"
\
int main(){
	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "for (i=3;i<=10;i=i+1;)\n\tif var2==5\n\t\tvar3=false\n\tend\n\n\n\n\n\nend\na=5\n\n\n\n\n\n";
	// Input will turn into:
	// 3 4 5 * 25 / + 4 - false == true <= 3 4 and 5 false not and or >=
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