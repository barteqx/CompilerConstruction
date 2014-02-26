
#include "error.h"
#include "identifier.h"
#include "data.h"
#include "ctype.h"
#include "ctree.h"
#include "varstore.h"
#include "tokenizer.h"
#include "parser.h"
#include "structstore.h" 
#include "functionstore.h"
#include "possibleconversions.h"

ctree typecheck(ctree t, varstore& local, structstore& st, functionstore& fs);

bool compatibile(ctype t1, ctype t2);

void testtokenizer( ) 
{

   tokenizer tt;

   tt. scan( );

   // Test the tokenizer:

   while( tt. lookahead. size( ) && 
          tt. lookahead. front( ). type != tkn_EOF )
   {
      std::cout << tt. lookahead. front( ) << "\n";
      tt. lookahead. pop_front( ); 
      tt. scan( ); 
   }

   // Actually if the size of lookahead gets 0, something went wrong.

   ASSERT( tt. lookahead. size( ));
   std::cout << tt. lookahead. front( ) << "\n";
      // This is an EOF token. 
}


ctree readtree( ) throw( error ) 
{
   std::cout << "\n"; 
   std::cout << "Please type a tree to be typechecked: ";
   tokenizer tt; 
   parser( tt, tkn_B1, 10 );

   if( tt. lookahead. size( ) != 2 ||
       tt. lookahead. front( ). type != tkn_B1 ||
       tt. lookahead. back( ). type != tkn_SEMICOLON )
   {
      error err;
      err. s << "stopped due to parsing error";
      throw err;
   }
   
   return tt. lookahead. front( ). ct. front( ); 
}



int main( int argc, char* argv [ ] )
{
   std::cout << "hallo\n";

   for( unsigned int i = 0; i < 1; ++ i )
   {

      // We introduce two structs:

      // struct vector
      // {
      //    double x;
      //    double y;
      //    double z;
      // }

      // struct list
      // {
      //    int elem;
      //    list* next;
      // };

      structstore st;
      st. structtypes[ identifier( "vector" ) ] = 
         std::vector< structstore::field > (
         { { identifier("x"), ctype( type_prim, identifier( "double" )) },
           { identifier("y"), ctype( type_prim, identifier( "double" )) },
           { identifier("z"), ctype( type_prim, identifier( "double" )) } } ); 

      st. structtypes[ identifier( "list" ) ] = 
         std::vector< structstore::field > ( 
         { { identifier( "elem" ), ctype( type_prim, identifier( "int" )) },
           { identifier( "next" ), 
              ctype( type_pointer, ctype( type_prim, identifier( "list" ))) }
         } );

      // Simple test of find( ):
 
      auto* p = st. find( identifier( "list" ), identifier( "next" ));
      if(p) std::cout << *p << "\n";
 
      std::cout << st << "\n";

      varstore local;
      local. init( "i", "$i", 
                    ctype( type_prim, identifier( "int")));
      local. init( "j", "$j", 
                    ctype( type_prim, identifier( "int" )));

      local. init( "l1", "$l1",
                    ctype( type_prim, identifier( "list" )));
      local. init( "l2", "$l2",
                    ctype( type_prim, identifier( "list" )));

      local. init( "v1", "$v1",
                    ctype( type_prim, identifier( "vector" )));
      local. init( "v2", "$v2",
                    ctype( type_prim, identifier( "vector" )));

      functionstore func; 
      func. insert( identifier( { "vector", "vector" } ), 
                    ctype( 
                       type_func,
                       ctype( type_prim, identifier( "vector" )),
                        { ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))) }
                         )); 

      func. insert( identifier("dotproduct"), 
                    ctype( 
                       type_func,
                       ctype( type_prim, identifier( "double" )),
                        { ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))),
                          ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))) }
                         )); 

      func. insert( identifier("crossproduct" ), 
                    ctype( 
                       type_func,
                       ctype( type_prim, identifier( "vector" )),
                        { ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))),
                          ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))) }
                         )); 

      func. insert( identifier( "length" ), 
                    ctype( 
                       type_func,
                       ctype( type_prim, identifier( "double" )),
                        { ctype( type_ref, 
                              ctype( type_const,
                                 ctype( type_prim, identifier( "vector" )))) }
                         )); 

      func. insert( identifier( {"list", "list"} ), 
                    ctype(
                       type_func,
                       ctype( type_prim, identifier( "list" )),
                        { ctype( type_ref,
                              ctype( type_const,
                                 ctype( type_prim, identifier( "list" )))) }
                         ));

      func. insert( identifier("first" ), 
                    ctype(
                       type_func,
                       ctype( type_prim, identifier( "int" )),
                        { ctype( type_ref,
                              ctype( type_const,
                                 ctype( type_prim, identifier( "list" )))) }
                         ));

      func. insert( identifier( "sum" ), 
                    ctype(
                       type_func,
                       ctype( type_prim, identifier( "int" )),
                        { ctype( type_ref,
                              ctype( type_const,
                                 ctype( type_prim, identifier( "list" )))) }
                         ));

      func. insert( identifier( "size" ), 
                    ctype(
                       type_func,
                       ctype( type_prim, identifier( "int" )),
                        { ctype( type_ref,
                              ctype( type_const,
                                 ctype( type_prim, identifier( "list" )))) }
                         ));


      
      std::cout << local << "\n";
      std::cout << func << "\n";

      try
      {
         ctree res = readtree( ); 
         std::cout << "read:\n";
         res. prettyprint( std::cout );
         std::cout << "Typecheck :\n";
         typecheck(res, local, st, func).prettyprint(std::cout);
      } 
      catch( const error& err )
      {
         std::cout << err << "\n";
      }
      return 0;

      // We make our first program: 
   }
}

