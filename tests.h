#ifndef test
#define test


int main();
void run_tests();

// Lex tests
void lex_single_number();
void lex_big_number();
void lex_lparen();
void lex_rparen();
void lex_comma();
void lex_greater();
void lex_less();
void lex_variable();
void lex_end();
void lex_keywords();
void lex_plus();
void lex_times();
void lex_slash();
void lex_minus();
void lex_newline();
void lex_string();

// Parse tests 
Programm parse_setup(std::string);
void parse_if();
void parse_for();
void parse_while();
void parse_func_def();
void parse_statements();
void parse_assignment();

#endif