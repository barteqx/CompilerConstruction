

// Written by Hans de Nivelle, June 2008.
// See the licence that was included with this code.


#include "action.h"
#include "cpp.h"


std::ostream& operator << ( std::ostream& stream, 
                            const globalvariables& glob )
{
   stream << "global variables\n";
   for( std::list< globalvariables::globalvar > :: const_iterator
           p = glob. list. begin( );
           p != glob. list. end( );
           ++ p )
   {
      stream << "   " << p -> name << " : " << p -> type << "\n";
   }
   return stream;
}


action::action( const std::string& filename, 
		unsigned int linenumber ) 
   : filename( filename ), linenumber( linenumber )
{
}


// We check for newlines inside s. If they occur, we split s.

void action::addline( const std::string& s )
{
   code. push_back( std::string( ));

   for( std::string::const_iterator 
           p = s. begin( );
	   p != s. end( );
	   ++ p )
   {
      if( *p != '\n' )
         code. back( ). push_back(*p);
      else
         code. push_back( std::string( ));
   }
}


std::list< unsigned int > 
action::identifierpositions( const std::string& s, unsigned int line ) const 
throw( actionerror ) 
{
   std::list< unsigned int > res;

   // We do some analysis, so that we will not be fooled by identifiers
   // inside quotes.

   unsigned int pos = 0;
   while( pos != s. size( ))
   {

      if(  ( s [ pos ] >= 'a' && s [ pos ] <= 'z' ) ||
           ( s [ pos ] >= 'A' && s [ pos ] <= 'Z' ) ||
           ( s [ pos ] == '_' ))
      {
         res. push_back( pos );

         while( ( s [ pos ] >= 'a' && s [ pos ] <= 'z' ) ||
                ( s [ pos ] >= 'A' && s [ pos ] <= 'Z' ) ||
                ( s [ pos ] >= '0' && s [ pos ] <= '9' ) ||
                ( s [ pos ] == '_' ))
         {
            ++ pos;
         }
         goto token_read;
      }

      if( s [ pos ] == '\"' )
      {
         ++ pos;
	 while( pos != s. size( ) && s [ pos ] != '\"' )
         {
            if( s [ pos ] == '\\' )
            {
               if( pos + 1 == s. size( ))
                  throw actionerror( "non-terminated C-string",
				     filename, 
				     linenumber + line ); 
               pos += 2;
            }
            else 
               pos += 1;
         }

	 if( pos == s. size( )) 
            throw actionerror( "non-terminated C-string",
			       filename, linenumber + line );

         ++ pos;
         goto token_read; 
      }

      if( s [ pos ] == '\'' )
      {
         ++ pos;
	 while( pos != s. size( ) && s [ pos ] != '\'' )
         {
            if( s [ pos ] == '\\' )
	    {
	       if( pos + 1 == s. size( ))
                  throw actionerror( "non-terminated character constant",
		                     filename, linenumber + line );
               pos += 2;
	    }
	    else
               pos += 1;
	 }

	 if( pos == s. size( ))
            throw actionerror( "non-terminated character constant",
	                       filename, linenumber + line );

         ++ pos; 
         goto token_read;
      }

      if( s [ pos ] == '/' )
      {
         if( pos + 1 != s. size( ) && s [ pos + 1 ] == '/' )
         {
            // We encountered a comment starting with //.
            // We can ignore everything until the end of the string.

            pos = s. size( );
            goto token_read;
         }
      }

      if( s [ pos ] == '/' )
      {
         if( pos + 1 != s. size( ) && s [ pos + 1 ] == '*' )
         { 
            // A comment starting with /* . If it ends within this line,
            // we accept it. 

            pos += 2;
            while( pos != s. size( ))
            {
               if( s [ pos ] == '*' && pos + 1 != s. size( ) && 
                   s [ pos + 1 ] == '/' )
               {
                  // We peacefully reached the end.

                  pos += 2;
                  goto token_read;
               }
               else
                  ++ pos;
            }

            // This means that the comment was not terminated within
            // this line.

            throw actionerror( "multiple line comment starting with /*", 
                               filename, linenumber + line );
         }
      }

      // We catch a few more simple tokens, just for the fun of it:

      if( s [ pos ] == '{' )
      {
         ++ pos;
         goto token_read;
      } 

      if( s [ pos ] == '}' )
      {
         ++ pos;
         goto token_read;
      } 

      
      // Default: The token is a single character. We move to the next. 

      ++ pos; 

   token_read:
      ;
   } 

   return res; 
}


