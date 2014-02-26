#include <vector>
#include "ctree.h"
#include "possibleconversions.h"

void possibleconversions::close () {

  conv.push_back(conversion(tree, 0));

  for (int i = 0; i < conv.size(); i++) {

    ctree t = conv[i].tree;
    ctype ct = t.ct;
    unsigned int cost = conv[i].cost;

    type_constr t_constr = ct.gettypeconstr();
    identifier id;

    switch(t_constr) {

      case type_prim:
        addConversion( ctree( identifier( "read" ),
          { ctree(t) },
          ctype( type_reg, ct )
        ), cost + 1);


        addConversion( ctree( identifier( "box" ),
          { ctree(t) },
          ctype( type_reg, ctype( type_ref, ct ))
        ), cost + 1);
        break;

      case type_reg:

        addConversion( ctree( identifier( "write" ),
          { ctree(t) },
          ct.getsubtype()
        ), cost + 1);

        if (ct.getsubtype().gettypeconstr() == type_ref) {
          addConversion( ctree( identifier( "inline-copy" ),
          { ctree(t) },
          ctype(type_reg, ct.getsubtype().getsubtype())
        ), cost + 1);

        } else {

          id = ct.getsubtype().getprimtype();

          if (!identifier::compare( id, identifier( "bool" ))) {

            addConversion(ctree( identifier( "bool2char" ),
                 { ctree(t) },
                 ctype( type_reg, ctype( type_prim, identifier( "char" )))
              ), cost + 1);

          } else if (!identifier::compare( id, identifier( "char" ))) {
              addConversion(ctree( identifier( "char2int" ),
                 { ctree(t) },
                 ctype( type_reg, ctype( type_prim, identifier( "int" )))
              ), cost + 1);

          } else if (!identifier::compare( id, identifier( "int" ))) {
              addConversion(ctree( identifier( "int2double" ),
                 { ctree(t) },
                 ctype( type_reg, ctype( type_prim, identifier( "double" )))
              ), cost + 1);
          } 
        }

        break;

      case type_ref:

        addConversion( ctree( identifier( "copy" ),
          { ctree(t) },
          ct.getsubtype()
        ), cost + 1);

        addConversion( ctree( identifier( "read" ),
          { ctree(t) },
          ctype( type_reg, ct )
        ), cost + 1);
        break;

    }
  }
}


bool possibleconversions::addConversion (ctree t, unsigned int cost) {
  ctype type = t.ct;
  bool convExists = false;

  for (auto c2 : conv) {
    if (ctype::compare(type, c2.tree.ct) == 0) {
      convExists = true;
      break;
    }
  }

  if (convExists) return false;

  conv.push_back(conversion(t, cost));
  return true;
}