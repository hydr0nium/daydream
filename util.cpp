
#include "util.h"
#include "lexer.h"
#include <iostream>
#include <optional>
#include "parser.h"
#include "statement.h"

void eat_whitespace(Tokens& tokens) {
	Token next_token = tokens.next();
	while (next_token.get_type()==WS){
		tokens.eat();
		next_token = tokens.next();
	}

}

std::string enum_to_string(TokenType t) {

	switch (t) {
		case NUM: return "Number";
		case VAR: return "Descriptor";
		case PLUS: return "Plus";
		case MINUS: return "Minus";
		case TIMES: return "Multiplication";
		case SLASH: return "Divide";
		case LPAREN: return "Left Parenthesis";
		case RPAREN: return "Right Parenthesis";
		case WS: return "White Space";
		case NL: return "New Line";
		case EQUAL: return "Equal";
		case KEYWORD: return "Keyword";
		case LESS: return "Less";
		case GREATER: return "Greater";
		case COMMA: return "Comma";
		case END: return "End of File";
		default: lex_error("Token index not found: " + t); exit(1); 
	}

}

void lex_error(int pos, std::string lexing_string) {

	std::string spaces_hat = "";
	std::string spaces_pipe = "";
	for (int i = 0; i < pos; i++) {
		spaces_hat += " ";
		spaces_pipe += " ";
	}
	spaces_hat += "^\n";
	spaces_pipe += "|\n";
	if (lexing_string[lexing_string.size()-1]!='\n') {
		lexing_string += "\n";
	}
	std::cout << "Encountered lexing error at position " << pos << "\n" << lexing_string << spaces_hat << spaces_pipe << std::endl;
	exit(1);

}

void lex_error(std::string error_message) {

	std::cout << error_message << std::endl;
	exit(1);

}

std::optional<Token> lex_keyword(std::string lexing_string, int& pos) {
	
	
	std::vector<std::string> keywords = {
	"if",
	"for",
	"while",
	"true",
	"false",
	"return",
	"and",
	"or",
	"not",
	"end"
	};
	int lexing_string_len = lexing_string.size();
	std::string current_token_value = "";
	int pos_before = pos;
	if (isalpha(lexing_string[pos])){
			current_token_value += lexing_string[pos];
			pos++;
			while (pos < lexing_string_len && (isalnum(lexing_string[pos]) || lexing_string[pos]=='_')){
				current_token_value += lexing_string[pos];
				pos++;
			}
			for(auto keyword: keywords){
				if (current_token_value==keyword){
					Token token(KEYWORD, current_token_value);
					return token;
				}
			}
			pos = pos_before;
			return std::nullopt;
	}
	else {
		pos = pos_before;
		return std::nullopt;
	}
}

void parse_error(std::string expected, std::string found){

	std::cout << "Parse Error!\nTried to parse Token as " << expected << " but found: " << found << std::endl;
	std::cout << "Quitting!" << std::endl;
	exit(1);

}

void parse_error(std::string error_message) {

	std::cout << error_message << std::endl;
	exit(1);

}

void pushOperatorToStack(std::stack<StatementHelper>& operatorStack, StatementHelper& op, std::vector<StatementHelper>& queue){
	int operator_prec = getOperatorPrecedence(op);
	if (op.type == "**") {
		while (operatorStack.size()>0 && getOperatorPrecedence(operatorStack.top())>operator_prec){
			StatementHelper operator_top = operatorStack.top();
			operatorStack.pop();
			queue.push_back(operator_top);
			
		}
	}
	else {
		while (operatorStack.size()>0 && getOperatorPrecedence(operatorStack.top())>=operator_prec){
			StatementHelper operator_top = operatorStack.top();
			operatorStack.pop();
			queue.push_back(operator_top);
		}
	}
	operatorStack.push(op);

}

int getOperatorPrecedence(StatementHelper& op){

		// Operator Precedence 
		/*
		Highest
		6 not
		5 **
		4 /, *
		3 + -
		2 and
		1 or
		0 ==, >, <, >=, <=
		Lowest
		*/

	std::string op_value = op.type;
	if (op_value == "not"){
		return 6;
	}
	else if (op_value == "**"){
		return 5;
	}
	else if (op_value == "/" || op_value == "*"){
		return 4;
	}
	else if (op_value == "+" || op_value == "-"){
		return 3;
	}
	else if (op_value == "and"){
		return 2;
	}
	else if (op_value == "or"){
		return 1;
	}
	else if (op_value == "==" || op_value == ">" || op_value == "<" || op_value == ">=" || op_value == "<="){
		return 0;
	}
	else if (op_value == "(") {
		return -1;
	}
	else {
		std::cout << "Operator not found: " +  op_value << std::endl;
		exit(1);
	}

}
