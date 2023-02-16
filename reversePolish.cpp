
#include "reversePolish.h"
#include "parser.h"
#include "lexer.h"
#include "util.h"
#include <iostream>
#include "statement.h"

ReversePolishNotation::ReversePolishNotation(std::vector<StatementHelper> rpn){
	this->rpn = rpn;
}

std::string ReversePolishNotation::toString(){
	std::string out = "";
	for(auto helper: this->rpn){
		out += helper.stm->toString() + " ";
	}
	return out;
}

std::vector<StatementHelper> ReversePolishNotation::getRpnVector(){
	return this->rpn;
};

ReversePolishNotation convertInfixToRPN(Tokens& tokens, Token* optional_token_beginning){

	std::vector<StatementHelper> rpn_queue; // output queue in reverse polish notation
	std::stack<StatementHelper> operatorStack;

	// This is again a hacky way of adding a "nice" for loop. Basically I add a opening parenthesis in the declaration such that it looks like this:
	// for (init;condition;<var_name>=(<statement>):
	if(optional_token_beginning != NULL && optional_token_beginning->get_type() == LPAREN) {
		StatementHelper helper("(", new LParen());
		operatorStack.push(helper);
	}

	// Change while condition to be not one of the statement types
	Token current = tokens.current();
	bool was_operator = true; // this is a dirty way to deal with negative numbers but it works so I will leave it for now
	while(current.type != END && current.type != NL && current.type != COMMA && current.get_type() != COLON && current.value != "end" && current.get_type() != SEMICOLON){
		current = tokens.current();
		switch (current.get_type()) {
		case NUM:
			parseNum(tokens, rpn_queue);
			was_operator = false;
			break;
		case STRING:
			parseString(tokens, rpn_queue);
			was_operator = false;
			break;
		case VAR:
			parseVariableOrFunctionCall(tokens, rpn_queue);
			was_operator = false;
			break;
		case EQUAL:
			parseEquality(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case TIMES:
			parseMultOrPower(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case KEYWORD:
			parseKeyword(tokens, operatorStack, rpn_queue, was_operator);
			break;
		case SLASH:
			parseSlash(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case LESS:
			parseLess(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case GREATER:
			parseGreater(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case PLUS:
			parsePlus(tokens, operatorStack, rpn_queue);
			was_operator = true;
			break;
		case MINUS:
			parseMinus(tokens, operatorStack, rpn_queue, was_operator);
			// was_operator set in parseMinus
			break;
		case LPAREN:
			parseLParen(tokens, operatorStack, rpn_queue);
			// Parenthesis don't change if there was a operator or not
			break;
		case RPAREN:
			parseRParen(tokens, operatorStack, rpn_queue);
			// Parenthesis don't change if there was a operator or not
			break;
		default:
			// Temporary Solution
			parse_error("Valid Statement Token", current.value, tokens);
		}
		current = tokens.current();
			
	}

	current = tokens.current();
	// Pop all operators that are still on the operatorStack to the queue
	while(!operatorStack.empty()){
		StatementHelper helper = operatorStack.top();
		if (helper.type == "("){
			parse_error(")", current.value, tokens);
		}
		operatorStack.pop();
		rpn_queue.push_back(helper);
	}

	// Construct RPN object
	ReversePolishNotation rpn(rpn_queue);
	return rpn;

}

Statement* parseRPN(ReversePolishNotation rpn, Tokens tokens){


	std::stack<Statement*> statement_stack;
	for(auto helper: rpn.getRpnVector()){
		std::string statement_type = helper.type;
		Statement* stm = helper.stm;
		if (statement_type == "number") {
			statement_stack.push(stm);
		}
		else if (statement_type == "variable"){
			statement_stack.push(stm);
		}
		else if (statement_type == "bool") {
			statement_stack.push(stm);
		}
		else if (statement_type == "functionCall") {
			statement_stack.push(stm);
		}
		else if (statement_type == "string"){
			statement_stack.push(stm);
		}
		else if (statement_type == "+") {
			if( !(statement_stack.size()>=2) ) {
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form an addition object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Plus* plus_stm = new Plus(LHS, RHS);
			statement_stack.push(plus_stm);
			delete stm;
		}
		else if (statement_type == "*") { // Build Multiplication Object
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a multiplication object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Multiplication* mult_stm = new Multiplication(LHS, RHS);
			statement_stack.push(mult_stm);
			delete stm;
		}
		else if (statement_type == "-") { // Build Minus Object
			std::cout << statement_stack.top()->toTreeString() << std::endl;
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a subtraction object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Minus* minus_stm = new Minus(LHS, RHS);
			statement_stack.push(minus_stm);
			delete stm;
		}
		else if (statement_type == "/") { // Build Division
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a division object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Divide* div_stm = new Divide(LHS, RHS);
			statement_stack.push(div_stm);
			delete stm;
		}
		else if (statement_type == "**") { // Build Power Object
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a power object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Power* pow_stm = new Power(LHS, RHS);
			statement_stack.push(pow_stm);
			delete stm;
		}
		else if (statement_type == "and") { // Build logical and
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical and object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			And* and_stm = new And(LHS, RHS);
			statement_stack.push(and_stm);
			delete stm;
		}
		else if (statement_type == "not") { // Build logical not
			if ( !(statement_stack.size()>=1) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical not object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Not* not_stm = new Not(RHS);
			statement_stack.push(not_stm);
			delete stm;
		}
		else if (statement_type == "or") { // Build logical or
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a logical or object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Or* or_stm = new Or(LHS, RHS);
			statement_stack.push(or_stm);
			delete stm;
		}
		else if (statement_type == "<") { // Build less than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a less than object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Less* less_stm = new Less(LHS, RHS);
			statement_stack.push(less_stm);
			delete stm;
		}
		else if (statement_type == "<=") { // Build less equal than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a less or equal than object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			LessEqual* lessEq_stm = new LessEqual(LHS, RHS);
			statement_stack.push(lessEq_stm);
			delete stm;
		}
		else if (statement_type == ">") { // Build greater than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a greater than object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Greater* greater_stm = new Greater(LHS, RHS);
			statement_stack.push(greater_stm);
			delete stm;
		}
		else if (statement_type == ">=") { // Build greater equal than
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a greater or equal than object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			GreaterEqual* greaterEq_stm = new GreaterEqual(LHS, RHS);
			statement_stack.push(greaterEq_stm);
			delete stm;
		}
		else if (statement_type == "==") { // Build equality
			if ( !(statement_stack.size()>=2) ){
				parse_error("Could not convert RPN to AST. Not enough values on the statement stack to form a equality object", tokens);
			}
			Statement* RHS = statement_stack.top();
			statement_stack.pop();
			Statement* LHS = statement_stack.top();
			statement_stack.pop();
			Equality* eq_stm = new Equality(LHS, RHS);
			statement_stack.push(eq_stm);
			delete stm;
		}
		
		//std::cout << "Statement Stack:" << std::endl;
		//std::cout << statement_stack << std::endl;


	}

	if (statement_stack.empty()){
		parse_error("non empty statement", "empty statement", tokens);
	}
	else if (!(statement_stack.size()==1)){
		parse_error("Could not form AST from RPN. Statement Stack had more than one element. Expected was one.", tokens);
	}
	Statement* return_stm = statement_stack.top();
	statement_stack.pop();
	return return_stm;
}
