

#include "varstore.h"
#include "assert.h"


varstore::varstore( )
{
   values [ "e" ] =   2.71828182845904523536;
   values  [ "pi" ] = 3.14159265358979323846264338327950288;
}


bool varstore::contains( const std::string& s ) const
{
   return values. find(s) != values. end( );
}


void varstore::assign( const std::string& s, double val )
{
   values [s] = val;
}


double varstore::lookup( const std::string& s ) const
{
   std::map< std::string, double > :: const_iterator 
                p = values. find(s);
   ASSERT( p != values. end( ));

   return p -> second;
}


void varstore::clear( )
{
   values. clear( );
}


std::ostream& operator << ( std::ostream& stream, const varstore& vs )
{
   stream << "Variable Store:\n";
   for( std::map< std::string, double > :: const_iterator
           p = vs. values. begin( );
           p != vs. values. end( );
           ++ p )
   {
      stream << "   " << ( p -> first ) << " = " << ( p -> second ) << "\n";
   }

   stream << "\n";

   return stream;
}



