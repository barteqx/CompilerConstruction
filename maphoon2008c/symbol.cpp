

// Code written by Hans de Nivelle, October 2006.
// See the licence that was included with this code.


#include "symbol.h"
#include "cpp.h"


symbol::symbol( const std::string& s )
{
   std::map< std::string, unsigned int > :: const_iterator
      p = lookuptable. find(s);

   if( p == lookuptable. end( ))
   {
      repr = strings. size( );
      lookuptable. insert( 
	     std::pair< std::string, unsigned int > ( s, repr ));
      strings. push_back(s);
   }
   else
   {
      repr = ( p -> second );
   }
}


const std::string& symbol::getstring( ) const
{
   ASSERT( repr < strings. size( ));
   return strings [ repr ];
}


void symbol::printtable( std::ostream& stream )
{
   stream << "Table of Symbols:\n";
   stream << "=================\n";
   stream << "\n";

   for( unsigned int i = 0; i < strings. size( ); ++ i )
   {
      stream << i << " : " << strings [i] << "\n";
   }
   stream << "\n";
}


std::ostream& operator << ( std::ostream& stream, const symbol& symb )
{
   stream << symb. getstring( );
   return stream;
}


