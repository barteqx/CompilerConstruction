

// Written by Hans de Nivelle, October 2005. 
// A rule is a thing that looks like
// A -> B1 ... Bn, in which A, B1, ..., Bn symbols and n >= 0. 
// 
// In June 2008, I removed the priority and associativity from rules,
// because I think that priorities are a thing between symbols.
//
// See the licence that was included with this code.


#ifndef RULE_INCLUDED
#define RULE_INCLUDED   1


#include <list>
#include <iostream>

#include "symbol.h"



struct rule 
{

   symbol lhs;
   std::list< symbol > rhs;
   unsigned int nr;
      // Normally, I have the opinion that an index is not part of an object,
      // but of the container in which it is kept, but here the number
      // is used as name of the rule, in the final switch statement. 

   rule( symbol lhs, const std::list< symbol > & rhs, unsigned int nr )
      : lhs( lhs ), rhs( rhs ), nr( nr ) 
   {
   }

};


std::ostream& operator << ( std::ostream& , const rule& );
   

#endif


