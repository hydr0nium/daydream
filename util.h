#ifndef util
#define util

#include <string>
#include "lexer.h"
#include <optional>

void eat_whitespace(Tokens& tokens);
std::string enum_to_string(TokenType t);
void lex_error(int pos, std::string lexing_string);
void lex_error(std::string error_message);
void parse_error(std::string expected, std::string found);
std::optional<Token> lex_keyword(std::string lexing_string, int& pos);

#endif
