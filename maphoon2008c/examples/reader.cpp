

#include <math.h>
#include <string>
#include <map>

#include "reader.h"
#include "parser.h"



reader::reader( )
   : next( ' ' ),
     linenumber(1),
     lookahead( std::list< token > :: list( )) 
{
}


void reader::moveforward( )
{
   if( std::cin && next != EOF )
   {
      if( next == '\n' )
         ++ linenumber;

      next = std::cin. get( );
   }
}


    
void reader::scan( )
{
   while( next == ' ' || next == '\n' || next == '\t' )
      moveforward( );

   if( next == EOF )
   {
      lookahead. push_back( tkn_EOF );
            // Implicit conversion to token
      return;
   }

   if( next == '#' )
   {
      moveforward( );
      lookahead. push_back( tkn_EOF );
      return;
   }

   if( next == '.' )
   {
      moveforward( );
      lookahead. push_back( tkn_DOT );
      return; 
   }

   if( next == ':' )
   {
      moveforward( );
      if( next == '=' )
      {
         moveforward( );
         lookahead. push_back( tkn_BECOMES );
         return; 
      }

      lookahead. push_back( tkn_COLON );
      return; 
   }

   if( next == '+' )
   {
      moveforward( ); 
      lookahead. push_back( tkn_PLUS );
      return; 
   }

   if( next == '*' )  
   {
      moveforward( );
      lookahead. push_back( tkn_TIMES );
      return; 
   }

   if( next == '-' )
   {
      moveforward( );
      lookahead. push_back( tkn_MINUS );
      return; 
   }

   if( next == '/' )
   {
      moveforward( );
      lookahead. push_back( tkn_DIVIDES );
      return; 
   }

   if( next == '(' )
   {
      moveforward( );
      lookahead. push_back( tkn_LPAR );
      return; 
   }

   if( next == ')' )
   {
      moveforward( );
      lookahead. push_back( tkn_RPAR );
      return; 
   }

   
   if( next >= '0' && next <= '9' )
   {
      // Going to read a floating point number:

      double mant = 0.0;
      int expo = 0;
      int signofexpo = 1;

      while( next >= '0' && next <= '9' )
      {
         mant *= 10.0; 
         mant += ( next - '0' );
         moveforward( );
      }

      if( next == '.' )
      {
         moveforward( );
         double pos = 10.0;

         if( ! ( next >= '0' && next <= '9' )) 
         {
            token t = tkn_SCANERROR;
            t. reason. push_back( "expecting digit after point" );
            return;
         }

         while( next >= '0' && next <= '9' )
         {
            mant += ( next - '0' ) / pos;
            moveforward( );
            pos *= 10.0;
         }
      }

      if( next == 'e' || next == 'E' )
      {
         moveforward( );
         if( next == '-' )
         {
            signofexpo = -1;
            moveforward( );
         }
         else
         {
            if( next == '+' )
               moveforward( );
         }

         if( ! ( next >= '0' && next <= '9' ))
         {
            token t = tkn_SCANERROR;
            t. reason. push_back( "expecting digit after E" );
            lookahead. push_back(t);
            return;  
         }

         while( next >= '0' && next <= '9' )
         {
            expo *= 10;
            expo += ( next - '0' );
            moveforward( );
            if( expo >= 10000 )
            {
               token t = tkn_SCANERROR;
               t. reason. push_back( "exponent too big" );
               lookahead. push_back(t); 
               return; 
            }
         }
      }
   
      token t = tkn_DOUBLE;
      t. val. push_back( mant * pow( 10.0, signofexpo * expo ));
      lookahead. push_back(t); 
      return; 
   }
 
   if( ( next >= 'a' && next <= 'z' ) || 
       ( next >= 'A' && next <= 'Z' ) ||
       ( next == '_' ))
   {
      token t = tkn_IDENTIFIER;
      t. id. push_back( std::string( ));

      while( ( next >= 'a' && next <= 'z' ) ||
             ( next >= 'A' && next <= 'Z' ) ||
             ( next >= '0' && next <= '9' ) ||
             ( next == '_' ))
      {
         t. id. front( ). push_back( next ); 
         moveforward( );
      }

      lookahead. push_back(t);
      return;  
   }

   token t = tkn_SCANERROR;
   t. reason. push_back( "garbage encountered: " );
   t. reason. front( ). push_back( next );
   lookahead. push_back(t); 

   moveforward( );
}
   

void reader::syntaxerror( ) const
{
   std::cout << "syntax error "; 
   std::cout << "in line " << linenumber << " of standard input\n\n";
}


int main( unsigned int nrargs, char* ( args [] ) )
{
   std::cout << "good morning. This is the first program generated";
   std::cout << " by maphoon 2008\n\n";

   reader r;
 
   std::map< std::string, double > memory;
 
   parser( r, memory, tkn_Session, 5 );
   
   std::cout << "{ "; 
   for( std::list< token > :: const_iterator
           p = r. lookahead. begin( );
           p != r. lookahead. end( );
           ++ p )
   {
      if( p != r. lookahead. begin( ))
         std::cout << ", ";
      std::cout << *p;
   }
   std::cout << " }\n"; 
}


