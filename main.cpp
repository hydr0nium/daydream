#include <iostream>
#include "lexer.h"
#include "parser.h"
\
int main(){

	// I just noticed that my language looks a lot like Ruby but that was unintentional. I haven't coded or seen basically any code in ruby at all.

	std::cout << "-------------\nStarting Interpreter:\n";
	std::string input = "while (3-2==1):\n\t3+4\nend";
	std::cout << "Using:\n" << input << std::endl;
	Lexer lexr;
	std::cout << "[!] Starting Lexing\n";
	Tokens tokens = lexr.lex(input);
	std::cout << "[!] Finished Lexing\n";
	Parser p;
	std::cout << "[!] Starting Parsing\n";
	Programm programm = p.parse(tokens);
	std::cout << "[!] Finished Parsing\n";
	std::cout << programm.toTreeString() << std::endl;
	return 0;

}