

// Written by Hans de Nivelle.
// See the licence that was included with this code.


#include "cpp.h"


bool cpp::wellformedalone( const std::string& s )
{
   std::string::const_iterator p = s. begin( );
   if( p == s. end( ))
      return false;

   if(  ( *p < 'a' || *p > 'z' ) &&
        ( *p < 'A' || *p > 'Z' ) &&
        *p != '_' )
   {
      return false;
   }
   ++ p;
   while( p != s. end( ))
   {
      if(  ( *p < 'a' || *p > 'z' ) &&
           ( *p < 'A' || *p > 'Z' ) &&
           ( *p < '0' || *p > '9' ) &&
           *p != '_' )
      {
         return false;
      }
      ++ p;
   }

   // Finally, we compare with a list of known reserved identifiers.

   if( s == "while" ) return false;
   if( s == "for" ) return false;
   if( s == "if" ) return false;
   if( s == "else" ) return false;
   if( s == "switch" ) return false;
   if( s == "case" ) return false;
   if( s == "goto" ) return false;
   if( s == "break" ) return false; 
   if( s == "return" ) return false;
   if( s == "throw" ) return false;
   if( s == "catch" ) return false;
   if( s == "do" ) return false;

   if( s == "main" ) return false;
   
   if( s == "bool" ) return false;
   if( s == "double" ) return false;
   if( s == "float" ) return false; 
   if( s == "unsigned" ) return false;
   if( s == "int" ) return false;

   if( s == "true" ) return false;
   if( s == "false" ) return false;

   if( s == "class" ) return false;
   if( s == "struct" ) return false;
   if( s == "private" ) return false;
   if( s == "public" ) return false;
   if( s == "protected" ) return false;
   if( s == "static" ) return false;
   if( s == "const" ) return false;
   if( s == "namespace" ) return false;

   return true; 
}


bool cpp::wellformedprefixed( const std::string& s )
{
   for( std::string::const_iterator 
           p = s. begin( );
           p != s. end( );
           ++ p )
   {
      if(  ( *p < 'a' || *p > 'z' ) &&
           ( *p < 'A' || *p > 'Z' ) &&
           ( *p < '0' || *p > '9' ) &&
           *p != '_' )
      {
         return false;
      }
   }

   return true;
}


bool cpp::wellformedalone( const std::list< std::string > & n )
{
   for( std::list< std::string> :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      if( ! wellformedalone( *p ))
         return false;
   }
   return true;
}


std::string cpp::makedeclaration( const std::list< std::string > & n )
{
   std::string res;

   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      res += "namespace ";
      res += *p;
      res += " { ";
   }
   return res;
}
 
     
std::string cpp::makeclosure( const std::list< std::string > & n )
{
   std::string res;

   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      if( p != n. begin( ))
         res += " "; 
      res += "}"; 
   }
   return res;
}

 
std::string cpp::indentation( const std::list< std::string > & n )
{
   std::string res;
   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      res += "   ";
   }
   return res;
}


namespace
{

   std::string makecapitals( const std::string& s )
   {
      std::string result;

      for( std::string::const_iterator
              p = s. begin( );
              p != s. end( );
              ++ p )
      {
         char c = *p;
         if( c >= 'a' && c <= 'z' )
         {
            c -= 'a';
            c += 'A';
         }
         result. push_back(c);
      }
      return result;
   }

}


std::string cpp::includeguard( const std::list< std::string > & n,
                               const std::string& s )
{
   std::string result; 

   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      if( p != n. begin( ))
         result. push_back( '_' );
  
      result += makecapitals( *p ); 
   }

   if( n. size( ))
      result. push_back( '_' );

   result += makecapitals(s);
   result += "_INCLUDED";
   return result;
}


std::string cpp::makename( const std::list< std::string > & n,  
                           const std::string& s )
{
   std::string result;
  
   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      if( p != n. begin( ))
         result += "::";

      result += *p;
   }

   if( n. size( ))
      result += "::"; 

   result += s; 
   return result;
}


std::string cpp::indentation( const std::list< std::string > & n,
                              const std::string& s,
                              unsigned int x )
{
   std::string res;
   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      res += "   ";
   }
   res += "   ";
   for( unsigned int i = 0; i < x; ++ i )
      res += "   "; 

   return res;
}


std::string cpp::attribute_name( const std::string& attr )
{
   return attr;
}


std::string cpp::symbol_name( symbol s )
{
   return "tkn_" + s. getstring( );
}


std::string cpp::symbol_name( const std::list< std::string > & n, 
                              symbol s )
{
   std::string result;

   for( std::list< std::string > :: const_iterator
           p = n. begin( );
           p != n. end( );
           ++ p )
   {
      result += *p;
      result += "::";
   }

   result += symbol_name( s ); 
   return result;  
}


