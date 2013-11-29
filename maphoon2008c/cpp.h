

// Code written by Hans de Nivelle, June 2008.
// See the licence that was included with this code.


#ifndef CPP_INCLUDED
#define CPP_INCLUDED   1


#include <string>
#include <list>


#include "symbol.h"


namespace cpp 
{

   bool wellformedalone( const std::string& s );
      // True if s, when used alone, is a valid C++ identifier.
 
   bool wellformedprefixed( const std::string& s );
      // True if s, when prefixed by something of form X_, would make a   
      // valid C++ identifier. 
      //
      // The difference with wellformedalone is in the reserved words,
      // like while or for.

   bool wellformedalone( const std::list< std::string > & n );
      // True if n can act as namespace.

   std::string makedeclaration( const std::list< std::string > & n );
      // Creates something of form
      // namespace s1 { namespace { s2 .... 

   std::string makeclosure( const std::list< std::string > & n );
      // Creates something of form } } .. }

   std::string indentation( const std::list< std::string > & n );

   std::string includeguard( const std::list< std::string > & n,
                             const std::string& s ); 
      // Build include guard based on s::n.

   std::string makename( const std::list< std::string > & n, 
                         const std::string& s );
      // Constructs string n::s.

   std::string indentation( const std::list< std::string > & n, 
                            const std::string& s, 
                            unsigned int x = 0 );


   std::string attribute_name( const std::string& attr );
      // The string by which the attribute (of token) will be represented.
      // It is just attr. 

   std::string symbol_name( symbol s );
   std::string symbol_name( const std::list< std::string > & n, symbol s ); 
      // The string by which symbol s will be represented in the
      // enumeration. It will be the string "tkn_" + s. 
      // The second variant constructs n + "tkn_ " + s. 

}


#endif


