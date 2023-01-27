#ifndef lexer
#define lexer

// Token types enum
enum TYPE {
	NUM,
	DES,
	PLUS,
	MINUS,
	MULT,
	DIV,
	LPAR,
	RPAR,
	WS,
	NL,
	EQUAL,
	KEYWORD,
	END

};

// Classes
class Token{
	public:
		TYPE type;
		std::string value;
		Token(TYPE t, std::string v);
		TYPE get_type();
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
		Tokens lex(std::string raw_input);
};
// Functions
bool is_keyword(std::string lexing_string, int pos);
Token extract_keyword(std::string lexing_string, int& pos);
void lex_error(int pos, std::string lexing_string);
void lex_error(std::string error_message);
std::string enum_to_string(TYPE);


#endif
