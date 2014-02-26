
#ifndef STRUCTSTORE_INCLUDED
#define STRUCTSTORE_INCLUDED  1

#include <map>
#include <vector>
#include <iostream>

#include "ctype.h"

// A structstore defines structs:

struct structstore
{

   struct field
   {
      identifier name;
      ctype t;
      unsigned int offset; 

      field( const identifier& name, const ctype& t, 
             unsigned int offset = 0 )
         : name( name ), t(t), offset( offset )
      { }

   };
  
   std::map< identifier, std::vector< field >> structtypes; 

   const field* find( const identifier& structname, 
                      const identifier& fieldname ) const; 
      // 0 if the struct or the field does not exist. 
};

std::ostream& operator << ( std::ostream& , const structstore::field& );
std::ostream& operator << ( std::ostream& , const structstore& s );
 
#endif
 