ctree typecheck(ctree t, varstore& local, structstore& st, functionstore& fs) {

  treetype type = t.gettreetype();
  identifier id;
  varstore::value *val;

  unsigned int arity;

  std::vector< possibleconversions > argument_conversions;
  std::vector< ctree > chosen_conversions;
  int costs = -1;
  bool func_found = false, ambigious = false;

  switch (type) {

  case treetype::data:
    return t;

  case treetype::var:
    id = t.getvarname();
    val = local.lookup(id[0]);

    if (val) t.ct = val -> t;
    else {
      std::cerr << "Variable not found" << std::endl;
      t.ct = ctype(type_prim, identifier("int"));
    }
    return t;

  case treetype::func:
    
    id = t.getfunction();
    arity = t.getarity();

    for (int i = 0; i < arity; i++) {
      const ctree checkedArg = typecheck(t[i], local, st, fs);

      possibleconversions p(checkedArg);
      p.close();

      //std::cout << p.conv.size() << std::endl;

      argument_conversions.push_back(p);
    }

    for (auto f : fs.functions) {
      if(identifier::compare(f.name, id)) continue;
      func_found = true;
      if (f. t. getarity( ) != arity) continue;

      int temp_cost = 0;
      std::vector< ctree > temp_args;

      bool compatibility;
      for (int i = 0; i < arity; i++) {
        compatibility = false;
        for (auto conv : argument_conversions[i].conv) {
          if (compatibile(f.t[i], conv.tree.ct)) {
            compatibility = true;
            temp_cost += conv.cost;
            temp_args.push_back(conv.tree);
            break;
          }
        }

        if (compatibility == false) break;
      }
      if (compatibility && (temp_cost < costs || costs == -1)) {
        chosen_conversions = temp_args;
        costs = temp_cost;
        ambigious = false;
        t = ctree(id, chosen_conversions, f.t.getrestype());
      } else if (compatibility && temp_cost == costs) ambigious = true;
    }

    if (func_found) {
      if (ambigious) std::cerr << "Ambigious declaration of " << id << std::endl;
      if (costs == -1) std::cerr << id << " undefined" << std::endl;
      return t;
    } 

    if(id[0] == "fieldfunction") {
      const identifier fieldname(id[1]);
      ctype argtype = argument_conversions[0].conv[0].tree.ct;

      if (argtype.gettypeconstr() != type_prim) {
        std::cerr << id << " undefined" << std::endl;
        return t;
      }
      ctree arg = argument_conversions[0].conv[3].tree;
      const identifier structname(argtype.getprimtype());
      identifier newId = structname;
      newId += fieldname;

      const structstore::field *field = st.find(structname, fieldname);

      if (field) {
        ctype retType = field -> t;

        t = ctree(newId, { arg }, retType);
        return t;
      } 
      std::cerr << newId << " undefined" << std::endl;


    }

    if (id[0] == "infix+" && arity == 2) {
      int costInt = 0, costDouble = 0;
      std::vector< ctree > matchInt, matchDouble;
      ctype t_int(type_reg, ctype(type_prim, identifier("int")));
      ctype t_double(type_reg, ctype(type_prim, identifier("double")));

      for (auto conv1 : argument_conversions[0].conv) {
        if (compatibile(conv1.tree.ct, t_int)) {
          costInt += conv1.cost;
          matchInt.push_back(conv1.tree);
        } else if (!ctype::compare(conv1.tree.ct, t_double)) {
          costDouble += conv1.cost;
          matchDouble.push_back(conv1.tree);
        }
      }

      for (auto conv1 : argument_conversions[1].conv) {
        if (compatibile(conv1.tree.ct, t_int)) {
          costInt += conv1.cost;
          matchInt.push_back(conv1.tree);
        } else if (!ctype::compare(conv1.tree.ct, t_double)) {
          costDouble += conv1.cost;
          matchDouble.push_back(conv1.tree);
        }
      }

      std::cout << matchInt.size() << matchDouble.size();

      if (matchInt.size() == 2 && matchDouble.size() == 2) {
        if (costDouble == costInt) { 
          std::cerr << "Ambigious declaration of " << id << std::endl;
        } else if (costDouble > costInt) {
          return ctree( identifier( "infix+"), matchInt, t_int);
        } else return ctree( identifier( "infix+"), matchDouble, t_double);
      } else if (matchInt.size() == 2) {
        return ctree( identifier( "infix+"), matchInt, t_int);
      } else if (matchDouble.size() == 2) {
        return ctree( identifier( "infix+"), matchDouble, t_double);
      } else {
        std::cerr << id << " undefined" << std::endl;
      }
    }

  }

  return t;
}


bool compatibile(ctype t1, ctype t2) {
   type_constr tconstr1, tconstr2;

   do {
      tconstr1 = t1.gettypeconstr();
      tconstr2 = t2.gettypeconstr();

      if (tconstr1 == type_const) {
         t1 = t1.getsubtype();
         continue;
      }
      if (tconstr2 == type_const) {
         t2 = t2. getsubtype();
         continue;
      }

      if (tconstr1 != tconstr2) return false;
      if (tconstr1 == type_prim) {

         if (identifier::compare(t1.getprimtype(), t2.getprimtype())) return false;
      }

      if (tconstr1 != type_prim && tconstr2 != type_prim) {
         t1 = t1.getsubtype();
         t2 = t2.getsubtype();
      }

   } while (tconstr1 != type_prim || tconstr2 != type_prim);
   return true;
}