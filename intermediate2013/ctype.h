
// Written by Hans de Nivelle for Class Compiler Construction.
// A Ctype is a type that can occur in C/C++.
//
// I thought about adding substitution to this class, but I
// think 2nd order operators are so few that they should be hardcoded.

#ifndef CTYPE_INCLUDED
#define CTYPE_INCLUDED  2013

#include <vector>
#include <map> 
#include "assert.h"
#include "identifier.h" 

// The type constructors: 

enum type_constr
{
   type_void,
   type_unknown,
   
   type_prim, 

   type_const, 
   type_raw,
   type_pointer,
   type_ref,
   type_rvalref, 
   type_reg, 
    
   type_array,
   type_func 
};


class ctype
{
   struct ct
   {
      type_constr t;
      unsigned int refcnt;

      ct( type_constr t ) : t(t), refcnt(1) { } 

      virtual const identifier& getprimtype( ) const { QUIT( ); }
         // Only prim has an identifier. 

      virtual const ctype& getsubtype( ) const { QUIT( ); } 
         // Only const,raw,pointer,ref,rvalref,reg, and array have a subtype.

      virtual unsigned int getsize( ) const { QUIT( ); } 
         // Only array has a size.

      virtual const ctype& getrestype( ) const { QUIT( ); }
         // Only function has a result type.

      virtual unsigned int getarity( ) const { QUIT( ); }
         // Only function has an arity. 
 
      virtual const ctype& operator [] ( unsigned int i ) const { QUIT( ); }
         // Gets the i-th argument type, only function has this.

      virtual void print( std::ostream& ) const = 0; 

      virtual ~ct( ) { } 
   };


   struct ct_nullary;
      // Used for void,unknown.

   struct ct_prim;

   struct ct_unary;
      // Used for const,raw,pointer,ref,rvalref,reg.

   struct ct_array;
   struct ct_func;


   ct* ref; 
public:
   ctype( type_constr t );
      // Constructs void or unknown.

   ctype( type_constr t, const identifier& id );
      // Constructs prim.

   ctype( type_constr t, const ctype& sub ); 
      // Constructs a unary type of form const(sub), raw(sub), pointer(sub), 
      // ref(sub), rvalref(sub) or reg(sub). 

   ctype( type_constr t, unsigned int s, const ctype& sub );
      // Constructs array( s, sub ). 

   ctype( type_constr t, const ctype& res, 
                         const std::vector< ctype > & args );
   ctype( type_constr t, const ctype& res,
                         std::vector< ctype > && args ); 
      // Construct func( res; args[0], ..., args[n-1] ), where res is 
      // the result type, and args are the argument types. 

   type_constr gettypeconstr( ) const { return ref -> t; }

   const identifier& getprimtype( ) const 
      { return ref -> getprimtype( ); } 
         // Only prim has an identifier. 

   const ctype& getsubtype( ) const { return ref -> getsubtype( ); } 
      // Get our subtype, when we are const,raw,pointer,ref,rvalref,reg,
      // or array. 

   unsigned int getsize( ) const { return ref -> getsize( ); } 
      // Gets the array size, in case we are an array.  

   unsigned int getarity( ) const { return ref -> getarity( ); } 
      // Gets our arity, when we are a function. 

   const ctype& getrestype( ) const { return ref -> getrestype( ); } 
      // Gets the type of our result, when are a function type.

   const ctype& operator [] ( unsigned int i ) const { return (*ref)[i]; } 
      // Gets the type of our i-th argument, when we are function, and 
      // 0 <= i < getarity( ). 

   ctype( const ctype& c ); 
   void operator = ( const ctype& c );
   void operator = ( ctype&& c ); 
   ~ctype( ); 

   static int compare( const ctype& c1, const ctype& c2 ); 

   void print( std::ostream& stream ) const { ref -> print( stream ); } 
      
   bool operator < ( const ctype& other ) const
      { return compare( *this, other ) == -1; }

   bool operator > ( const ctype& other ) const
      { return compare( *this, other ) == 1; }

   bool operator == ( const ctype& other ) const
      { return compare( *this, other ) == 0; }

   bool operator != ( const ctype& other ) const
      { return compare( *this, other ) != 0; }

   bool operator >= ( const ctype& other ) const
      { return compare( *this, other ) != -1; }

   bool operator <= ( const ctype& other ) const
      { return compare( *this, other ) != 1; }

};

std::ostream& operator << ( std::ostream& , type_constr t ); 

inline std::ostream& operator << ( std::ostream& stream, const ctype& c )
{
   c. print( stream ); return stream;
}

#endif

