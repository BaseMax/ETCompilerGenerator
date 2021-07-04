%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdint.h>
  #include "kernel.h"
  #define YYERROR_VERBOSE 1
  void yyerror(data_char const *s)
  {
    fprintf(stderr, "%s\n", s);
    exit(1);
  }
  extern data_int yylex(void);
  extern data_int64 line;
  data_int state=STATE_MAIN;
%}
//%define api.value.type { expression* }
%union
{
  data_int        number;
  expression*     expression;
  expressions*    expressions;
  statements*     statements;
  statement*      statement;
}

%token<expression> TOKEN_INTEGER
%token<expression> TOKEN_FLOAT
%token<expression> TOKEN_BOOL
%token<expression> TOKEN_NULL
%token<expression> TOKEN_STRING_DOUABLE
%token<expression> TOKEN_STRING_SINGLE

%token<expression> TOKEN_IDENT

%token<number> TOKEN_OPERATOR_ADD
%token<number> TOKEN_OPERATOR_DOT
%token<number> TOKEN_OPERATOR_MINUS
%token<number> TOKEN_OPERATOR_ZARB
%token<number> TOKEN_OPERATOR_TAGH
%token<number> TOKEN_OPERATOR_POW
%token<number> TOKEN_OPERATOR_EQUAL_EQUAL_EQUAL
%token<number> TOKEN_OPERATOR_EQUAL_EQUAL
%token<number> TOKEN_OPERATOR_EQUAL
%token<number> TOKEN_OPERATOR_SHIFT_LEFT
%token<number> TOKEN_OPERATOR_SHIFT_RIGHT
%token<number> TOKEN_OPERATOR_NOT_EQUAL
%token<number> TOKEN_OPERATOR_NOT_EQUAL_EQUAL
%token<number> TOKEN_OPERATOR_EQUAL_BAGHI
%token<number> TOKEN_OPERATOR_BAGHI

%token TOKEN_OPERATOR_ADD_ADD TOKEN_OPERATOR_MINUS_MINUS 

%token TOKEN_OPERATOR_MINUS_MANFI

%token TOKEN_OPERATOR_DONOGHTE TOKEN_OPERATOR_QUESTION

%right TOKEN_OPERATOR_EQUAL

%right TOKEN_OPERATOR_QUESTION TOKEN_OPERATOR_DONOGHTE 

%left TOKEN_OPERATOR_OR

%left TOKEN_OPERATOR_AND

//%left IDENT
%nonassoc TOKEN_OPERATOR_EQUAL_EQUAL TOKEN_OPERATOR_EQUAL_EQUAL_EQUAL TOKEN_OPERATOR_NOT_EQUAL  TOKEN_OPERATOR_NOT_EQUAL_EQUAL TOKEN_OPERATOR_ARGAND



%left TOKEN_OPERATOR_BOOLING_MORE TOKEN_OPERATOR_BOOLING_EQUAL_MORE TOKEN_OPERATOR_BOOLING_LESS TOKEN_OPERATOR_BOOLING_EQUAL_LESS TOKEN_OPERATOR_SHIFT_RIGHT TOKEN_OPERATOR_SHIFT_LEFT

%left TOKEN_OPERATOR_ADD TOKEN_OPERATOR_MINUS TOKEN_OPERATOR_ADD_ADD TOKEN_OPERATOR_MINUS_MINUS

%left TOKEN_OPERATOR_ZARB TOKEN_OPERATOR_TAGH TOKEN_OPERATOR_BAGHI

%token<number> TOKEN_OPERATOR_BRACKET_OPEN
%token<number> TOKEN_OPERATOR_BRACKET_CLOSE

//%nonassoc TOKEN_OPERATOR_BRACKET_OPEN TOKEN_OPERATOR_BRACKET_CLOSE

%right UNARY

%token<number> TOKEN_OPERATOR_EQUAL_ADD
%token<number> TOKEN_OPERATOR_EQUAL_MINUS
%token<number> TOKEN_OPERATOR_EQUAL_ZARB
%token<number> TOKEN_OPERATOR_EQUAL_TAGH
%token<number> TOKEN_OPERATOR_EQUAL_POW


