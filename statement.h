#ifndef statement
#define statement

#include "parser.h"

class Number: public Statement {
	public:
		Number(std::string);
		std::string toString();
		std::string toTreeString();
	private:
		std::string value;
};

class Params: public Statement {
	public:
		std::vector<Statement*> params;
		Params();
		Params(std::vector<Statement*>);
		std::string toString();
		std::string toTreeString();
};

class FunctionCall: public Statement {
	public:
		Params params;
		std::string functionName;
		FunctionCall(std::string, Params);
		std::string toTreeString();
		std::string toString();
};

class Variable: public Statement {
	public:
		Variable(std::string, VariableType);
		Variable(std::string);
		std::string toTreeString();
		std::string toString();
	private:
		std::string name;
		VariableType varType;

};

class Multiplication: public Statement {
	public:
		Multiplication();
		std::string toString();
		std::string toTreeString();
		Multiplication(Statement*, Statement*);
	private:
		Statement* RHS;
		Statement* LHS;
};

class Minus: public Statement {
	public:
		Minus();
		std::string toString();
		std::string toTreeString();
		Minus(Statement*, Statement*);
	private:
		Statement* RHS;
		Statement* LHS;

};

class Plus: public Statement {
	public:
		Plus();
		Plus(Statement*, Statement*);
		std::string toTreeString();
		std::string toString();
	private:
		Statement* RHS;
		Statement* LHS;

};

class Divide: public Statement {
	public:
		Divide();
		std::string toString();
		std::string toTreeString();
		Divide(Statement*, Statement*);
	private:
		Statement* LHS;
		Statement* RHS;
};

class Equality: public Statement {
	public:
		Equality();
		std::string toString();
		std::string toTreeString();
		Equality(Statement*, Statement*);
	private:
		Statement* LHS;
		Statement* RHS;
};

class Power: public Statement {
	public:
		Power();
		std::string toString();
		std::string toTreeString();
		Power(Statement*, Statement*);
	private:
		Statement* LHS;
		Statement* RHS;
};

class Not: public Statement {
	public:
		Not();
		std::string toString();
		std::string toTreeString();
		Not(Statement*);
	private:
		Statement* RHS;
};

class And: public Statement {
	public:
		And();
		std::string toString();
		std::string toTreeString();
		And(Statement*, Statement*);
	private:
		Statement* LHS;
		Statement* RHS;
};

class Or: public Statement {
	public:
		Or();
		std::string toString();
		std::string toTreeString();
		Or(Statement*, Statement*);
	private:
		Statement* LHS;
		Statement* RHS;
};

class Bool: public Statement {
	public:
		std::string truth;
		Bool(std::string);
		std::string toTreeString();
		std::string toString();
};

class StatementHelper {
	public:
		Statement* stm;
		std::string type;
		std::string toTreeString();
		StatementHelper(std::string type, Statement* stm);
};

class LParen: public Statement {
	public:
		LParen();
		std::string toString();
		std::string toTreeString();
};

class RParen: public Statement {
	public:
		RParen();
		std::string toString();
		std::string toTreeString();
};

class Less: public Statement {
	public:
		Less();
		std::string toString();
		Less(Statement*, Statement*);
		std::string toTreeString();
	private:
		Statement* LHS;
		Statement* RHS;
};

class Greater: public Statement {
	public:
		Greater();
		std::string toString();
		Greater(Statement*, Statement*);
		std::string toTreeString();
	private:
		Statement* LHS;
		Statement* RHS;

};

class LessEqual: public Statement {
	public:
		LessEqual();
		std::string toString();
		LessEqual(Statement*, Statement*);
		std::string toTreeString();
	private:
		Statement* LHS;
		Statement* RHS;
};

class GreaterEqual: public Statement {
	public:
		GreaterEqual();
		std::string toString();
		GreaterEqual(Statement*, Statement*);
		std::string toTreeString();
	private:
		Statement* LHS;
		Statement* RHS;
};

class String: public Statement {
    public:
        String(std::string);
        std::string toString();
        std::string toTreeString();
    private:
        std::string value;
};

void parseFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseNum(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseString(Tokens&, std::vector<StatementHelper>&);
void parseVariableOrFunctionCall(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseMultiplication(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseEquality(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue);
void parsePower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseMultOrPower(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseLogicalOperator(Tokens& tokens, std::stack<StatementHelper>& operators, std::vector<StatementHelper>& queue);
void parseBool(Tokens& tokens, std::vector<StatementHelper>& queue);
void parseKeyword(Tokens&, std::stack<StatementHelper>&, std::vector<StatementHelper>&, bool&);
void parsePlus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
Params parseParams(Tokens& tokens);
void parseGreater(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseLess(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseLessEqual(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseGreaterEqual(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parsePlus(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseMinus(Tokens&, std::stack<StatementHelper>&, std::vector<StatementHelper>&, bool&);
void parseSlash(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseRParen(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);
void parseLParen(Tokens& tokens, std::stack<StatementHelper>& operatorStack, std::vector<StatementHelper>& queue);


std::ostream& operator<<(std::ostream& os, const std::vector<StatementHelper> queue);
std::ostream& operator<<(std::ostream& os, const std::stack<StatementHelper> operatorStack);

#endif