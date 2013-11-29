

// Written by Hans de Nivelle,
// see the licence that was included with this code.


#include "rule.h"


std::ostream& operator << ( std::ostream& stream, const rule& r )
{

   stream << " [" << r. nr << "]      ";
   stream << r. lhs;
   stream << " --> ";
   for( std::list< symbol > :: const_iterator
	   p = r. rhs. begin( ); p != r. rhs. end( ); ++ p )
   {
      if( p != r. rhs. begin( ))
         stream << ", ";
      stream << *p;
   }


   return stream;
}


