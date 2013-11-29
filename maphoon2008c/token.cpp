
// This code was produced by Maphoon 2008.
// Code of struct token:


#include "token.h"


bool token::iswellformed( ) const
{
   switch( type )
   {
   case tkn__recover:
   case tkn__defaultred:
   case tkn_Eof:
   case tkn_S:
   case tkn_plus:
   case tkn_T:
   case tkn_U:
   case tkn_identifier:
   case tkn_lpar:
   case tkn_rpar:
      return true;
   }
   return false; // Because of unknown type.
}


// If you see an insane error message originating from
// this point, then the most probable reason is that
// one of the attribute types has no print function defined.

std::ostream& operator << ( std::ostream& stream, const token& t )
{
   switch( t. type )
   {
   case tkn__recover:
      stream << "_recover( "; break;
   case tkn__defaultred:
      stream << "_defaultred( "; break;
   case tkn_Eof:
      stream << "Eof( "; break;
   case tkn_S:
      stream << "S( "; break;
   case tkn_plus:
      stream << "plus( "; break;
   case tkn_T:
      stream << "T( "; break;
   case tkn_U:
      stream << "U( "; break;
   case tkn_identifier:
      stream << "identifier( "; break;
   case tkn_lpar:
      stream << "lpar( "; break;
   case tkn_rpar:
      stream << "rpar( "; break;
   default:
      stream << "UNKNOWNTOKEN( ";
   }

   unsigned int arg = 0;
   unsigned int arginlist;

   stream << " )";
   return stream;
}


