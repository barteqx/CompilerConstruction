

// Code written by Hans de Nivelle, November 2006.
// Simplified June 2008.
// See the licence that was included with the code.


#include "parsetable.h"


parsetable parsetable::construct( const grammar& g )
{
   parsetable res;

   // We create for each start symbol an initial state:

   for( std::map< symbol, symbolset > :: const_iterator
           s = g. starts. begin( );
           s != g. starts. end( );
           ++ s )
   {

      res. startstates [ s -> first ] = res. nrstates( );
    
      res. table. push_back( state( ));

      // We go through the grammar and look for rules that have
      //  s -> first as lhs. Those we insert into the first state.

      for( std::vector< rule > :: const_iterator
              r = g. rules. begin( );
	      r != g. rules. end( );
	      ++ r )
      {
         if( r -> lhs == s -> first )
         {
            res. table. back( ). set. insert( lr1item( & (*r), 
			                  r -> rhs. begin( ), s -> second ));
         }
      }
   }

   // Time to collect the symbols from the grammar:

   symbolset leftsymbols = g. leftsymbols( );
   symbolset lambdasymbols = g. nullablesymbols( );

   symbolset allsymbols = g. allsymbols( );

   std::map< symbol, symbolset > firstsets = g. firstsets( lambdasymbols );

   std::map< unsigned int, bool > unchecked;
      // All states reachable from unchecked states must be checked,
      // if something changes in the reachable states, they become 
      // unchecked as well. 
      // At this moment, the initial states are unchecked.

   for( unsigned int s = 0; s < res. nrstates( ); ++ s )
      unchecked [s] = true;  

   while( unchecked. size( ))
   {
      std::map< unsigned int, bool > :: iterator p = unchecked. begin( );
      unsigned int ss = ( p -> first );
      unchecked. erase(p);

      itemset closure = res. table [ss]. set;
      closure. close( g, leftsymbols, lambdasymbols, firstsets );

      // We are going to check the transitions from ss. 

      for( symbolset::const_iterator 
              p = allsymbols. begin( );
	      p != allsymbols. end( );
	      ++ p )
      {
         itemset trans = closure. computepush( *p );

	 if( res. table [ ss ]. transitions. find( *p ) != 
	     res. table [ ss ]. transitions. end( ))
         {
            // We already have the transition ss * *p -> tt. 

            unsigned int tt = res. table [ ss ]. transitions [ *p ];

	    ASSERT( res. table [tt]. set. samebase( trans ));
	       // The bases must be equal.

            bool b = res. table [tt]. set. insert( trans );
	       // Merge the lookaheads and check whether tt was
	       // changed. 

	    if(b)
               unchecked [tt] = true;

         }
	 else
         {
            // No transitions exists yet. If trans is nonempty, 
	    // we should create a transition.

	    if( trans. size( ))
            {
               // We check if there is a state with the same base.

               for( unsigned int tt = 0; tt < res. table. size( ); ++ tt )
               {
                  if( res. table [tt]. set. samebase( trans ))
                  {
                     // If we found it, we merge lookaheads,
		     // and add the transition. If lookahead is 
		     // changed, we add the state tt to unchecked.

                     bool b = res. table [tt]. set. insert( trans );
		     if(b)
                        unchecked [tt] = true;
		     res. table [ss]. transitions [ *p ] = tt; 
		     goto statefound; 
                  }
               }

	       // It is a new state. We add the transition, and mark 
	       // the new state as unchecked. 

	       {
	          unsigned int tt = res. table. size( ); 

	          res. table. push_back( state( ));
	          res. table [tt]. set = trans;

	          res. table [ss]. transitions [ *p ] = tt; 
	          unchecked [ tt ] = true;

               }
            statefound:
	       ;
            }
         }
      }
   }

   // We have to add reductions with empty rhs, because they can be 
   // introduced only by closure, and all our states were introduced
   // by shifts.

   for( unsigned int ss = 0; ss < res. table. size( ); ++ ss )
   {
      itemset closure = res. table [ss]. set;
      closure. close( g, leftsymbols, lambdasymbols, firstsets );
      closure = closure. getlambdareductions( );  
      res. table [ss]. set. insert( closure ); 
   }

   // There is a remaining problem: For the correct functioning of the
   // parser, it is necessary that from the initial states of the parser,
   // a transition under the start symbol is necessary. For each start
   // symbol, we check if such a transition exists. If not, then we 
   // add one. 

   for( std::map< symbol, unsigned int > :: const_iterator
           p = res. startstates. begin( );
           p != res. startstates. end( );
           ++ p )
   {
      std::map< symbol, unsigned int > :: const_iterator
            it = res. table [ p -> second ]. transitions. find( p -> first );

      if( it == res. table [ p -> second ]. transitions. end( ))
      {
         // No transition exists, we add one. 
         // The new state remains empty.

         res. table [ p -> second ]. transitions [ p -> first ] = 
                 res. nrstates( );    
         res. table. push_back( state( )); 
      }
   }
   return res; 
}


