

#include "assert.h"
#include "reader.h"
#include "tokenizer.h"
#include "parsestack.h"


int main( int argc, char* argv [] )
{

   tokenizer tt;

   parsestack p( tkn_Start, 0 ); 
      // Start in state 0 for token tkn_Start.

   while( ! p. accepts( tt ))
   {
      std::cout << p << "\n";
      if( tt. lookahead. size( ) == 0 )
         tt. scan( );

      if( p. size( ) == 0 )
      {
         std::cout << "unexpected end of file\n"; 
         return 0; 
      }

      const position& top = p. top( );
      std::cout << "lookahead symbol = " << tt. lookahead. front( ) << "\n\n";
      token tok = tt. lookahead. front( );
      // You can use nested switches, nested ifs, or use tables,
      // whatever you think gives the nices solution.

      switch( top. type )
      {
      case tkn_Start:
         switch( top. state )
         {
         case 0:
            if (tok.type == tkn_LPAR) {
               p.read(1);
               tt.lookahead.pop_front();
            } else if (tok.type == tkn_EOF) p.close();
            else {
               ASSERT(false);
            }
            break;
         case 1:
            p.descend(2, tkn_Start, 0);
            break;
         case 2:
            if (tok.type == tkn_RPAR) {
               p.read(0);
               tt.lookahead.pop_front();
            } else { ASSERT(false); }
            break;
         default:
            ASSERT ( false );

         }

         break;
      default:
         ASSERT ( false );
         
            // In the incomplete version, we just throw away the
            // lookahead, so that the program will reach end of file
            // at least. `
      }
   }

   // Actually if the size of lookahead gets 0, something went wrong.

   ASSERT( tt. lookahead. size( ));
   std::cout << tt. lookahead. front( ) << "\n";
      // This is an EOF token. 
   return 0;
 
}


