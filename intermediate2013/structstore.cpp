
#include "structstore.h"

const structstore::field* 
structstore::find( const identifier& structname,
                   const identifier& fieldname ) const 
{
   auto p = structtypes. find( structname );
   if( p == structtypes. end( ))
      return 0;

   for( auto q = p -> second. begin( ); q != p -> second. end( ); ++ q )
   {
      if( q -> name == fieldname )
         return & (*q);
   }
   return 0;
}


std::ostream& operator << ( std::ostream& out, 
                            const structstore::field& f )
{
   out << f. name << " : " << f. t << "         ";
   out << "( offset = " << f. offset << " )";
   return out;  
}

std::ostream& operator << ( std::ostream& out,
                            const structstore& s )
{
   out << "Declared Struct Types:\n"; 
   for( auto p = s. structtypes. begin( ); p != s. structtypes. end( ); ++ p )
   {
      out << "   " << ( p -> first ) << "\n";
      for( unsigned int i = 0; i < ( p -> second ). size( ); ++ i )
      {
         out << "      " << i << ":     " << ( p -> second )[i] << "\n";
      }
      out << "\n";
   }
   return out;
}

