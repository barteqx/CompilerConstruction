

// Code written by Hans de Nivelle, July 2008.
// See the licence that is included with the code. 


#include <fstream>


#include "buildparsercode.h"
#include "cpp.h"
#include <cstdlib>


void buildparsercode::builddeclaration( 
                       const std::list< std::string > & parsernamespace,
		       const std::list< std::string > & tokennamespace,
		       const std::list< std::string > & tokenizernamespace,
                       const globalvariables& glob, 
		       std::ostream& stream )
{
   stream << "\n"; 
   stream << "// Code produced by Maphoon 2008.\n";
   stream << "// Declaration of parser:\n\n\n";

   std::string guard = cpp::includeguard( parsernamespace, "parser" );
   stream << "#ifndef " << guard << "\n";
   stream << "#define " << guard << "     1\n";

   stream << "\n\n";

   stream << "#include <list>\n";
   stream << "#include <iostream>\n";
   stream << "\n\n";

   stream << "#include \"token.h\"\n";
   stream << "#include \"tokenizer.h\"\n";


   stream << "\n\n";

   stream << cpp::makedeclaration( parsernamespace ) << "\n"; 
   
   std::string indent = cpp::indentation( parsernamespace );

   stream << indent << "void parser( "; 
   stream << cpp::makename( tokenizernamespace, "tokenizer" ) << "& r";

   indent += "      ";
   for( std::list< globalvariables::globalvar > :: const_iterator
           p = glob. list. begin( );
           p != glob. list. end( );
           ++ p )
   {
      stream << ",\n";
      stream << indent << p -> type << " & " << p -> name; 
   }
   stream << ",\n" << indent;
   stream << cpp::makename( tokennamespace, "tokentype start" );
   stream << ",\n" << indent << "unsigned int recoverlength";
   
   stream << " );\n";

   stream << indent << "   // In case of success, the parser returns a\n";
   stream << indent << "   // start token, possibly followed by a \n";
   stream << indent << "   // lookahead token. Otherwise, it returns  \n";
   stream << indent << "   // some other token. \n";
   stream << "\n\n"; 
   
   stream << cpp::makeclosure( parsernamespace ); 
   stream << "\n\n"; 
   stream << "#endif\n";
   stream << "\n\n"; 
}


unsigned int buildparsercode::encoding::size( ) const
{
   return 2 + reductions. size( );
}


void buildparsercode::encoding::addreduction( unsigned int rulenr )
{
   reductions. push_back( rulenr );
}



// Actions are encoded into integers as follows: 
//     10000 + X : push state X.
//     -10000 - R : reduce rule R.
//     0 : error.

void buildparsercode::encoding::printtableentry(
          const std::list< std::string > & tokennamespace, 
          std::ostream& stream ) const 
{
   stream << cpp::symbol_name( tokennamespace, s ) << ", ";
   for( std::list< unsigned int > :: const_iterator
           p = reductions. begin( );
           p != reductions. end( );
           ++ p )
   {
      stream << -10000 - ( static_cast<int> ( *p )) << ", ";
   }
   
   if( haspush )
      stream << 10000 + push << ", ";
   else
      stream << "0, ";
}
 

std::list< buildparsercode::encoding > 
buildparsercode::encodestate( const parsetable& pt, 
                              unsigned int ss, unsigned int def )
{
   std::list< encoding > enc;

   ASSERT( ss < pt. nrstates( ) && def < pt. nrstates( ));

   // Collect symbols for which one of the states has a transition:

   symbolset relevant = pt. getrelevantsymbols( ss );
   relevant. insert( pt. getrelevantsymbols( def ));

   ASSERT( ! relevant. contains( symbol::defaultred ));
      // We cannot handle this correctly.

   for( symbolset::const_iterator
           p = relevant. begin( );
           p != relevant. end( );
           ++ p )
   {
      bool diff = false;

      std::map< symbol, unsigned int > :: const_iterator p_ss =
                   pt. table[ ss ]. transitions. find( *p );
      std::map< symbol, unsigned int > :: const_iterator p_def =
                   pt. table[ def ]. transitions. find( *p );

      // We compare the pushing transitions and the reduction sets
      // for character *p.

      if( p_ss == pt. table [ ss ]. transitions. end( ))
      {
         if( p_def != pt. table [ def ]. transitions. end( ))
            diff = true;
      }
      else
      {
         if( p_def == pt. table [ def ]. transitions. end( ))
            diff = true;
         else
         {
            if( p_ss -> second != p_def -> second )
               diff = true;
         }
      }


      std::list< const rule* > 
                    r_ss = pt. table [ ss ]. set. getreducingrules(*p);
      std::list< const rule* > 
                    r_def = pt. table [ def ]. set. getreducingrules(*p);

      if( r_ss. size( ) == r_def. size( ))
      {
         std::list< const rule* > :: const_iterator q1 = r_ss. begin( );
         std::list< const rule* > :: const_iterator q2 = r_def. begin( );
         while( q1 != r_ss. end( ) && !diff )
         {
            if( *q1 != *q2 )
               diff = true;
            ++ q1;
            ++ q2;
         }
      }
      else
         diff = true;

      // If there is a difference, we store the symbol,
      // the list of reductions, and the push.
      // If there is no push, we store an error instead of it.  

      if( diff )
      {
         if( p_ss != pt. table [ss]. transitions. end( ))
            enc. push_back( encoding( *p, p_ss -> second )); 
         else
            enc. push_back( encoding( *p ));

         for( std::list< const rule* > :: const_iterator
                 q = r_ss. begin( ); 
                 q != r_ss. end( );
                 ++ q )
         {
            enc. back( ). addreduction( (*q) -> nr );
         }

      }
   }
   return enc;
}


