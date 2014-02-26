
%token EOF SCANERROR
%token COLON SEMICOLON COMMA
%token IDENTIFIER NUMBER 
%token PLUS TIMES MINUS DIVIDES
%token EQUAL NOTEQUAL LESSTHAN GREATERTHAN LESSEQUALTHAN GREATEREQUALTHAN
%token AND OR NOT 
%token LPAR RPAR
%token LSQPAR RSQPAR
%token LCURLY RCURLY
%token ADDRESSOF DOT ARROW
%token PLUSPLUS MINUSMINUS
%token QUESTIONMARK
%token ASSIGN
%token IF THEN ELSE WHILE 
%token RETURN

// Non-terminal symbols:

%token B1 B2 B3 B4 
%token A1
%token N1 N2 N3 
%token E1 E2 
%token FUNCARGS 

%startsymbol B1 SEMICOLON

%attribute ct            ctree
%attribute id            identifier 
%attribute reason        std::string

%constraint B1 ct 1 2
%constraint B2 ct 1 2 
%constraint B3 ct 1 2 
%constraint B4 ct 1 2
%constraint A1 ct 1 2
%constraint N1 ct 1 2
%constraint N2 ct 1 2
%constraint N3 ct 1 2
%constraint E1 ct 1 2
%constraint E2 ct 1 2
%constraint IDENTIFIER ct 1 2
%constraint NUMBER ct 1 2
%constraint FUNCARGS ct 0
%constraint SCANERROR reason 0 2 

%intokenheader #include "identifier.h"
%intokenheader #include "ctree.h" 
%intokenheader #include "assert.h"


% B1 : B2 

        B21 -> type = tkn_B1;
        return B21;
 
%    | B1 OR B2	

        token t = tkn_B1;
        t. ct. push_back( ctree( identifier( "infix||" ),
                             { B11 -> ct. front( ), 
                               B23 -> ct. front( ) } ));
        return t;

%         ;

%  B2  : B3 
   
        B31 -> type = tkn_B2;
        return B31;

%      | B2 AND B3 

          token t = tkn_B2;
          t. ct. push_back( ctree( identifier( "infix&&" ),
                              { B21 -> ct. front( ),
                                B33 -> ct. front( ) } ));
          return t; 

%      ; 

% B3   : B4 

          B41 -> type = tkn_B3;
          return B41;

%      | B4 LESSTHAN B4

          token t = tkn_B3;
          t. ct. push_back( ctree( identifier( "infix<" ), 
                               { B41 -> ct. front( ),
                                 B43 -> ct. front( ) } ));
          return t; 

%      | B4 LESSEQUALTHAN B4

          token t = tkn_B3;
          t. ct. push_back( ctree( identifier( "infix<=" ), 
                                { B41 -> ct. front( ),
                                  B43 -> ct. front( ) } ));
          return t; 

%      | B4 GREATERTHAN B4 

          token t = tkn_B3;
          t. ct. push_back( ctree( identifier( "infix>" ), 
                               { B41 -> ct. front( ),
                                 B43 -> ct. front( ) } ));
          return t; 

%      | B4 GREATEREQUALTHAN B4 

          token t = tkn_B3;
          t. ct. push_back( ctree( identifier( "infix>=" ),
                                { B41 -> ct. front( ), 
                                  B43 -> ct. front( ) } ));
          return t; 

%      | B4 EQUAL B4

          token t = tkn_B3;
          t. ct. push_back( ctree( identifier( "infix==" ), 
                               { B41 -> ct. front( ),
                                 B43 -> ct. front( ) } ));
          return t; 

%      | B4 NOTEQUAL B4

          token t = tkn_B4;
          t. ct. push_back( ctree( identifier( "infix!=" ), 
                               { B41 -> ct. front( ),
                                 B43 -> ct. front( ) } ));
          return t; 

%      ;  

% B4   : A1 

       A11 -> type = tkn_B4;
       return A11;

%      | NOT B4

       token t = tkn_B4;
       t. ct. push_back( ctree( identifier( "infix!" ), 
                            { B42 -> ct. front( ) } ));  
       return t; 

%      ;

% A1   : N1 

       N11 -> type = tkn_A1;
       return N11;

%      | N1 ASSIGN N1

       token t = tkn_A1;
       t. ct. push_back( ctree( identifier( "infix=" ),
                           { N11 -> ct. front( ),
                             N13 -> ct. front( ) } ));
       return t;

%      ;

% N1   : N2 

       N21 -> type = tkn_N1;
       return N21;

%      | N1 PLUS N2 

       token t = tkn_N1;
       t. ct. push_back( ctree( identifier( "infix+" ), 
                                { N11 -> ct. front( ),
                                  N23 -> ct. front( ) } ));
       return t;

