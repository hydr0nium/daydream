#include <iostream>
#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]){


	// I know this is a bad way because the runtime will be bad (reading file then turning it into a string and then reading it again)
	// but this works for now and the runtime gain will not be too much.
	if (argc != 2) {
		std::cout << "Usage: daydream <file>.dd\n";
		exit(1);
	}
	std::ifstream fs(argv[1]);
	if(fs.fail()) {
		std::cout << "Could not find file with path: " << argv[1] << std::endl;
		exit(1);
	}
	std::stringstream source_code;
	source_code << fs.rdbuf();
	std::string input = source_code.str();
	


	// I just noticed that my language looks a lot like Ruby but that was unintentional. I haven't coded or seen basically any code in ruby at all.

	//std::cout << "-------------\nStarting Interpreter:\n";
	//std::cout << "Source Code:\n" << input << std::endl;
	Lexer lexr;
	//std::cout << "[!] Starting Lexing\n";
	Tokens tokens = lexr.lex(input);
	//std::cout << "[!] Finished Lexing\n";
	Parser p;
	//std::cout << "[!] Starting Parsing\n";
	Programm programm = p.parse(tokens);
	//std::cout << programm.toTreeString() << std::endl;
	//std::cout << "[!] Finished Parsing\n";
	//std::cout << "[!] Running Programm\n\n";
	programm.run();
	std::cout << std::endl;
	return 0;

}