# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++

parser : Makefile   parser.o tokenizer.o token.o list.o bigint.o
	$(CPP) $(Flags) -o parser   parser.o tokenizer.o token.o list.o bigint.o


parser.o : Makefile   parser.cpp  token.h tokenizer.h reader.h assert.h list.h
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


tokenizer.o : Makefile   tokenizer.cpp   tokenizer.h reader.h token.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o


token.o : Makefile   token.cpp   token.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o


list.o : Makefile   list.cpp   list.h assert.h bigint.h 
	$(CPP) -c $(Flags) list.cpp -o  list.o


bigint.o : Makefile   bigint.cpp   assert.h bigint.h 
	$(CPP) -c $(Flags) bigint.cpp -o  bigint.o