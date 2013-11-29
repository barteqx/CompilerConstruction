

// Code written by Hans de Nivelle, November 2006.
// Rewritten June 2008.
// See the licence that was included with this code.


#include "attributesystem.h"
#include "cpp.h"


bool attributesystem::isattribute( const std::string& attrname ) const
{
   return attributetypes. find( attrname ) != attributetypes. end( );
}


void attributesystem::addattribute( const std::string& attrname,
                                    const std::string& cpptype )
{
   attributetypes [ attrname ] = cpptype;
}


bool attributesystem::hasconstraint( symbol symb, 
                                     const std::string& attr ) const
{
   std::map< symbol, std::map< std::string, std::list< unsigned int > > > 
      :: const_iterator p = constraints. find( symb );
   if( p != constraints. end( ))
   {
      std::map< std::string, std::list< unsigned int > > :: const_iterator
         q = p -> second. find( attr );
      if( q != p -> second. end( ))
         return true;
   }

   return false;
}


void attributesystem::addconstraint( symbol symb, const std::string& attr,
                                     unsigned int lowerbound )
{
   ASSERT( attributetypes. find( attr ) != attributetypes. end( ));

   std::list< unsigned int > & e = constraints [ symb ] [ attr ]; 
   e. clear( ); 
   e. push_back( lowerbound );
}


void attributesystem::addconstraint( symbol symb, const std::string& attr,
                                     unsigned int lowerbound,
                                     unsigned int upperbound )
{
   ASSERT( attributetypes. find( attr ) != attributetypes. end( ));

   // We try to keep constraints normalized.

   // We normalize unsatisfiable constraints:

   if( lowerbound >= upperbound )
   {
      lowerbound = 0;
      upperbound = 0;
   }

   // In case the only solution is 0, we erase the entry:

   if( lowerbound == 0 && upperbound == 1 )
   {
      std::map< symbol, std::map< std::string, std::list< unsigned int > > > 
         :: iterator p = constraints. find( symb );
      if( p != constraints. end( ))
      {
         std::map< std::string, std::list< unsigned int > > :: iterator
            q = p -> second. find( attr );
         if( q != p -> second. end( ))
            p -> second. erase(q);
      
         if( p -> second. size( ) == 0 )
            constraints. erase(p);
      }
   }
   else
   {
      std::list< unsigned int > & e = constraints [ symb ] [ attr ]; 
      e. clear( ); 
      e. push_back( lowerbound );
      e. push_back( upperbound ); 
   } 
}
 

void attributesystem::printtokendecl( const symbolset& alphabet, 
                                      std::ostream& stream,
                                      const std::list< std::string > & n,
                                      const std::string& s,
                                      const std::list< std::string > & i ) 
{
   stream << "\n"; 
   stream << "// This code was produced by Maphoon 2008.\n";
   stream << "// Definition of struct token:\n\n\n"; 

   for( std::list< std::string > :: const_iterator
           p = i. begin( );
           p != i. end( );
           ++ p )
   {
      stream << *p << "\n";
   }

   if( i. size( ))
      stream << "\n\n\n";

   std::string guard = cpp::includeguard( n, s );
   stream << "#ifndef " << guard << "\n";
   stream << "#define " << guard << "    1\n";

   stream << "\n\n";

   stream << "#include <list>\n";
   stream << "#include <iostream>\n"; 
   stream << "\n\n"; 

   stream << cpp::makedeclaration(n) << "\n";
   
   std::string indent = cpp::indentation(n);

   stream << indent << "enum tokentype\n";
   stream << indent << "{\n";

   indent += "   ";
   for( symbolset::const_iterator 
           p = alphabet. begin( );
	   p != alphabet. end( );
	   ++ p )
   {
      if( p != alphabet. begin( ))
         stream << ",\n";
      stream << indent << cpp::symbol_name(*p); 
   }

   indent = cpp::indentation(n); 

   stream << "\n"; 
   stream << indent << "};\n\n\n";

   stream << indent << "struct " << s << "\n";
   stream << indent << "{\n\n";

   indent = cpp::indentation(n,s,0);

   stream << indent << "tokentype type;\n\n"; 

   // We define the attributes:

   for( std::map< std::string, std::string > :: const_iterator
           p = attributetypes. begin( );
           p != attributetypes. end( );
           ++ p )
   {
      stream << indent << "std::list< " << ( p -> second ) << " > ";
      stream << cpp::attribute_name( p -> first ) << ";\n";
   }
   stream << "\n";

   stream << indent << "token( tokentype t )\n";
   stream << indent << "   : type(t)\n";
   stream << indent << "{\n";
   stream << indent << "}\n";

   stream << "\n";
   stream << indent << "token( );\n";
   stream << indent << "   // Should have no definition.\n";
   stream << "\n";

   stream << indent << "bool iswellformed( ) const;\n";
   stream << indent << "   // Check if the attributes satisfy the\n";  
   stream << indent << "   // constraints.\n";
  
   indent = cpp::indentation(n); 
   stream << indent << "};\n";
   stream << "\n";
   stream << indent; 
   stream << "std::ostream& operator << ( std::ostream& , const token& );";
   stream << "\n\n";

   stream << cpp::makeclosure(n);
   stream << "\n"; 

   stream << "\n";
   stream << "#endif\n";
   stream << "\n\n";

}