namespace
{

   // True if conflicts exist:

   bool conflictexists( const std::list< const rule* > & pushing,
                        const std::list< const rule* > & reducing )
   {
      if( pushing. size( ) && reducing. size( ))
         return true;

      if( reducing. size( ) > 1 )
         return true;

      return false;
   }


 
   void printrulelist( const std::list< const rule* > & list )
   {
      std::cout << "rule list:\n"; 
      for( std::list< const rule* > :: const_iterator
              p = list. begin( );
	      p != list. end( );
	      ++ p )
      {
         std::cout << "   " << *(*p) << "\n";
      }
      std::cout << "\n";
   }

}


void parsetable::printconflicts( std::ostream& stream )
{
   for( unsigned int ss = 0; ss < table. size( ); ++ ss )
   {
      symbolset relevantsymbols = getrelevantsymbols( ss );

      symbolset shiftreduce;
      symbolset reducereduce;

      for( symbolset::const_iterator
              p = relevantsymbols. begin( );
	      p != relevantsymbols. end( );
	      ++ p )
      {

	 std::list< const rule* > reducing =
		        table [ ss ]. set. getreducingrules( *p );

         if( table [ss]. transitions. find(*p) != 
                table [ss]. transitions. end( ) && 
                   reducing. size( ))
         {
            shiftreduce. insert(*p); 
         }

         if( reducing. size( ) > 1 )
         {
            reducereduce. insert(*p); 
         }
      }
  
      if( shiftreduce. size( ))
      {
         stream << "State S" << ss << " has shift/reduce conflicts for ";
         stream << shiftreduce << "\n";
      }

      if( reducereduce. size( ))
      {
         stream << "State S" << ss << " has reduce/reduce conflicts for ";
         stream << reducereduce << "\n";
      }
   } 
}


void parsetable::setdefaultreductions( )
{

   for( unsigned int ss = 0; ss < table. size( ); ++ ss )
   {
      if( table [ss]. set. getnrreductions( ) == 1 )
      {
         symbolset lookahead = table [ss]. set. getlookaheads( );
        
         bool conflictfound = false; 
 
         for( symbolset::const_iterator 
                 p = lookahead. begin( );
                 p != lookahead. end( ) && !conflictfound;
                 ++ p )
         {
            if( table [ss]. transitions. find(*p) !=
                table [ss]. transitions. end( ))
            {
               conflictfound = true;
            }
         }

         if( ! conflictfound ) 
         {
            unsigned int k = table [ss]. set. setdefaultlookahead( );
            ASSERT( k == 1 ); 
         }
      }
   }
}
    

symbolset parsetable::getrelevantsymbols( unsigned int ss ) const
{
   ASSERT( ss < table. size( ));

   symbolset result = table [ss]. set. getlookaheads( );

   for( std::map< symbol, unsigned int > :: const_iterator 
           p = table [ss]. transitions. begin( );
           p != table [ss]. transitions. end( );
           ++ p )
   {
      result. insert( p -> first );
   }

   return result;
}


std::ostream& operator << ( std::ostream& stream,
                            const parsetable::state& s )
{
   stream << s. set;

   if( s. transitions. size( ) != 0 )
   {
      stream << "Transitions:\n";
      for( std::map< symbol, unsigned int > :: const_iterator
              p = s. transitions. begin( );
              p != s. transitions. end( );
	      ++ p )
      {
         stream << "   " << (p -> first) << " : S" << (p -> second) << "\n";
      }
      stream << "\n";
   }

   itemset red = s. set. getreductions( );
   if( red. size( ))
   {
      stream << "Reductions:\n";
      stream << red << "\n";
   } 
   stream << "\n"; 
   return stream; 
}


std::ostream& operator << ( std::ostream& stream,
		            const parsetable& pt )
{
   stream << "Parsetable:\n\n";
   stream << "==========================================\n\n"; 

   for( std::map< symbol, unsigned int > :: const_iterator
           p = pt. startstates. begin( );
           p != pt. startstates. end( );
           ++ p )
   {
      stream << "   " << ( p -> first ) << " : S" << ( p -> second ) << "\n";
   }
   stream << "\n\n";

   for( unsigned int ss = 0; ss < pt. table. size( ); ++ ss )
   {
      stream << "S" << ss << ":" << "\n";
      stream << pt. table [ss]; 
      stream << "\n"; 
   }
   stream << "\n\n";
   return stream; 
}




