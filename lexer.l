%{
#include <stdio.h>
#include <stdint.h>
#include "kernel.h"
#include "parser.h"
data_int64 line=1;
//"+"|"*"|"-"|"/"|"^"|"."|"==="|"++"|"--"|"+="|"-="|"*="|"/="|"=="|"="
////
char *str 	 , *curptr;
char *str_buf, *curptr_buf;
////
int cursz 	 ,  chars;
int cursz_buf,  chars_buf;
////
#define SZMUL 128
////
void str_init()
{
	str = malloc(SZMUL);
	curptr = str;
	cursz = SZMUL;
	chars = 0;
}
void str_putc(char c)
{
	*curptr++ = c;
	chars++;
	if(chars >= cursz)
	{
		str = realloc(str, cursz + SZMUL);
		curptr = str + chars;
		cursz += SZMUL;
	}
}
void str_buf_init()
{
	str_buf = malloc(SZMUL);
	curptr_buf = str_buf;
	cursz_buf = SZMUL;
	chars_buf = 0;
}
void str_buf_putc(char c)
{
	*curptr_buf++ = c;
	chars_buf++;
	if(chars_buf >= cursz_buf)
	{
		str_buf = realloc(str_buf, cursz_buf + SZMUL);
		curptr_buf = str_buf + chars_buf;
		cursz_buf += SZMUL;
	}
}
%}

DIGIT [0-9]
HEXDIGIT [0-9a-fA-F]
ALPHA [a-zA-Z]
IDENT [a-zA-Z_][a-zA-Z0-9_]*
LABEL [a-zA-Z0-9_\x80-\xff]
LABELS [a-zA-Z_\x80-\xff][a-zA-Z0-9_\x80-\xff]*

%option noyywrap
%x STRING_DOUABLE COMMENT_MULTILINE IMPORT

%%

<INITIAL>"/*" {
	BEGIN(COMMENT_MULTILINE);
}
<COMMENT_MULTILINE>"*/" {
	BEGIN(INITIAL);
}
<COMMENT_MULTILINE>. {

}
<COMMENT_MULTILINE>\n {
	line++;
}


<INITIAL>"//"[^\n]* {

}


<INITIAL>"import"|"using" {
	LEXER_PRINT("IMPORT begin");
	str_init();
	str_buf_init();
	BEGIN(IMPORT);
}
<IMPORT>[ \t\r]+ {
	LEXER_PRINT("IMPORT space,tab");
}
<IMPORT>"\n"|";" {
	str_putc('\0');
	str_buf_putc('\0');
	yylval.expression = value_create_string(EXPRESSION_STRING_DOUABLE,str,str_buf);
	LEXER_PRINTF("IMPORT close %s",str);
	BEGIN(INITIAL);
	return TOKEN_IMPORT;
}
<IMPORT>. {
	str_putc(*yytext);
	str_buf_putc(*yytext);
	LEXER_PRINTF("IMPORT %s",yytext);
}


<INITIAL>"function"|"func" {
	LEXER_PRINT("function");
	return TOKEN_FUNCTION;
}
<INITIAL>"print"|"_" {
	LEXER_PRINT("print");
	return TOKEN_TAG_PRINT;
}
<INITIAL>"if" {
	LEXER_PRINT("if");
	return TOKEN_TAG_IF;
}
<INITIAL>"else" {
	LEXER_PRINT("else");
	return TOKEN_TAG_ELSE;
}
<INITIAL>"title" {
	LEXER_PRINT("title");
	return TOKEN_TAG_UI_TITLE;
}
<INITIAL>"description"|"desc" {
	LEXER_PRINT("description");
	return TOKEN_TAG_UI_DESCRIPTION;
}
<INITIAL>"keywords"|"keywords"|"keys" {
	LEXER_PRINT("keywords");
	return TOKEN_TAG_UI_KEYWORDS;
}
<INITIAL>"author" {
	LEXER_PRINT("author");
	return TOKEN_TAG_UI_AUTHOR;
}
<INITIAL>"printnl"|"__" {
	LEXER_PRINT("printnl");
	return TOKEN_TAG_PRINTNL;
}
<INITIAL>[0-9]+\.[0-9]+ {
	LEXER_PRINTF("float %s",yytext);
	yylval.expression = value_create(EXPRESSION_FLOAT,yytext);
	return TOKEN_FLOAT;
}
<INITIAL>[0-9]+ {
	LEXER_PRINTF("int %s",yytext);
	if(strlen(yytext)>7)
	{
		yylval.expression = value_create(EXPRESSION_INT64,yytext);
	}
	else
	{
		yylval.expression = value_create(EXPRESSION_INT64,yytext);
		//yylval.expression = value_create(EXPRESSION_INT,yytext);
	}
	return TOKEN_INTEGER;
}
<INITIAL>"null"|"nil" {
	LEXER_PRINT("null");
	yylval.expression = value_create(EXPRESSION_NULL,yytext);
	return TOKEN_NULL;
}
<INITIAL>"false"|"true" {
	LEXER_PRINTF("bool %s",yytext);
	yylval.expression = value_create(EXPRESSION_BOOL,yytext);
	return TOKEN_BOOL;
}




