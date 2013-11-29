

// Code written by Hans de Nivelle, November 2006. 
// See the licence that was included with this code.


#include "itemset.h"


bool lr1item::iswellformed( ) const
{
   for( std::list< symbol > :: const_iterator
           q = r -> rhs. begin( );
           q != r -> rhs. end( );
           ++ q )
   {
      if( p == q )
         return true;
   }
   if( p == r -> rhs. end( ))
      return true;
   return false;
}


bool lr1item::isreduction( ) const
{
   return p == r -> rhs. end( );
}


std::ostream& operator << ( std::ostream& stream,
		            const lr1item& it )
{
   stream << (( it. r ) -> lhs ) << " --> ";
   for( std::list< symbol > :: const_iterator
           p = it. r -> rhs. begin( );
	   p != it. r -> rhs. end( );
	   ++ p )
   {
      if( p == it. p )
         stream << " # ";
      else
         stream << " ";

      stream << *p; 
   }

   if( it. p == it. r -> rhs. end( ))
      stream << " #";
        
   stream << "          ";
   stream << it. lookahead;
   stream << "\n";

   return stream;
}


bool itemset::insert( const lr1item& i ) 
{

   for( std::list< lr1item > :: iterator
           it = repr. begin( );
	   it != repr. end( );
	   ++ it )
   {
      if( ( it -> r ) == i.r && ( it -> p ) == i.p )
      {
         return it -> lookahead. insert( i. lookahead );
      }
   }

   repr. push_back(i); 
   return true;
}


bool itemset::insert( const itemset& other )
{
   bool change = false;
   for( std::list< lr1item > :: const_iterator
           it = other. repr. begin( );
	   it != other. repr. end( );
	   ++ it )
   {
      bool b = insert( *it );
      change = change | b;
   }
   return change;
}


namespace 
{

   bool basesubset( const std::list< lr1item > & it1,
		    const std::list< lr1item > & it2 )
   {
      for( std::list< lr1item > :: const_iterator
              q1 = it1. begin( );
	      q1 != it1. end( );
	      ++ q1 )
      {
         for( std::list< lr1item > :: const_iterator
                 q2 = it2. begin( );
		 q2 != it2. end( );
		 ++ q2 )
         {
            if( ( q1 -> r ) == ( q2 -> r ) && ( q1 -> p ) == ( q2 -> p ) )
               goto found;
         }
	 return false;
      found:
	 ;
      }
      return true;
   }

}


bool itemset::samebase( const itemset& other ) const
{
   return basesubset( this -> repr, other. repr ) &&
	  basesubset( other. repr, this -> repr );
}



void itemset::close( const grammar& g,
		     const symbolset& leftsymbols, 
                     const symbolset& lambdasymbols, 
	             std::map< symbol, symbolset > & firstsymbols )
{
   
   bool change = true;
   while( change )
   {
      change = false;

      for( std::list< lr1item > :: const_iterator 
              it = repr. begin( );
	      it != repr. end( );
	      ++ it )
      {
         if( ( it -> p ) != ( ( it -> r ) -> rhs. end( )) 
             && leftsymbols. contains( * ( it -> p )))
         {
            symbol s = *( it -> p );

	    // p_next is the position in item *it that comes after p.

	    std::list< symbol > :: const_iterator p_next = ( it -> p );
            ++ p_next;

	    // p_nonlambda is going to be the first position after p_next 
	    // containing a symbol that cannot be rewritten into lambda.
	    // In case all symbols after p_next can be rewritten into lambda,
	    // p_nonlambda will be the end of *it. 

	    std::list< symbol > :: const_iterator p_nonlambda = p_next; 
            while( p_nonlambda != ( it -> r ) -> rhs. end( ) && 
		   lambdasymbols. contains( *p_nonlambda ))
            {
               ++ p_nonlambda; 
            }

	    // We are going to compute the lookahead that
	    // item of form s -> _ W will receive:
	            
            symbolset lookahead;
		  
	    for( std::list< symbol > :: const_iterator
                    p = p_next;
		    p != p_nonlambda;
		    ++ p )
            {
               lookahead. insert( firstsymbols [ *p ] );
            }

	    // If p_nonlambda is not the end of *it, then we also
	    // add firstsymbols( *p_nonlambda ) to lookahead, otherwise,
	    // we add the lookahead of *it. 

	    if( p_nonlambda != ( it -> r ) -> rhs. end( ))
               lookahead. insert( firstsymbols [ * p_nonlambda ] );
	    else
               lookahead. insert( it -> lookahead );

	    // Now it's time to gradually start looking for rules of 
	    // form s -> W : 

	    for( std::vector< rule > :: const_iterator
                    r = g. rules. begin( );
		    r != g. rules. end( );
		    ++ r )
            {
               if( r -> lhs == s )
               {
                  // We found a rule of form s -> W.

                  bool b = insert( lr1item( & (*r), 
					    r -> rhs. begin( ), lookahead ));  
                  change = change | b;
               }
            }
         }
      }      
   }
}


