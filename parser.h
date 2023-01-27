#ifndef parser
#define parser

class Parser;
enum Type;

// Classes
class Ast {
	public:
		Ast();

};
class Parser {
	public:
		Parser(){};
		Ast parse(Tokens tokens);
};
class Programm;
class Block{
	public:
		Block(){};

};
class Exp {
	public:
		Exp(){};
		virtual std::string get_value() = 0;
};
class Descriptor{
	public:
		std::string value;
		Descriptor(){};
		Descriptor(std::string v);
	
};
class Int: public Exp {
	public:
		std::string value;
		Int(std::string v);
		std::string get_value();

};
class String;
class Float;
class LogicalEqual: public Exp {
	public:
		Exp* left_side;
		Exp* right_side;
		
};
class LogicalNot: public Exp {
	public:
		Exp* left_side;
		Exp* right_side;
};
class Function: public Exp {
	public:
		Function();
		std::string name;
		std::vector<Descriptor> parameters;
		Block body;

};
class Assignment {
	public:
		Descriptor name;
		Exp* value;
		Assignment(Descriptor n, Exp* v);
		
};
class If {
	public:
		If();
		Exp* condition;
		Block statements_in_if; 
};
class While;
class For;

// Functions
Assignment parse_assignment(Tokens& tokens);
Exp* parse_expression(Tokens& tokens);


#endif
