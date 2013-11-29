

// Code written by Hans de Nivelle, April 2006.
// See the licence that was included with the code.


#include "errors.h"


std::ostream& operator << ( std::ostream& stream, const syntaxerror& err )
{
   stream << "Syntax error: " << err. problem;
   stream << " in line " << err. linenumber;
   stream << " of inputstream " << err. streamname;  

   return stream;
}
            

std::ostream& operator << ( std::ostream& stream, const actionerror& err )
{
   stream << "Action error: " << err. problem;
   stream << " in line " << err. linenumber;
   stream << " of inputstream " << err. streamname;

   return stream;
}


