
// Written by Hans de Nivelle in November 2013 for the course
// Compiler Construction.

#ifndef IDENTIFIER_INCLUDED
#define IDENTIFIER_INCLUDED   1

#include <string>
#include <list> 

#include "assert.h"

#include <iostream>

// Identifiers are C++ style identifiers with namespaces, so 
// that an identifier has form s1::s2:: ... :: sn, where each si is a string.
// Of course it is possible to use n = 1. Even n = 0 is allowed. 


class identifier
{
   std::list< std::string > s;

public: 
   identifier( ) { } 
      // Default identifier is empty.

   identifier( const std::string& s ) 
      : s( { s } ) { } 

   identifier( std::initializer_list< std::string > s ) 
      : s(s) { } 

   unsigned int size( ) const { return s. size( ); }
      // The size of an identifier is the number of its components.

   const std::string& operator [ ] ( unsigned int i ) const;
      // Returns the i-th component of the identifier.


   // Concatenation of identifiers. Operator += is robust against
   // self concatenation. 

   void operator += ( const identifier& id );
   identifier operator + ( const identifier& id )
   {
      identifier res = *this; res += id; return res;
   }

   static int compare( const identifier& id1, const identifier& id2 );
      // Comparison is alphabetic from left to right.

   friend bool operator == ( const identifier& id1, const identifier& id2 );
   friend bool operator != ( const identifier& id1, const identifier& id2 );
   friend bool operator < ( const identifier& id1, const identifier& id2 );
   friend bool operator > ( const identifier& id1, const identifier& id2 );
   friend bool operator <= ( const identifier& id1, const identifier& id2 );
   friend bool operator >= ( const identifier& id1, const identifier& id2 );

};


inline bool operator == ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == 0;
}

inline bool operator != ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != 0; 
}

inline bool operator < ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == -1;
}

inline bool operator > ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == 1;
}

inline bool operator <= ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != 1; 
}

inline bool operator >= ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != -1; 
}

std::ostream& operator << ( std::ostream& stream , const identifier& id ); 

#endif

