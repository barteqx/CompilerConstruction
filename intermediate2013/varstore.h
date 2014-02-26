
#ifndef VARSTORE_INCLUDED
#define VARSTORE_INCLUDED   1

#include <map>
#include <list> 
#include <string>
#include <iostream>
#include "ctype.h"


class varstore
{

public: 
   struct value
   {
      std::string reg; 
      ctype t;    // type of the variable without ref.

      value( const std::string& reg, const ctype& t )
         : reg( reg ), t(t)
      { }
   };

private:
   std::map< std::string, std::list< value > > values;
      // The first value is always the current value. 

   std::vector< std::string > known;
      // This is a stack of known variables, in the order in which
      // they were initialized. We need this because variables
      // are created and removed in a stack-like fashion.

   void restore( unsigned int len );
      // Restore known to length len.

public:
   const value* lookup( const std::string& s ) const;
   value* lookup( const std::string& s );
      // Both functions return 0 if s is unknown.
      // In the second case, it is possible to assign to the pointer.
   
   void init( const std::string& s, const std::string& reg, const ctype& t );
      // Init creates (a new occurrence) of variable s with value val.

   struct backtrackpoint
   {
      varstore* vs;
      unsigned int len;

      backtrackpoint( varstore* vs, unsigned int len )
         : vs( vs ), len( len ) { }

      ~backtrackpoint( ) { vs -> restore( len ); }
 
   };

   backtrackpoint getbacktrackpoint( ) 
      { return backtrackpoint( this, known. size( )); }

      // The varstore is restored when the backtrackpoint goes out of scope
   
   friend std::ostream& operator << ( std::ostream& , const varstore& );

   friend class backtrackpoint; 
};

std::ostream& operator << ( std::ostream&, const varstore::value& val ); 
std::ostream& operator << ( std::ostream&, const varstore& vs );

#endif


