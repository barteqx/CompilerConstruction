
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c++11
CPP = g++


lisp : Makefile   list.o lisp.o bigint.o varstore.o 
	$(CPP) $(Flags) -o lisp   list.o lisp.o bigint.o varstore.o 

list.o : Makefile   list.cpp   list.h assert.h bigint.h 
	$(CPP) -c $(Flags) list.cpp -o  list.o


lisp.o : Makefile   lisp.cpp   varstore.h list.h assert.h bigint.h 
	$(CPP) -c $(Flags) lisp.cpp -o  lisp.o


bigint.o : Makefile   bigint.cpp   assert.h bigint.h 
	$(CPP) -c $(Flags) bigint.cpp -o  bigint.o


varstore.o : Makefile   varstore.cpp   varstore.h list.h assert.h bigint.h 
	$(CPP) -c $(Flags) varstore.cpp -o  varstore.o


