#ifndef lexer
#define lexer

#include <vector>
#include <string>

// Token types enum
enum TokenType {
	INT,
	VAR,
	PLUS,
	MINUS,
	TIMES,
	SLASH,
	LPAREN,
	RPAREN,
	WS,
	NL,
	EQUAL,
	KEYWORD,
	END

};

// Classes
class Token{
	public:
		TokenType type;
		std::string value;
		Token(TokenType type, std::string value);
		TokenType get_type();
		// Overwriting stream operator << to print the Token
		friend std::ostream& operator<<(std::ostream& os, const Token& t);
};

class Tokens {
	public:
		std::vector<Token> tokens;
		Tokens(std::vector<Token> t);
		Token next();
		Tokens next(int n);
		void eat();
		void eat(int n);
};

class Lexer {
	public:
		Tokens lex(std::string programm);
};

#endif
