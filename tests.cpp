#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "tests.h"
#include <cassert>

int main(){
    std::cout << "[!] Starting Tests\n";
	run_tests();
}

void run_tests(){
    lex_single_number();
    lex_big_number();
    lex_lparen();
    lex_rparen();
    lex_comma();
    lex_greater();
    lex_less();
    lex_variable();
    lex_end();
    lex_keywords();
    lex_plus();
    lex_times();
    lex_slash();
    lex_minus();
    lex_newline();
    std::cout << "[!] All tests passed\n";
    
}

void lex_single_number(){
    std::string input = "5";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="5" && tokens.current().type == NUM);
    
    input = "9";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="9" && tokens.current().type == NUM);
}

void lex_big_number() {
    std::string input = "1234567890";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="1234567890" && tokens.current().type == NUM);
    
    input = "23784";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="23784" && tokens.current().type == NUM);
}

void lex_lparen() {
    std::string input = "(";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="(" && tokens.current().type == LPAREN);
}

void lex_rparen() {
    std::string input = ")";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value==")" && tokens.current().type == RPAREN);
}

void lex_keywords() {
    std::string input = "if";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="if" && tokens.current().type == KEYWORD);

    input = "for";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="for" && tokens.current().type == KEYWORD);

    input = "while";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="while" && tokens.current().type == KEYWORD);

    input = "true";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="true" && tokens.current().type == KEYWORD);

    input = "false";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="false" && tokens.current().type == KEYWORD);

    input = "and";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="and" && tokens.current().type == KEYWORD);

    input = "or";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="or" && tokens.current().type == KEYWORD);

    input = "not";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="not" && tokens.current().type == KEYWORD);

}

void lex_plus(){
    std::string input = "+";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="+" && tokens.current().type == PLUS);
}

void lex_minus(){
    std::string input = "-";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="-" && tokens.current().type == MINUS);
}

void lex_times(){
    std::string input = "*";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="*" && tokens.current().type == TIMES);
}

void lex_slash(){
    std::string input = "/";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="/" && tokens.current().type == SLASH);
}

void lex_newline(){
    std::string input = "\n";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="newline" && tokens.current().type == NL);
}

void lex_end(){
    std::string input = "";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="EOF" && tokens.current().type == END);
}

void lex_equal(){
    std::string input = "=";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="=" && tokens.current().type == EQUAL);
}

void lex_less(){
    std::string input = "<";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="<" && tokens.current().type == LESS);
}

void lex_greater(){
    std::string input = ">";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value==">" && tokens.current().type == GREATER);
}

void lex_comma() {
    std::string input = ",";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="," && tokens.current().type == COMMA);
}

void lex_variable() {
    std::string input = "wijf2874";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="wijf2874" && tokens.current().type == VAR);

    input = "test_var";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="test_var" && tokens.current().type == VAR);

    input = "function(";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="function" && tokens.current().type == VAR);
}