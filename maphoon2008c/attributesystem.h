

// Written by Hans de Nivelle, June 2008.
// See the licence that was included with this code.


#ifndef ATTRIBUTESYSTEM_INCLUDED
#define ATTRIBUTESYSTEM_INCLUDED   1


// The attribute system manages the attachment of attributes to 
// symbols. 
// It is important to understand our attribute model:
// A symbol is a structure that one field that determines the type 
// of the symbol.
// In addition, it has fields for storing attributes.
// Each field is an std::list. 
// As a consequence, each possible attribute can occur 
// 0, 1, ..., any number of times. It is also possible that one symbol
// has different attributes of different types. 
// For example, we may have:
// struct symbol 
// {
//    enum symboltype type;
//    std::list< std::string > aa;
//    std::list< formula > bb;
//    std::list< unsigned int > cc;
// };
//
// In this case, a symbol can have any number of std::strings, formulas,
// and unsigned ints. 
// 
// The types of the attributes should always have object semantics, so 
// that they can be safely put in lists.
// 
// In the actions that are attached to the grammar rules, the attributes
// of the tokens are read from and assigned to. 
// With such a liberal attribute model, it is difficult to check
// at parser generation time, that the accessed attributes exist.
// We do not try to do that. Instead, we offer very strict run time checks.
// Eventually, these checks can be switched of, but the user should do 
// this only after many test runs. 
 


#include <iostream>
#include <string>
#include <map>
#include <list>


#include "errors.h" 
#include "symbol.h"
#include "symbolset.h"


struct attributesystem 
{

   std::map< std::string, std::string > attributetypes; 
      // Maps the attribute names to the C++ types.
      // In the example above, we would have
      // attributetypes [aa] = std::string,
      // attributetypes [bb] = formula, 
      // attributetypes [cc] = unsigned int.

   std::map< symbol, std::map< std::string, std::list< unsigned int > > >
      constraints;
         // Stores for each (token,attribute) the possible sizes
         // that the attribute list can have. 
         // The semantics of the list of unsigned ints is as follows:
         // No list or empty list: The list has size 0. 
         // List of length 1 with contents n. The list has at least
         // size n. 
         // List of length 2 with contents (n,m) The list has at least   
         // size n, but the size is less than m.

         // If you want to specify that a message has exactly one string,
         // you put:
         // constraints [ message ] [ aa ] = ( 1, 2 ).
         // If you want to say that a formulalist has 0 or more formulas,
         // you put
         // constraints [ formula ] [ bb ] = ( 0 ).

public:
   bool isattribute( const std::string& attrname ) const;

   void addattribute( const std::string& attrname, 
                      const std::string& cpptype ); 
      // We do not check that attrname is a good attribute. The user
      // has to do this by himself (by using cpp::wellformedalone) 

   bool hasconstraint( symbol symb, const std::string& attr ) const;
      // True if we have a constraint for this combination.

   void addconstraint( symbol symb, const std::string& attr,  
                       unsigned int lowerbound );
   void addconstraint( symbol symb, const std::string& attr,
                       unsigned int lowerbound, unsigned int upperbound );

   void printtokendecl( const symbolset& alphabet, 
                        std::ostream& stream, 
                        const std::list< std::string > & n,
                        const std::string& s,
                        const std::list< std::string > & i );
      // Prints the declaration (.h file) of token in C++ style.  
      // n is the namespace, s is the name of token, i is copied into
      // the (.h file). 

   void printtokencode( const symbolset& alphabet,
                        std::ostream& stream,
                        const std::list< std::string > & n,
                        const std::string& s ); 
      // Print the definition (.cpp file) of token. The token declaration
      // should be in a file called token.h, because we include a file    
      // with this name. 

};


std::ostream& operator << ( std::ostream& stream, const attributesystem& sys );


#endif


