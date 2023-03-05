#ifndef parser
#define parser

#include "lexer.h"
#include <string>
#include <stack>
#include "interpreter.h"

// Enum


class FuncInScope;
class VarInScope;
class ReturnValue;
class VarScope;
class FuncScope;


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
		virtual ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&) = 0;
};

class Program: public Expression {
	public:
		Program(std::vector<Expression*>);
		std::string toTreeString();
		void run();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
	private:
		std::vector<Expression*> expressions;
};

class Parser {
	public:
		Program parse(Tokens);
		Parser();
};

class Statement: public Expression {
	public:
		Statement();
		virtual std::string toString() = 0;
		virtual std::string toTreeString() = 0;
		virtual ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&) = 0;

};

class VariableDeclaration: public Expression {

	public:
		VariableDeclaration(std::string, Statement*);
		std::string toTreeString();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
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
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
		std::vector<Expression*> getExpressions();
	private:
		BlockType type;
		std::vector<Expression*> expressions;

};

class If: public Expression {
	public:
		If(Statement*, Block*);
		std::string toTreeString();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
	private:
		Statement* condition;
		Block* body;
};

class While: public Expression {
	public:
		While(Statement*, Block*);
		std::string toTreeString();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
	private:
		Statement* condition;
		Block* body;
};

class For: public Expression {
	public:
		For(VariableDeclaration*, Statement*, VariableDeclaration*, Block*);
		std::string toTreeString();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
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
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
	private:
		Statement* return_value;
};

class Break: public Expression {
	public:
		Break();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
		std::string toTreeString();
};

class FunctionDefinition: public Expression {
	public:
		FunctionDefinition(std::string, std::vector<std::string>, Block*);
		std::string toTreeString();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
	private:
		std::string name;
        std::vector<std::string> param_names;
		Block* body;
};

class Debug: public Expression {
	public:
		Debug();
		ReturnValue eval(VarScope&, VarScope&, FuncScope&, FuncScope&);
		std::string toTreeString();

};

// Functions
Expression* parseDeclaration(Tokens&);
Expression* parseIf(Tokens&);
Expression* parseNextExpression(Tokens&, bool = true);
Expression* parseWhile(Tokens&);
Expression* parseFor(Tokens&);
Expression* parseForChangerDeclaration(Tokens&);
Expression* parseFunctionDefinition(Tokens&);
Expression* parseReturn(Tokens&);
Expression* parseBreak(Tokens&);
Expression* parseDebug(Tokens&);
std::vector<std::string> parseFunctionDefinitionParams(Tokens&);
Statement* buildStatement(Tokens&, Token* = NULL);
Block* parseBody(Tokens&, bool);


#endif