#ifndef lexer
#define lexer

#include <vector>
#include <string>

// Token types enum
enum TokenType {
	NUM,
	VAR,
	PLUS,
	MINUS,
	TIMES,
	SLASH,
	LPAREN,
	RPAREN,
	WS,
	NL,
	TAB,
	EQUAL,
	KEYWORD,
	COMMA,
	COLON,
	LESS,
	GREATER,
	SEMICOLON,
	STRING_TOKEN,
	END
};

// Classes
class Token{
	public:
		TokenType type;
		std::string value;
		Token(TokenType type, std::string value);
		Token();
		TokenType get_type();
		// Overwriting stream operator << to print the Token
		friend std::ostream& operator<<(std::ostream& os, const Token& t);
};

class Tokens {
	public:
		std::vector<Token> tokens;
		Tokens(std::vector<Token>, std::string);
		Token current();
		Token next();
		Token next(int index);
		void eat();
		std::string getCodeContext();
		int getLineNumber();
		void print();
	private:
		int lineNumber;
		std::string source_code;
};

class Lexer {
	public:
		Tokens lex(std::string programm);
		void print(Tokens);
};

#endif
