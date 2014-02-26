
// A C/C++ style type. Written for Course Compiler Construction,
// Hans de Nivelle, 2013.

#include "ctype.h"


struct ctype::ct_nullary : public ct
{
   ct_nullary( type_constr t ) 
      : ct(t) 
   { 
      ASSERT( t == type_void || t == type_unknown ); 
   }

   void print( std::ostream& stream ) const override
   {
      stream << t; 
   }
};

struct ctype::ct_unary : public ct
{
   ctype subtype;

   ct_unary( type_constr t, const ctype& subtype )
      : ct(t),
        subtype( subtype )
   {
      ASSERT( t == type_const || t == type_raw || t == type_pointer ||
              t == type_ref || t == type_rvalref || t == type_reg );
   }

   const ctype& getsubtype( ) const override 
   {
      return subtype;
   }

   void print( std::ostream& stream ) const override
   {
      stream << t << "( ";
      subtype. ref -> print( stream );
      stream << " )"; 
   }
};

struct ctype::ct_prim : public ct
{
   identifier id;

   ct_prim( type_constr t, const identifier& id )
      : ct( t ),
        id( id )
   { 
      ASSERT( t == type_prim );
   }

   const identifier& getprimtype( ) const override { return id; }

   void print( std::ostream& stream ) const override
   {
      stream << t << "( " << id << " )"; 
   }

};

struct ctype::ct_array : public ct
{
   unsigned int s;
   ctype subtype;

   ct_array( type_constr t, unsigned int s, const ctype& subtype )
      : ct( t ),
        s( s ),
        subtype( subtype )
   {
      ASSERT( t == type_array );
   }

   const ctype& getsubtype( ) const override { return subtype; }
   unsigned int getsize( ) const override { return s; } 
   
   void print( std::ostream& stream ) const override
   {
      stream << t << "( " << s << ", " << subtype << " )";
   }
};

struct ctype::ct_func : public ct
{
   ctype result;
   std::vector< ctype > args;

   ct_func( type_constr t, const ctype& result, 
                           const std::vector< ctype > & args )
      : ct(t),
        result( result ), 
        args( args )
   {
      ASSERT( t == type_func );
   }

   ct_func( type_constr t, const ctype& result,
                           std::vector< ctype > && args )
      : ct(t),
        result( result ),
        args( std::move(args) )
   {
      ASSERT( t == type_func );
   }

   const ctype& getrestype( ) const override { return result; }
   unsigned int getarity( ) const override { return args. size( ); }
   const ctype& operator [] ( unsigned int i ) const override
   {
      ASSERT( i < args. size( ));
      return args [i];
   }

   
   void print( std::ostream& stream ) const override
   {
      stream << t << "( " << result; 
      if( args. size( ))
      {
         stream << "; ";
         for( auto p = args. begin( ); p != args. end( ); ++ p )
         {
            if( p != args. begin( ))
               stream << ", "; 
            stream << *p;
         }
         stream << " )";
      }
   }
};

// Life cycle operators: (constructors, assignment, destructor)

ctype::ctype( type_constr t )
   : ref( new ct_nullary(t))
{ }

ctype::ctype( type_constr t, const identifier& id )
   : ref( new ct_prim( t, id ))
{ }

ctype::ctype( type_constr t, const ctype& sub )
   : ref( new ct_unary( t, sub ))
{ }

ctype::ctype( type_constr t, unsigned int s, const ctype& sub )
   : ref( new ct_array( t, s, sub ))
{ }

ctype::ctype( type_constr t, const ctype& res, 
                             const std::vector< ctype > & args )
   : ref( new ct_func( t, res, args ))
{ }

ctype::ctype( type_constr t, const ctype& res,
                             std::vector< ctype > && args )
   : ref( new ct_func( t, res, std::move(args) ))
{ }


ctype::ctype( const ctype& c )
   : ref( c. ref )
{
   ++ ( c. ref -> refcnt );
}

void ctype::operator = ( const ctype& c )
{
   ct* cref = c. ref;

   ++ ( cref -> refcnt );
   -- ( ref -> refcnt );

   if( ( ref -> refcnt ) == 0 )
      delete ref;

   ref = cref;
}

void ctype::operator = ( ctype&& c )
{
   ct* p = ref;
   ref = c. ref;
   c. ref = p;
}

ctype:: ~ctype( )
{
   -- ( ref -> refcnt );
   if( ( ref -> refcnt ) == 0 )
      delete ref;
}


int ctype::compare( const ctype& c1, const ctype& c2 )
{
   type_constr t1 = c1. gettypeconstr( );
   type_constr t2 = c2. gettypeconstr( );
   if( t1 < t2 ) return -1;
   if( t1 > t2 ) return 1;

   switch( t1 )
   {
   case type_void:
   case type_unknown:
      return 0;

   case type_prim:
      return identifier::compare( c1. getprimtype( ), c2. getprimtype( ));
   
   case type_const:
   case type_raw:
   case type_pointer:
   case type_ref:
   case type_rvalref:
   case type_reg:
      return compare( c1. getsubtype( ), c2. getsubtype( )); 

   case type_array:
      {
         int c = compare( c1. getsubtype( ), c2. getsubtype( ));
         if(c) return c;
         unsigned int s1 = c1. getsize( );
         unsigned int s2 = c2. getsize( );
         if( s1 < s2 ) return -1;
         if( s1 > s2 ) return 1;
         return 0;
      }

   case type_func:
      {
         int a1 = c1. getarity( );
         int a2 = c2. getarity( );
         for( unsigned int i = 0; i < a1 && i < a2; ++ i )
         {
            int c = compare( c1[i], c2[i] );
            if(c) return c;
         }

         if( a1 < a2 ) return -1;
         if( a1 > a2 ) return 1;

         int c = compare( c1. getrestype( ), c2. getrestype( ));
         if(c) return c;
         return 0;
      }

   }
   QUIT( );  
}


std::ostream& operator << ( std::ostream& stream, type_constr t )
{
   switch(t)
   {
   case type_void:
      stream << "void";
      return stream;
   case type_unknown:
      stream << "unknown";
      return stream;

   case type_prim:
      stream << "primitive";
      return stream;

   case type_const:
      stream << "const";
      return stream;
   case type_raw:
      stream << "raw";
      return stream;
   case type_pointer:
      stream << "pointer";
      return stream;
   case type_ref:
      stream << "ref";
      return stream;
   case type_rvalref:
      stream << "rvalref";
      return stream;
   case type_reg:
      stream << "reg";
      return stream;

   case type_array:
      stream << "array";
      return stream;
   case type_func:
      stream << "func";
      return stream;
   }
   QUIT( ); // Unreachable. 
}

