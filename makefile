all: hw4

hw4: arithmetic.o booleanexp.o linenumber.o interpreter.o command.o 
	g++ -g -Wall -std=c++11 main.cpp arithmetic.o booleanexp.o linenumber.o interpreter.o command.o -o hw4

arithmetic.o: interpreter/arithmetic.h interpreter/arithmetic.cpp
	g++ -g -Wall -std=c++11 -c interpreter/arithmetic.cpp -o arithmetic.o

booleanexp.o: interpreter/booleanexp.h interpreter/booleanexp.cpp
	g++ -g -Wall -std=c++11 -c interpreter/booleanexp.cpp -o booleanexp.o

linenumber.o: interpreter/linenumber.h interpreter/linenumber.cpp
	g++ -g -Wall -std=c++11 -c interpreter/linenumber.cpp -o linenumber.o

interpreter.o: interpreter/interpreter.h interpreter/interpreter.cpp
	g++ -g -Wall -std=c++11 -c interpreter/interpreter.h interpreter/interpreter.cpp

command.o: interpreter/command.h interpreter/command.cpp
	g++ -g -Wall -std=c++11 -c interpreter/command.h interpreter/command.cpp


