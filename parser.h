#ifndef parser
#define parser

#include "lexer.h"
#include <string>
#include <stack>

// Enums

enum VariableType {
	UNKNOWN,
	INT,
	FLOAT,
	STRING,
	OBJECT
};

// Classes
class Programm {
	public:
		Programm();
};

class Parser {
	public:
		Programm parse(Tokens);
		Parser();
};

class Expression {
	public:
		Expression();
		virtual std::string toTreeString() = 0;
};

class Statement: public Expression {
	public:
		Statement();
		virtual std::string toString() = 0;
		virtual std::string toTreeString() = 0;

};

class VariableDeclaration: public Expression {

	public:
		VariableDeclaration(std::string, Statement*);
		std::string toTreeString();
	private:
		std::string var_name;
		Statement* assigned_stm;

};

class Block: public Expression {

	public:
		Block(std::vector<Expression*>);
		std::string toTreeString();
	private:
		std::vector<Expression*> expressions;

};

class If: public Expression {
	public:
		If(Statement*, Block*);
		std::string toTreeString();
	private:
		Statement* condition;
		Block* body;
};

class While: public Expression {
	public:
		While(Statement*, Block*);
		std::string toTreeString();
	private:
		Statement* condition;
		Block* body;
};

class For: public Expression {
	public:
		For(VariableDeclaration*, Statement*, VariableDeclaration*, Block*);
		std::string toTreeString();
	private:
		VariableDeclaration* init;
		Statement* condition;
		VariableDeclaration* changer;
		Block* body;
};


// Functions
Expression* parseDeclaration(Tokens&);
Expression* parseIf(Tokens&);
Expression* parseLine(Tokens&);
Expression* parseWhile(Tokens&);
Expression* parseFor(Tokens&);
Block* parseBody(Tokens&, bool);



#endif