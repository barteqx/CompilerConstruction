
#include "assert.h"
#include "reader.h"
#include "tokenizer.h"
#include "parsestack.h"


tokentype OPERATORS[6] = {tkn_LT, tkn_GT, tkn_PLUS, tkn_MINUS, tkn_TIMES, tkn_DIVIDES};

bool is_operator(tokentype);

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
      case tkn_LISTARGS:
         switch( top. state )
         {
         case 0:
            if (tok.type == tkn_RPAR) {
               p.read(1);
               tt.lookahead.pop_front();
            } else {
               p.read(2);
            }
            break;

         case 1:
            p.close();
            break;

         case 2:
            if (tok.type == tkn_RPAR) {
               p.read(1);
               tt.lookahead.pop_front();
            } else if (tok.type == tkn_LPAR) {
               p.descend(2, tkn_LISTARGS, 0);
               tt.lookahead.pop_front();
            } else if (tok.type == tkn_IDENTIFIER || tok.type == tkn_NUMBER || is_operator(tok.type)) {
               tt.lookahead.pop_front();
            }
            break;

         default:
            ASSERT ( false );
         }
         break;

      case tkn_Start:
         switch (top.state) {
            case 0:
               if (tok.type == tkn_EOF) {
                  p.close();
               } else if (tok.type == tkn_LPAR) {
                  p.descend(0, tkn_LISTARGS, 0);
                  tt.lookahead.pop_front();
               }
            break;             

            default:
               ASSERT(false);
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

bool is_operator(tokentype tok) {
   for (int i = 0; i < 6; i++) {
      if (tok == OPERATORS[i]) return true;
   }
   return false;
}


