

// Code written by Hans de Nivelle, November 2006.
// Simplified, June 2008.
// See the licence that was included with this code.


#ifndef PARSETABLE_INCLUDED
#define PARSETABLE_INCLUDED    1


#include <iostream>
#include <vector>
#include <map>


#include "assert.h"
#include "itemset.h"



struct parsetable
{

   struct state  
   {

      itemset set;
      std::map< symbol, unsigned int > transitions; 
         // These are only pushing transitions.
         // Reductions have to be read of from the itemset. 
   };

   std::vector< state > table;

   std::map< symbol, unsigned int > startstates;
      // Stores for each possible start symbol the state in
      // which the parser starts. 

   static parsetable construct( const grammar& g );
      // The parsetable is initialized as follows:
      // For each start symbol S with lookahead set { L1, ..., Ln },  
      // create a state containing all items of form 
      //    S -> # X / { L1,...,Ln }, for each rule S -> X. 
      // After that, the complete parsetable is computed.

   unsigned int nrstates( ) const { return table. size( ); } 

   void printconflicts( std::ostream& stream );
      // Print the conflicts in the parse table.

   void setdefaultreductions( );
      // Check the parse table for states that have only one reduction,
      // which is also conflict free. For these reductions,
      // the lookahead set is made requal to { $defaultred }. 

   symbolset getrelevantsymbols( unsigned int ss ) const;
      // Returns the symbols that are relevant in state ss.
      // These are the symbols that occur in a reduction or a push. 

   friend std::ostream& operator << ( std::ostream& , const parsetable& );
};

std::ostream& operator << ( std::ostream& , const parsetable::state& ); 
std::ostream& operator << ( std::ostream& , const parsetable& ); 


#endif


