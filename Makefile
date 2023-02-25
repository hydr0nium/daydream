CC = g++

FILES = parser.cpp lexer.cpp util.cpp reversePolish.cpp statement.cpp interpreter.cpp

all: main.cpp $(FILES)
	$(CC) -o bin/daydream main.cpp $(FILES)

run: all
	bin/daydream

test: tests.cpp $(FILES)
	$(CC) -o bin/testing tests.cpp $(FILES)
	bin/testing

		

