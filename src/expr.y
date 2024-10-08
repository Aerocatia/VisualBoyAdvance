%{
#include <cstdio>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
 
#include "System.h"
#include "elf.h"
#include "exprNode.h" 

extern int yyerror(char *);
extern int yylex(); 
extern char *yytext;

  
//#define YYERROR_VERBOSE 1
//#define YYDEBUG 1

 Node *result = NULL;
%}

%token TOKEN_IDENTIFIER TOKEN_DOT TOKEN_STAR TOKEN_ARROW TOKEN_ADDR
%token TOKEN_SIZEOF TOKEN_NUMBER
%left TOKEN_DOT TOKEN_ARROW '['
%expect 6
%%

final: expression { result = $1; }
;

expression:
  simple_expression { $$ = $1; } |
  '(' expression ')' { $$ = $2; } |
  expression TOKEN_DOT ident { $$ = exprNodeDot($1, $3); } |
  expression TOKEN_ARROW ident { $$ = exprNodeArrow($1, $3); } |
  expression '[' number ']' { $$ = exprNodeArray($1, $3); }
;
simple_expression:
  ident  { $$ = $1; } |
  TOKEN_STAR expression { $$ = exprNodeStar($2); } |
  TOKEN_ADDR expression { $$ = exprNodeAddr($2); } |
  TOKEN_SIZEOF '(' expression ')' { $$ = exprNodeSizeof($3); }
;

number:
  TOKEN_NUMBER { $$ = exprNodeNumber(); }
;

ident:
  TOKEN_IDENTIFIER {$$ = exprNodeIdentifier(); }
;

%%

int yyerror(char *s)
{
  return 0;
}

#ifndef SDL
extern FILE *yyin;
int main(int argc, char **argv)
{
  //  yydebug = 1;
  ++argv, --argc;
  if(argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;
  if(!yyparse())
    result->print();
}
#endif