namespace
{

   // True if s [p] contains identifier as a complete identifier.
   // contains( "aaa", 0, "a" ) should return false,
   // contains( "a+a", 0, "a" ) should return true.


   bool contains( const std::string& s, unsigned int p,
                  const std::string& fragment )
   {
      unsigned int i = 0;
      while( p + i < s. size( ) && fragment [i] == s [ p + i ] )
         ++ i;

      if( i == fragment. size( ))
      {
         // We check that we reached the end of the identifier in s as well.

         if( p + i == s. size( ))
            return true; 
         
         char c = s [ p + i ];
         if( ( c >= 'a' && c <= 'z' ) ||
             ( c >= 'A' && c <= 'Z' ) ||
             ( c >= '0' && c <= '9' ) ||
               c == '_' )
            return false;       // Because identifier in s did not end.

         return true; 
      }
      else
         return false;
   }
}


bool action::uses( const std::string& identifier ) const
{
   unsigned int line = 0; 
   for( std::list< std::string > :: const_iterator
           p = code. begin( );
           p != code. end( );
           ++ p )
   {
      try 
      {
         std::list< unsigned int > res = identifierpositions( *p, line );
         for( std::list< unsigned int > :: const_iterator
                 q = res. begin( );
                 q != res. end( );
                 ++ q )
         {
            if( contains( *p, *q, identifier ))
               return true;
         }
      }
      catch( actionerror err )
      {
         // We catch them and ignore them. 
      }
      ++ line; 
   }
   return false;
}
 

bool action::isempty( ) const
{
   for( std::list< std::string > :: const_iterator
           p = code. begin( );
	   p != code. end( );
	   ++ p )
   {
      for( std::string::const_iterator 
              q = p -> begin( );
	      q != p -> end( );
	      ++ q )
      {
         if( *q != ' ' && *q != '\t' )
            return false;
      }
   }
   return true;
}



void action::printcall( std::ostream& stream, 
                        const globalvariables& glob,
                        const rule& r ) const
{
   stream << "reduction_" << r. nr << "( ";
   stream << "parsestack";
   for( std::list< globalvariables::globalvar > :: const_iterator
           p = glob. list. begin( );
           p != glob. list. end( );
           ++ p )
   {
      if( uses( p -> name ))
      {
         stream << ", " << p -> name;
      }
   }
   stream << ", parsestack. end( ) - " << r. rhs. size( );

   for( unsigned int i = 0; i < r. rhs. size( ); ++ i )
   {
      stream << ", parsestack. end( ) - " << r. rhs. size( ) - i;
   }

   if( uses( "lookahead" ))
      stream << ", input. lookahead";
   stream << " );\n";
}


namespace
{
   // we read the thing that can come after return. It must be
   // an identifier followed by a ;

   std::string readreturnvalue( const std::string& s, unsigned int& i,
                                const std::string& filename,
                                unsigned int linenumber ) 
   throw( actionerror )
   {
    
      while( i != s. size( ) && ( s[i] == ' ' || s[i] == '\t' ))
         ++ i;

      if( i == s. size( ) ||
          ! (  ( s[i] >= 'a' && s[i] <= 'z' ) ||
               ( s[i] >= 'A' && s[i] <= 'Z' ) ||
               ( s[i] == '_' )))
      {
         throw actionerror( "incorrect return value", filename, linenumber );
      }
      
      std::string result;
      while( i != s. size( ) &&
             ( ( s[i] >= 'a' && s[i] <= 'z' ) ||
               ( s[i] >= 'A' && s[i] <= 'Z' ) ||
               ( s[i] >= '0' && s[i] <= '9' ) ||
                 s[i] == '_' ))
      {
         result. push_back( s[i] );
         ++ i;
      }

      while( i != s. size( ) && ( s[i] == ' ' || s[i] == '\t' ))
         ++ i;

      if( i == s. size( ) || s[i] != ';' )
         throw actionerror( "expected ;", filename, linenumber );
      ++ i; 

      return result; 
   }
 
}

 
void action::printdefinition( std::ostream& stream,
                              const globalvariables& glob, 
                              const std::list< std::string > & parsernamespace,
                              const std::list< std::string > & tokennamespace,
		              const rule& r ) const 
