

// Written by Hans de Nivelle, November 2006. 
// See the licence that was included with the code.


#ifndef BUILDPARSERCODE_INCLUDED 
#define BUILDPARSERCODE_INCLUDED    1


#include <string>
#include <map>
#include <list>
#include <iostream>


#include "symbol.h"
#include "symbolset.h"
#include "parsetable.h"


namespace buildparsercode 
{

   void builddeclaration( const std::list< std::string > & parsernamespace,
		          const std::list< std::string > & tokennamespace, 
		          const std::list< std::string > & tokenizernamespace,
                          const globalvariables& glob, 
		          std::ostream& stream ); 

   struct encoding
   {
      symbol s;
      std::list< unsigned int > reductions;
      unsigned int push;
      bool haspush;

      unsigned int size( );
 
      // Encoding without push:
 
      encoding( symbol s )
         : s(s), push(0), haspush( false )
      {  
      }

      // Encoding with a push:
    
      encoding( symbol s, unsigned int push )
         : s(s), push( push ), haspush( true ) 
      {
      }

      unsigned int size( ) const;
         // In C++ parse table. 

      void addreduction( unsigned int rulenr ); 

      void printtableentry( const std::list< std::string > & tokennamespace,
                            std::ostream& stream ) const; 
         // Print the encoding so that it fits in the C++ table.
         // The tokennamespace is added to the tokens when they
         // are printed. 
   };


   std::list< encoding > 
   encodestate( const parsetable& pt, unsigned int ss );
      // Encode state ss stand alone.

   std::list< encoding > 
   encodestate( const parsetable& pt, unsigned int ss, unsigned int def );
      // Encode state ss, relative to def. 


   static unsigned int length( const std::list< encoding > & );
     
   void builddefinition( const std::list< std::string > & parsernamespace,
		         const std::list< std::string > & tokennamespace, 
		         const std::list< std::string > & tokenizernamespace, 
		         const grammar& gram,
		         const parsetable& pt, 
                         const globalvariables& glob, 
		         std::ostream& outputstream ) throw( actionerror ); 

}


#endif


