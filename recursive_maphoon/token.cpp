
// This code was produced by Maphoon 2008.
// Code of struct token:


#include "token.h"


bool token::iswellformed( ) const
{
   switch( type )
   {
   case tkn__recover:
   case tkn__defaultred:
   case tkn_EOF:
   case tkn_SEMICOLON:
   case tkn_BECOMES:
   case tkn_COMMA:
   case tkn_PLUS:
   case tkn_TIMES:
   case tkn_MINUS:
   case tkn_DIVIDES:
   case tkn_FACTORIAL:
   case tkn_LPAR:
   case tkn_RPAR:
   case tkn_Session:
   case tkn_Command:
   case tkn_Start:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( value. size( ) >= 1 ) return false;
      return true;
   case tkn_SCANERROR:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) < 1 ) return false;
      if( reason. size( ) >= 2 ) return false;
      if( value. size( ) >= 1 ) return false;
      return true;
   case tkn_IDENTIFIER:
      if( id. size( ) < 1 ) return false;
      if( id. size( ) >= 2 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( value. size( ) >= 1 ) return false;
      return true;
   case tkn_NUMBER:
   case tkn_LISTARGS:
   case tkn_LIST:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( value. size( ) < 1 ) return false;
      if( value. size( ) >= 2 ) return false;
      return true;
   case tkn_E:
   case tkn_F:
   case tkn_G:
   case tkn_H:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( value. size( ) >= 2 ) return false;
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
   case tkn_EOF:
      stream << "EOF( "; break;
   case tkn_SCANERROR:
      stream << "SCANERROR( "; break;
   case tkn_SEMICOLON:
      stream << "SEMICOLON( "; break;
   case tkn_BECOMES:
      stream << "BECOMES( "; break;
   case tkn_COMMA:
      stream << "COMMA( "; break;
   case tkn_IDENTIFIER:
      stream << "IDENTIFIER( "; break;
   case tkn_NUMBER:
      stream << "NUMBER( "; break;
   case tkn_PLUS:
      stream << "PLUS( "; break;
   case tkn_TIMES:
      stream << "TIMES( "; break;
   case tkn_MINUS:
      stream << "MINUS( "; break;
   case tkn_DIVIDES:
      stream << "DIVIDES( "; break;
   case tkn_FACTORIAL:
      stream << "FACTORIAL( "; break;
   case tkn_LPAR:
      stream << "LPAR( "; break;
   case tkn_RPAR:
      stream << "RPAR( "; break;
   case tkn_E:
      stream << "E( "; break;
   case tkn_F:
      stream << "F( "; break;
   case tkn_G:
      stream << "G( "; break;
   case tkn_H:
      stream << "H( "; break;
   case tkn_LISTARGS:
      stream << "LISTARGS( "; break;
   case tkn_Session:
      stream << "Session( "; break;
   case tkn_Command:
      stream << "Command( "; break;
   case tkn_Start:
      stream << "Start( "; break;
   case tkn_LIST:
      stream << "LIST( "; break;
   default:
      stream << "UNKNOWNTOKEN( ";
   }

   unsigned int arg = 0;
   unsigned int arginlist;

   arginlist = 0;
   for( std::list< std::string > :: const_iterator
           p = t. id. begin( );
           p != t. id. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   arginlist = 0;
   for( std::list< std::string > :: const_iterator
           p = t. reason. begin( );
           p != t. reason. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   arginlist = 0;
   for( std::list< list > :: const_iterator
           p = t. value. begin( );
           p != t. value. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   stream << " )";
   return stream;
}


