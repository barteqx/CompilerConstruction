

// Code written by Hans de Nivelle, November 2006.
// See the licence that was included with this code. 


#ifndef ITEMSET_INCLUDED
#define ITEMSET_INCLUDED   1


#include "symbolset.h"
#include "rule.h"
#include "grammar.h"



// Items do not maintain copies of their rules. 
// They only contain pointers to their rules. 
// As a consequence, it is probably not a good idea to move or delete
// a rule at a moment when there exist itemsets on this rule. 


struct lr1item 
{
   const rule* r;
   std::list< symbol > :: const_iterator p;

   symbolset lookahead;

   lr1item( const rule* r,
	    std::list< symbol > :: const_iterator p,
	    const symbolset& lookahead )
      : r(r), p(p), lookahead( lookahead )
   {
   }


   // Check whether item is well-formed, i.e. whether
   // the position is inside the rule. We cannot check everything, 
   // so a better name would be 'seemswellformed( )'.

   bool iswellformed( ) const;

   bool isreduction( ) const; 
      // True if the lr1item is a reduction. This is the case
      // when the pointer stands at the end of the rule. 
   
};

std::ostream& operator << ( std::ostream& , const lr1item& it );


class itemset 
{

private: 
   std::list< lr1item > repr;

public:  
   bool insert( const lr1item& i ); 
      // If an item with the same base exists, we merge the lookaheads.
      // We return true if the itemset is changed. 

   bool insert( const itemset& other );
      // Returns true if the itemset is changed. 

   bool samebase( const itemset& other ) const;
      // True if the other itemset has the same base as this one. 
      // Informally this means that the itemsets are the same
      // if one ignores the lookahead sets. 

   void close( const grammar& g,
               const symbolset& leftsymbols, 
	       const symbolset& lambdasymbols,
	       std::map< symbol, symbolset > & firstsymbols ); 
      // Close the itemset. 
      // First define FIRST( A1, ..., An ), where Ai is a symbol or 
      // a set of symbols as FIRST(A1) cup FIRST(A2) ... FIRST(Ai),
      // where Ai is the first symbol that cannot be rewritten
      // into the empty string. 
      // Then the closure is the smallest fixed point of the following
      // closure rule:  
      // If S contains an item of form
      //   A -> B1 ... B[i-1] _ B[i] B[i+1] ... Bk / La, and there 
      //   exists a rule B[i] -> X1 ... Xm, then S contains 
      //      B[i] -> _ X1 ... Xm / First( B[i+1], ..., La ).

   void undoclosure( ); 
      // Undo the effect of a closure. This is done by removing all items 
      // of form A -> _ B1 ... Bn. In case the itemset was
      // obtained by a transition and a closure, this should return 
      // the initial item set. In case the itemset contains a rule of form
      // start -> _ X, the result may be empty. 

   itemset computepush( const symbol s ) const;
      // Constructs the itemset that one obtains when the 
      // transition under s is made. In order to obtain the complete result,
      // the itemset has to be closed first. 

   itemset getreductions( ) const;
      // Collects the set of reductions in this itemset. These are the
      // lr1 items that are positioned at the end of their rules. 

   unsigned int getnrreductions( ) const;
      // Equal to getreductions. size( ).

   itemset getlambdareductions( ) const;
      // Collects the set of reductions in this itemset the rule of which
      // has an empty rhs.

   std::list< const rule* > getreducingrules( const symbol s ) const; 
      // Constructs the list of rules that can be reduced when 
      // seeing symbol s. The list is sorted by rule number, lower
      // numbers first.

   symbolset getlookaheads( ) const;
      // Returns the union of all lookahead sets. These are the 
      // symbols under which a reduction is possible in this itemset.

   unsigned int setdefaultlookahead( );
      // Set the lookahead set of each reduction equal to { defaultred }.
      // Return the number of lookahead sets that were changed.
      // The result should not be greater than one.

   unsigned int size( ) const { return repr. size( ); }
   
   bool iswellformed( ) const;

   friend std::ostream& operator << ( std::ostream& , const itemset& ); 

};


std::ostream& operator << ( std::ostream& stream, const itemset& it );


#endif


