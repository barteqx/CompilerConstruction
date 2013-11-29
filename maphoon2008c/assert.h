
// Written by Hans de Nivelle,
// December 2004.


#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED    1


#include <assert.h>
#include <iostream>


// #define ASSERT( X ) { assert( ( X ) ); }
   // Do a system assert.

// #define ASSERT( X ) { ; }
   // Do nothing.

#define ASSERT(X) { if(!(X)) { std::cout << "ASSERT-ERROR\n\n";assert((X)); }}
   // Usual ASSERT writes into std::error. This one writes an additional
   // message into std::cout, so that it can be more easily detected. 

#endif