%token<number> TOKEN_OPERATOR_BOOLING_MORE
%token<number> TOKEN_OPERATOR_BOOLING_LESS
%token<number> TOKEN_OPERATOR_BOOLING_EQUAL_MORE
%token<number> TOKEN_OPERATOR_BOOLING_EQUAL_LESS


%token<number> TOKEN_OPERATOR_AND
%token<number> TOKEN_OPERATOR_OR
%token<number> TOKEN_OPERATOR_NOT




%token<expression> TOKEN_IMPORT
%type<expression>  import_value
  
//UI TAGS

%token TOKEN_TAG_UI_TITLE TOKEN_TAG_UI_DESCRIPTION TOKEN_TAG_UI_KEYWORDS TOKEN_TAG_UI_AUTHOR

%token TOKEN_LINE TOKEN_SPACE TOKEN_SEMICOLON

%token TOKEN_SECTION_OPEN TOKEN_SECTION_CLOSE

%token TOKEN_TAG_PRINT TOKEN_TAG_PRINTNL TOKEN_TAG_IF TOKEN_TAG_ELSE
%token TOKEN_FUNCTION


%type<statements>           statements
%type<statement>            statement
//%type<expression>         expression_may
%type<expression>           expression_single
//%type<expression>           import_expression
%type<expressions>          expression_multi
%type<expressions>          expressions
%type<expression>           term


%%
program :
  statements
  {
    program_init();
    program_statements($1);
    program_statements_delete($1);
    program_finish();
  }
;
statements :
  {
    PARSER_PRINT("statements init empty");
    $$=program_statements_create(1);
  }
  | statements close
  | statements trims_may statement
  {
    $$=program_statements_append($1,$3);
    PARSER_PRINT("statement append");
  }
;

statement :

  import_value
  {
    PARSER_PRINT("import statement");
    $$=statement_import(state,$1);
  }

  | TOKEN_FUNCTION TOKEN_IDENT TOKEN_OPERATOR_BRACKET_OPEN expression_multi TOKEN_OPERATOR_BRACKET_CLOSE TOKEN_SECTION_OPEN statements TOKEN_SECTION_CLOSE
  {
    PARSER_PRINT("function statement");
    $$=statement_function(state,$2,$4,$7);
  }



  | TOKEN_TAG_PRINT trims_may expressions spaces_may close
  {
    PARSER_PRINT("print statement");
    $$=statement_print(state,$3,0);
  }

  | TOKEN_TAG_PRINTNL trims_may expressions spaces_may close
  {
    PARSER_PRINT("printnl statement");
    $$=statement_print(state,$3,1);
  }

  | TOKEN_IDENT TOKEN_OPERATOR_EQUAL expression_single spaces_may close
  {
    PARSER_PRINT("variable set-equal statement");
    $$=statement_variable_set(state,$1,$3);  
  }
