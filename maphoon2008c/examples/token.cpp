
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
   case tkn_Fp:
   case tkn_Sign:
   case tkn_Int:
   case tkn_Frac:
   case tkn_Exp:
   case tkn_plus:
   case tkn_minus:
   case tkn_digit:
   case tkn_dot:
   case tkn_e:
   case tkn_E:
      return true;
   }
   return false; // Because of unknown type.
}


// If you see an insane error message originating from
// this point, then the most probably reason is that
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
   case tkn_Fp:
      stream << "Fp( "; break;
   case tkn_Sign:
      stream << "Sign( "; break;
   case tkn_Int:
      stream << "Int( "; break;
   case tkn_Frac:
      stream << "Frac( "; break;
   case tkn_Exp:
      stream << "Exp( "; break;
   case tkn_plus:
      stream << "plus( "; break;
   case tkn_minus:
      stream << "minus( "; break;
   case tkn_digit:
      stream << "digit( "; break;
   case tkn_dot:
      stream << "dot( "; break;
   case tkn_e:
      stream << "e( "; break;
   case tkn_E:
      stream << "E( "; break;
   default:
      stream << "UNKNOWNTOKEN( ";
   }

   unsigned int arg = 0;
   unsigned int arginlist;

   stream << " )";
   return stream;
}


