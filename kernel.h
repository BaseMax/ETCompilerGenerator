#ifndef __kernel
#define __kernel

#define DEBUG_VM 				1
#define DEBUG_GEN 				1
#define DEBUG_LEXER 			1
#define DEBUG_PARSER 			1
#define DEBUG_ERROR 			1


#if DEBUG_ERROR
	#define ERROR_NOTICE(v)  	printf("[  ERROR ] ===> %s\n",v);exit(0);
	#define ERROR_NOTICEF(...)  printf("[  ERROR ] ===> ");printf(__VA_ARGS__);printf("\n");exit(0);
#else
	#define ERROR_NOTICE(v)		exit(0);
	#define ERROR_NOTICEF(...)	exit(0);
#endif


#if DEBUG_VM
	#define VM_PRINT(v)  		printf("[   VM   ] ===> %s\n",v);
	#define VM_PRINTF(...)  	printf("[   VM   ] ===> ");printf(__VA_ARGS__);printf("\n");
#else
	#define VM_PRINT(v)
	#define VM_PRINTF(...)
#endif

#if DEBUG_GEN
	#define GEN_PRINT(v)  		printf("[   GEN  ] ===> %s\n",v);
	#define GEN_PRINTF(...)  	printf("[   GEN  ] ===> ");printf(__VA_ARGS__);printf("\n");
#else
	#define GEN_PRINT(v)
	#define GEN_PRINTF(...)
#endif

#if DEBUG_LEXER
	#define LEXER_PRINT(v)		printf("[  LEXR  ] ===> %s\n",v);
	#define LEXER_PRINTF(...)	printf("[  LEXR  ] ===> ");printf(__VA_ARGS__);printf("\n");
#else
	#define LEXER_PRINT(v)
	#define LEXER_PRINTF(...)
#endif

#if DEBUG_PARSER
	#define PARSER_PRINT(v)		printf("[ PARSER ] ===> %s\n",v);
	#define PARSER_PRINTF(...)  printf("[ PARSER ] ===> ");printf(__VA_ARGS__);printf("\n");
#else
	#define PARSER_PRINT(v)
	#define PARSER_PRINTF(...)
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {FALSE,TRUE} 							  data_bool;
typedef char*                                   	  data_string;
typedef char                                     	  data_char;
typedef double                                        data_float;
typedef long double                                   data_float64;
typedef int                                           data_int;
typedef int64_t                                    	  data_int64;
//typedef uint64_t                                      data_uint64;
typedef size_t                                     	  data_size;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {EXPRESSION_BRACKET,EXPRESSION_NEEDRUNTIME,EXPRESSION_IDENT,EXPRESSION_IDENT_EVAL,EXPRESSION_OPERATOR_DO,EXPRESSION_OPERATOR,EXPRESSION_NULL,EXPRESSION_BOOL,EXPRESSION_INT,EXPRESSION_INT64,EXPRESSION_FLOAT,EXPRESSION_FLOAT64,EXPRESSION_STRING_DOUABLE,EXPRESSION_STRING_SINGLE,EXPRESSION_CHAR} 	expression_types;
//typedef enum {OPERATION_ADD} 	operation_types;
typedef enum   {STATEMENT_VARIABLE_SET,STATEMENT_PRINT,STATEMENT_PRINTNL,STATEMENT_UI_TITLE,STATEMENT_UI_DESCRIPTION,STATEMENT_UI_KEYWORDS,STATEMENT_UI_AUTHOR,STATEMENT_IF,STATEMENT_ELSEIF,STATEMENT_ELSE,STATEMENT_IMPORT,STATEMENT_FUNCTION} statement_types;
typedef enum   {STATE_MAIN,STATE_FUNCTION} 				state_types;
#define SOURCE_APPEND(...)  							strcat(__VA_ARGS__)
#define STATE(type) 									(type == STATE_MAIN ? source_main : source_code)

typedef struct statement statement;
typedef struct statements statements;
typedef struct variables variables;

typedef struct expression expression;
typedef struct expressions expressions;

expression*			value_create(data_int type,data_string str);
expression*			value_create_string(data_int type,data_string str,data_string buf);
expression* 		expression_operator(expression* v1,data_int op,expression* v2);
expression* 		expression_operator2(expression* v1,expression* v2,int op1,int op2);
void 				program_statements_delete(statements* pArrayList);
statements* 		program_statements_append(statements* pArrayList,statement* array);
statements* 		program_statements_create(data_size block_size);
///
void 				program_expressions_delete(expressions* pArrayList);
expressions* 		program_expressions_append(expressions* pArrayList,expression* array);
expressions* 		program_expressions_append_single(expression* array);
expressions* 		program_expressions_create(data_size block_size);
///

///////


void 				program_init();
void 				program_finish();
void 				program_statement(statement* statement);
void 				program_statements(statements* statements);
statement* 			statement_print(state_types type,expressions* v,data_int newline);
statement* 			statement_import(state_types type,expression* v);
statement* 			statement_function(state_types state,expression* name,expressions* args,statements* stmts);
statement* 			statement_variable_set(state_types state,expression* n,expression* v);
statement* 			statement_ui_title(state_types state,expression* v);
statement*			statement_if(state_types state,expression* v,statements* stmts);

void program_prints(state_types state,expressions* v);
void program_print(state_types state,expression* v);

#define EXPRESSION_VALs(v)  												\
	v->type == EXPRESSION_INT ? v->vint : (								\
	    v->type == EXPRESSION_INT64 ? v->vint64 : (						\
		    v->type == EXPRESSION_FLOAT ? v->vfloat : (					\
			    /*v->type == EXPRESSION_FLOAT64 ? */v->vfloat64		 	\ 
			)															\
		)																\
	)																	\

///


#define EXPRESSION_VAL(v) v->type==EXPRESSION_INT64 ? v->vint64 : v->vfloat64
#define EXPRESSION_VAL_INT(v) 		v->vint
#define EXPRESSION_VAL_INT64(v) 	v->vint64
#define EXPRESSION_VAL_FLOAT(v) 	v->vfloat
#define EXPRESSION_VAL_FLOAT64(v) 	v->vfloat64
#define EXPRESSION_VAL2(v) vint64

//void 			evaluator_operator(expression* v);
void 			evaluator_operator(state_types state,expression* v);

data_string 	data_int64_to_string(data_int64 num);
data_string 	data_float64_to_string(data_float64 num);

data_int64 		data_string_to_int64(const data_string s);
data_float64 	data_string_to_float64(const data_string s);

#endif
