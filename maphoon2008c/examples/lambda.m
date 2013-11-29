

// I think that this language originates from a LEGO-style
// type checker for lambda expressions.


%token VAR METAVAR EOF 
%token ARROW COLON COMMA SEMICOLON   
%token LEFTACCOL RIGHTACCOL LEFTPAR RIGHTPAR LEFTSQR RIGHTSQR 
%startsymbol Command EOF

% Command   : Lambda SEMICOLON 
%           ;         

% Lambda    : LEFTSQR Decls RIGHTSQR Lambda 
%           | LEFTACCOL Decls RIGHTACCOL Lambda
%           | Lambda1 
%           ;

% Lambda1   : Lambda2 
%           | Lambda2 ARROW Lambda1
%           ;

% Lambda2   : Lambda3
%           | Lambda2 Lambda3 
%           ;

% Lambda3   : LEFTPAR Lambda RIGHTPAR 
%           | VAR
%           | METAVAR 
%           ;

% Decls     : Names COLON Lambda
%           ;

% Names     : VAR
%           | Names COMMA VAR
%           ;



