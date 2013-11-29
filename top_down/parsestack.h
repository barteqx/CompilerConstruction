

#ifndef PARSESTACK_INCLUDED
#define PARSESTACK_INCLUDED     1


#include <vector>
#include <iostream>

#include "token.h"
#include "tokenizer.h"
#include "assert.h"


// The not-so-meaningful word 'position' means 
// state-in-an-automaton: 

struct position
{
   tokentype type;
   unsigned int state;

   position( tokentype type, unsigned int state )
      : type( type ),
        state( state )
   { }
};

std::ostream& operator << ( std::ostream& , const position& p );

struct parsestack
{
   std::vector< position > stack;
       // I would have used std::stack, but it seems impossible
       // to print stacks. 

   parsestack( tokentype startsymbol, unsigned int startstate )
   {
      stack. push_back( position( startsymbol, startstate ));
   }

   void read( unsigned int nextstate );
      // Move to a next state in the same automaton.

   void close( ); 
      // Close top automaton. This is called 'return' in the slides, 
      // but because it is a reserved word, I have to call it 'close'.

   
   void descend( unsigned int nextstate, 
                 tokentype newtoken, unsigned int startingstate );
      // Move current automaton to next state, and start a new automaton
      // for newtoken in startingstate.

   bool accepts( const tokenizer& t ) const;
      // We can accept if the stack is empty, and there is only
      // token left in the tokenizer, which is end-of-file. 

   unsigned int size( ) const { return stack. size( ); }

   const position& top( ) const { ASSERT( size( )); return stack. back( ); }
   position& top( ) { ASSERT( size( )); return stack. back( ); }
   
};

std::ostream& operator << ( std::ostream& , const parsestack& );

#endif

