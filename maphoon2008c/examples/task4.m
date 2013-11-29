
%startsymbol Fp Eof

% Fp  : Sign Int Frac Exp 
%     ;

% Sign : 
%      | plus 
%      | minus
%      ;

% Int : digit 
%     | Int digit 
%     ;

% Frac : 
%      | dot Int
%      ;

% Exp  : 
%      | e Sign Int
%      | E Sign Int
%      ;
