
#include "list.h" 
#include "varstore.h"

int main( int argc, char* argv [] )
{
   std::cout << "welcome to lisp\n";

   list l1 = 4444;
   list l2 = std::string( "a" );
   list l3 = std::string( "b" );
   list l4 = list( list(1), list( "nil" ));

   list res = list( l1, list( l2, list( l2, 
                list( l3, list( l4, std::string("nil") )))));
   std::cout << res << "\n";

}

