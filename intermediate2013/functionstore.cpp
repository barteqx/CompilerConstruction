
#include "functionstore.h"

std::ostream& operator << ( std::ostream& out, 
                            const functionstore::func& f )
{
   out << "func( ";
   out << f. name << " : ";
   out << f. t << " )";
   return out;
}

 
std::ostream& operator << ( std::ostream& out, const functionstore& fs )
{
   out << "Functionstore:\n"; 
   for( unsigned int i = 0; i < fs. functions. size( ); ++ i )
   {
      out << "   " << fs. functions[i] << "\n";
   }
   return out;
}