;
expression_single :
  term
  {
    PARSER_PRINT("value");
    $$=$1;
  }



  | TOKEN_OPERATOR_NOT expression_single %prec UNARY
  {
    $$=expression_operator($2,$1,NULL);
  }
  | TOKEN_OPERATOR_MINUS expression_single %prec UNARY
  {
    $$=expression_operator($2,TOKEN_OPERATOR_MINUS_MANFI,NULL);
  }


  | expression_single trims_may TOKEN_OPERATOR_DOT trims_may expression_single
  {
    PARSER_PRINT(". operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_ADD trims_may expression_single
  {
    PARSER_PRINT("+ operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_MINUS trims_may expression_single
  {
    PARSER_PRINT("- operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_ZARB trims_may expression_single
  {
    PARSER_PRINT("* operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_TAGH trims_may expression_single
  {
    PARSER_PRINT("/ operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_POW trims_may expression_single
  {
    PARSER_PRINT("^ operator");
    $$ = expression_operator($1,$3,$5);
  }

  | expression_single trims_may TOKEN_OPERATOR_EQUAL trims_may expression_single
  {
    PARSER_PRINT("== operator(bool)");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_ADD trims_may expression_single
  {
    PARSER_PRINT("+= operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_MINUS trims_may expression_single
  {
    PARSER_PRINT("-= operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_ZARB trims_may expression_single
  {
    PARSER_PRINT("*= operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_TAGH trims_may expression_single
  {
    PARSER_PRINT("/= operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_POW trims_may expression_single
  {
    PARSER_PRINT("^= operator");
    $$ = expression_operator($1,$3,$5);
  }



  | expression_single trims_may TOKEN_OPERATOR_BOOLING_MORE trims_may expression_single
  {
    PARSER_PRINT("> operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_BOOLING_LESS trims_may expression_single
  {
    PARSER_PRINT("< operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_BOOLING_EQUAL_MORE trims_may expression_single
  {
    PARSER_PRINT("=> operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_BOOLING_EQUAL_LESS trims_may expression_single
  {
    PARSER_PRINT("=< operator");
    $$ = expression_operator($1,$3,$5);
  }


  | expression_single trims_may TOKEN_OPERATOR_AND trims_may expression_single
  {
    PARSER_PRINT("and operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_OR trims_may expression_single
  {
    PARSER_PRINT("or operator");
    $$ = expression_operator($1,$3,$5);
  }

  | expression_single trims_may TOKEN_OPERATOR_SHIFT_RIGHT trims_may expression_single
  {
    PARSER_PRINT(">> operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_SHIFT_LEFT trims_may expression_single
  {
    PARSER_PRINT("<< operator");
    $$ = expression_operator($1,$3,$5);
  }



  | expression_single trims_may TOKEN_OPERATOR_NOT_EQUAL trims_may expression_single
  {
    PARSER_PRINT("!= operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_NOT_EQUAL_EQUAL trims_may expression_single
  {
    PARSER_PRINT("!== operator");
    $$ = expression_operator($1,$3,$5);
  }



  | expression_single trims_may TOKEN_OPERATOR_BAGHI trims_may expression_single
  {
    PARSER_PRINT("% operator");
    $$ = expression_operator($1,$3,$5);
  }
  | expression_single trims_may TOKEN_OPERATOR_EQUAL_BAGHI trims_may expression_single
  {
    PARSER_PRINT("%= operator");
    $$ = expression_operator($1,$3,$5);
  }
  



  | TOKEN_OPERATOR_BRACKET_OPEN trims_may expression_single trims_may TOKEN_OPERATOR_BRACKET_CLOSE
  {
    PARSER_PRINT("( expression_single )");
    //$$ = $3;
    $$ = expression_operator($3,EXPRESSION_BRACKET,NULL);
  }


;
expression_multi :
  {
    PARSER_PRINT("expressions init empty");
    $$=program_expressions_create(2);
  }
  | expression_multi trims_may TOKEN_OPERATOR_ARGAND trims_may expression_single trims_may
  {
    PARSER_PRINT("expressions append");
    $$=program_expressions_append($1,$5);
  }
;
expressions :
  expression_single
  {
    $$=program_expressions_append_single($1);
  }
  | expression_multi
  {
    $$=$1;
  }
;
/*
expression_may :
  {
    $$=value_create(EXPRESSION_BOOL,"true");
  }
  | expression_single
  {
    $$=$1;
  }
;
*/
import_value :
  TOKEN_IMPORT
term :
  TOKEN_INTEGER
  | TOKEN_FLOAT
  | TOKEN_BOOL
  | TOKEN_NULL
  | TOKEN_STRING_DOUABLE
  | TOKEN_STRING_SINGLE
  | TOKEN_IDENT
;

space :
  TOKEN_SPACE
  {
    PARSER_PRINT("space");
  }
;
trim :
  TOKEN_SPACE
  {
    PARSER_PRINT("space");
  }
  | TOKEN_LINE
  {
    PARSER_PRINT("line");
  }
;
/*
trim_may :
  | trim
;
space_may :
  | space
;
*/
trims_may :
  | trims_may trim
;
spaces_may :
  | spaces_may space
;
close :
  TOKEN_SEMICOLON
  {
    PARSER_PRINT("close with ;");
  }
  /*
  | TOKEN_LINE
  {
    PARSER_PRINT("close with line");
  }
  */
;
%%