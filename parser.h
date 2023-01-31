#ifndef parser
#define parser

#include "lexer.h"
#include <string>

// Classes
class Ast {
	public:
		Ast();
};

class Parser {
	public:
		Ast parse(Tokens tokens);
		Parser();
};

class Statement {
	public:
		Statement();

};

class Number: public Statement {
	public:
		std::string value;
		Number(std::string value);
};

class FunctionCall: public Statement {
	public:
		FunctionCall();
};

class Variable: public Statement {
	public:
		std::string name;
		Variable(std::string name);

};

class Multiplication: public Statement {
	public:
		Multiplication();
};

class Minus: public Statement {
	public:
		Minus();

};

class Plus: public Statement {
	public:
		Plus();

};

class Divide: public Statement {
	public:
		Divide();
};

class Equality: public Statement {
	public:
		Equality();
};

class Power: public Statement {
	public:
		Power();
};

class Not: public Statement {
	public:
		Not();
};

class And: public Statement {
	public:
		And();
};

class Or: public Statement {
	public:
		Or();
};

class Bool: public Statement {
	public:
		std::string truth;
		Bool(std::string truth);
};

class StatementHelper {
	public:
		Statement* statement;
		std::string type;
		StatementHelper(std::string type, Statement* statement);

};



// Functions
//Assignment parse_assignment(Tokens& tokens);
//Exp* parse_expression(Tokens& tokens);
Statement* buildStatement(Tokens tokens);
void parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue);


#endif
