#ifndef util
#define util

#include <string>
#include <vector>
#include <iostream>


class Tokens;
class Token;
enum TYPE;


void eat_whitespace(Tokens& tokens) {
	Token next_token = tokens.next();
	while (next_token.get_type()==WS){
		tokens.eat();
		next_token = tokens.next();
	}

}

std::string enum_to_string(TYPE t) {

	switch (t) {
		case NUM: return "Number";
		case DES: return "Descriptor";
		case PLUS: return "Plus";
		case MINUS: return "Minus";
		case MULT: return "Multiplication";
		case DIV: return "Divide";
		case LPAR: return "Left Parenthesis";
		case RPAR: return "Right Parenthesis";
		case WS: return "White Space";
		case NL: return "New Line";
		case EQUAL: return "Equal";
		case KEYWORD: return "Keyword";
		case END: return "End of File";
		default: lex_error("Token index not found: " + t); exit(1); 
	}

}

void lex_error(int pos, std::string lexing_string) {

	std::string spaces_hat = "";
	std::string spaces_pipe = "";
	for (int i = 0; i < pos; i++) {
		spaces_hat += " ";
		spaces_pipe += " ";
	}
	spaces_hat += "^\n";
	spaces_pipe += "|\n";
	if (lexing_string[lexing_string.size()-1]!='\n') {
		lexing_string += "\n";
	}
	std::cout << "Encountered lexing error at position " << pos << "\n" << lexing_string << spaces_hat << spaces_pipe << std::endl;
	exit(1);

}

void lex_error(std::string error_message) {

	std::cout << error_message;
	exit(1);

}


#endif
