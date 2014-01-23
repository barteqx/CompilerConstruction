
#include "list.h"
#include <list>


struct list::lst_bigint : public lst
{
   bigint i;

   lst_bigint( unsigned int i )
      : i(i)
   { }

   lst_bigint( const bigint& i )
      : i(i)
   { }

   bool isbigint( ) const override { return true; }
   const bigint& getbigint( ) const override { return i; }

   bool istrue( ) { return i != 0; }
   
   void print( std::ostream& out ) const override 
   {
      out << "bigint( " << i << " )";
   }
};


struct list::lst_string : public lst
{
   std::string s;

   lst_string( const std::string& s )
      : s(s)
   { }

   bool isstring( ) const override { return true; }

   bool isnil( ) const override { return s == "nil"; }

   bool istrue( ) { return s != "false" && s != "nil"; }

   bool isEOF( ) const override { return s == "EOF"; }
   
   const std::string& getstring( ) const override { return s; }

   void print( std::ostream& out ) const override 
   {
      out << "string( " << s << " )";  
   }
};


struct list::lst_cons : public lst
{
   list first;
   list rest;

   lst_cons( const list& first, const list& rest )
      : first( first ), rest( rest )
   { }

   bool iscons( ) const override { return true; }
   bool islambda( ) const override { return first.getstring() == "lambda"; }
   const list& getfirst( ) const override { return first; }
   const list& getrest( ) const override { return rest; }
  
   void print( std::ostream& out ) const override
   {
      out << "cons( ";
      first. print( out );
      out << ", ";
      rest. print( out );
      out << " )"; 
   }
};

// Constructors:


list::list( unsigned int i ) 
   : ref( new lst_bigint( i ))
{ }

list::list( const bigint& i )
   : ref( new lst_bigint( i ))
{ }

list::list( const std::string& s )
   : ref( new lst_string(s))
{ }

list::list( const list& first, const list& rest )
   : ref( new lst_cons( first, rest ))
{ }

// Structural:

list::list( const list& l )
   : ref( l. ref )
{
   ++ ( ref -> refcnt );
}

void list::operator = ( const list& l )
{
   ++ ( l. ref -> refcnt );
   -- ( ref -> refcnt );
   if( ( ref -> refcnt ) == 0 )
   {
      delete ref; 
   }

   ref = l. ref;
}

void list::operator = ( list&& l )
{
   // We exchange the pointers:

   lst* p = ref;
   ref = l. ref;
   l. ref = p;
}

list:: ~list( )
{
   -- ( ref -> refcnt );
   if( ( ref -> refcnt ) == 0 )
   {
      delete ref;
   }
}


std::ostream& operator << ( std::ostream& stream, list l )
{
   if( l. isbigint( ))
   {
      stream << l. getbigint( );  
      return stream;
   }

   if( l. isstring( ))
   {
      stream << l. getstring( ); 
      return stream;
   }

   ASSERT( l. iscons( ));
   std::list< list > contents;
  
   while( l. iscons( ))
   {
      contents. push_back( l. getfirst( ));
      l = l. getrest( );
   }

   stream << "(";
   for( auto p = contents. begin( ); p != contents. end( ); ++ p )
   {
      stream << " " << *p;
   }
   if( l. isstring( ) && l. getstring( ) == "nil" )
      stream << " )";
   else
      stream << ". " << l << " )";
   return stream;
}
 
