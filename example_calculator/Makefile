
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++


calculator : Makefile   calculator.o tokenizer.o token.o varstore.o parser.o
	$(CPP) $(Flags) -o calculator calculator.o tokenizer.o token.o varstore.o parser.o

calculator.o : Makefile calculator.cpp reader.h token.h parser.h tokenizer.h varstore.h
	$(CPP) -c $(Flags) calculator.cpp -o calculator.o 

tokenizer.o : Makefile  tokenizer.cpp  reader.h token.h tokenizer.h assert.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o

token.o : Makefile   token.cpp   token.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o

varstore.o : Makefile varstore.cpp varstore.h
	$(CPP) -c $(Flags) varstore.cpp -o varstore.o

parser.o : Makefile   parser.cpp   parser.h token.h reader.h tokenizer.h assert.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


