

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

%startsymbol Session EOF 

%attribute value         double
%attribute id            std::string
%attribute reason        std::string

%constraint IDENTIFIER  id 1 2
%constraint E value 0 2
%constraint F value 0 2
%constraint G value 0 2
%constraint H value 0 2
%constraint SCANERROR reason 1 2
%constraint NUMBER value 1 2
%constraint LISTARGS value 0
   // You may write the others.

%global memory varstore
   // Contains stored variables.

#include "varstore.h"
#include "assert.h"
#include <math.h>


% Start : Session EOF
   std::cout << "bye bye\n";
%   ;

% Session : Session Command 
%         |
%         ;


% Command : E SEMICOLON

   if( E1 -> value. size( ))
      std::cout << " Result:  " << E1 -> value. front( ) << "\n";
   else
      std::cout << " Result is undefined\n";

%         | IDENTIFIER BECOMES E SEMICOLON

   if( E3 -> value. size( ))
   {
      std::cout << " Assigning: " << IDENTIFIER1 -> id. front( ) << " := ";
      std::cout << E3 -> value. front( ) << "\n";
      memory. assign( IDENTIFIER1 -> id. front( ), E3 -> value. front( ));
   }
   else
      std::cout << " Result is undefined\n";

%         | _recover SEMICOLON

   std::cout << "recovered from error\n\n";

%         ;

% E   : E PLUS F 

   // If both E1 and F3 are defined, then compute result:

   if( E1 -> value. size( ) && F3 -> value. size( ))
      E1 -> value. front( ) += F3 -> value. front( );
   else
      E1 -> value. clear( );
   return E1;

%     | E MINUS F 

   if( E1 -> value. size( ) && F3 -> value. size( ))
      E1 -> value. front( ) -= F3 -> value. front( );
   else
      E1 -> value. clear( );
   return E1;

%     | F 

   // Change F into E, don't touch attribute.

   F1 -> type = tkn_E;
   return F1;

%     ;

% F   : F TIMES G 
   if( F1 -> value. size( ) && G3 -> value. size( ))
      F1 -> value. front( ) *= G3 -> value. front( );
   else
      F1 -> value. clear( );
   return F1;

%     | F DIVIDES G

   if( F1 -> value. size( ) && G3 -> value. size( ))
   {
      if( G3 -> value. front( ) != 0.0 )
         F1 -> value. front( ) /= G3 -> value. front( );
      else
      {
         std::cout << "division by zero!\n";
         F1 -> value. clear( );
      }
   }
   else
      F1 -> value. clear( );
 
   return F1;
 
%     | G 
   G1 -> type = tkn_F;
   return G1;

%     ;


%  G : MINUS G
  if( G2 -> value. size( ) ) {
      G2 -> value. front( ) *= -1;
  } else G2 -> value.clear();
  return G2;
%    | PLUS G 
  if( ! G2 -> value. size( ) ) 
    G2 -> value.clear();
  return G2;

%    | H 

   H1 -> type = tkn_G;
   return H1;

%    ;


% H   : H FACTORIAL

   if( ! H1 -> value. size( ))
      return H1;

   if( H1 -> value. front( ) >= 0 &&
       floor( H1 -> value. front( )) == H1 -> value. front( ))
   {
      unsigned int f = static_cast< unsigned int >
         ( floor( H1 -> value. front( ) + 0.1 ));

      double res = 1.0;
      while( f != 0 )
      {
         res *= f;
         -- f;
      }
   
      H1 -> value. front( ) = res;
   }
   else
   {
      std::cout << "cannot compute " << H1 -> value. front( ) << " !\n\n";
      H1 -> value. clear( );
   }

   return H1;
 
%     | LPAR E RPAR 
 
   E2 -> type = tkn_H;
   return E2;

%     | IDENTIFIER 

   token h = tkn_H;
   if( memory. contains( IDENTIFIER1 -> id. front( )))
   {
      h. value. push_back( memory. lookup( IDENTIFIER1 -> id. front( )));
      return h; 
   }
   else
   {
      std::cout << "variable " << IDENTIFIER1 -> id. front( );
      std::cout << " has no value!\n";
      return tkn_H;
   }

%     | NUMBER 

   NUMBER1 -> type = tkn_H;
   return NUMBER1;

%     | IDENTIFIER LPAR LISTARGS RPAR 

   token h = tkn_H;
   if( IDENTIFIER1 -> id. front( ) == "sin" &&
       LISTARGS3 -> value. size( ) == 1 ) 
   {
      h. value. push_back( sin( LISTARGS3 -> value. front( )));
      return h;
   }

   if( IDENTIFIER1 -> id. front( ) == "exp" &&
       LISTARGS3 -> value. size( ) == 1 ) 
   {
      h. value. push_back( exp( LISTARGS3 -> value. front( )));
      return h;
   }

   if( IDENTIFIER1 -> id. front( ) == "hello_world" &&
       LISTARGS3 -> value. size( ) == 0 ) 
   {
      std::cout << "Hello, World!!!" << std::endl;
      return h;
   }

   if( IDENTIFIER1 -> id. front( ) == "avg" &&
       LISTARGS3 -> value. size( ) > 0 ) 
   { 
    double avg = 0;
    for (auto i = LISTARGS3 -> value.begin(); i != LISTARGS3 -> value.end(); i++) {
      avg += (*i);
    }
    avg /= LISTARGS3 -> value. size( );
    h. value. push_back( avg );
    return h;
   }

   if( IDENTIFIER1 -> id. front( ) == "pow" &&
       LISTARGS3 -> value. size( ) == 2 )
   {
      h. value. push_back( pow( LISTARGS3 -> value. front( ),
                                LISTARGS3 -> value. back( )));
      return h;
   }

   std::cout << "unrecognized function!\n";
   return h;
      // With empty attribute.

%     ;


% LISTARGS : 
%          | E 
             E1 -> type = tkn_LISTARGS;
             return E1;
%          | LISTARGS COMMA E
             LISTARGS1 -> value.push_back(E3 -> value.front());
             return LISTARGS1;
%          ;


