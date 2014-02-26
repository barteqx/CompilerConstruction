

#include "identifier.h"
#include "assert.h"


const std::string& identifier::operator [ ] ( unsigned int i ) const
{
   ASSERT( i < s. size( ));
   auto p = s. begin( );

   while( i )
   {
      ++ p; 
      -- i; 
   }
  
   return *p; 
}
 
 
void identifier::operator += ( const identifier& id )
{
   unsigned int i = id. s. size( );
   auto p = id. s. begin( );

   while( i )
   {
      s. push_back( * ( p ++ ));
      -- i; 
   }
}


int identifier::compare( const identifier& id1, const identifier& id2 )
{
   auto p1 = id1. s. begin( );
   auto p2 = id2. s. begin( );

   while( p1 != id1. s. end( ) && p2 != id2. s. end( ))
   {
      if( *p1 < *p2 ) return -1;
      if( *p1 > *p2 ) return 1;

      ++ p1;
      ++ p2;
   }

   if( p1 == id1. s. end( ) && p2 != id2. s. end( ))
      return -1;
   if( p1 != id1. s. end( ) && p2 == id2. s. end( ))
      return 1;

   return 0;
}


std::ostream& operator << ( std::ostream& stream, const identifier& id )
{
   for( unsigned int i = 0; i < id. size( ); ++ i )
   {
      if(i)
         stream << "::";
      stream << id [i];
   }
   return stream; 
}


