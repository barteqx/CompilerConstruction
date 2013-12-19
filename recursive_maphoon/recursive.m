

%token EOF SCANERROR
%token SEMICOLON BECOMES COMMA 
%token IDENTIFIER NUMBER 
%token PLUS TIMES MINUS DIVIDES
%token FACTORIAL
%token LPAR RPAR

// Non-terminal symbols:

%token E F G H LISTARGS 
%token Session Command 
%token Start 
%token LIST

%intokenheader #include "list.h"

%startsymbol Session EOF 

%attribute value         list
%attribute id            std::string
%attribute reason        std::string

%constraint IDENTIFIER  id 1 2
%constraint E value 0 2
%constraint F value 0 2
%constraint G value 0 2
%constraint H value 0 2
%constraint SCANERROR reason 1 2
%constraint NUMBER value 1 2
%constraint LISTARGS value 1 2
%constraint LIST value 1 2
   // You may write the others.

%global memory varstore
   // Contains stored variables.

#include "list.h"
#include "assert.h"
#include "varstore.h"
#include <math.h>


% Start : Session EOF
   std::cout << "exiting...\n";
%   ;

% Session : Session LPAR LISTARGS RPAR
  std::cout << ">>> " << LISTARGS3 -> value. front( ) << "\n";
%         |
%         ;

% LIST : IDENTIFIER   
   token l = tkn_LIST;
   l. value. push_back( list( IDENTIFIER1 -> id. front( ) ) );
   return l;

%      | NUMBER
   
   token l = tkn_LIST;
   l. value. push_back( list( NUMBER1 -> value. front( ) ) );
   return l;

%      | PLUS
   
   token l = tkn_LIST;
   l. value. push_back( list( "+") );
   return l;

%      | MINUS
   
   token l = tkn_LIST;
   l. value. push_back( list( "-") );
   return l;

%      | DIVIDES
   
   token l = tkn_LIST;
   l. value. push_back( list( "/") );
   return l;

%      | TIMES
   
   token l = tkn_LIST;
   l. value. push_back( list( "*") );
   return l;
   
%      | LPAR LISTARGS RPAR
   LISTARGS2 -> type = tkn_LIST;
   return LISTARGS2;
%      ;


% LISTARGS :
   token l = tkn_LISTARGS;
   l. value. push_back( list( "nil" ) );
   return l;

%          | LIST LISTARGS
   LIST1 -> type = tkn_LISTARGS;
   list head = LIST1 -> value. front( );
   LIST1 -> value. pop_front( );
   LIST1 -> value. push_back ( list( head, LISTARGS2 -> value. front( ) ) );
   return LIST1;
%          ;
