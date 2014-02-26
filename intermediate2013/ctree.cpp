

// Code written by Hans de Nivelle, december 2013.

#include "assert.h"
#include "ctree.h"


struct ctree::ctr_data : public ctr
{
   std::vector< char > data;
   
   ctr_data( const std::vector< char > & data ) 
      : ctr( treetype::data ), data( data )
   { }

   ctr_data( std::vector< char > && data )
      : ctr( treetype::data ), data( std::move( data ))
   { }

   std::vector< char > :: const_iterator data_begin( ) const override 
   {
      return data. begin( );
   }

   std::vector< char > :: const_iterator data_end( ) const override
   {
      return data. end( );
   }

   void prettyprint( std::ostream& stream, unsigned int depth ) const override 
   {
      printindent( stream, depth ); 
      stream << "data{ " << data << " }";
   }

   void print( std::ostream& stream, unsigned int depth ) const override
   {
      if( depth <= 1 )
         stream << "data{ " << data << " }";
      else
         stream << "data{ ... }"; 
   }

};


struct ctree::ctr_var : public ctr
{
   identifier name;

   ctr_var( const identifier& name )
      : ctr( treetype::var ), name( name )
   { }

   const identifier& getvarname( ) const override { return name; }

   void prettyprint( std::ostream& stream, unsigned int depth ) const override
   {
      printindent( stream, depth );
      stream << name;
   }

   void print( std::ostream& stream, unsigned int depth ) const override
   {
      stream << name;
   }
};


struct ctree::ctr_func : public ctr
{
   identifier f;
   std::vector< ctree > args; 

   ctr_func( const identifier& f, const std::vector< ctree > & args )
      : ctr( treetype::func ), f(f), args(args)
   { }

   ctr_func( const identifier& f, std::vector< ctree > && args )
      : ctr( treetype::func ), f(f), args( std::move( args ))
   { }

   const identifier& getfunction( ) const { return f; }
   unsigned int getarity( ) const { return args. size( ); }
   const ctree& operator [] ( unsigned int i ) const 
   {
      ASSERT( i < args. size( ));
      return args [i];
   }

   void prettyprint( std::ostream& stream, unsigned int depth ) const override
   {
      printindent( stream, depth );
      stream << f;
      for( unsigned int i = 0; i < args. size( ); ++ i )
      {
         stream << "\n";
         args [i]. ref -> prettyprint( stream, depth + 1 );
      }
   }

   void print( std::ostream& stream, unsigned int depth ) const override
   {
      stream << f << "(";
      for( unsigned int i = 0; i < args. size( ); ++ i )
      {
         if(i)
            stream << ", ";
         else
            stream << " ";
         args [i]. ref -> print( stream, depth + 1 ); 
      }
      stream << " )";
   }

};

ctree::ctree( const std::vector< char > & data, const ctype& ct )
   : ref( new ctr_data( data )),
     ct( ct )
{ }

ctree::ctree( std::vector< char > && data, const ctype& ct )
   : ref( new ctr_data( std::move(data) )),
     ct( ct )
{ }

ctree::ctree( const identifier& varname, const ctype& ct )
   : ref( new ctr_var( varname )),
     ct( ct )
{ }

ctree::ctree( const identifier& f, const std::vector< ctree > & args,
              const ctype& ct )
   : ref( new ctr_func( f, args )),
     ct( ct )
{ }

ctree::ctree( const identifier& f, std::vector< ctree > && args,
              const ctype& ct )
   : ref( new ctr_func( f, std::move( args ))),
     ct( ct )
{ }


ctree::ctree( const ctree& other )
   : ref( other. ref ),
     ct( other. ct )
{
   ++ ( other. ref -> refcnt );
}

void ctree::operator = ( const ctree& other )
{
   ctr* otherref = other. ref;

   ++ ( otherref -> refcnt );
   -- ( ref -> refcnt );

   if( ( ref -> refcnt ) == 0 )
      delete ref;

   ref = otherref;
   ct = other. ct; 
}

void ctree::operator = ( ctree&& other )
{
   ct = std::move( other. ct );

   ctr* p = ref;
   ref = other. ref;
   other. ref = p;    
}

ctree:: ~ctree( ) 
{
   -- ( ref -> refcnt );
   if( ( ref -> refcnt ) == 0 )
      delete ref;
}

int ctree::compare( const ctree& ct1, const ctree& ct2 )
{
   treetype t1 = ct1. gettreetype( );
   treetype t2 = ct2. gettreetype( ); 
 
   if( t1 < t2 ) return -1;
   if( t1 > t2 ) return 1;
 
   switch( t1 )
   {
   case treetype::data:
      {
         auto p1 = ct1. data_begin( );
         auto q1 = ct1. data_end( );
         auto p2 = ct2. data_begin( );
         auto q2 = ct2. data_end( );

         while( p1 < q1 && p2 < q2 )
         {
            if( *p1 < *p2 ) return -1;
            if( *p1 > *p2 ) return 1;
            ++ p1; ++ p2;
         }

         if( p1 == q1 && p2 != q2 ) return -1;
         if( p1 != q1 && p2 == q2 ) return 1;
         return 0;
      }
   
   case treetype::var:
         return identifier::compare( ct1. getvarname( ), ct2. getvarname( )); 

   case treetype::func:
      {
         int c = identifier::compare( ct1. getfunction( ), 
                                      ct2. getfunction( ));

         if(c) return c;

         unsigned int a1 = ct1. getarity( ); 
         unsigned int a2 = ct2. getarity( ); 

         for( unsigned int i = 0; i < a1 && i < a2; ++ i )
         {
            c = compare( ct1[i], ct2[i] );
            if(c) return c;
         }
         if( a1 < a2 ) return -1;
         if( a1 > a2 ) return 1;
         return 0;
      } 
   }
   QUIT( );
}
      