%      | N1 MINUS N2

       token t = tkn_N1;
       t. ct. push_back( ctree( identifier( "infix-" ), 
                                { N11 -> ct. front( ),
                                  N23 -> ct. front( ) } ));
       return t;

%      ; 

% N2   : N3

       N31 -> type = tkn_N2;
       return N31;

%      | N2 TIMES N3

       token t = tkn_N2;
       t. ct. push_back( ctree( identifier( "infix*" ), 
                             { N21 -> ct. front( ), N33 -> ct. front( ) } ));
       return t;

%      | N2 DIVIDES N3

       token t = tkn_N2;
       t. ct. push_back( ctree( identifier( "infix/" ), 
                             { N21 -> ct. front( ), N33 -> ct. front( ) } ));
       return t;

%      ;

% N3   : E1 

       E11 -> type = tkn_N3;
       return E11;

%      | MINUS N3

       token t = tkn_N3;
       t. ct. push_back( ctree( identifier( "prefix-" ), 
                                { N32 -> ct. front( ) } ));
       return t;

%      ;

%  E1  : E2 
    
        E21 -> type = tkn_E1;
        return E21;

%      | ADDRESSOF E1

       token t = tkn_E1;
       t. ct. push_back( ctree( identifier( "prefix&" ), 
                                { E12 -> ct. front( ) } ));
       return t;

%      | TIMES E1 
       
       token t = tkn_E1;
       t. ct. push_back( ctree( identifier( "prefix*" ), 
                                { E12 -> ct. front( ) } ));
       return t;

%      | PLUSPLUS E1

       token t = tkn_E1;
       t. ct. push_back( ctree( identifier( "prefix++" ), 
                                { E12 -> ct. front( ) } ));
       return t;

%      | MINUSMINUS E1

       token t = tkn_E1; 
       t. ct. push_back( ctree( identifier( "prefix--" ), 
                                { E12 -> ct. front( ) } ));
       return t;

%      ;

% E2   : E2 DOT IDENTIFIER
 
       token t = tkn_E2;
       const ctree& ct = IDENTIFIER3 -> ct. front( );
       ASSERT( ct. gettreetype( ) == treetype::var );
 
       t. ct. push_back( 
                 ctree( 
                    identifier( { "fieldfunction" } ) + ct. getvarname( ),
                    { E21 -> ct. front( ) } ));
       return t;

%      | E2 ARROW IDENTIFIER

       // It is an abbreviation for (*e). id 

       token t = tkn_E2;
       const ctree& ct = IDENTIFIER3 -> ct. front( ); 
       ASSERT( ct. gettreetype( ) == treetype::var );

       t. ct. push_back( 
                 ctree( 
                    identifier( { "fieldfunction" } ) + ct. getvarname( ),
                     { ctree( identifier( "prefix*" ), 
                              { E21 -> ct. front( ) } ) } ));
       return t;

%      | E2 LSQPAR B1 RSQPAR 

       // It is an abbreviation for *( E2 + B1 )

       token t = tkn_E2;
       t. ct. push_back( ctree( identifier( "prefix*" ), 
                            { ctree( identifier( "infix+" ), 
                                     { E21 -> ct. front( ),
                                       B13 -> ct. front( ) } ) } ));
       return t; 

%      | E2 PLUSPLUS
      
       token t = tkn_E2;  
       t. ct. push_back( ctree( identifier( "postfix++" ), 
                                { E21 -> ct. front( ) } ));
       return t;

%      | E2 MINUSMINUS

       token t = tkn_E2;
       t. ct. push_back( ctree( identifier( "postfix--" ), 
                                { E21 -> ct. front( ) } ));
       return t;

%      | NUMBER 

       NUMBER1 -> type = tkn_E2; 
       return NUMBER1;

%      | IDENTIFIER

       IDENTIFIER1 -> type = tkn_E2;
       return IDENTIFIER1; 

%      | LPAR B1 RPAR

       B12 -> type = tkn_E2;
       return B12;

%      | IDENTIFIER LPAR FUNCARGS RPAR 

       const ctree& id = IDENTIFIER1 -> ct. front( );
       ASSERT( id. gettreetype( ) == treetype::var );
       std::vector< ctree > args;
       for( auto p = FUNCARGS3 -> ct. begin( ); 
                 p != FUNCARGS3 -> ct. end( ); 
                 ++ p )
       {
          args. push_back( *p );
       }

       token t = tkn_E2;
       t. ct. push_back( ctree( id. getvarname( ), args ));
       return t;

%      ;

% FUNCARGS : B1 

       B11 -> type = tkn_FUNCARGS;
       return B11;

%          | FUNCARGS COMMA B1

       FUNCARGS1 -> ct. push_back( B13 -> ct. front( ));
       return FUNCARGS1;

%          ;