<INITIAL>\" {
	LEXER_PRINT("STRING_DOUABLE begin");
	str_init();
	str_buf_init();
	BEGIN(STRING_DOUABLE);
}
<STRING_DOUABLE>\\n {
	LEXER_PRINTF("STRING_DOUABLE \* : %s",yytext);
	str_putc('\\');
	str_putc('n');
	str_buf_putc('\\');
	str_buf_putc('n');
}
<STRING_DOUABLE>\\t {
	LEXER_PRINTF("STRING_DOUABLE \* : %s",yytext);
	str_putc('\\');
	str_putc('t');
	str_buf_putc('\\');
	str_buf_putc('t');
}
<STRING_DOUABLE>\\\" {
	LEXER_PRINTF("STRING_DOUABLE \* : %s",yytext);
	str_putc('\\');
	str_putc('"');
	str_buf_putc('"');
}
<STRING_DOUABLE>\\\' {
	LEXER_PRINTF("STRING_DOUABLE \* : %s",yytext);
	str_putc('\\');
	str_putc('\'');
	str_buf_putc('\'');
}
<STRING_DOUABLE>\\s {
	LEXER_PRINTF("STRING_DOUABLE \* : %s",yytext);
	str_putc('\s');
	str_buf_putc(' ');
}
<STRING_DOUABLE>\\. {
	LEXER_PRINTF("STRING_DOUABLE custom \* : %c",yytext[1]);
	str_putc(yytext[1]);
	str_buf_putc(yytext[1]);
}
<STRING_DOUABLE>\" {
	LEXER_PRINT("STRING_DOUABLE close");
	str_putc('\0');
	str_buf_putc('\0');
	yylval.expression = value_create_string(EXPRESSION_STRING_DOUABLE,str,str_buf);
	BEGIN(INITIAL);
	return TOKEN_STRING_DOUABLE;
}
<STRING_DOUABLE>. {
	str_putc(*yytext);
	str_buf_putc(*yytext);
	LEXER_PRINTF("STRING_DOUABLE data_char %s",yytext);
}



<INITIAL>"{" {
	LEXER_PRINT("{");
	return TOKEN_SECTION_OPEN;
}
<INITIAL>"}" {
	LEXER_PRINT("}");
	return TOKEN_SECTION_CLOSE;
}




<INITIAL>"+=" {
	LEXER_PRINT("+=");
	yylval.number=TOKEN_OPERATOR_EQUAL_ADD;
	return TOKEN_OPERATOR_EQUAL_ADD;
}
<INITIAL>"-=" {
	LEXER_PRINT("-=");
	yylval.number=TOKEN_OPERATOR_EQUAL_MINUS;
	return TOKEN_OPERATOR_EQUAL_MINUS;
}
<INITIAL>"*=" {
	LEXER_PRINT("*=");
	yylval.number=TOKEN_OPERATOR_EQUAL_ZARB;
	return TOKEN_OPERATOR_EQUAL_ZARB;
}
<INITIAL>"/=" {
	LEXER_PRINT("/=");
	yylval.number=TOKEN_OPERATOR_EQUAL_TAGH;
	return TOKEN_OPERATOR_EQUAL_TAGH;
}
<INITIAL>"^=" {
	LEXER_PRINT("^=");
	yylval.number=TOKEN_OPERATOR_EQUAL_POW;
	return TOKEN_OPERATOR_EQUAL_POW;
}
<INITIAL>"%=" {
	LEXER_PRINT("%=");
	yylval.number=TOKEN_OPERATOR_EQUAL_BAGHI;
	return TOKEN_OPERATOR_EQUAL_BAGHI;
}


<INITIAL>">" {
	LEXER_PRINT(">");
	yylval.number=TOKEN_OPERATOR_BOOLING_MORE;
	return TOKEN_OPERATOR_BOOLING_MORE;
}
<INITIAL>"<" {
	LEXER_PRINT("<");
	yylval.number=TOKEN_OPERATOR_BOOLING_LESS;
	return TOKEN_OPERATOR_BOOLING_LESS;
}
<INITIAL>"=>" {
	LEXER_PRINT("=>");
	yylval.number=TOKEN_OPERATOR_BOOLING_EQUAL_MORE;
	return TOKEN_OPERATOR_BOOLING_EQUAL_MORE;
}
<INITIAL>"=<" {
	LEXER_PRINT("=<");
	yylval.number=TOKEN_OPERATOR_BOOLING_EQUAL_LESS;
	return TOKEN_OPERATOR_BOOLING_EQUAL_LESS;
}



<INITIAL>"and"|"&&" {
	LEXER_PRINT("and");
	yylval.number=TOKEN_OPERATOR_AND;
	return TOKEN_OPERATOR_AND;
}
<INITIAL>"or"|"||" {
	LEXER_PRINT("or");
	yylval.number=TOKEN_OPERATOR_OR;
	return TOKEN_OPERATOR_OR;
}



