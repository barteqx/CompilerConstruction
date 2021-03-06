
#include "varstore.h"

void varstore::init( const std::string& s, 
                     const std::string& reg, const ctype& t )
{
   known. push_back(s);
   values [ s ]. push_front( value( reg, t ));
}


void varstore::restore( unsigned int len )
{
   ASSERT( len <= known. size( ))
   while( len < known. size( ))
   {
      auto p = values. find( known. back( ));
      ASSERT( p != values. end( ));
      p -> second. pop_front( );
      if( p -> second. size( ) == 0 )
         values. erase(p);       
      known. pop_back( );
   }
}


const varstore::value* varstore::lookup( const std::string& s ) const 
{
   auto p = values. find( s );
   if( p == values. end( ))
      return 0;
   else
      return & ( p -> second. front( ));
}


varstore::value* varstore::lookup( const std::string& s )
{
   auto p = values. find( s );
   if( p == values. end( ))
      return 0;
   else
      return & ( p -> second. front( ));
}


std::ostream& operator << ( std::ostream& stream, const varstore::value& val )
{
   stream << "value( ";
   stream << val. reg << " : ";
   stream << val. t << " )";
   return stream;
}

 
std::ostream& operator << ( std::ostream& stream, const varstore& vs )
{
   stream << "Varstore:\n"; 
   stream << "known variables:\n   { "; 
   for( auto p = vs. known. begin( ); p != vs. known. end( ); ++ p )
   {
      if( p != vs. known. begin( ))  
         stream << ", ";
      stream << *p;
   }
   stream << " }\n";

   stream << "values:\n";
   for( auto p = vs. values. begin( ); p != vs. values. end( ); ++ p )
   {
      stream << "   " << ( p -> first ) << " : { "; 
      for( auto q = p -> second. begin( ); q != p -> second. end( ); ++ q )
      {
         if( q != p -> second. begin( ))
            stream << ", ";
         stream << *q; 
      }
      stream << " }\n";
   }
   return stream;
}