namespace
{
   bool equalconstraints( const std::list< unsigned int > & u1,
                          const std::list< unsigned int > & u2 )
   {
      if( u1. size( ) != u2. size( ))
         return false;

      std::list< unsigned int > :: const_iterator p1 = u1. begin( );
      std::list< unsigned int > :: const_iterator p2 = u2. begin( );

      while( p1 != u1. end( ))
      {
         if( *p1 != *p2 ) return false;
         ++ p1;
         ++ p2;
      }

      return true;
   }

 
   bool equalconstraints( const std::map< std::string, 
                                          std::list< unsigned int > > & c1,
                          const std::map< std::string,
                                          std::list< unsigned int > > & c2 )
   {
      if( c1. size( ) != c2. size( ))
         return false;

      std::map< std::string, std::list< unsigned int > > :: const_iterator 
         p1 = c1. begin( );
      std::map< std::string, std::list< unsigned int > > :: const_iterator
         p2 = c2. begin( );

      while( p1 != c1. end( ))
      {
         if( p1 -> first != p2 -> first ) return false; 
         if( ! equalconstraints( p1 -> second, p2 -> second )) return false;
         ++ p1; 
         ++ p2;
      }

      return true;
   }
}
 

void attributesystem::printtokencode( const symbolset& alphabet,
                                      std::ostream& stream,
                                      const std::list< std::string > & n,
                                      const std::string& s )
{
   stream << "\n"; 
   stream << "// This code was produced by Maphoon 2008.\n";
   stream << "// Code of struct token:\n\n";
 
   stream << "\n";
   stream << "#include \"token.h\"\n\n\n";

   stream << "bool " << cpp::makename(n,s) << "::iswellformed( ) const\n";
   stream << "{\n";
   std::string indent = "   ";

   stream << indent << "switch( type )\n";
   stream << indent << "{\n";

   symbolset remainingalphabet = alphabet;
   while( remainingalphabet. size( ))
   {
      symbol s = *( remainingalphabet. begin( ));
      std::map< symbol,
                std::map< std::string, 
                          std::list< unsigned int > > > :: const_iterator
         it_s = constraints. find(s);

      // We selected one symbol s from remainingalphabet.
      // We now look for all elements in remainingalphabet that have the 
      // same constraints on the attribute, and put them in 
      // equivalenceclass. They can be treated in one case of the switch 
      // statement.
  
      symbolset equivalenceclass;

      for( symbolset::const_iterator
              p = remainingalphabet. begin( );
              p != remainingalphabet. end( );
              ++ p )
      {
         std::map< symbol, 
                   std::map< std::string, 
                             std::list< unsigned int > > > :: const_iterator
            f = constraints. find(*p);

         if( it_s == constraints. end( ) && f == constraints. end( ))
         {
            equivalenceclass. insert(*p);
         }

         if( it_s != constraints. end( ) && f != constraints. end( ) && 
             equalconstraints( it_s -> second, f -> second ))
         {
            equivalenceclass. insert(*p);
         }
      }

      for( symbolset::const_iterator
              p = equivalenceclass. begin( );
              p != equivalenceclass. end( );
              ++ p )
      {
         stream << indent << "case " << cpp::symbol_name(*p) << ":\n"; 
      }

      for( std::map< std::string, std::string > :: const_iterator
              p = attributetypes. begin( );
              p != attributetypes. end( );
              ++ p )
      {
         std::list< unsigned int > constr;
         if( it_s != constraints. end( ))
         {
            std::map< std::string, std::list< unsigned int > > :: 
            const_iterator
               q = it_s -> second. find( p -> first );
            if( q != it_s -> second. end( ))
               constr = q -> second;
            else
            {
               constr. push_back(0);
               constr. push_back(1);
            }
         }
         else
         {
            constr. push_back(0);
            constr. push_back(1);
         }

         // We don't print the tautological constraint 
         //    if( attr < 0 ) return false;

         if( constr. front( ) != 0 )
         {
            stream << indent << "   if( " << cpp::attribute_name( p -> first );
            stream << ". size( )"; 
            stream << " < " << constr. front( ) << " ) return false;\n";
         }

         if( constr. size( ) == 2 )
         {
            stream << indent << "   if( " << cpp::attribute_name( p -> first );
            stream << ". size( )"; 
            stream << " >= " << constr. back( ) << " ) return false;\n";
         }
      }

      stream << indent << "   return true;\n";

      remainingalphabet. remove( equivalenceclass );
   }          

   stream << indent << "}\n";
   stream << indent << "return false; // Because of unknown type.\n";
   stream << "}\n\n\n";

   // operator <<

   stream << "// If you see an insane error message originating from\n";
   stream << "// this point, then the most probable reason is that\n";
   stream << "// one of the attribute types has no print function defined.\n";
   stream << "\n";

   stream << "std::ostream& ";
   stream << cpp::makename( n, "operator << " );
   stream << "( std::ostream& stream, ";
   stream << "const " << cpp::makename(n,s) << "& t )\n";
   stream << "{\n";

   indent = "   ";

   stream << indent << "switch( t. type )\n";
   stream << indent << "{\n";
   
   for( symbolset::const_iterator
           p = alphabet. begin( );
           p != alphabet. end( );
           ++ p )
   {
      stream << indent << "case ";
      stream << cpp::symbol_name(*p) << ":\n";
      stream << indent << "   stream << \"";
      stream << *p;
      stream << "( \"; break;\n"; 
   }
   stream << indent << "default:\n";
   stream << indent << "   stream << \"UNKNOWNTOKEN( \";\n";
   stream << indent << "}\n\n";
 

   stream << indent << "unsigned int arg = 0;\n";
   stream << indent << "unsigned int arginlist;\n\n";

   for( std::map< std::string, std::string > :: const_iterator
           p = attributetypes. begin( );
           p != attributetypes. end( );
           ++ p )
   {
      stream << indent << "arginlist = 0;\n";
      stream << indent << "for( std::list< ";
      stream << p -> second << " > :: const_iterator\n";
      stream << indent << "        p = t. " << p -> first << ". begin( );\n";
      stream << indent << "        p != t. " << p -> first << ". end( );\n";
      stream << indent << "        ++ p )\n";
      stream << indent << "{\n";
      stream << indent << "   if( arg != 0 && arginlist == 0 ) ";
      stream << "stream << \"; \";\n";
      stream << indent << "   if( arg != 0 && arginlist != 0 ) "; 
      stream << "stream << \", \";\n";

      stream << indent << "   stream << *p;\n";
      stream << indent << "   ++ arg;\n";
      stream << indent << "   ++ arginlist;\n";
      stream << indent << "}\n\n";
   }

   stream << indent << "stream << \" )\";\n";
   stream << indent << "return stream;\n";
   stream << "}\n\n";
   stream << "\n";  
}


