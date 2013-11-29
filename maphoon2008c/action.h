

// Code written by Hans de Nivelle, June 2008.
// See the licence that was included with this code.
//
// An action is attached to a rule and it specifies what has to 
// be done when the rule is reduced. Ideally, actions have
// no side effects. In that case, they simply describe how the 
// attribute of the left hand side are computed from the
// attributes of the right hand sides. 
//
// The way that one refers to the parse stack is different from
// Maphoon, and also from Yacc. 
// 
// Instead of using variables $$,$1,$2, we provide
// variables of type  std::list< token > :: iterator 
// 
// The name of the variable is determined by the type.
// If A is the type of the token, then the variable will be called
// A as well. In case there are more tokens of the same type, the
// variable will be called A1, A2, A3, etc. 
//
// The left hand side is constructed by a statement of type
// return A1, or return X.
// In principle, the user is himself responsible for maintaining
// the right type of attribute.
//
// Maphoon creates for each action a separate function of form
// void redX( std::list< token > & stack, 
//            G1& g1, ..., Gn& gn, 
//            const std::list< token > :: iterator position,
//            std::list< token > :: iterator a1, 
//            std::list< token > :: iterator a2,
//            ...,
//            std::list< token > :: iterator an, 
//            const std::list< token > & lookahead ) throw( refused )
//
// g1, ..., gn are the global variables. 
// The variables position and length indicate the point on the stack
// where the reduction is made, and how long the rhs of the rule is.
// The user should not look at these variables. Their purpose is to
// be able to correctly modify the stack after the reduction is made.
// Inside the action, 
// return ai is replaced by reduce( stack, position, length, ai ); return;
// return x is replaced by reduce( stack, position, length, x ); return;
// 
// Two types of return statements are possible: 
//    Return ai, where ai is one of the iterators. reduce( ) will 
//    erase the other iterators. It is important that *ai is not moved.
//    In this way, it is possible to build up large attributes without
//    overhead caused by the need to move the object. This is the
//    primary reason why we chose std::list instead of std::vector for
//    maintaining the parse stack.
// 
//    Return t, where t is a newly constructed token. reduce( ) will
//    erase all other iterators. It will ASSERT that t has the right type.
// 
// In addition to make a valid reduction, it is also possible to
// throw( refused ). This is maphoon's method for handling conflicts.
// In case there is a conflict, maphoon will try all possible reductions,
// and perform the first one that does not refuse. When making the
// decision, the user could look at lookahead. 
// (But, in case the rule occurs in different states, it is possible that
// not all states have a lookahead. Therefore, one should check that
// lookahead. size( ) != 0 before looking at lookahead. front( ). 



#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED   1


#include <map>
#include <string>
#include <list>


#include "symbol.h"
#include "attributesystem.h"
#include "rule.h"


// Global variables are passed to every action that uses them.
// A structure of type globalvars declares the global variables and their
// types. Typical things that you want to store in global variables
// are identifier lookup tables, maybe type declarations, definitions
// of infix-postfix-prefix operators.


struct globalvariables 
{

   struct globalvar
   {
      std::string type;
      std::string name;

      globalvar( const std::string& type, const std::string& name )
         : type( type ), name( name )
      {
      }

   };

   std::list< globalvar > list;

   void add( const std::string& type, const std::string& name )
   {
      list. push_back( globalvar( type, name ));
   }

};


std::ostream& operator << ( std::ostream& stream,  
                            const globalvariables& glob );


class action
{
   std::string filename;
   unsigned int linenumber;

   std::list< std::string > code;

public: 
   action( const std::string& filename, unsigned int linenumber );

   void addline( const std::string& s );
      // Add line s. In case s contains \n, s is split into
      // multiple lines. 

   std::list< unsigned int > 
   identifierpositions( const std::string& s, unsigned int line ) const 
   throw( actionerror );
      // Returns the set of indices in s, on which an identifier starts.
      // "aa+bb4" would return 0,3. The line is used only for creating
      // error messages. Identifiers inside quotes, or in comments are 
      // not added to the list. Action errors are thrown when there is 
      // something wrong with quotes or with comments. We allow comments 
      // of form // bla bla bla \n, and of form /* bla bla bla */, but 
      // multiline comments of form /*   ... */ are not allowed.
     
   bool uses( const std::string& identifier ) const;
      // True if the action uses the identifier. 

   bool isempty( ) const;
      // True if the action is empty. 

   void printcall( std::ostream& stream, const globalvariables& glob,
                   const rule& r ) const;
   void printdefinition( std::ostream& stream,
                         const globalvariables& glob,
                         const std::list< std::string > & parsernamespace, 
                         const std::list< std::string > & tokennamespace, 
                         const rule& r ) const throw( actionerror );
      // Prints the function definition of the action.

   friend std::ostream& operator << ( std::ostream& stream, const action& a );
};


std::ostream& operator << ( std::ostream& stream, const action& a );

#endif


