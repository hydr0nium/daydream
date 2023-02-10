CC = g++

FILES = parser.cpp lexer.cpp util.cpp

all: main.cpp $(FILES)
	$(CC) -o bin/run main.cpp $(FILES)

run: all
	bin/run

test: tests.cpp $(FILES)
	$(CC) -o bin/testing tests.cpp $(FILES)
	bin/testing

		

