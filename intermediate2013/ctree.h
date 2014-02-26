
// Written by Hans de Nivelle, december 2013, for the lab on 
// compiler construction. 

#ifndef CTREE_INCLUDED 
#define CTREE_INCLUDED    1

#include "identifier.h"
#include "ctype.h"
#include "data.h"

#include <iostream> 
#include <vector>


enum class treetype
{
   data, 
      // Contains a vector of char, representing the value of the data. 

   var, 
      // Carries an identifier, which is just the name of the variable. 

   func
      // A functional tree has form ident( t1, ..., tn ). 
};


class ctree 
{

   struct ctr
   {
      treetype t;
      unsigned int refcnt;

      ctr( treetype t ) : t(t), refcnt(1) { }

      virtual std::vector< char > :: const_iterator data_begin( ) const 
         { QUIT( ); }
      virtual std::vector< char > :: const_iterator data_end( ) const 
         { QUIT( ); }

      virtual const identifier& getvarname( ) const { QUIT( ); } 

      virtual const identifier& getfunction( ) const { QUIT( ); }
      virtual unsigned int getarity( ) const { QUIT( ); } 
      virtual const ctree& operator [] ( unsigned int ) const { QUIT( ); } 

      void printindent( std::ostream& stream, unsigned int indent ) const
      { 
          for( unsigned int i = 0; i < indent; ++ i )
             stream << ".  ";
      }

      virtual void prettyprint( std::ostream &, unsigned int depth ) const = 0; 
      virtual void print( std::ostream& , unsigned int depth ) const = 0; 
      virtual ~ctr( ) { } 
  
   };

   struct ctr_data;
   struct ctr_var;
   struct ctr_func;

private:
   ctr* ref;
   
public: 
   ctype ct;  
      // It must be a public field because it must be possible to
      // reassign. 

   ctree( const std::vector< char > & data, const ctype& = type_unknown );
   ctree( std::vector< char > && data, const ctype& = type_unknown );

   ctree( const identifier& varname, const ctype& = type_unknown ); 

   ctree( const identifier& f, const std::vector< ctree > & args,
          const ctype& = type_unknown );
   ctree( const identifier& f, std::vector< ctree > && args,
          const ctype& = type_unknown ); 

   ctree( const ctree& other );
   void operator = ( const ctree& other );
   void operator = ( ctree&& other ); 
   ~ctree( );
   
   treetype gettreetype( ) const {  return ref -> t; }
   std::vector< char > :: const_iterator data_begin( ) const 
      { return ref -> data_begin( ); } 
   std::vector< char > :: const_iterator data_end( ) const
      { return ref -> data_end( ); }

   const identifier& getvarname( ) const { return ref -> getvarname( ); } 

   const identifier& getfunction( ) const { return ref -> getfunction( ); }
   unsigned int getarity( ) const { return ref -> getarity( ); } 
   const ctree& operator [] ( unsigned int i ) const { return (*ref)[i]; } 

   static int compare( const ctree& t1, const ctree& t2 );
      // -1 means:      t1 < t2. 
      //  0 means:      t1 == t2. 
      //  1 means:      t1 > t2. 
      // The order is not a well-order. Comparison ignores the types. 


   bool operator < ( const ctree& other ) const
      { return compare( *this, other ) == -1; }

   bool operator > ( const ctree& other ) const
      { return compare( *this, other ) == 1; }

   bool operator == ( const ctree& other ) const
      { return compare( *this, other ) == 0; }

   bool operator != ( const ctree& other ) const  
      { return compare( *this, other ) != 0; }

   bool operator >= ( const ctree& other ) const
      { return compare( *this, other ) != -1; }

   bool operator <= ( const ctree& other ) const
      { return compare( *this, other ) != 1; }

   void print( std::ostream& stream, unsigned int depth ) const
      { ref -> print( stream, depth ); }
      // Prints the term without atributes. 
 
   void prettyprint( std::ostream& stream, unsigned int depth ) const
      { ref -> prettyprint( stream, depth ); } 

   void prettyprint( std::ostream& stream ) const
      { prettyprint( stream, 0 ); stream << "\n   : " << ct << "\n"; }

};

inline std::ostream& operator << ( std::ostream& stream, const ctree& t )
{
   t. print( stream, 0 );
   stream << " : " << t. ct;
   return stream;
}


#endif