void itemset::undoclosure( )
{
   std::list< lr1item > :: iterator it = repr. begin( );
   while( it != repr. end( ))
   { 
      std::list< lr1item > :: iterator it1 = it;
      ++ it1;

      if( ( it -> p ) == (( it -> r ) -> rhs. begin( )) )
         repr. erase( it );
      
      it = it1; 
   }
}


itemset itemset::computepush( const symbol s ) const
{
   itemset result;
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
	   it != repr. end( );
	   ++ it )
   {
      if( ( it -> p ) != (( it -> r ) -> rhs. end( )) &&
          *( it -> p ) == s )
      {
         std::list< symbol > :: const_iterator p_next = ( it -> p ); 
	 ++ p_next;
	 result. insert( lr1item( it -> r, p_next, it -> lookahead ));
      }
   }
   return result;
}


itemset itemset::getreductions( ) const
{
   itemset result;
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
	   it != repr. end( );
	   ++ it )
   {
      if( it -> isreduction( ))
         result. repr. push_back( *it ); 
   }
   return result;
}


unsigned int itemset::getnrreductions( ) const
{
   unsigned int nr = 0;
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
           it != repr. end( );
           ++ it )
   {
      if( it -> isreduction( ))
         ++ nr; 
   }
   return nr;
}


itemset itemset::getlambdareductions( ) const
{
   itemset result;
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
           it != repr. end( );
           ++ it )
   {
      if( it -> isreduction( ) && ( it -> r ) -> rhs. size( ) == 0 )
         result. repr. push_back( *it );
   }
   return result;
}


std::list< const rule* > itemset::getreducingrules( const symbol s ) const 
{
   std::list< const rule* > result;
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
	   it != repr. end( );
	   ++ it )
   {
      if( it -> isreduction( ) && it -> lookahead. contains(s) )
      {
         result. push_back( it -> r );
      }
   }

   // Next follows an example of an inefficient sorting algorithm:

   for( std::list< const rule* > :: iterator
           q = result. begin( );
           q != result. end( );
           ++ q )
   {
      for( std::list< const rule* > :: iterator
              p = result. begin( );
              p != q; 
              ++ p )
      {
         if( (*p) -> nr > (*q) -> nr )
         {
            const rule* r = *p;
            *p = *q;
            *q = r;
         }
      }
   }
   return result; 
}


symbolset itemset::getlookaheads( ) const
{
   symbolset result;

   for( std::list< lr1item > :: const_iterator
           p = repr. begin( );
           p != repr. end( );
           ++ p )
   {
      if( p -> isreduction( ))
         result. insert( p -> lookahead );
   }

   return result; 
}


unsigned int itemset::setdefaultlookahead( )
{
   unsigned int cntr = 0;

   for( std::list< lr1item > :: iterator
           p = repr. begin( );
           p != repr. end( );
           ++ p )
   {
      if( p -> isreduction( ))
      {
         p -> lookahead. clear( );
         p -> lookahead. insert( symbol::defaultred ); 

         ++ cntr;
      }
   }
   return cntr; 
}

 
bool itemset::iswellformed( ) const
{
   for( std::list< lr1item > :: const_iterator
           it = repr. begin( );
	   it != repr. end( );
	   ++ it )
   {
      if( ! it -> iswellformed( ))
         return false;
   }
   return true;
}


std::ostream& operator << ( std::ostream& stream, const itemset& s ) 
{

   for( std::list< lr1item > :: const_iterator
           it = s. repr. begin( );
	   it != s. repr. end( );
	   ++ it )
   {
      stream << "   ";
      stream << *it; 
   }

   stream << "\n";
   return stream;
}

  

