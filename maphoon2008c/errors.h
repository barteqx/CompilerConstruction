

// this file defines syntaxerror and readerror. A readerror 
// means that reading of input was attempted and failed, for some
// reason different than encountering end of file.
//
// A syntaxerrors means that it was attempted to read a term, 
// that enough characters were successfully read, but that they could not
// be parsed according to the input grammar.
//
// Code written by Hans de Nivelle, April 2006.
// See the licence that was included with the code.


#ifndef ERRORS_INCLUDED
#define ERRORS_INCLUDED    1 


#include <string>
#include <iostream>


struct syntaxerror
{
   std::string problem;
      
   std::string streamname;
   unsigned int linenumber;


   syntaxerror( const std::string& problem,
		const std::string& streamname,
		unsigned int linenumber )
      : problem( problem ),
        streamname( streamname ),
	linenumber( linenumber )
   {
   }

};


std::ostream& operator << ( std::ostream&, const syntaxerror& ); 
 


// An action error is an error that occurs in the conversion of an
// action associated to a reduction.

struct actionerror
{

   std::string problem;

   std::string streamname;
   unsigned int linenumber;

   // The description of the problem should be such that in can
   // be followed by 'in line ... of file ...'.

   actionerror( const std::string& problem,
                const std::string& streamname,
		unsigned int linenumber )
      : problem( problem ),
        streamname( streamname ),
	linenumber( linenumber )
   {
   }

};


std::ostream& operator << ( std::ostream&, const actionerror& );


#endif


