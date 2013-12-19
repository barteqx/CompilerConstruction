
#include "assert.h"
#include "reader.h"
#include "tokenizer.h"
#include "list.h"


const list parse_listargs ( tokenizer & tt, int level ) {

   token tok = tt.lookahead.front();
   list current("nil");

   switch (tok.type) {

   case tkn_IDENTIFIER: {
      tt.lookahead.pop_front();
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list(tok.id.front()), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_NUMBER: {
      tt.lookahead.pop_front();
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list(tok.value.front()), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_LPAR: {
      tt.lookahead.pop_front(); 
      if ( tt.lookahead.empty() ) tt.scan();
      list head = parse_listargs(tt, level+1);
      if (tt.lookahead.front().type == tkn_RPAR) {
         tt.lookahead.pop_front();
         if ( tt.lookahead.empty() ) tt.scan();
         current = list( head, parse_listargs(tt, level+1));
      } else { ASSERT(false); }
      break;
      }

   case tkn_RPAR: {
      current = list("nil");
      break;
      }

   case tkn_PLUS: {
      tt.lookahead.pop_front(); 
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list("+"), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_MINUS: {
      tt.lookahead.pop_front(); 
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list("-"), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_TIMES: {
      tt.lookahead.pop_front(); 
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list("*"), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_DIVIDES: {
      tt.lookahead.pop_front();
      if ( tt.lookahead.empty() ) tt.scan();
      current = list(list("/"), list(parse_listargs(tt, level+1)));
      break;
      }

   case tkn_EOF: {
      tt.lookahead.pop_front();
      if ( tt.lookahead.empty() ) tt.scan();
      current = list("EOF");
      break;
      }

   default:
      ASSERT(false);
   }

   if (current.isEOF()) {
      std::cout << "unexpected EOF" << std::endl;
      QUIT();
   }
   return current;
}


const list parse_Start( tokenizer & tt, int level ) {

   token tok = tt.lookahead.front();
   list current("nil");

   switch (tok.type) {

   case tkn_LPAR: {
      tt.lookahead.pop_front(); 
      if ( tt.lookahead.empty() ) tt.scan();
      current = parse_listargs(tt, level+1);
      if (tt.lookahead.front().type == tkn_RPAR) {
         tt.lookahead.pop_front();
      } else { ASSERT(false); }
      break;
      }

   case tkn_EOF: {
      current = list("EOF");
      break;
      }

   default:
      ASSERT(false);
   }

   return current;
}

int main( int argc, char* argv [] )
{

   tokenizer tt;
   
   list a = list("nil");

   while (!a.isEOF()) {
      tt.scan();
      a = parse_Start( tt, 0);
      std::cout << ">>> " << a << std::endl;
   }

   std::cout << "exiting..." << std::endl;
 
   return 0;
 
}