throw( actionerror ) 
{

   // We first construct the string "std::list< tokennamespace :: token > ",
   // because we need it a couple of times: 

   std::string fulllistoftokensname = 
           std::string( "std::list < " ) + 
           cpp::makename( tokennamespace, std::string( "token" )) +
           std::string( " >" ); 

   stream << "void reduction_" << r. nr << "(\n";
   stream << "   " << fulllistoftokensname << " & stack,\n";

   for( std::list< globalvariables::globalvar > :: const_iterator
           p = glob. list. begin( );
           p != glob. list. end( );
           ++ p )
   {
      if( uses( p -> name ))
      {
         stream << "   " << p -> type << " & " << p -> name << ", \n"; 
      }
   }

   stream << "   " << fulllistoftokensname << " :: iterator position";

   unsigned int nr = 1; 
   for( std::list< symbol > :: const_iterator
           p = r. rhs. begin( );
           p != r. rhs. end( );
           ++ p )
   {
      stream << ",\n";
      stream << "   " << fulllistoftokensname << " :: iterator ";
      stream << *p << nr;
      ++ nr;
   }

   if( uses( "lookahead" ))
   {
      stream << ",\n"; 
      stream << "   const " << fulllistoftokensname << " & lookahead";
   }

   stream << " ) throw( refused )\n";
   stream << "{\n\n"; 

   stream << "#line " << linenumber << " ";
   stream << "\"" << filename << "\"" << "\n"; 

   unsigned int line = 0; 
   for( std::list< std::string > :: const_iterator
           p = code. begin( );
           p != code. end( );
           ++ p )
   {
      std::list< unsigned int > ids = identifierpositions( *p, line );
         // This may throw an action error, and we let it escape.

      std::list< unsigned int > :: const_iterator q = ids. begin( );

      unsigned int pos = 0; 
      while( pos < p -> size( ))  
      {
         if( q != ids. end( ) && pos == *q )
         {
            if( contains( *p, pos, "return" ))
            {
               pos += 6;
               std::string par = readreturnvalue( *p, pos, filename,
                                                           linenumber + line ); 
                  // pos is modified until after the value. 
               stream << "{ reduce( stack, position, ";
               stream << cpp::symbol_name( tokennamespace, r. lhs ) << ", ";
               stream << par << " ); return; }"; 
            }
            else
            {
               if( contains( *p, pos, "refuse" ))
               {
                  pos += 6;
                  stream << "throw refused( )"; 
               }
               else
               {
                  stream << (*p) [ pos ];
                  ++ pos;
               }
            }
         }
         else 
         {
            stream << (*p) [ pos ];
            ++ pos;
         }

         while( q != ids. end( ) && pos > *q )
            ++ q;

      }

      ++ line; 
      stream << "\n"; 
   }

   // There is no guarantee that this works, but it might work in   
   // case of empty attribute.

   stream << "{ reduce( stack, position, ";
   stream << cpp::symbol_name( tokennamespace, r. lhs ) << ", ";
   stream << cpp::symbol_name( tokennamespace, r. lhs ) << " ); return; }\n";

   stream << "\n"; 
   stream << "}\n\n";

}



std::ostream& operator << ( std::ostream& stream, const action& a )
{
   stream << "file " << a. filename << ", line " << a. linenumber << " :\n";
   stream << "------------------------------------\n";
   for( std::list< std::string > :: const_iterator
           p = a. code. begin( );
	   p != a. code. end( );
	   ++ p )
   {
      stream << "      " << *p << "\n";
   }
   stream << "\n"; 
   return stream;
}