<INITIAL>"===" {
	LEXER_PRINT("==");
	yylval.number=TOKEN_OPERATOR_EQUAL_EQUAL_EQUAL;
	return TOKEN_OPERATOR_EQUAL;
}
<INITIAL>"==" {
	LEXER_PRINT("==");
	yylval.number=TOKEN_OPERATOR_EQUAL_EQUAL;
	return TOKEN_OPERATOR_EQUAL;
}
<INITIAL>"!=" {
	LEXER_PRINT("!=");
	yylval.number=TOKEN_OPERATOR_NOT_EQUAL;
	return TOKEN_OPERATOR_NOT_EQUAL;
}
<INITIAL>"!==" {
	LEXER_PRINT("!==");
	yylval.number=TOKEN_OPERATOR_NOT_EQUAL_EQUAL;
	return TOKEN_OPERATOR_NOT_EQUAL_EQUAL;
}
<INITIAL>"=" {
	LEXER_PRINT("=");
	yylval.number=TOKEN_OPERATOR_EQUAL;
	return TOKEN_OPERATOR_EQUAL;
}

<INITIAL>">>" {
	LEXER_PRINT(">>");
	yylval.number=TOKEN_OPERATOR_SHIFT_RIGHT;
	return TOKEN_OPERATOR_SHIFT_RIGHT;
}
<INITIAL>"<<" {
	LEXER_PRINT("<<");
	yylval.number=TOKEN_OPERATOR_SHIFT_LEFT;
	return TOKEN_OPERATOR_SHIFT_LEFT;
}


<INITIAL>"?" {
	LEXER_PRINT("?");
	return TOKEN_OPERATOR_QUESTION;
}
<INITIAL>":" {
	LEXER_PRINT(":");
	return TOKEN_OPERATOR_DONOGHTE;
}
<INITIAL>"," {
	LEXER_PRINT(",");
	return TOKEN_OPERATOR_ARGAND;
}



<INITIAL>"++" {
	LEXER_PRINT("++");
	yylval.number=TOKEN_OPERATOR_ADD_ADD;
	return TOKEN_OPERATOR_ADD_ADD;
}
<INITIAL>"+" {
	LEXER_PRINT("+");
	yylval.number=TOKEN_OPERATOR_ADD;
	return TOKEN_OPERATOR_ADD;
}
<INITIAL>"." {
	LEXER_PRINT(".");
	yylval.number=TOKEN_OPERATOR_DOT;
	return TOKEN_OPERATOR_DOT;
}
<INITIAL>"--" {
	LEXER_PRINT("--");
	yylval.number=TOKEN_OPERATOR_MINUS_MINUS;
	return TOKEN_OPERATOR_MINUS_MINUS;
}
<INITIAL>"-" {
	LEXER_PRINT("-");
	yylval.number=TOKEN_OPERATOR_MINUS;
	return TOKEN_OPERATOR_MINUS;
}
<INITIAL>"*" {
	LEXER_PRINT("*");
	yylval.number=TOKEN_OPERATOR_ZARB;
	return TOKEN_OPERATOR_ZARB;
}
<INITIAL>"/" {
	LEXER_PRINT("/");
	yylval.number=TOKEN_OPERATOR_TAGH;
	return TOKEN_OPERATOR_TAGH;
}
<INITIAL>"^" {
	LEXER_PRINT("^");
	yylval.number=TOKEN_OPERATOR_POW;
	return TOKEN_OPERATOR_POW;
}

<INITIAL>"%" {
	LEXER_PRINT("%");
	yylval.number=TOKEN_OPERATOR_BAGHI;
	return TOKEN_OPERATOR_BAGHI;
}

<INITIAL>\( {
	LEXER_PRINT("(");
	yylval.number=TOKEN_OPERATOR_BRACKET_OPEN;
	return TOKEN_OPERATOR_BRACKET_OPEN;
}
<INITIAL>\) {
	LEXER_PRINT(")");
	yylval.number=TOKEN_OPERATOR_BRACKET_CLOSE;
	return TOKEN_OPERATOR_BRACKET_CLOSE;
}

<INITIAL>"!"|"not" {
	LEXER_PRINT("not");
	yylval.number=TOKEN_OPERATOR_NOT;
	return TOKEN_OPERATOR_NOT;
}


<INITIAL>{LABELS} {
	LEXER_PRINTF("ident %s",yytext);
	yylval.expression = value_create(EXPRESSION_IDENT,yytext);
	return TOKEN_IDENT;
}


<INITIAL>[ \t\r]+ {
	LEXER_PRINT("space,tab");
	//return TOKEN_SPACE;
}
<INITIAL>[\n]+ {
	LEXER_PRINT("line(s)");
	line+=strlen(yytext);
	//return TOKEN_LINE;
}
<INITIAL>";" {
	LEXER_PRINT(";");
	return TOKEN_SEMICOLON;
}


<INITIAL>.
{
	printf("UNKNOWN TOKEN: %s\n",yytext);
}
%%