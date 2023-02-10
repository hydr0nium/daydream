#ifndef reversePolish
#define reversePolish


#include "parser.h"
#include "lexer.h"
#include "statement.h"

class ReversePolishNotation {
    public:
		ReversePolishNotation(std::vector<StatementHelper>);
		std::string toString();
		std::vector<StatementHelper> getRpnVector();
	private:
		std::vector<StatementHelper> rpn;
};

ReversePolishNotation convertInfixToRPN(Tokens&);
Statement* parseRPN(ReversePolishNotation rpn);


#endif