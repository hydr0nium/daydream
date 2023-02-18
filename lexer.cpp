
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

// Constructor for Token Class empty
Token::Token(){}

// Return type of Token e.g KEYWORD or LPAREN
TokenType Token::get_type() {
	return type;
}

// Stream operator << for Token object to print it 
std::ostream& operator<<(std::ostream& os, const Token& token) {
			os << "Token(" << enum_to_string(token.type) << ", '" << token.value << "')";
			return os;
		}


// Construtor for Tokens Class
Tokens::Tokens(std::vector<Token> tokens, std::string source_code){
	this->tokens = tokens;	
	this->lineNumber = 1;
	this->source_code = source_code;
}

// Get next token in queue without removing them
Token Tokens::current(){
	return tokens.front();	
}

Token Tokens::next(){
	if ((this->current()).get_type()==EOF){
		std::cout << "Reached end of file. Can't get next Token! Quitting" << std::endl;
		exit(1);
	}
	return tokens.at(1);
}


// Get nth token in queue without removing it
Token Tokens::next(int index) {
	Token token;
	if (index>tokens.size()-1){
		std::cout << "Can not get token at index: " << index << " because tokens end at " << tokens.size()-1 << std::endl; 
		exit(1);
	}
	return tokens.at(index);
}

void Tokens::eat() {
	if (this->tokens.empty()) {
		std::cout << "Can not eat token because there are no tokens left." << std::endl; 
		exit(1);
	}
	Token current = this->current();
	if (current.get_type() == NL) {
		this->lineNumber += 1;
	}
	tokens.erase(tokens.begin());
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
			TokenType type = NUM;
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
			Token token(type, "newline");
			tokens.push_back(token);
			pos++;
		}
		// Scann for tab
		else if (programm[pos]=='\t') {
			TokenType type = TAB;
			Token token(type, "tab");
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
		// Scanning for Equal Sign
		else if (programm[pos]=='='){
			TokenType type = EQUAL;
			Token token(type, "=");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for less then
		else if (programm[pos]=='<') {
			TokenType type = LESS;
			Token token(type, "<");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for greater then
		else if (programm[pos]=='>') {
			TokenType type = GREATER;
			Token token(type, ">");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for comma
		else if (programm[pos]==',') {
			TokenType type = COMMA;
			Token token(type, ",");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for column
		else if (programm[pos]==':'){
			TokenType type = COLON;
			Token token(type, ":");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for semicolon
		else if (programm[pos]==';'){
			TokenType type = SEMICOLON;
			Token token(type, ";");
			tokens.push_back(token);
			pos++;
		}
		// Scanning for single quote 
		else if (programm[pos]=='\''){
			TokenType type = STRING;
			std::string value = "";
			pos++;

			// "Start reading string until another ' is read"
			while(programm[pos]!='\''){
				value += programm[pos];
				pos++;
			}
			Token token(type, value);
			tokens.push_back(token);
			pos++;
		}
		// Scanning for double quotes
		else if (programm[pos]=='"') {
			TokenType type = STRING;
			std::string value = "";
			pos++;

			// "Start reading string until another " is read"
			while(programm[pos]!='"'){
				value += programm[pos];
				pos++;
			}
			Token token(type, value);
			tokens.push_back(token);
			pos++;
		}

		// Scanning for keywords
		else if (std::optional<Token> token_opt = lex_keyword(programm, pos); token_opt.has_value()){
			Token token = token_opt.value();
			tokens.push_back(token);
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
	std::vector<Token> tokens_without_whitespace;
	for(auto token: tokens){
		if (token.get_type() != WS && token.get_type() != TAB){
			tokens_without_whitespace.push_back(token);
		}
	}

	return Tokens(tokens_without_whitespace, programm);
}

void Lexer::print(Tokens tokens){
	for (auto token:tokens.tokens){
		std::cout << token << std::endl;
	}
}

int Tokens::getLineNumber() {
	return this->lineNumber;
}

std::string Token::get_value() {
	return this->value;
}

std::string Tokens::getCodeContext() {

	// the line number feature is kinda broken will fix that later
	int ln = 1;
	int context = 2;
	std::string programm_context = "...\n" + std::to_string(ln) + "     ";
	if (ln == this->getLineNumber()){
				programm_context += "--> ";
	}
	for (auto chr: this->source_code){
		if (ln >= this->getLineNumber()-context && ln<= this->getLineNumber()+context) {
			programm_context += chr;
		}	
		if (chr == '\n'){
			ln++;
			if (ln == this->getLineNumber()){
				programm_context = programm_context + std::to_string(ln) + " --> ";
			}
			else {
				programm_context = programm_context + std::to_string(ln) + "     ";
			}
		}
	}
	programm_context += "\n...";
	return programm_context;

}



void Tokens::print() {
	for (auto token: this->tokens) {
		std::cout << token.value << std::endl;
	}
}