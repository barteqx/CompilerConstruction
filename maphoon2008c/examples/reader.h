

#ifndef READER_INCLUDED
#define READER_INCLUDED   1


#include <iostream>
#include <string>

#include "../assert.h"
#include "token.h"


class reader
{
private:
   char next;
   unsigned int linenumber;

   void moveforward( );


public: 
   // The reader must have a public field lookahead:

   std::list< token > lookahead; 

   reader( );

   // The reader must have a method that reads a token and appends it
   // to lookahead. 
   // In case EOF is encountered, or some other problem with the input 
   // occurs, the reader should return some designated token for that,
   // for example EOF, READERROR or SCANERROR. 
 
   void scan( );


   // It must have a method of the following type: 

   void syntaxerror( ) const;
      // It is reasonable to put this function in the reader because
      // it needs to print something line 'line line L of file F'.

};


#endif


