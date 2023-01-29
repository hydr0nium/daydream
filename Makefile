CC = g++

all: parser.cpp lexer.cpp util.cpp
	$(CC) -o bin/parser parser.cpp lexer.cpp util.cpp

run: all
	bin/parser
		

