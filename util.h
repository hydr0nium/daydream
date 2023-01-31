#ifndef util
#define util

#include <string>
#include "lexer.h"
#include <optional>
#include <stack>
#include "parser.h"
#include <vector>

void eat_whitespace(Tokens& tokens);
std::string enum_to_string(TokenType t);
void lex_error(int pos, std::string lexing_string);
void lex_error(std::string error_message);
void parse_error(std::string expected, std::string found);
void pushOperatorToStack(std::stack<StatementHelper>& operatorStack, StatementHelper& op, std::vector<StatementHelper>& queue);
int getOperatorPrecedence(StatementHelper& op);
std::optional<Token> lex_keyword(std::string lexing_string, int& pos);

#endif
