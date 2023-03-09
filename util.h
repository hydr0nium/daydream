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
std::string enum_to_string(ReturnType t);
std::string enum_to_string(ValueType t);
void lex_error(int pos, std::string lexing_string);
void lex_error(std::string error_message);
void parse_error(std::string, std::string, Tokens);
void parse_error(std::string, Tokens);
void eval_error(std::string, std::string, int id);
void eval_error(std::string, int id);
void pushOperatorToStack(std::stack<StatementHelper>& operatorStack, StatementHelper& op, std::vector<StatementHelper>& queue);
int getOperatorPrecedence(StatementHelper& op);
void debug(Tokens);
std::string boolToString(bool);
bool stringToBool(std::string);
int ipow(int, int);
Statement* convertReturnToExpression(ReturnValue);
std::optional<Token> lex_keyword(std::string lexing_string, int& pos);

#endif
