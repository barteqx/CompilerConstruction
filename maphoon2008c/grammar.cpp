

// Written by Hans de Nivelle, November 2006.
// See the licence that was included with this code.


#include "grammar.h"


grammar::grammar( )
{
}


void grammar::insert( const rule& r, const action& a )
{
   ASSERT( r. lhs != symbol::recover );
   ASSERT( r. lhs != symbol::defaultred );

   rules. push_back(r);
   rules. back( ). nr = actions. size( );
   actions. push_back(a);
}


void grammar::insert( symbol start, const symbolset& lookahead )
{
   ASSERT( !lookahead. contains( start ));
   starts [ start ]. insert( lookahead );
}


void grammar::insert( symbol start, symbol lookahead )
{
   ASSERT( start != lookahead );
   starts [ start ]. insert( lookahead );
}


symbolset grammar::leftsymbols( ) const
{
   symbolset result;
   for( std::vector< rule > :: const_iterator
           r = rules. begin( );
	   r != rules. end( );
	   ++ r )
   {
      result. insert( r -> lhs );
   }
   return result;
}


symbolset grammar::rightsymbols( ) const
{
   symbolset result; 
   for( std::vector< rule > :: const_iterator
           r = rules. begin( );
	   r != rules. end( );
	   ++ r )
   {
      for( std::list< symbol > :: const_iterator
              p = r -> rhs. begin( );
	      p != r -> rhs. end( );
	      ++ p )
      {
         result. insert( *p );
      }
   }
   return result;
}


symbolset grammar::startsymbols( ) const
{
   symbolset result;
   for( std::map< symbol, symbolset > :: const_iterator
           p = starts. begin( );
           p != starts. end( );
           ++ p )
   {
      result. insert( p -> first );
   }
   return result;
}


symbolset grammar::lookaheadsymbols( ) const
{
   symbolset result;
   for( std::map< symbol, symbolset > :: const_iterator
           p = starts. begin( );
           p != starts. end( );
           ++ p )
   {
      result. insert( p -> second );
   }
   return result;
}


symbolset grammar::allsymbols( ) const
{
   symbolset result;
   result. insert( leftsymbols( ));
   result. insert( rightsymbols( ));
   result. insert( startsymbols( ));
   result. insert( lookaheadsymbols( ));
   return result;
}


symbolset grammar::nullablesymbols( ) const
{
   symbolset result;

   bool change = true;
   while( change )
   {
      change = false;

      for( std::vector< rule > :: const_iterator 
              p = rules. begin( );
	      p != rules. end( );
	      ++ p )
      {
         if( ! result. contains( p -> lhs ))
         {
            bool rhs_empty = true; 
	       // We check whether all symbols in the rhs can rewrite
	       // into the empty word. 

	    for( std::list< symbol > :: const_iterator 
                    q = p -> rhs. begin( );
		    q != p -> rhs. end( ) && rhs_empty; 
		    ++ q )
            {
               if( ! result. contains(*q) )
                  rhs_empty = false;
            }

	    if( rhs_empty )
            {
               result. insert( p -> lhs );
               change = true;
            }
         }
      }
   }

   return result;
}


std::map< symbol, symbolset > grammar::reachabilitysets( ) const
{
   std::map< symbol, symbolset > result;

   // Every symbol of the grammar is reachable from itself:

   symbolset all = allsymbols( );
   for( symbolset::const_iterator
           p = all. begin( );
           p != all. end( );
           ++ p )
   {
      result[ *p ]. insert( *p );
   }

   // For every rule A -> B1 ... Bk, each Bi is reachable from A.
 
   for( std::vector< rule > :: const_iterator
           r = rules. begin( );
           r != rules. end( );
           ++ r )
   {
      for( std::list< symbol > :: const_iterator
               p = r -> rhs. begin( );
               p != r -> rhs. end( );
               ++ p )
      {
         result [ r -> lhs ]. insert( *p );
      } 
   }

   // And we compute the transitive closure:
   // If B in result[A], C in result[B] then C must be in result [A].

   bool change = true;
   while( change )
   {
      change = false;
      for( std::map< symbol, symbolset > :: iterator
              p = result. begin( );
              p != result. end( );
              ++ p )
      {
         symbolset s; 
         for( symbolset::const_iterator
                 q = p -> second. begin( );
                 q != p -> second. end( );
                 ++ q )
         {
            s. insert( result [ *q ] );
         }

         if( p -> second. insert(s))
            change = true;
      } 
   }
   return result; 
}


