#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include "trunits.h"
#define ASSERT(X) { assert( (X) ); }

#define DEBUGMODE_OFF

//terminals
%token tAnd
%token tAssign
%token tBool
%token tComma
%token tDivision
%token tDo
%token tDouble
%token tElse
%token tEqual
%token tFalse
%token tIdentifier
%token tIf
%token tGreater
%token tGreaterEqual
%token tLeftBrace
%token tLeftPar
%token tLess
%token tLessEqual
%token tMinus
%token tMultiplication
%token tNot
%token tNotEqual
%token tNumber
%token tOr
%token tPlus
%token tReturn
%token tRightBrace
%token tRightPar
%token tSemicolon
%token tString
%token tTrue
%token tVoid
%token tWhile

//nonterminals
%token Start
%token TranslationUnits
%token FunctionDefinition
%token ReturnType_FunctionIdentifier
%token Type
%token FormalArguments
%token FunctionBody
%token LocalDefinitions
%token InstructionsBlock
%token Instruction
%token Assignment
%token FunctionCall
%token FunctionArguments
%token ArithmeticalExpression
%token Addend
%token Factor
%token ReturnInstruction
%token IfElseStatement
%token IfElseCondition
%token LogicalExpression
%token Conjuction
%token LogicalValue
%token Comparison
%token LoopStatement
%token LoopStatementCondition

%attribute tr std::string
%attribute trlen int
%attribute tp std::string

%constraint Start tr 0 1
%constraint Start trlen 0 1
%constraint Start tp 0 1
%constraint TranslationUnits tr 1
%constraint TranslationUnits trlen 1
%constraint TranslationUnits tp 0 1
%constraint FunctionDefinition tr 1 2
%constraint FunctionDefinition trlen 1 2
%constraint FunctionDefinition tp 0 1
%constraint ReturnType_FunctionIdentifier tr 1 2
%constraint ReturnType_FunctionIdentifier trlen 0 1
%constraint ReturnType_FunctionIdentifier tp 0 1
%constraint Type tr 1 2
%constraint Type trlen 0 1
%constraint Type tp 0 1
%constraint FormalArguments tr 1 2
%constraint FormalArguments trlen 1 2
%constraint FormalArguments tp 0 1
%constraint FunctionBody tr 1 2
%constraint FunctionBody trlen 1 2
%constraint FunctionBody tp 0 1
%constraint LocalDefinitions tr 1 2
%constraint LocalDefinitions trlen 1 2
%constraint LocalDefinitions tp 0 1
%constraint InstructionsBlock tr 1 2
%constraint InstructionsBlock trlen 1 2
%constraint InstructionsBlock tp 0 1
%constraint Instruction tr 1 2
%constraint Instruction trlen 1 2
%constraint Instruction tp 0 1
%constraint Assignment tr 1 2
%constraint Assignment trlen 1 2
%constraint Assignment tp 0 1
%constraint FunctionCall tr 2 3
%constraint FunctionCall trlen 1 2
%constraint FunctionCall tp 1 2
%constraint FunctionArguments tr 1 2
%constraint FunctionArguments trlen 2 3
%constraint FunctionArguments tp 1 2
%constraint ArithmeticalExpression tr 1 2
%constraint ArithmeticalExpression trlen 1 2
%constraint ArithmeticalExpression tp 1 2
%constraint Addend tr 1 2
%constraint Addend trlen 1 2
%constraint Addend tp 1 2
%constraint Factor tr 1 2
%constraint Factor trlen 1 2
%constraint Factor tp 1 2
%constraint ReturnInstruction tr 1 2
%constraint ReturnInstruction trlen 1 2
%constraint ReturnInstruction tp 0 1
%constraint IfElseStatement tr 1 2
%constraint IfElseStatement trlen 1 2
%constraint IfElseStatement tp 0 1
%constraint IfElseCondition tr 1 2
%constraint IfElseCondition trlen 1 2
%constraint IfElseCondition tp 1 2
%constraint LogicalExpression tr 1 2
%constraint LogicalExpression trlen 1 2
%constraint LogicalExpression tp 1 2
%constraint Conjuction tr 1 2
%constraint Conjuction trlen 1 2
%constraint Conjuction tp 1 2
%constraint LogicalValue tr 1 2
%constraint LogicalValue trlen 1 2
%constraint LogicalValue tp 1 2
%constraint Comparison tr 1 2
%constraint Comparison trlen 1 2
%constraint Comparison tp 1 2
%constraint BracedBlock tr 1 2
%constraint BracedBlock trlen 1 2
%constraint BracedBlock tp 0 1

