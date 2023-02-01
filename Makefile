CC = g++

all: main.cpp parser.cpp lexer.cpp util.cpp
	$(CC) -o bin/run main.cpp parser.cpp lexer.cpp util.cpp

run: all
	bin/run

test: tests.cpp parser.cpp lexer.cpp util.cpp
	$(CC) -o bin/testing tests.cpp parser.cpp lexer.cpp util.cpp
	bin/testing

		

