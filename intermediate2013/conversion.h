#ifndef CONVERSION_INCLUDED
#define CONVERSION_INCLUDED 1

#include "ctree.h"

class conversion {

public:
  ctree tree;
  unsigned int cost;

  conversion( ctree t, unsigned int c ) : tree(t), cost(c) { }

};

#endif