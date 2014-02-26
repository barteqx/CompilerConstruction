
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++


intermediate : Makefile   identifier.o ctype.o ctree.o intermediate.o data.o varstore.o token.o tokenizer.o parser.o structstore.o functionstore.o  possibleconversions.o
	$(CPP) $(Flags) -o intermediate   identifier.o ctype.o ctree.o intermediate.o data.o varstore.o token.o tokenizer.o parser.o structstore.o functionstore.o possibleconversions.o

identifier.o : Makefile   identifier.cpp   assert.h identifier.h 
	$(CPP) -c $(Flags) identifier.cpp -o  identifier.o


ctype.o : Makefile   ctype.cpp   ctype.h assert.h identifier.h 
	$(CPP) -c $(Flags) ctype.cpp -o  ctype.o


ctree.o : Makefile   ctree.cpp   ctree.h identifier.h assert.h ctype.h data.h 
	$(CPP) -c $(Flags) ctree.cpp -o  ctree.o


intermediate.o : Makefile   intermediate.cpp   functionstore.h ctype.h assert.h identifier.h structstore.h parser.h token.h ctree.h data.h tokenizer.h reader.h varstore.h error.h 
	$(CPP) -c $(Flags) intermediate.cpp -o  intermediate.o


data.o : Makefile   data.cpp   data.h 
	$(CPP) -c $(Flags) data.cpp -o  data.o


varstore.o : Makefile   varstore.cpp   varstore.h ctype.h assert.h identifier.h 
	$(CPP) -c $(Flags) varstore.cpp -o  varstore.o


token.o : Makefile   token.cpp   token.h identifier.h assert.h ctree.h ctype.h data.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o


tokenizer.o : Makefile   tokenizer.cpp   data.h tokenizer.h reader.h token.h identifier.h assert.h ctree.h ctype.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o


parser.o : Makefile   parser.cpp   parser.h token.h identifier.h assert.h ctree.h ctype.h data.h tokenizer.h reader.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


structstore.o : Makefile   structstore.cpp   structstore.h ctype.h assert.h identifier.h 
	$(CPP) -c $(Flags) structstore.cpp -o  structstore.o


functionstore.o : Makefile   functionstore.cpp   functionstore.h ctype.h assert.h identifier.h 
	$(CPP) -c $(Flags) functionstore.cpp -o  functionstore.o


possibleconversions.o : Makefile possibleconversions.cpp possibleconversions.h ctree.h conversion.h
	$(CPP) -c $(Flags) possibleconversions.cpp -o possibleconversions.o