std::list< buildparsercode::encoding >
buildparsercode::encodestate( const parsetable& pt, unsigned int ss ) 
{
   std::list< encoding > enc;
   ASSERT( ss < pt. nrstates( ));

   symbolset relevant = pt. getrelevantsymbols(ss);

   for( symbolset::const_iterator
           p = relevant. begin( );
           p != relevant. end( );
           ++ p )
   {
      std::map< symbol, unsigned int > :: const_iterator
                   q = pt. table[ss]. transitions. find(*p);

      std::list< const rule* > r = pt. table [ss]. set. getreducingrules(*p);
     
      if( q != pt. table [ss]. transitions. end( ))
         enc. push_back( encoding( *p, q -> second ));
      else
         enc. push_back( encoding( *p ));

      for( std::list< const rule* > :: const_iterator
              q = r. begin( );
              q != r. end( );
              ++ q )
      {
         enc. back( ). addreduction( (*q) -> nr );
      }
   }

   return enc;
}


unsigned int buildparsercode::length( 
                  const std::list< buildparsercode::encoding > & enc )
{
   unsigned int len = 0;
   for( std::list< buildparsercode::encoding > :: const_iterator
           p = enc. begin( );
           p != enc. end( );
           ++ p )
   {
      len += p -> size( );
   }
   return len;
}



namespace
{


   void printmaintables( const parsetable& pt, 
                         const std::list< std::string > & tokennamespace,
                         std::ostream& stream ) 
   {
      std::list< unsigned int > starts;
         // Where each rule starts in the following table.
         // Starts is one longer than the number of states, so that
         // we can also use it for determining the end.

      std::list< unsigned int > defaults; 
         // For every state, we store the most similar state.
         // Only the differences are in the table.

      std::list< buildparsercode::encoding > maintable;
      unsigned int maintablelength = 0;

      // We first create the tables, after that we print them.
       
      for( unsigned int ss = 0; ss < pt. nrstates( ); ++ ss )
      {
         // We first construct stand alone encoding: 

         unsigned int bestdef = ss; 
         std::list< buildparsercode::encoding > bestencoding =
                         buildparsercode::encodestate( pt, ss );
         unsigned int bestlength = length( bestencoding ); 

         // After that, we check if using a default is better.

         // Unfortunately, encodestate( pt, ss, def ) does not work
         // correctly in the presence of default reductions.
         // We therefore exclude states with default reductions, because
         // there seems to be no way to fix it. 

         if( pt. table [ss]. set. 
                getreducingrules( symbol::defaultred ). size( ) == 0 )
         {
            for( unsigned int def = 0; def < ss; ++ def )
            {
               if( pt. table [def]. set.
                       getreducingrules( symbol::defaultred ). size( ) == 0 )
               {
                  std::list< buildparsercode::encoding > enc = 
                                buildparsercode::encodestate( pt, ss, def );
                  unsigned int enclength = length( enc );

                  if( enclength < bestlength )
                  {
                     bestdef = def;
                     bestencoding = enc;
                     bestlength = enclength; 
                  }
               }
            }
         }

         starts. push_back( maintablelength ); 
         defaults. push_back( bestdef );
          
         maintablelength += length( bestencoding );
         while( bestencoding. size( ))
         {
            maintable. push_back( bestencoding. front( ));
            bestencoding. pop_front( );
         }
      }

      starts. push_back( maintablelength );

      unsigned int pos = 0;

      stream << "   unsigned int starts [] = \n";
      stream << "   {\n";
      stream << "      ";

      pos = 0; 
      for( std::list< unsigned int > :: const_iterator 
              p = starts. begin( );
              p != starts. end( );
              ++ p )
      {
         stream << *p << ", ";
         if( pos >= 10 )
         {
            stream << "\n";
            stream << "      "; 
            pos = 0;
         }
         ++ pos; 
      }
      stream << "0 };\n\n";

      stream << "   unsigned int defaults [] = {\n";
      stream << "      ";

      pos = 0;
      for( std::list< unsigned int > :: const_iterator
              p = defaults. begin( );
              p != defaults. end( );
              ++ p )
      {
         stream << *p << ", ";
         if( pos >= 10 )
         {
            stream << "\n";
            stream << "      "; 
            pos = 0;
         }
         ++ pos; 
      }
      stream << "0 };\n\n";

      stream << "   int parsetable [] = \n";
      stream << "   {\n";

      {
         for( std::list< buildparsercode::encoding > :: const_iterator
                 p = maintable. begin( );
                 p != maintable. end( );
                 ++ p )
         {
            stream << "      "; 
            p -> printtableentry( tokennamespace, stream ); 
            stream << "\n"; 
         }
      }

      stream << "   0 };\n\n";
   }

  
   void printentrypoints( const parsetable& pt, const grammar& gram,
                          const std::list< std::string > & tokennamespace, 
                          std::ostream& stream )
   {
      stream << "   int entrypoints [] = \n";
      stream << "   {\n";

      ASSERT( pt. startstates. size( ) == gram. starts. size( ));

      std::map< symbol, unsigned int > :: const_iterator 
            p = pt. startstates. begin( );
      std::map< symbol, symbolset > :: const_iterator 
            g = gram. starts. begin( );
 
      while( p != pt. startstates. end( ))
      {
         ASSERT( p -> first == g -> first );
         stream << "      "; 
         stream << cpp::symbol_name( tokennamespace, p -> first ) << ", ";
         stream << p -> second << ", ";
         for( symbolset::const_iterator
                 q = g -> second. begin( );
                 q != g -> second. end( );
                 ++ q )
         {
            stream << cpp::symbol_name( tokennamespace, *q ) << ", ";
         }
         stream << "-1,\n";

         ++ p; ++ g; 
      }
      stream << "      -1\n";
      stream << "   };\n\n";
   }                 
 
}



