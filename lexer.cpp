
#include <cctype>
#include "lexer.h"
#include "util.h"
#include <iostream>
#include <optional>


// Constructor for Token Class
Token::Token(TokenType type, std::string value){
			this->type = type;
			this->value = value;
}

// Return type of Token e.g KEYWORD or LPAREN
TokenType Token::get_type() {
	return type;
}

// Stream operator << for Token object to print it 
std::ostream& operator<<(std::ostream& os, const Token& token) {
			os << "Token(" << enum_to_string(token.type) << ", '" << token.value << "')\n";
			return os;
		}


// Construtor for Tokens Class
Tokens::Tokens(std::vector<Token> tokens){
	this->tokens = tokens;	
}

// Get next token in queue without removing them
Token Tokens::next(){
	return tokens.front();	
}


// Get next n tokens in queue without removing them
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

// Lexing the inputed programm
Tokens Lexer::lex(std::string programm) {

	int pos = 0;
	std::string current_token_value = "";
	std::vector<Token> tokens;
	int input_len = programm.size();
	while (pos<input_len) {
		// Scanning for Number
		if (std::isdigit(programm[pos])){
			current_token_value += programm[pos];
			pos++;
			while (pos < input_len && std::isdigit(programm[pos])) {
				current_token_value += programm[pos];
				pos++;
			}
			TokenType type = INT;
			Token token(type, current_token_value);
			tokens.push_back(token);
			current_token_value = "";
		}
		// Scanning for Multiplication
		else if (programm[pos]=='*') {
			TokenType type = TIMES;
			Token token(type, "*");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Addition
		else if (programm[pos]=='+') {
			TokenType type = PLUS;
			Token token(type, "+");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Division
		else if (programm[pos]=='/') {
			TokenType type = SLASH;
			Token token(type, "/");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Subtraction
		else if (programm[pos]=='-') {
			TokenType type = MINUS;
			Token token(type, "-");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for White Spaces
		else if (programm[pos]==' ') {
			TokenType type = WS;
			Token token(type, " ");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for New Line
		else if (programm[pos]=='\n') {
			TokenType type = NL;
			Token token(type, "\\n");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Left Parenthesis
		else if (programm[pos]=='('){
			TokenType type = LPAREN;
			Token token(type, "(");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for Right Parenthesis
		else if (programm[pos]==')'){
			TokenType type = RPAREN;
			Token token(type, ")");
			tokens.push_back(token);
			pos++;
		}
		else if (programm[pos]=='='){
			TokenType type = EQUAL;
			Token token(type, "=");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for keywords
		else if (std::optional<Token> token_opt = lex_keyword(programm, pos); token_opt.has_value()){
			Token token = token_opt.value();
			tokens.push_back(token);
			pos++;
		}
		// Scanning for variable names
		else if (isalpha(programm[pos])){
			current_token_value += programm[pos];
			pos++;
			while (pos < input_len && (isalnum(programm[pos]) || programm[pos]=='_')){
				current_token_value += programm[pos];
				pos++;
			}
			TokenType type = VAR;
			Token token(type, current_token_value);
			tokens.push_back(token);
			current_token_value = "";

		}
		// Raise an exception because the next token could not be lexed
		else {
			lex_error(pos,programm);
		}
	}
	TokenType type = END;
	Token token(type, "EOF");
	tokens.push_back(token);
	return Tokens(tokens);
}
