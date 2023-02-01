#ifndef parser
#define parser

#include "lexer.h"
#include <string>
#include <stack>

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
		virtual std::string toString() = 0;

};

class Number: public Statement {
	public:
		std::string value;
		Number(std::string value);
		std::string toString();
};

class Params: public Statement {
	public:
		std::vector<Statement*> params;
		Params();
		Params(std::vector<Statement*> params);
		std::string toString();
};

class FunctionCall: public Statement {
	public:
		Params params;
		FunctionCall(Params params);
		std::string toString();
};

class Variable: public Statement {
	public:
		std::string name;
		Variable(std::string name);
		std::string toString();

};

class Multiplication: public Statement {
	public:
		Multiplication();
		std::string toString();
};

class Minus: public Statement {
	public:
		Minus();
		std::string toString();

};

class Plus: public Statement {
	public:
		Plus();
		std::string toString();

};

class Divide: public Statement {
	public:
		Divide();
		std::string toString();
};

class Equality: public Statement {
	public:
		Equality();
		std::string toString();
};

class Power: public Statement {
	public:
		Power();
		std::string toString();
};

class Not: public Statement {
	public:
		Not();
		std::string toString();
};

class And: public Statement {
	public:
		And();
		std::string toString();
};

class Or: public Statement {
	public:
		Or();
		std::string toString();
};

class Bool: public Statement {
	public:
		std::string truth;
		Bool(std::string truth);
		std::string toString();
};

class StatementHelper {
	public:
		Statement* statement;
		std::string type;
		StatementHelper(std::string type, Statement* statement);
};



// Functions
Statement* buildStatement(Tokens tokens);
void parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseNum(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseVariableOrFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseMultiplication(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseEquality(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue);
void parsePower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseMultOrPower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseLogicalOperator(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue);
void parseBool(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseKeyword(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue);
void parsePlus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);

std::ostream& operator<<(std::ostream& os, const std::vector<StatementHelper> queue);
std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack);


#endif