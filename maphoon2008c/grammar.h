

// Code written by Hans de Nivelle, november 2006.
// Rewritten July 2008. (Added the possibility of multiple start 
// symbols)
// See the licence that was included with this code.


#ifndef GRAMMAR_INCLUDED
#define GRAMMAR_INCLUDED   1


#include <vector>
#include <iostream>

#include "assert.h" 
#include "rule.h"
#include "action.h" 
#include "symbolset.h"


struct grammar 
{
   std::map< symbol, symbolset > starts;
      // Defines the start symbols and their lookahead sets.
      // A start symbol should not occur in its own lookahead set. 
      // The question whether a symbol in the lookahead set can be
      // reachable from the start symbol is much more delicate.

   std::vector< rule > rules;
   std::vector< action > actions; 

   grammar( );

   void insert( const rule& r, 
		const action& a = action( "nofile", 0 )); 
      // Insert a rule to the grammar. It is simply appended, we
      // don't check for uniqueness of rules. The rule receives
      // a new number in the grammar. 

   void insert( symbol start, const symbolset& lookahead );
   void insert( symbol start, symbol lookahead );

   symbolset leftsymbols( ) const;
      // Collect the set of symbols that occur in the left hand side
      // of a rule. 

   symbolset rightsymbols( ) const;
      // Collect the set of symbols that occur in the right hand side
      // of a rule. 

   symbolset startsymbols( ) const;
      // Collect the set of all symbols of the grammar. 

   symbolset lookaheadsymbols( ) const;
      // Collect the set of all lookahead symbols of the grammar.

   symbolset allsymbols( ) const;
      // Collect all symbols of the grammar.

   symbolset nullablesymbols( ) const;
      // Compute the set of symbols that are nullable. These
      // are the symbols that can be rewritten into the empty word. 
      // It is the smallest fixed point F of the following conditions:
      //
      // If there is a rule A -> B1 ... Bk such that all B1,B2,..,Bk are in
      // F, then A in F.

   std::map< symbol, symbolset > reachabilitysets( ) const;
      // Constructs a map that attaches to each symbol the set of symbols
      // that are reachable from it.
      // It is the smallest fixed point F of the following conditions:
      // - A in F(A),
      // - For every rule A -> B1 ... Bk, each Bi in F(A), 
      // - If A in F(B) and B in F(C), then A in F(C). 

   std::map< symbol, symbolset > 
   firstsets( const symbolset& nullable ) const;
      // Constructs a map that attaches to each symbol the
      // set first(A). 
      // The function first is the smallest fixed point F of the 
      // following conditions:
      //   - A in F(A),
      //   -For every rule A -> B1 ... Bk, (k > 0), and for every m < k, 
      //       s.t all B1,...,Bm are nullable, 
      //          we have F( B[m+1] ) subset F(A). 

   static grammar example1( ); 
      // A simple grammar with expressions and operators.

   static grammar example2( );
      // Example2 is a grammar that fooled maphoon89, because the 
      // algorithm of maphoon89 computes look ahead sets that are too big. 

};


std::ostream& operator << ( std::ostream& stream, const grammar& g );
std::ostream& operator << ( std::ostream& stream,
		            const std::map< symbol, symbolset > & first ); 


#endif


