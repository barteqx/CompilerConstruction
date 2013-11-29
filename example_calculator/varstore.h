

#ifndef VARSTORE_INCLUDED
#define VARSTORE_INCLUDED   1


#include <map>
#include <string>
#include <iostream>


class varstore
{

   std::map< std::string, double > values;

public:
   varstore( ); 
   bool contains( const std::string& s ) const;
   void assign( const std::string& s, double val );

   double lookup( const std::string& s ) const;
      // You must check that the variable is present, before
      // you do a lookup. 

   void clear( );

   friend std::ostream& operator << ( std::ostream& ,
                                      const varstore& ); 
};


std::ostream& operator << ( std::ostream&, const varstore& vs ); 


#endif

