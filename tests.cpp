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

    // Lexing tests
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
    lex_string();
    std::cout << "[!] All lexing tests passed\n";

    // Parsing tests
    parse_for();
    parse_while();
    parse_assignment();
    parse_statements();
    parse_if();
    parse_func_def();

    std::cout << "[!] All parsing tests passed\n";
    
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

    input = "break";
    tokens = lexr.lex(input);
    assert(tokens.current().get_value()=="break" && tokens.current().get_type() == KEYWORD);

    input = "return";
    tokens = lexr.lex(input);
    assert(tokens.current().get_value()=="return" && tokens.current().get_type() == KEYWORD);

    input = "def";
    tokens = lexr.lex(input);
    assert(tokens.current().get_value()=="def" && tokens.current().get_type() == KEYWORD);

    input = "end";
    tokens = lexr.lex(input);
    assert(tokens.current().get_value()=="end" && tokens.current().get_type() == KEYWORD);

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

void lex_string() {
    std::string input = "\"hello world\"";
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    assert(tokens.current().value=="hello world" && tokens.current().get_type() == STRING);

    input = "'hello world 2'";
    tokens = lexr.lex(input);
    assert(tokens.current().value=="hello world 2" && tokens.current().get_type() == STRING);


}

Program parse_setup(std::string input) {
    Lexer lexr;
    Tokens tokens = lexr.lex(input);
    Parser parsr;
    Program programm = parsr.parse(tokens);
    return programm;
}

void parse_while() {

    Program programm = parse_setup("while (true): my_var");
    assert(programm.toTreeString() == "Program(While(Bool(true);Block(Variable(my_var))))");
    programm = parse_setup("while (3-2==1):\n\t3+4\nend");
    assert(programm.toTreeString() == "Program(While(Equality(Minus(Number(3);Number(2));Number(1));Block(Plus(Number(3);Number(4)))))");


}


void parse_for() {

    Program programm = parse_setup("for (i=0;i<10;i = i+1): a=5");
    assert(programm.toTreeString() == "Program(For(VariableDeclaration(i;Number(0));Less(Variable(i);Number(10));VariableDeclaration(i;Plus(Variable(i);Number(1)));Block(VariableDeclaration(a;Number(5)))))");

    programm = parse_setup("for (i=0;i<10;i = i+1):\n\ta=5\nend");
    assert(programm.toTreeString() == "Program(For(VariableDeclaration(i;Number(0));Less(Variable(i);Number(10));VariableDeclaration(i;Plus(Variable(i);Number(1)));Block(VariableDeclaration(a;Number(5)))))");

}


void parse_statements() {

    Program programm = parse_setup("'hello world'");
    assert(programm.toTreeString() == "Program(String(hello world))");

    programm = parse_setup("a**b");
    assert(programm.toTreeString() == "Program(Power(Variable(a);Variable(b)))");

    programm = parse_setup("3==5");
    assert(programm.toTreeString() == "Program(Equality(Number(3);Number(5)))");

    programm = parse_setup("a and b");
    assert(programm.toTreeString() == "Program(And(Variable(a);Variable(b)))");

    programm = parse_setup("5 or true");
    assert(programm.toTreeString() == "Program(Or(Number(5);Bool(true)))");

    programm = parse_setup("not true");
    assert(programm.toTreeString() == "Program(Not(Bool(true)))");

    programm = parse_setup("func(a,b)");
    assert(programm.toTreeString() == "Program(FunctionCall(func;Params(Variable(a);Variable(b))))");

    programm = parse_setup("a/5");
    assert(programm.toTreeString() == "Program(Divide(Variable(a);Number(5)))");

    programm = parse_setup("a<5");
    assert(programm.toTreeString() == "Program(Less(Variable(a);Number(5)))");

    programm = parse_setup("a>5");
    assert(programm.toTreeString() == "Program(Greater(Variable(a);Number(5)))");

    programm = parse_setup("break");
    assert(programm.toTreeString() == "Program(Break())");


}


void parse_func_def() {

    Program programm = parse_setup("def add(a,b): return a+b");
    assert(programm.toTreeString() == "Program(FunctionDefinition(add;Params(a;b);Block(Return(Plus(Variable(a);Variable(b))))))");

    programm = parse_setup("def mult(a,b):\n\treturn a*b\nend");
    assert(programm.toTreeString() == "Program(FunctionDefinition(mult;Params(a;b);Block(Return(Multiplication(Variable(a);Variable(b))))))");

}


void parse_if() {

    Program programm = parse_setup("if a==b: i=3");
    assert(programm.toTreeString() == "Program(If(Equality(Variable(a);Variable(b));Block(VariableDeclaration(i;Number(3)))))");

    programm = parse_setup("if my_var>=4:\n\tthis=that\nend");
    assert(programm.toTreeString() == "Program(If(GreaterEqual(Variable(my_var);Number(4));Block(VariableDeclaration(this;Variable(that)))))");


}

void parse_assignment() {
    Program programm = parse_setup("var = 50");
    assert(programm.toTreeString()=="Program(VariableDeclaration(var;Number(50)))");

    programm = parse_setup("var50 = 3+4");
    assert(programm.toTreeString() == "Program(VariableDeclaration(var50;Plus(Number(3);Number(4))))");

}
