#ifndef POSSIBLECONVERSIONS_INCLUDED
#define POSSIBLECONVERSIONS_INCLUDED 1

#include <vector>
#include "ctree.h"
#include "conversion.h"

class possibleconversions {
public:
   ctree tree;
   std::vector<conversion> conv;
   possibleconversions(const ctree t) : tree(t) { }

   void close();

   bool addConversion(ctree tree, unsigned int cost);

};

#endif