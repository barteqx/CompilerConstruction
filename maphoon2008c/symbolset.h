
// Code written by Hans de Nivelle, october 2006. 
// See the licence that was included with this code.


#ifndef SYMBOLSET_INCLUDED
#define SYMBOLSET_INCLUDED   1

#include <map>
#include <iostream>

#include "assert.h" 
#include "symbol.h"


class symbolset
{
   
   std::map< symbol, bool > repr;


public: 
   bool contains( symbol s ) const;

   bool insert( symbol s );
      // Returns true if s did not occur before.

   unsigned int insert( const symbolset& s );
      // Returns the number of elements actually inserted. 
      // (those that did not occur already)

   bool remove( symbol s );
      // Returns true if s occurred.

   unsigned int remove( const symbolset& s );
      // Returns the number of elements actually removed.

   unsigned int size( ) const;

   void clear( );

   friend std::ostream& operator << ( std::ostream& , const symbolset& s );


   struct const_iterator
   {
      std::map< symbol, bool > :: const_iterator p;

      explicit const_iterator( std::map< symbol, bool > :: const_iterator p )
         : p(p)
      {
      }

      const_iterator operator ++ ( int )
      {
         return const_iterator( p ++ );
      }

      const_iterator operator -- ( int )
      {
         return const_iterator( p -- );
      }

      const_iterator operator ++ ( ) 
      {
         return const_iterator( ++ p );
      }

      const_iterator operator -- ( ) 
      {
         return const_iterator( -- p );
      }

      bool operator == ( const_iterator other )
      {
         return ( this -> p ) == other. p;
      }

      bool operator != ( const_iterator other )
      {
         return ( this -> p ) != other. p;
      }

      symbol operator* ( ) 
      {
         ASSERT( p -> second );
	 return p -> first;
      }
   };

   const_iterator begin( ) const 
   { return const_iterator( repr. begin( )); }

   const_iterator end( ) const
   { return const_iterator( repr. end( )); }

};


std::ostream& operator << ( std::ostream& stream, const symbolset& s );


#endif


