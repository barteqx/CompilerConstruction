

// Written by Hans de Nivelle, october 2006.
// See the licence that was included with this code.


#include "symbolset.h"


bool symbolset::contains( symbol s ) const
{
   return repr. find(s) != repr. end( );
}


bool symbolset::insert( symbol s ) 
{
   std::map< symbol, bool > :: iterator p = repr. find(s);
   
   if( p == repr. end( ))
   {
      repr. insert( std::pair< symbol, bool > ( s, true ));
      return true;
   }
   else
   {
      ASSERT( p -> second );
      return false;
   }
}


unsigned int symbolset::insert( const symbolset& s )
{
   unsigned int nr = 0;
   for( std::map< symbol, bool > :: const_iterator
           p = s. repr. begin( );
	   p != s. repr. end( );
	   ++ p )
   {
      if( p -> second )
      {
         bool b = insert( p -> first );
	 if(b)
            ++ nr;
      }
   }
   return nr;
}


bool symbolset::remove( symbol s )
{
   std::map< symbol, bool > :: iterator p = repr. find(s);
   if( p == repr. end( ))
      return false;
   else
   {
      repr. erase(p);
      return true;
   }
}


unsigned int symbolset::remove( const symbolset& s )
{
   unsigned int nr = 0;
   for( std::map< symbol, bool > :: const_iterator
           p = s. repr. begin( );
	   p != s. repr. end( );
	   ++ p )
   {
      if( p -> second )
      {
         bool b = remove( p -> first );
	 if(b)
	    ++ nr;
      }
   }
   return nr;
}


unsigned int symbolset::size( ) const
{
   return repr. size( );
}


void symbolset::clear( ) 
{
   repr. clear( );
}


std::ostream& operator << ( std::ostream& stream, const symbolset& s )
{
   stream << "{ ";
   for( symbolset::const_iterator 
           p = s. begin( );
	   p != s. end( );
	   ++ p )
   {
      if( p != s. begin( ))
         stream << ", ";
      stream << *p;
   }
   stream << " }";
   return stream; 
}


