
#ifndef FUNCTIONSTORE_INCLUDED
#define FUNCTIONSTORE_INCLUDED   1

#include <map>
#include <list> 
#include <string>
#include <iostream>
#include "ctype.h"


class functionstore
{

public: 
   struct func 
   {
      identifier name; 
      ctype t;    // type of the function.

      func( const identifier& name, const ctype& t )
         : name( name ), t(t)
      { } 

   };

   std::vector< func > functions;

public:
   void insert( const identifier& name, const ctype& t )
   {
      functions. push_back( func( name, t ));  
   }
 
};

std::ostream& operator << ( std::ostream&, const functionstore::func& val ); 
std::ostream& operator << ( std::ostream&, const functionstore& fs );

#endif


