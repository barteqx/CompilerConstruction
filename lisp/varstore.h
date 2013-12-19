
#ifndef VARSTORE_INCLUDED
#define VARSTORE_INCLUDED   1


#include <map>
#include <list> 
#include <string>
#include <iostream>
#include "list.h"

// Plays no role in exercise 8. Maybe there will be an exercise
// about evaluation later.

class varstore
{

   std::map< std::string, std::list< list > > values;
      // The first value is always the current value. 

   std::vector< std::string > known;
      // This is a stack of known variables, in the order in which
      // they were initialized. We need this because variables
      // are created and removed in a stack-like fashion.

   void restore( unsigned int len );
      // Restore known to length len.

public:
   const list* lookup( const std::string& s ) const;
   list* lookup( const std::string& s );
      // Both functions return 0 if s is unknown.
      // In the second case, it is possible to assign to the pointer.
   
   void init( const std::string& s, const list& l );
      // Init creates (a new occurrence) of variable s with value l.

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

std::ostream& operator << ( std::ostream&, const varstore& vs );

#endif


