
// This code was produced by Maphoon 2008.
// Definition of struct token:


#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED    1


#include <list>
#include <iostream>



enum tokentype
{
   tkn__recover,
   tkn__defaultred,
   tkn_Eof,
   tkn_S,
   tkn_plus,
   tkn_T,
   tkn_U,
   tkn_identifier,
   tkn_lpar,
   tkn_rpar
};


struct token
{

   tokentype type;


   token( tokentype t )
      : type(t)
   {
   }

   token( );
      // Should have no definition.

   bool iswellformed( ) const;
      // Check if the attributes satisfy the
      // constraints.
};

std::ostream& operator << ( std::ostream& , const token& );



#endif


