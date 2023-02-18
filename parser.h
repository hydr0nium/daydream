#ifndef parser
#define parser

#include "lexer.h"
#include <string>
#include <stack>

// Enums

enum ReturnType {
	NONE_TYPE,
	INT_TYPE,
	FLOAT_TYPE,
	STRING_TYPE,
	OBJECT_TYPE,
	VAR_ASSIGNMENT_TYPE,
	FUNC_ASSIGNMENT_TYPE,
	BREAK_TYPE
};

enum BlockType {
	FUNC,
	WHILE,
	FOR,
	IF
};

// Classes

class Expression {
	public:
		Expression();
		virtual std::string toTreeString() = 0;
		// virtual Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&) = 0;
};

class Programm: public Expression {
	public:
		Programm(std::vector<Expression*>);
		std::string toTreeString();
		void run();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		std::vector<Expression*> expressions;
};

class Parser {
	public:
		Programm parse(Tokens);
		Parser();
};

class Statement: public Expression {
	public:
		Statement();
		virtual std::string toString() = 0;
		virtual std::string toTreeString() = 0;
		// virtual Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&) = 0;

};

class VariableDeclaration: public Expression {

	public:
		VariableDeclaration(std::string, Statement*);
		std::string toTreeString();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		std::string var_name;
		Statement* assigned_stm;

};

class Block: public Expression {

	public:
		Block(std::vector<Expression*>);
		std::string toTreeString();
		void setBlockType(BlockType);
		BlockType getBlockType();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		BlockType type;
		std::vector<Expression*> expressions;

};

class If: public Expression {
	public:
		If(Statement*, Block*);
		std::string toTreeString();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		Statement* condition;
		Block* body;
};

class While: public Expression {
	public:
		While(Statement*, Block*);
		std::string toTreeString();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		Statement* condition;
		Block* body;
};

class For: public Expression {
	public:
		For(VariableDeclaration*, Statement*, VariableDeclaration*, Block*);
		std::string toTreeString();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		VariableDeclaration* init;
		Statement* condition;
		VariableDeclaration* changer;
		Block* body;
};

class Return: public Expression {
	public:
		Return(Statement*);
		std::string toTreeString();
	private:
		Statement* return_value;
};

class Break: public Expression {
	public:
		Break();
		std::string toTreeString();
};

class FunctionDefinition: public Expression {
	public:
		FunctionDefinition(std::string, std::vector<std::string>, Block*);
		std::string toTreeString();
		// Return eval(VariableScope&, VariableScope&, FunctionScope&, FunctionScope&);
	private:
		std::string name;
        std::vector<std::string> param_names;
		Block* body;
};


// Functions
Expression* parseDeclaration(Tokens&);
Expression* parseIf(Tokens&);
Expression* parseLine(Tokens&);
Expression* parseWhile(Tokens&);
Expression* parseFor(Tokens&);
Expression* parseForChangerDeclaration(Tokens&);
Expression* parseFunctionDefinition(Tokens&);
Expression* parseReturn(Tokens&);
Expression* parseBreak(Tokens&);
std::vector<std::string> parseFunctionDefinitionParams(Tokens&);
Statement* buildStatement(Tokens&, Token* = NULL);
Block* parseBody(Tokens&, bool);



#endif