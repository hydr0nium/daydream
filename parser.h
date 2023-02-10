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
		VariableDeclaration();
	private:
		std::string var_name;
		Statement* assigned_stm;

};


// Functions



#endif