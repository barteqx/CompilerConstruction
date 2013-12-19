
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++


parser : Makefile   tokenizer.o token.o varstore.o parser.o list.o bigint.o parser.o recursive.o
	$(CPP) $(Flags) -o parser tokenizer.o token.o varstore.o parser.o list.o bigint.o recursive.o

tokenizer.o : Makefile  tokenizer.cpp  reader.h token.h tokenizer.h assert.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o

token.o : Makefile   token.cpp   token.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o

list.o : Makefile   list.cpp   list.h assert.h bigint.h 
	$(CPP) -c $(Flags) list.cpp -o  list.o

bigint.o : Makefile   bigint.cpp   assert.h bigint.h 
	$(CPP) -c $(Flags) bigint.cpp -o  bigint.o

varstore.o : Makefile varstore.cpp varstore.h
	$(CPP) -c $(Flags) varstore.cpp -o varstore.o

parser.o : Makefile   parser.cpp   parser.h token.h reader.h tokenizer.h assert.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o

recursive.o : Makefile   recursive.cpp   varstore.h list.h assert.h bigint.h 
	$(CPP) -c $(Flags) recursive.cpp -o  recursive.o
