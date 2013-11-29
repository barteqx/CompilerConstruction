
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++


parser : Makefile   parser.o parsestack.o tokenizer.o token.o 
	$(CPP) $(Flags) -o parser   parser.o parsestack.o tokenizer.o token.o 

lisp_parser : Makefile   lisp_parser.o parsestack.o tokenizer.o token.o 
	$(CPP) $(Flags) -o lisp_parser   lisp_parser.o parsestack.o tokenizer.o token.o 

parser.o : Makefile   parser.cpp   parsestack.h token.h tokenizer.h reader.h assert.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o

lisp_parser.o : Makefile   lisp_parser.cpp   parsestack.h token.h tokenizer.h reader.h assert.h 
	$(CPP) -c $(Flags) lisp_parser.cpp -o  lisp_parser.o


parsestack.o : Makefile   parsestack.cpp   assert.h parsestack.h token.h tokenizer.h reader.h 
	$(CPP) -c $(Flags) parsestack.cpp -o  parsestack.o


tokenizer.o : Makefile   tokenizer.cpp   tokenizer.h reader.h token.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o


token.o : Makefile   token.cpp   token.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o