void buildparsercode::builddefinition( 
          const std::list< std::string > & parsernamespace,
          const std::list< std::string > & tokennamespace, 
	  const std::list< std::string > & tokenizernamespace, 
	  const grammar& gram,
	  const parsetable& pt,
          const globalvariables& glob, 
	  std::ostream& outputstream ) throw( actionerror ) 
{

   std::ifstream inputstream( "idee.x" );
   if( ! inputstream )
   {
      std::cout << "ERROR: could not open idee.x\n\n";
      exit(0);
   }

   // Some headers:

   outputstream << "\n\n"; 
   outputstream << "#include \"token.h\"\n";
   outputstream << "#include \"tokenizer.h\"\n";
   outputstream << "#include \"parser.h\"\n";
   outputstream << "\n";
   outputstream << "#include <cstdlib>\n";
   outputstream << "\n\n";

   while( true )
   {

      std::string line;

      char nextchar = inputstream. get( );
      if( nextchar == EOF )
         return;

      while( nextchar != '\n' && nextchar != EOF )
      {
         line. push_back( nextchar );
	 nextchar = inputstream. get( );

         if( ! inputstream )
         {
            std::cout << "ERROR: problems with idee.x\n\n";
	    exit(0);
         }
      }

      if( nextchar == EOF )
      {
         std::cout << "ERROR: EOF unexpectedly came in idee.x\n\n";
	 exit(0);
      }

      // We copy the line. If we encounter $X$, we insert something
      // special which depends on the character X. 

      unsigned int pp = 0;
      while( pp < line. size( ))
      {

         if( pp + 3 <= line. size( ) &&
             line [ pp + 0 ] == '$' && 
	     line [ pp + 2 ] == '$' )
         {
            switch( line [ pp + 1 ] ) 
            {
	    case 'P':
               outputstream << cpp::makename( parsernamespace, "parser" ); 
	       break;

            case 'T':
	       outputstream << cpp::makename( tokennamespace, "token" ); 
	       break;

            case 'E':
               outputstream << cpp::makename( tokennamespace, "tkn_" );
               break;

            case 'R':
	       outputstream << cpp::makename( tokenizernamespace, 
                                                       "tokenizer" );
	       break;

	    case 'X':
               printmaintables( pt, tokennamespace, outputstream ); 
               printentrypoints( pt, gram, tokennamespace, outputstream );
               break; 

            case 'A':
	       for( unsigned int nr = 0; nr < gram. rules. size( ); ++ nr )
               {
		  gram. actions [nr]. printdefinition( 
                                           outputstream, glob, 
                                           parsernamespace, tokennamespace, 
                                           gram. rules [nr] );
		  outputstream << "\n";  
               }
	       break;

            case 'C':
               for( unsigned int nr = 0; nr < gram. rules. size( ); ++ nr )
               {
                  outputstream << "case " << nr << ":\n";

                  outputstream << "   "; 
                  gram. actions [nr]. printcall( outputstream,
                                                 glob, gram. rules [nr] );
                  outputstream << "   break;\n";
               }
               break;

            case 'G':
               for( std::list< globalvariables::globalvar > :: const_iterator
                       p = glob. list. begin( );
                       p != glob. list. end( );
                       ++ p )
               {
                  outputstream << ",\n";
                  outputstream << "      ";
                  outputstream << p -> type << " & " << p -> name;
               }
               break;
 
            default:
               std::cout << "ERROR: unknown special insert: ";
	       std::cout << line [ pp + 1 ];
	       std::cout << "\n"; 
	       exit(0);
            }

	    pp += 3; 
         }
	 else
         {
            outputstream << line [ pp ];
	    ++ pp; 
         }
      }
      outputstream << "\n";
   }

}