std::ostream& operator << ( std::ostream& stream, const attributesystem& sys )
{
   stream << "attributesystem:\n\n";
   stream << "types of attributes:\n"; 
   for( std::map< std::string, std::string > :: const_iterator 
           p = sys. attributetypes. begin( );
           p != sys. attributetypes. end( );
           ++ p ) 
   {
      stream << "   attribute " << ( p -> first ) << " :  ";
      stream << ( p -> second ) << "\n";
   }
   stream << "\n\n";
   stream << "constraints on attributes:\n"; 
   for( std::map< symbol, std::map< std::string, std::list< unsigned int > > > 
           :: const_iterator p = sys. constraints. begin( );
           p != sys. constraints. end( );
           ++ p )
   {
      for( std::map< std::string, std::list< unsigned int > > 
              :: const_iterator q = p -> second. begin( );
              q != p -> second. end( );
              ++ q )
      {
         ASSERT( q -> second. size( ) && q -> second. size( ) <= 2 ); 
         if( q -> second. size( ) >= 1 )
         {
            stream << "   " << q -> second. front( ) << " <= ";
            stream << p -> first << ". " << q -> first << ". size( )";
            if( q -> second. size( ) == 2 )
            {
               stream << " < " << q -> second. back( ); 
            }
            stream << "\n";
         }
      }
   }
   stream << "\n\n";
   return stream;
}


