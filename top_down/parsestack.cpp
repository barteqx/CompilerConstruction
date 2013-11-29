
#include "parsestack.h"


std::ostream& operator << ( std::ostream& stream, const position& p )
{
   stream << "( " << token( p. type ) << ", " << p. state << " )";
   return stream; 
}


void parsestack::read( unsigned int nextstate )
{
   ASSERT( stack. size( ));
   stack. back( ). state = nextstate;
}


void parsestack::close( )
{
   ASSERT( stack. size( ));
   stack. pop_back( );
}

      
void parsestack::descend( unsigned int nextstate, 
                          tokentype newtoken, unsigned int startingstate )
{
   ASSERT( stack. size( ))
   stack. back( ). state = nextstate;
   stack. push_back( position( newtoken, startingstate ));
}


bool parsestack::accepts( const tokenizer& t ) const
{
   return stack. size( ) == 0 && t. lookahead. front( ). type == tkn_EOF;
}


std::ostream& operator << ( std::ostream& stream, const parsestack& s ) 
{
   stream << "parsestack("; 

   for( auto p = s. stack. begin( ); p != s. stack. end( ); ++ p )
   {
      if( p != s. stack. begin( ))
         stream << ", ";
      else
         stream << " "; 
      stream << *p;
   }
   stream << " )";
   return stream;
}


