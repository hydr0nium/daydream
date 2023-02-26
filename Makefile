CC = g++

FILES = parser.cpp lexer.cpp util.cpp reversePolish.cpp statement.cpp interpreter.cpp

all: main.cpp $(FILES)
	$(CC) -o daydream main.cpp $(FILES)

run: all
	./daydream

test: tests.cpp $(FILES)
	$(CC) -o testing tests.cpp $(FILES)
	./testing

		

