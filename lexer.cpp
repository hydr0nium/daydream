
#include <cctype>
#include "lexer.h"
#include "util.h"

Token::Token(TYPE t, std::string v){
			type = t;
			value = v;
}

TYPE Token::get_type() {
	return type;
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
			os << "Token(" << enum_to_string(t.type) << ", '" << t.value << "')\n";
			return os;
		}

Tokens::Tokens(std::vector<Token> t){
	tokens = t;	
}


Token Tokens::next(){
	return tokens.front();	
}

Tokens Tokens::next(int n) {
	std::vector<Token> t;
	for(int i = 0; i < n; i++){
		t.push_back(tokens.at(i));
	}
	return Tokens(t);
}

void Tokens::eat() {
	tokens.erase(tokens.begin());
}

void Tokens::eat(int n) {

	for (int i = 0; i < n; i++) {
		tokens.erase(tokens.begin());
	}

}


bool is_keyword(std::string lexing_string, int pos) {
	
	
	std::vector<std::string> keywords = {
	"if",
	"for",
	"while",
	"true",
	"false",
	"print",
	"return"
	};
	int lexing_string_len = lexing_string.size();
	std::string current_token_value = "";
	if (isalpha(lexing_string[pos])){
			current_token_value += lexing_string[pos];
			pos++;
			while (pos < lexing_string_len && (isalnum(lexing_string[pos]) || lexing_string[pos]=='_')){
				current_token_value += lexing_string[pos];
				pos++;
			}
			for(auto keyword: keywords){
				if (current_token_value==keyword){
					return true;
				}
			}
			return false;
	}
	else {
		return false;
	}
}

Token extract_keyword(std::string lexing_string, int& pos) {

	TYPE t = KEYWORD;
	int lexing_string_len = lexing_string.size();
	std::string current_token_value = "";
	current_token_value += lexing_string[pos];
	pos++;
	while (pos < lexing_string_len && (isalnum(lexing_string[pos]) || lexing_string[pos]=='_')){
			current_token_value += lexing_string[pos];
			pos++;
	}
	Token token(t, current_token_value);
	return token;
}



Tokens Lexer::lex(std::string raw_input) {

	int pos = 0;
	std::string current_token_value = "";
	std::vector<Token> tokens;
	int input_len = raw_input.size();
	while (pos<input_len) {
		// Scanning for Number
		if (std::isdigit(raw_input[pos])){
			current_token_value += raw_input[pos];
			pos++;
			while (pos < input_len && std::isdigit(raw_input[pos])) {
				current_token_value += raw_input[pos];
				pos++;
			}
			TYPE t = NUM;
			Token token(t, current_token_value);
			tokens.push_back(token);
			current_token_value = "";
		}
		// Scanning for Multiplication
		else if (raw_input[pos]=='*') {
			TYPE t = MULT;
			Token token(t, "*");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Addition
		else if (raw_input[pos]=='+') {
			TYPE t = PLUS;
			Token token(t, "+");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Division
		else if (raw_input[pos]=='/') {
			TYPE t = DIV;
			Token token(t, "/");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Subtraction
		else if (raw_input[pos]=='-') {
			TYPE t = MINUS;
			Token token(t, "-");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for White Spaces
		else if (raw_input[pos]==' ') {
			TYPE t = WS;
			Token token(t, " ");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for New Line
		else if (raw_input[pos]=='\n') {
			TYPE t = NL;
			Token token(t, "\\n");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Left Parenthesis
		else if (raw_input[pos]=='('){
			TYPE t = LPAR;
			Token token(t, "(");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Right Parenthesis
		else if (raw_input[pos]==')'){
			TYPE t = RPAR;
			Token token(t, ")");
			tokens.push_back(token);
			pos++;
		}
		else if (raw_input[pos]=='='){
			TYPE t = EQUAL;
			Token token(t, "=");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for keywords
		else if (is_keyword(raw_input, pos)){
			Token token = extract_keyword(raw_input, pos);
			tokens.push_back(token);
			pos++;
		}
		// Scanning for variable names
		else if (isalpha(raw_input[pos])){
			current_token_value += raw_input[pos];
			pos++;
			while (pos < input_len && (isalnum(raw_input[pos]) || raw_input[pos]=='_')){
				current_token_value += raw_input[pos];
				pos++;
			}
			TYPE t = DES;
			Token token(t, current_token_value);
			tokens.push_back(token);
			current_token_value = "";

		}
		// Raise an exception because the next token could not be lexed
		else {
			lex_error(pos,raw_input);
		}
	}
	TYPE t = END;
	Token token(t, "EOF");
	tokens.push_back(token);
	return Tokens(tokens);
}


/*
int main(int argc , char** argv){
	
	std::string input = argv[1];
	std::cout << "Lexing String: " << input << std::endl;
	Lexer l;
	std::vector<Token> lexed = l.lex(input);
	for (Token t: lexed) {
	std::cout << t;
	}
	return 0;
}
*/
