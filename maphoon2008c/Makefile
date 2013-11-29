
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef
CPP = g++


maphoon : Makefile   maphoon.o symbol.o symbolset.o cpp.o attributesystem.o rule.o grammar.o itemset.o action.o errors.o parsetable.o buildparsercode.o 
	$(CPP) $(Flags) -o maphoon   maphoon.o symbol.o symbolset.o cpp.o attributesystem.o rule.o grammar.o itemset.o action.o errors.o parsetable.o buildparsercode.o 

maphoon.o : Makefile   maphoon.cpp   symbol.stat symbol.h assert.h rule.h symbolset.h cpp.h attributesystem.h errors.h action.h grammar.h parsetable.h itemset.h buildparsercode.h 
	$(CPP) -c $(Flags) maphoon.cpp -o  maphoon.o


symbol.o : Makefile   symbol.cpp   cpp.h symbol.h assert.h 
	$(CPP) -c $(Flags) symbol.cpp -o  symbol.o


symbolset.o : Makefile   symbolset.cpp   symbolset.h assert.h symbol.h 
	$(CPP) -c $(Flags) symbolset.cpp -o  symbolset.o


cpp.o : Makefile   cpp.cpp   cpp.h symbol.h assert.h 
	$(CPP) -c $(Flags) cpp.cpp -o  cpp.o


attributesystem.o : Makefile   attributesystem.cpp   cpp.h symbol.h assert.h attributesystem.h errors.h symbolset.h 
	$(CPP) -c $(Flags) attributesystem.cpp -o  attributesystem.o


rule.o : Makefile   rule.cpp   rule.h symbol.h assert.h 
	$(CPP) -c $(Flags) rule.cpp -o  rule.o


grammar.o : Makefile   grammar.cpp   grammar.h assert.h rule.h symbol.h action.h attributesystem.h errors.h symbolset.h 
	$(CPP) -c $(Flags) grammar.cpp -o  grammar.o


itemset.o : Makefile   itemset.cpp   itemset.h symbolset.h assert.h symbol.h rule.h grammar.h action.h attributesystem.h errors.h 
	$(CPP) -c $(Flags) itemset.cpp -o  itemset.o


action.o : Makefile   action.cpp   cpp.h symbol.h assert.h action.h attributesystem.h errors.h symbolset.h rule.h 
	$(CPP) -c $(Flags) action.cpp -o  action.o


errors.o : Makefile   errors.cpp   errors.h 
	$(CPP) -c $(Flags) errors.cpp -o  errors.o


parsetable.o : Makefile   parsetable.cpp   parsetable.h assert.h itemset.h symbolset.h symbol.h rule.h grammar.h action.h attributesystem.h errors.h 
	$(CPP) -c $(Flags) parsetable.cpp -o  parsetable.o


buildparsercode.o : Makefile   buildparsercode.cpp   cpp.h symbol.h assert.h buildparsercode.h symbolset.h parsetable.h itemset.h rule.h grammar.h action.h attributesystem.h errors.h 
	$(CPP) -c $(Flags) buildparsercode.cpp -o  buildparsercode.o


