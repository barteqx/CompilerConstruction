

// Written by Hans de Nivelle, october 2006.
// Revised June 2008.
// See the licence that was included with this code.


#ifndef SYMBOL_INCLUDED
#define SYMBOL_INCLUDED   1


#include "assert.h"

#include <string>
#include <map>
#include <vector>


class symbol 
{

   static std::map< std::string, unsigned int > lookuptable;

   static std::vector< std::string > strings;

   unsigned int repr;


public: 
   explicit symbol( const std::string& s );
      // Produces a symbol from a string.

   const std::string& getstring( ) const;
      // Produces the string that represents the symbol. 

   static void printtable( std::ostream& stream );
      // Prints table of all symbols. 

   static symbol recover; 
      // Recover can be used for setting recovery points in 
      // rules. Like in Expression -> RECOVER dot.
      // Maphoon will try to recover on a dot after a syntax error.

   static symbol defaultred;
      // Defaultred is used internally for setting default reductions. 
      // The user should not try to use this token by himself. We 
      // would have preferred to call it just 'default', but this is a 
      // reserved word in C++. 

   friend bool operator == ( symbol, symbol );
   friend bool operator != ( symbol, symbol );
   friend bool operator < ( symbol, symbol );
   friend bool operator > ( symbol, symbol );
   friend bool operator <= ( symbol, symbol );
   friend bool operator >= ( symbol, symbol );


   friend std::ostream& operator << ( std::ostream& , const symbol& symb );

};



inline bool operator == ( symbol symb1, symbol symb2 )
{
   return symb1. repr == symb2. repr;
}

inline bool operator != ( symbol symb1, symbol symb2 )
{
   return symb1. repr != symb2. repr;
}

inline bool operator < ( symbol symb1, symbol symb2 )
{
   return symb1. repr < symb2. repr;
}

inline bool operator > ( symbol symb1, symbol symb2 )
{
   return symb1. repr > symb2. repr;
}

inline bool operator <= ( symbol symb1, symbol symb2 )
{
   return symb1. repr <= symb2. repr;
}

inline bool operator >= ( symbol symb1, symbol symb2 )
{
   return symb1. repr >= symb2. repr;
}


#endif


