

#ifndef ASSERT_INCLUDED
#define ASSERT_INCLUDED    1

#include <assert.h>
#include <iostream>

#define ASSERT( X ) { assert( ( X ) ); }
   // Do a system assert.

// #define ASSERT( X ) { ; }
   // Do nothing.

#endif