std::map< symbol, symbolset > 
grammar::firstsets( const symbolset& nullable ) const
{
   std::map< symbol, symbolset > result;

   symbolset symbols = allsymbols( ); 

   // Every symbol is in its own first set:

   for( symbolset::const_iterator 
           p = symbols. begin( );
	   p != symbols. end( );
	   ++ p )
   {
      result [ *p ]. insert( *p );
   }

   bool change = true;
   while( change )
   {
      change = false;
      for( std::vector< rule > :: const_iterator
              r = rules. begin( );
	      r != rules. end( );
	      ++ r )
      {
	 std::list< symbol > :: const_iterator pos = r -> rhs. begin( );
         bool nullablebeforepos = true;

         while( pos != r -> rhs. end( ) && nullablebeforepos ) 
         {
	    bool b = result [ r -> lhs ]. insert( result [ *pos ] );
            if(b)
               change = true;

            if( !nullable. contains( *pos ))
               nullablebeforepos = false;
            ++ pos; 
         }
      }
   }

   return result;
}


grammar grammar::example1( )
{
   grammar g;

   g. insert( symbol( "S" ), symbol( "EOF" ));

   std::list< symbol > rhs;
   rhs. push_back( symbol( "E" ));
   rhs. push_back( symbol( "dot" ));
   g. insert( rule( symbol( "S" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "op" ));
   rhs. push_back( symbol( "E" ));
   g. insert( rule( symbol( "E" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "E" ));
   rhs. push_back( symbol( "op" ));
   rhs. push_back( symbol( "E" ));
   g. insert( rule( symbol( "E" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "E" ));
   rhs. push_back( symbol( "op" ));

   rhs. clear( );
   rhs. push_back( symbol( "lpar" ));
   rhs. push_back( symbol( "E" ));
   rhs. push_back( symbol( "rpar" ));

   g. insert( rule( symbol( "E" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "id" ));
   g. insert( rule( symbol( "E" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "num" ));
   g. insert( rule( symbol( "E" ), rhs, 0 ));

   rhs. clear( );
   rhs. push_back( symbol( "id" ));
   g. insert( rule( symbol( "op" ), rhs, 0 ));

   return g;
}


grammar grammar::example2( )
{
   symbol sss = symbol( "S" );

   symbol aaa = symbol( "A" );
   symbol bbb = symbol( "B" );
   symbol ccc = symbol( "C" );

   symbol aa = symbol( "a" );
   symbol bb = symbol( "b" );
   symbol cc = symbol( "c" );

   std::list< symbol > rhs;
   grammar g;
   g. insert( sss, symbol( "EOF" ));

   rhs. clear( );
   rhs. push_back( aaa ); rhs. push_back( aa ); rhs. push_back( aaa ); 
   rhs. push_back( bb );
   g. insert( rule( sss, rhs, 0 ));

   rhs. clear( );
   rhs. push_back( cc ); rhs. push_back( bb );
   rhs. push_back( cc ); rhs. push_back( aa );
   g. insert( rule( sss, rhs, 0 ));

   rhs. clear( );
   rhs. push_back( bbb );
   g. insert( rule( aaa, rhs, 0 ));

   rhs. clear( );
   // rhs. push_back( cc );
   g. insert( rule( bbb, rhs, 0 ));

   return g;
}



std::ostream& operator << ( std::ostream& stream, const grammar& g )
{
   stream << "Start symbols / Lookaheads:\n";
   stream << g. starts << "\n\n";

   stream << "Rules:\n\n"; 
   for( unsigned int nr = 0; nr < g. rules. size( ); ++ nr ) 
   {
      stream << g. rules [nr] << "\n";
   }
   stream << "\n\n";

   stream << "Actions:\n\n";
   for( unsigned int nr = 0; nr < g. rules. size( ); ++ nr )
   {
      if( ! g. actions [nr]. isempty( ))
         stream << nr << ": " << g. actions [nr] << "\n";
   } 
   stream << "\n";

   return stream; 
}


std::ostream& operator << ( std::ostream& stream, 
                            const std::map< symbol, symbolset > & firstsets )
{
   for( std::map< symbol, symbolset > :: const_iterator
           p = firstsets. begin( );
	   p != firstsets. end( );
	   ++ p )
   {
      std::cout << "   " << p -> first << " : " << p -> second << "\n";
   }

   return stream;
}


