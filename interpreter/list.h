 
// Written 27 nov 2013 for class compiler construction.
// Simple datastructure for LISP-style lists.

#ifndef LIST_INCLUDED
#define LIST_INCLUDED  1

#include <iostream>

#include "assert.h"
#include "bigint.h"

class list
{
   struct lst
   {
      unsigned int refcnt;

      lst( ) : refcnt(1) { } 

      virtual bool isbigint( ) const { return false; }
      virtual bool isstring( ) const { return false; }
      virtual bool iscons( ) const { return false; } 
      virtual bool isEOF( ) const { return false; }
      virtual bool istrue( ) const { return false; }
      virtual bool islambda( ) const { return false; } 
      virtual bool isnil( ) const { return false; } 

  
      virtual const bigint& getbigint( ) const { QUIT( ); }
      virtual const std::string& getstring( ) const { QUIT( ); }
      virtual const list& getfirst( ) const{ QUIT( ); }
      virtual const list& getrest( ) const{ QUIT( ); }
      
      virtual void print( std::ostream& out ) const = 0; 
     
      lst( const lst& ) = delete;
      void operator = ( const lst& ) = delete;
      virtual ~lst( ) { }  
   };

   struct lst_bigint;
   struct lst_string;
   struct lst_cons;       

   lst* ref;

public: 
   // Constructors:
   list( unsigned int i );
   list( const bigint& i );
   list( const std::string&s );
   list( const list& first, const list& rest );

   // Structural operators:

   list( const list& l ); 
   void operator = ( const list& l );
   void operator = ( list&& l );
   ~list( );

   // Inspectors:

   bool isbigint( ) const { return ref -> isbigint( ); }
   bool isstring( ) const { return ref -> isstring( ); }
   bool iscons( ) const { return ref -> iscons( ); }
   bool isEOF( ) const { return ref -> isEOF( ); }
   bool istrue( ) const { return ref -> istrue( ); }
   bool islambda( ) const { return ref -> islambda( ); }
   bool isnil( ) const { return ref -> isnil( ); }
    
   const bigint& getbigint( ) const { return ref -> getbigint( ); }
   const std::string& getstring( ) const { return ref -> getstring( ); }
   const list& getfirst( ) const { return ref -> getfirst( ); }
   const list& getrest( ) const{ return ref -> getrest( ); }
    
   void print( std::ostream& out ) const { ref -> print( out ); } 

   friend std::ostream& operator << ( std::ostream& , list );

};


#endif


