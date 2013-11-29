

// Language with tricky look aheads:


%startsymbol S EOF

% S : c b c a
%   | A a A b
%   ;

% A : B 
%   ;

% B : c
%   ;


