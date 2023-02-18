#ifndef util
#define util

#include <string>
#include "lexer.h"
#include <optional>
#include <stack>
#include "parser.h"
#include "statement.h"
#include <vector>

void eat_whitespace(Tokens& tokens);
std::string enum_to_string(TokenType t);
void lex_error(int pos, std::string lexing_string);
void lex_error(std::string error_message);
void parse_error(std::string, std::string, Tokens);
void parse_error(std::string, Tokens);
void eval_error(std::string, std::string);
void pushOperatorToStack(std::stack<StatementHelper>& operatorStack, StatementHelper& op, std::vector<StatementHelper>& queue);
int getOperatorPrecedence(StatementHelper& op);
void debug(Tokens);
std::optional<Token> lex_keyword(std::string lexing_string, int& pos);

#endif