%constraint tIdentifier tr 1 2
%constraint tIdentifier trlen 0 1
%constraint tIdentifier tp 0 1
%constraint tNumber tr 1 2
%constraint tNumber trlen 0 1
%constraint tNumber tp 0 1
%constraint tString tr 1 2
%constraint tString trlen 0 1
%constraint tString tp 0 1

%global trunits TrUnits

%startsymbol Start tEof

% Start : TranslationUnits
%       ;

% TranslationUnits : FunctionDefinition
%                  | TranslationUnits FunctionDefinition
%                  ;

% FunctionDefinition : ReturnType_FunctionIdentifier tLeftPar FormalArguments tRightPar tLeftBrace FunctionBody tRightBrace
%                    | ReturnType_FunctionIdentifier tLeftPar tRightPar tLeftBrace FunctionBody tRightBrace
%                    ;

% ReturnType_FunctionIdentifier : Type tIdentifier
%                               ;

% Type : tDouble
%      | tBool
%      | tVoid
%      ;

% FormalArguments : Type tIdentifier
%                   | FormalArguments tComma Type tIdentifier
%                   ;

% FunctionBody : LocalDefinitions InstructionsBlock
%              | InstructionsBlock
%              ;

% LocalDefinitions : Type tIdentifier tSemicolon
%                  | LocalDefinitions Type tIdentifier tSemicolon
%                  ;

% InstructionsBlock : InstructionsBlock Instruction
%                   | Instruction
%                   ;

% Instruction : Assignment
%             | FunctionCall tSemicolon
%             | tPrint tString
%             | tPrint tIdentifier
%             | tPrint tNumber
%             | ReturnInstruction
%             | tSemicolon
%             | IfElseStatement
%             ;

% Assignment : tIdentifier tAssign ArithmeticalExpression tSemicolon
%            | tIdentifier tAssign FunctionCall tSemicolon
%            ;

% FunctionCall : tIdentifier tLeftPar FunctionArguments tRightPar
%              | tIdentifier tLeftPar tRightPar
%              ;

% FunctionArguments : ArithmeticalExpression
%                   | FunctionArguments tComma ArithmeticalExpression
%                   ;

% ArithmeticalExpression : ArithmeticalExpression tPlus Addend
%            | ArithmeticalExpression tMinus Addend
%            | Addend
%            ;

% Addend : Addend tMultiplication Factor
%        | Addend tDivision Factor
%        | Factor
%        ;

% Factor : tIdentifier
%        | tMinus tIdentifier
%        | tNumber
%        | tMinus tNumber
%        | FunctionCall
%        | tLeftPar ArithmeticalExpression tRightPar
%        | tMinus tLeftPar ArithmeticalExpression tRightPar
%        ;

% ReturnInstruction : tReturn ArithmeticalExpression tSemicolon
%                   | tReturn tSemicolon
%                   ;

% IfElseStatement : tIf tLeftPar IfElseCondition tRightPar BracedBlock tElse BracedBlock
%                 ;

% IfElseCondition : LogicalExpression
%                 ;

% LogicalExpression : LogicalExpression tOr Conjuction
%                   | Conjuction
%                   ;

% Conjuction : Conjuction tAnd LogicalValue
%            | LogicalValue
%            ;

% LogicalValue : tTrue
%              | tFalse
%              | Comparison
%              | tLeftPar LogicalExpression tRightPar
%              | tNot tLeftPar LogicalExpression tRightPar
%              ;

% Comparison : ArithmeticalExpression tEqual ArithmeticalExpression
%            | ArithmeticalExpression tNotEqual ArithmeticalExpression
%            | ArithmeticalExpression tLess ArithmeticalExpression
%            | ArithmeticalExpression tLessEqual ArithmeticalExpression
%            | ArithmeticalExpression tGreater ArithmeticalExpression
%            | ArithmeticalExpression tGreaterEqual ArithmeticalExpression
%            ;

% BracedBlock : tLeftBrace InstructionsBlock tRightBrace
%             | Instruction
%             ;

% LoopStatement : tWhile tLeftPar LoopStatementCondition tRightPar BracedBlock
%               ;

% LoopStatementCondition : LogicalExpression
%                        ;
