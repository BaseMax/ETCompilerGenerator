#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "kernel.h"
#include "parser.h"
extern data_int64 line;
///
typedef struct expression
{
	expression_types 	type;
	//////////////////////////////////
	data_int 			vint;
	data_int64			vint64;
	data_float 			vfloat;
	data_float64		vfloat64;
	data_int 			vbool;
	data_string 		vstring;
	data_char 			vchar;
	data_string 		value;
	data_string 		input;
	//////////////////////////////////
	data_int			voperator;
	expression*			vleft;
	expression*			vright;
	//////////////////////////////////
	data_int64 			length;
} expression;
///
struct statement
{
	state_types 		state;
	statement_types 	type;
	////////////////////////////
	expression*			value1;
	expression*			value2;
	////////////////////////////
	expressions*		values1;
	expressions*		values2;
	////////////////////////////
	statement*			stmt;
	statements*			stmts;
};
struct statements
{
	statement**			arrays;
	data_size 			nArrays;
	data_size 			size;
	data_size 			block_size;
};
struct variables
{
	data_string**			arrays;
	data_size 			nArrays;
	data_size 			size;
	data_size 			block_size;
};
struct expressions
{
	expression**		arrays;
	data_size 			nArrays;
	data_size 			size;
	data_size 			block_size;
};
///
data_string 	source;
data_string		source_main;
data_string		source_library;
data_string 	source_code;
data_int 		lib_stdio=1;
data_int 		lib_stdlib;
data_int 		lib_string;
data_int 		lib_ctype;
data_int 		lib_inttypes;
data_int 		lib_stdint;
data_int 		lib_assert;
///
data_int 		func_print_null;
data_int 		func_print_bool;
data_int 		func_print_int;
data_int 		func_print_int64;
data_int 		func_print_float;
data_int 		func_print_float64;
data_int 		func_print_char;
data_int 		func_print_string;
data_int 		func_print_data;
data_int 		func_print_inline_data;
data_int 		func_print_operator;
data_int 		func_print_operator_do;
///
data_int 		class_data;
data_int 		class_define;
data_int 		class_operator;
data_int 		class_expression_type;
/*
data_float64 data_string_to_float64(const data_string s)
{
	return (data_float64)atolf(s);
}
*/
data_int64 temp_variable_add_id=0;
data_string temp_variable_add_id_str;
//temp_variable_add_id_str=malloc(50000);
///
#define EVALUATOR_OPERATOR(ooooo,sssss)																\
if(tl == EXPRESSION_IDENT || tr == EXPRESSION_IDENT || tl == EXPRESSION_IDENT_EVAL || tr == EXPRESSION_IDENT_EVAL)\
{\
	v->value=malloc(50000*sizeof(char *));\
	strcpy(v->value,"");\
	strcat(v->value,"evaluator_operator(");\
	if(tl == EXPRESSION_IDENT)\
	{\
		strcat(v->value,"variable_user_");\
		strcat(v->value,state_type(state));\
		strcat(v->value,"_");\
		strcat(v->value,v->vleft->value);\
	}\
	else if(tl == EXPRESSION_IDENT_EVAL)\
	{\
		strcat(v->value,v->vleft->value);\
	}\
	else\
	{\
		temp_variable_add();\
		SOURCE_APPEND(STATE(state),"data variable_temp_");\
		SOURCE_APPEND(STATE(state),state_type(state));\
		SOURCE_APPEND(STATE(state),"_");\
		SOURCE_APPEND(STATE(state),temp_variable_add_id_str);\
		if(strcmp(expression_type(tl),"null") == 0){}else\
		{\
			SOURCE_APPEND(STATE(state),"={");\
			SOURCE_APPEND(STATE(state),strtoupper(runtime_expression_type(tl)));\
			SOURCE_APPEND(STATE(state),"};");\
		}\
		SOURCE_APPEND(STATE(state),"\n");\
		if(strcmp(expression_type(tl),"null") == 0){}else\
		{\
			SOURCE_APPEND(STATE(state),"variable_temp_");\
			SOURCE_APPEND(STATE(state),state_type(state));\
			SOURCE_APPEND(STATE(state),"_");\
			SOURCE_APPEND(STATE(state),temp_variable_add_id_str);\
			SOURCE_APPEND(STATE(state),".value.");\
			SOURCE_APPEND(STATE(state),runtime_expression_type(tl));\
			SOURCE_APPEND(STATE(state),"=");\
			SOURCE_APPEND(STATE(state),v->vleft->value);\
			SOURCE_APPEND(STATE(state),";\n");\
		}\
		v->vleft->value=temp_variable_add_id_str;\
		strcat(v->value,"variable_temp_");\
		strcat(v->value,state_type(state));\
		strcat(v->value,"_");\
		strcat(v->value,temp_variable_add_id_str);\
	}\
	strcat(v->value,",");\
	strcat(v->value,data_int64_to_string(v->voperator));\
	strcat(v->value,",");\
	if(tr == EXPRESSION_IDENT)\
	{\
		strcat(v->value,"variable_user_");\
		strcat(v->value,state_type(state));\
		strcat(v->value,"_");\
		strcat(v->value,v->vright->value);\
	}\
	else if(tr == EXPRESSION_IDENT_EVAL)\
	{\
		strcat(v->value,v->vright->value);\
	}\
	else\
	{\
		temp_variable_add();\
		SOURCE_APPEND(STATE(state),"data variable_temp_");\
		SOURCE_APPEND(STATE(state),state_type(state));\
		SOURCE_APPEND(STATE(state),"_");\
		SOURCE_APPEND(STATE(state),temp_variable_add_id_str);\
		if(strcmp(expression_type(tr),"null") == 0){}else\
		{\
			SOURCE_APPEND(STATE(state),"={");\
			SOURCE_APPEND(STATE(state),strtoupper(runtime_expression_type(tr)));\
			SOURCE_APPEND(STATE(state),"};");\
		}\
		SOURCE_APPEND(STATE(state),"\n");\
		if(strcmp(expression_type(tr),"null") == 0){}else\
		{\
			SOURCE_APPEND(STATE(state),"variable_temp_");\
			SOURCE_APPEND(STATE(state),state_type(state));\
			SOURCE_APPEND(STATE(state),"_");\
			SOURCE_APPEND(STATE(state),temp_variable_add_id_str);\
			SOURCE_APPEND(STATE(state),".value.");\
			SOURCE_APPEND(STATE(state),runtime_expression_type(tr));\
			SOURCE_APPEND(STATE(state),"=");\
			SOURCE_APPEND(STATE(state),v->vright->value);\
			SOURCE_APPEND(STATE(state),";\n");\
		}\
		/*											\
		v->vright->value=temp_variable_add_id_str;\
		*/											\
		strcat(v->value,"variable_temp_");\
		strcat(v->value,state_type(state));\
		strcat(v->value,"_");\
		strcat(v->value,temp_variable_add_id_str);\
	}\
	strcat(v->value,")");\
	v->type=EXPRESSION_IDENT;\
	v->type=EXPRESSION_IDENT_EVAL;\
}\
/*\
int+int\
*/\
/*\
int+int64\
*/\
/*\
int+float\
*/\
/*\
int+float64\
*/\
/*\
int+bool\
*/\
/*\
int+null\
*/\
/*\
int+string\
*/\
/*\
int+char\
*/\
/*\
int64+int\
*/\
/*\
int64+int64\
*/\
else if(tl == EXPRESSION_INT64 && tr == EXPRESSION_INT64)\
{\
	v->vint64=v->vleft->vint64 ooooo v->vright->vint64;\
	v->type=EXPRESSION_INT64;\
}\
/*\
int64+float\
*/\
else if(tl == EXPRESSION_INT64 && tr == EXPRESSION_FLOAT)\
{\
	v->type=EXPRESSION_FLOAT;\
	v->vfloat=v->vleft->vint64;\
	(v->vfloat) ooooo##=v->vright->vfloat;\
}\
/*\
int64+float64\
*/\
/*\
int64+bool\
*/\
else if(tl == EXPRESSION_INT64 && tr == EXPRESSION_BOOL)\
{\
	v->type=v->vleft->type;\
	if(v->vright->vbool == TRUE)\
	{\
		v->vint64=v->vleft->vint64 ooooo 1;\
	}\
	else\
	{\
		v->vint64=v->vleft->vint64;\
	}\
}\
/*
int64+null\
*/\
else if(tl == EXPRESSION_INT64 && tr == EXPRESSION_NULL)\
{\
	v->type=v->vleft->type;\
	v->vint64=v->vleft->vint64;\
}\
/*
int64+string\
*/\
/*
int64+char\
*/\
/*
float+int\
*/\
/*
float+int64\
*/\
else if(tl == EXPRESSION_FLOAT && tr == EXPRESSION_INT64)\
{\
	v->type=EXPRESSION_FLOAT;\
	v->vfloat=v->vleft->vfloat;\
	(v->vfloat) ooooo##=v->vright->vint64;\
}\
/*
float+float\
*/\
else if(tl == EXPRESSION_FLOAT && tr == EXPRESSION_FLOAT)\
{\
	if((v->vleft->vfloat ooooo v->vright->vfloat) == (data_int64)(v->vleft->vfloat ooooo v->vright->vfloat))\
	{\
		v->vint64=(data_int64)(v->vleft->vfloat ooooo v->vright->vfloat);\
		v->type=EXPRESSION_INT64;\
	}\
	else\
	{\
		v->vfloat=(v->vleft->vfloat ooooo v->vright->vfloat);\
		v->type=EXPRESSION_FLOAT;\
	}\
}\
/*
float+float64\
*/\
/*
float+bool\
*/\
else if(tl == EXPRESSION_FLOAT && tr == EXPRESSION_BOOL)\
{\
	if(v->vleft->vfloat == (data_int64)v->vleft->vfloat)\
	{\
		v->type=EXPRESSION_INT64;\
		if(v->vright->vbool == TRUE)\
		{\
			v->vint64=(data_int64)v->vleft->vfloat ooooo 1;\
		}\
		else\
		{\
			v->vint64=(data_int64)v->vleft->vfloat;\
		}\
	}\
	else\
	{\
		v->type=EXPRESSION_FLOAT;\
		if(v->vright->vbool == TRUE)\
		{\
			v->vfloat=v->vleft->vfloat ooooo 1;\
		}\
		else\
		{\
			v->vfloat=v->vleft->vfloat;\
		}\
	}\
}\
/*
float+null\
*/\
else if(tl == EXPRESSION_FLOAT && tr == EXPRESSION_NULL)\
{\
	if(v->vleft->vfloat == (data_int64)v->vleft->vfloat)\
	{\
		v->type=EXPRESSION_INT64;\
		v->vint64=(data_int64)v->vleft->vfloat;\
	}\
	else\
	{\
		v->type=EXPRESSION_FLOAT;\
		v->vfloat=v->vleft->vfloat;\
	}\
}\
/*
float+string\
*/\
/*
float+char\
*/\
/*
float64+int\
*/\
/*
float64+int64\
*/\
/*
float64+float\
*/\
/*
float64+float64\
*/\
/*
float64+bool\
*/\
/*
float64+null\
*/\
/*
float64+string\
*/\
/*
float64+char\
*/\
/*
bool+int\
*/\
/*
bool+int64\
*/\
else if(tl == EXPRESSION_BOOL && tr == EXPRESSION_INT64)\
{\
	v->type=v->vright->type;\
	if(v->vleft->vbool == TRUE)\
	{\
		v->vint64=v->vright->vint64 ooooo 1;\
	}\
	else\
	{\
		v->vint64=v->vright->vint64;\
	}\
}\
/*
bool+float\
*/\
else if(tl == EXPRESSION_BOOL && tr == EXPRESSION_FLOAT)\
{\
	if(v->vright->vfloat == (data_int64)v->vright->vfloat)\
	{\
		v->type=EXPRESSION_INT64;\
		if(v->vleft->vbool == TRUE)\
		{\
			v->vint64=(data_int64)v->vright->vfloat ooooo 1;\
		}\
		else\
		{\
			v->vint64=(data_int64)v->vright->vfloat;\
		}\
	}\
	else\
	{\
		v->type=EXPRESSION_FLOAT;\
		if(v->vleft->vbool == TRUE)\
		{\
			v->vfloat=v->vright->vfloat ooooo 1;\
		}\
		else\
		{\
			v->vfloat=v->vright->vfloat;\
		}\
	}\
}\
/*
bool+float64\
*/\
/*
bool+bool\
*/\
else if(tl == EXPRESSION_BOOL && tr == EXPRESSION_BOOL)\
{\
	v->type=EXPRESSION_BOOL;\
	if(v->vleft->vbool == TRUE && v->vright->vbool == TRUE)\
	{\
		v->vbool=TRUE;\
	}\
	else if(v->vleft->vbool == TRUE && v->vright->vbool == FALSE || v->vleft->vbool == FALSE && v->vright->vbool == TRUE)\
	{\
		v->vbool=FALSE;\
	}\
	else\
	{\
		v->vbool=TRUE;\
	}\
}\
/*
bool+null\
*/\
else if(tl == EXPRESSION_BOOL && tr == EXPRESSION_NULL)\
{\
	v->type=EXPRESSION_BOOL;\
	v->vbool=v->vleft->vbool;\
}\
/*
bool+string\
*/\
/*
bool+char\
*/\
/*
null+int\
*/\
/*
null+int64\
*/\
else if(tl == EXPRESSION_NULL && tr == EXPRESSION_INT64)\
{\
	v->type=v->vright->type;\
	v->vint64=v->vright->vint64;\
}\
/*
null+float\
*/\
else if(tl == EXPRESSION_NULL && tr == EXPRESSION_FLOAT)\
{\
	if(v->vright->vfloat == (data_int64)v->vright->vfloat)\
	{\
		v->type=EXPRESSION_INT64;\
		v->vint64=(data_int64)v->vright->vfloat;\
	}\
	else\
	{\
		v->type=EXPRESSION_FLOAT;\
		v->vfloat=v->vright->vfloat;\
	}\
}\
/*
null+float64\
*/\
/*
null+bool\
*/\
else if(tl == EXPRESSION_NULL && tr == EXPRESSION_BOOL)\
{\
	v->type=EXPRESSION_BOOL;\
	v->vbool=v->vright->vbool;\
}\
/*
null+null\
*/\
else if(tl == EXPRESSION_NULL && tr == EXPRESSION_NULL)\
{\
	v->type=EXPRESSION_NULL;\
}\
/*
null+string\
*/\
/*
null+char\
*/\
/*
string+int\
*/\
/*
string+int64\
*/\
else if(tl == EXPRESSION_STRING_DOUABLE && tr == EXPRESSION_INT64)\
{\
	v->type=EXPRESSION_STRING_DOUABLE;\
	char * vr=data_convert_int64_string(v->vright->vint64);\
	v->vstring=malloc((strlen(v->vleft->vstring)+strlen(vr))*sizeof(data_string)+1);\
	strcpy(v->vstring,v->vleft->vstring);\
	strcat(v->vstring,vr);\
}\
/*
string+float\
*/\
/*
string+float64\
*/\
else if(tl == EXPRESSION_STRING_DOUABLE && tr == EXPRESSION_FLOAT64)\
{\
	v->type=EXPRESSION_STRING_DOUABLE;\
	char * vr=data_convert_float64_string(v->vright->vbool);\
	v->vstring=malloc((strlen(v->vleft->vstring)+strlen(vr))*sizeof(data_string)+1);\
	strcpy(v->vstring,v->vleft->vstring);\
	strcat(v->vstring,vr);\
}\
/*
string+bool\
*/\
else if(tl == EXPRESSION_STRING_DOUABLE && tr == EXPRESSION_BOOL)\
{\
	v->type=EXPRESSION_STRING_DOUABLE;\
	char * vr=data_convert_bool_string(v->vright->vbool);\
	v->vstring=malloc((strlen(v->vleft->vstring)+strlen(vr))*sizeof(data_string)+1);\
	strcpy(v->vstring,v->vleft->vstring);\
	strcat(v->vstring,vr);\
}\
/*
string+null\
*/\
else if(tl == EXPRESSION_STRING_DOUABLE && tr == EXPRESSION_NULL)\
{\
	v->type=EXPRESSION_STRING_DOUABLE;\
	char * vr=data_convert_null_string();\
	v->vstring=malloc((strlen(v->vleft->vstring)+strlen(vr))*sizeof(data_string)+1);\
	strcpy(v->vstring,v->vleft->vstring);\
	strcat(v->vstring,vr);\
}\
/*
string+string\
*/\
else if(tl == EXPRESSION_STRING_DOUABLE && tr == EXPRESSION_STRING_DOUABLE)\
{\
	v->type=EXPRESSION_STRING_DOUABLE;\
	v->vstring=malloc((strlen(v->vleft->vstring)+strlen(v->vright->vstring))*sizeof(data_string)+1);\
	strcpy(v->vstring,v->vleft->vstring);\
	strcat(v->vstring,v->vright->vstring);\
}\
/*
string+char\
*/\
/*
char+int\
*/\
/*
char+int64\
*/\
/*
char+float\
*/\
/*
char+float64\
*/\
/*
char+bool\
*/\
/*
char+null\
*/\
/*
char+string\
*/\
/*
char+char\
*/\
else\
{\
	ERROR_NOTICEF("can not do this operator %s %s %s",expression_type(v->vleft->type),operator_type(v->voperator),expression_type(v->vright->type));\
	return;\
}\
doing=1;\
\

///
#define GET_VALUE_EXPRESSION(value)																\
	(value->type == EXPRESSION_INT) ? value->vint : 											\
	(																							\
		(value->type == EXPRESSION_INT64) ? value->vint64 :										\
		(																						\
			(value->type == EXPRESSION_FLOAT) ? value->vfloat :									\
			(																					\
				(value->type == EXPRESSION_FLOAT64) ? value->vfloat64 :							\
				(																				\
					(value->type == EXPRESSION_BOOL) ? value->vbool :							\
					(																			\
						(value->type == EXPRESSION_STRING_DOUABLE) ? value->vstring :			\
						(																		\
							value->type == EXPRESSION_CHAR ? value->vchar : value					\
						)																		\
					)																			\
				)																				\
			)																					\
		)																						\
	)
///
void *data_convert_int_int(data_int *value)
{

}
void *data_convert_int_int64(data_int *value)
{

}
void *data_convert_int_float(data_int *value)
{

}
void *data_convert_int_float64(data_int *value)
{

}
void *data_convert_int_bool(data_int *value)
{

}
void *data_convert_int_null(data_int *value)
{

}
void * data_convert_int_string(data_int *value)
{
	data_string data = malloc(((data_int)value)*sizeof(data_string)+1);
	sprintf(data,"%d",value);
	return data;
}
void *data_convert_int_char(data_int *value)
{

}
void *data_convert_int64_int(data_int64 *value)
{

}
void *data_convert_int64_int64(data_int64 *value)
{

}
void *data_convert_int64_float(data_int64 *value)
{

}
void *data_convert_int64_float64(data_int64 *value)
{

}
void *data_convert_int64_bool(data_int64 *value)
{

}
void *data_convert_int64_null(data_int64 *value)
{

}
void *data_convert_int64_string(data_int64 *value)
{
	data_string data = malloc(((data_int)value)*sizeof(data_string)+1);
	sprintf(data,"%"PRId64,(int64_t)value);
	return data;
}
void *data_convert_int64_char(data_int64 *value)
{

}
void *data_convert_float_int(data_float *value)
{

}
void *data_convert_float_int64(data_float *value)
{

}
void *data_convert_float_float(data_float *value)
{

}
void *data_convert_float_float64(data_float *value)
{

}
void *data_convert_float_bool(data_float *value)
{

}
void *data_convert_float_null(data_float *value)
{

}
void *data_convert_float_string(data_float value)
{
	data_string data = malloc(((data_int)value)*sizeof(data_string)+1);
	sprintf(data,"%g",value);
	return data;
}
void *data_convert_float_char(data_float *value)
{

}
void *data_convert_float64_int(data_float64 *value)
{

}
void *data_convert_float64_int64(data_float64 *value)
{

}
void *data_convert_float64_float(data_float64 *value)
{

}
void *data_convert_float64_float64(data_float64 *value)
{

}
void *data_convert_float64_bool(data_float64 *value)
{

}
void *data_convert_float64_null(data_float64 *value)
{

}
void *data_convert_float64_string(data_float64 value)
{
	data_string data = malloc((5000)*sizeof(data_string)+1);
	sprintf(data,"%g",value);
	//printf("\n\n==>%f\n==>%s\n",(data_float64) value,data);
	return data;
}
void *data_convert_float64_char(data_float64 *value)
{

}
void *data_convert_bool_int(data_bool *value)
{

}
void *data_convert_bool_int64(data_bool *value)
{

}
void *data_convert_bool_float(data_bool *value)
{

}
void *data_convert_bool_float64(data_bool *value)
{

}
void *data_convert_bool_bool(data_bool *value)
{

}
void *data_convert_bool_null(data_bool *value)
{

}
void *data_convert_bool_string(data_bool *value)
{
	if(value == TRUE)
	{
		return "true";
	}
	return "false";	
}
void *data_convert_bool_char(data_bool *value)
{

}
void *data_convert_null_int()
{

}
void *data_convert_null_int64()
{

}
void *data_convert_null_float()
{

}
void *data_convert_null_float64()
{

}
void *data_convert_null_bool()
{

}
void *data_convert_null_null()
{

}
void *data_convert_null_string()
{
	return "null";
}
void *data_convert_null_char()
{

}
void *data_convert_string_int(data_string *value)
{

}
void *data_convert_string_int64(data_string *value)
{

}
void *data_convert_string_float(data_string *value)
{

}
void *data_convert_string_float64(data_string *value)
{

}
void *data_convert_string_bool(data_string *value)
{

}
void *data_convert_string_null(data_string *value)
{

}
void *data_convert_string_string(data_string *value)
{

}
void *data_convert_string_char(data_string *value)
{

}
void *data_convert_char_int(data_char *value)
{

}
void *data_convert_char_int64(data_char *value)
{

}
void *data_convert_char_float(data_char *value)
{

}
void *data_convert_char_float64(data_char *value)
{

}
void *data_convert_char_bool(data_char *value)
{

}
void *data_convert_char_null(data_char *value)
{

}
void *data_convert_char_string(data_char *value)
{

}
void *data_convert_char_char(data_char *value)
{

}
///
data_int max_offset_data_type=8;
data_int offset_data_type(int type)
{
	/*
	int
	int64
	float
	float64
	bool
	null
	string
	char
	*/
	switch(type)
	{
		case EXPRESSION_INT:
			return 1;
		break;
		case EXPRESSION_INT64:
			return 2;
		break;
		case EXPRESSION_FLOAT:
			return 3;
		break;
		case EXPRESSION_FLOAT64:
			return 4;
		break;
		case EXPRESSION_BOOL:
			return 5;
		break;
		case EXPRESSION_NULL:
			return 6;
		break;
		case EXPRESSION_STRING_DOUABLE:
			return 7;
		break;
		case EXPRESSION_CHAR:
			return 8;
		break;
		default:
			return 0;
		break;
	}
}
data_int data_convert(int type,int ntype)
{
	data_int offset=0;
	data_int offset_type=offset_data_type(type);
	data_int offset_ntype=offset_data_type(ntype);
	//offset=(offset_type-1)*(max_offset_data_type) + offset_ntype;
	offset=(offset_type-1)*(max_offset_data_type) + offset_ntype - 1;
	//offset--;
	return offset;
}
///
const static struct
{
	void* (*call)(void*);
	void* (*call_no_arg)();
}
data_convert_function[]=
{
	//0
	//{data_convert_int_int},
	//1
	{data_convert_int_int64},
	//2
	{data_convert_int_float},
	//3
	{data_convert_int_float64},
	//4
	{data_convert_int_bool},
	//5
	{data_convert_int_null},
	//6
	{data_convert_int_string},
	//7
	{data_convert_int_char},
	//8
	{data_convert_int64_int},
	//9
	//{data_convert_int64_int64},
	//10
	{data_convert_int64_float},
	//11
	{data_convert_int64_float64},
	//12
	{data_convert_int64_bool},
	//13
	{data_convert_int64_null},
	//14
	{data_convert_int64_string},
	//15
	{data_convert_int64_char},
	//16
	{data_convert_float_int},
	//17
	{data_convert_float_int64},
	//18
	//{data_convert_float_float},
	//19
	{data_convert_float_float64},
	//20
	{data_convert_float_bool},
	//21
	{data_convert_float_null},
	//22
	{data_convert_float_string},
	//23
	{data_convert_float_char},
	//24
	{data_convert_float64_int},
	//25
	{data_convert_float64_int64},
	//26
	{data_convert_float64_float},
	//27
	//{data_convert_float64_float64},
	//28
	{data_convert_float64_bool},
	//29
	{data_convert_float64_null},
	//30
	{data_convert_float64_string},
	//31
	{data_convert_float64_char},
	//32
	{data_convert_bool_int},
	//33
	{data_convert_bool_int64},
	//34
	{data_convert_bool_float},
	//35
	{data_convert_bool_float64},
	//36
	//{data_convert_bool_bool},
	//37
	{data_convert_bool_null},
	//38
	{data_convert_bool_string},
	//39
	{data_convert_bool_char},
	//40
	{data_convert_null_int},
	//41
	{data_convert_null_int64},
	//42
	{data_convert_null_float},
	//43
	{data_convert_null_float64},
	//44
	{data_convert_null_bool},
	//45
	//{data_convert_null_null},
	//46
	{data_convert_null_string},
	//47
	{data_convert_null_char},
	//48
	{data_convert_string_int},
	//49
	{data_convert_string_int64},
	//50
	{data_convert_string_float},
	//51
	{data_convert_string_float64},
	//52
	{data_convert_string_bool},
	//53
	{data_convert_string_null},
	//54
	//{data_convert_string_string},
	//55
	{data_convert_string_char},
	//56
	{data_convert_char_int},
	//57
	{data_convert_char_int64},
	//58
	{data_convert_char_float},
	//59
	{data_convert_char_float64},
	//60
	{data_convert_char_bool},
	//61
	{data_convert_char_null},
	//62
	{data_convert_char_string},
	//63
	//{data_convert_char_char},
};
///
//#define DATA_CONVERT(value,type,ntype)		(data_converts_function)(value);
void temp_variable_add()
{
	temp_variable_add_id++;
	temp_variable_add_id_str=data_int64_to_string(temp_variable_add_id);
}
///
#define ADD_FUNCTION(var) if(var==0){var=1;}
///
data_string strtoupper(data_string s)
{
	data_int i = 0;
	char    *str = strdup(s);
	while (str[i])
	{
		if (str[i] >= 97 && str[i] <= 122)
			str[i] -= 32;
		i++;
	}
	return (str);
}
data_int64 data_string_to_int64(const data_string s)
{
	data_int64 i;
	data_char c ;
	data_int scanned = sscanf(s, "%" SCNd64 "%c", &i, &c);
	if (scanned == 1) return i;
	if (scanned > 1)
	{
		return i;
	}
	return 0;
}
data_string data_int64_to_string(data_int64 num)
{
	data_string data=malloc((sizeof(num)+num+1)*sizeof(data_string));
	//data->length=0;
	*(data) = '\0';
	do {
		*--(data) = (char) (num % 10) + '0';
		//data->length++;
		num /= 10;
	} while (num > 0);
	return data;
}
data_string data_float64_to_string(data_float64 num)
{
	data_string data=malloc((sizeof(num)+num+1)*sizeof(data_string));
	sprintf(data,"%Lf",num);
	//snprintf(data,sizeof(num),"%.*f",(data_float64) num);
	return data;
}
///
statements* program_statements_create(data_size block_size)
{
	statements* pArrayList = malloc(sizeof(statements));
	if(pArrayList != NULL)
	{
		pArrayList->nArrays = 0;
		pArrayList->size = block_size;
		pArrayList->block_size = block_size;
		pArrayList->arrays = malloc(sizeof(statement*)*block_size);
		if(pArrayList->arrays == NULL)
		{
			free(pArrayList);
			return NULL;    
		}
	}
	return pArrayList;
}
void program_statements_delete(statements* pArrayList)
{
	free(pArrayList);
}
statements* program_statements_append(statements* pArrayList,statement* array)
{
	size_t nArrays = pArrayList->nArrays;
	if(nArrays >= pArrayList->size)
	{
		size_t newSize = pArrayList->size + pArrayList->block_size;
		void* newArrays = realloc(pArrayList->arrays, sizeof(statement*)*newSize); 
		if(NULL == newArrays)
		{
			return 0;
		}
		else
		{    
			pArrayList->size = newSize;
			pArrayList->arrays = (statement**)newArrays;
		}
	}
	pArrayList->arrays[nArrays] = array;
	++pArrayList->nArrays;
	return pArrayList;
}
//////////
///
///
///
variables* program_variables_create(data_size block_size)
{
	variables* pArrayList = malloc(sizeof(variables));
	if(pArrayList != NULL)
	{
		pArrayList->nArrays = 0;
		pArrayList->size = block_size;
		pArrayList->block_size = block_size;
		pArrayList->arrays = malloc(sizeof(data_string*)*block_size);
		if(pArrayList->arrays == NULL)
		{
			free(pArrayList);
			return NULL;    
		}
	}
	return pArrayList;
}
void program_variables_delete(variables* pArrayList)
{
	free(pArrayList);
}
variables* program_variables_append(variables* pArrayList,data_string* array)
{
	size_t nArrays = pArrayList->nArrays;
	if(nArrays >= pArrayList->size)
	{
		size_t newSize = pArrayList->size + pArrayList->block_size;
		void* newArrays = realloc(pArrayList->arrays, sizeof(data_string*)*newSize); 
		if(NULL == newArrays)
		{
			return 0;
		}
		else
		{    
			pArrayList->size = newSize;
			pArrayList->arrays = (data_string**)newArrays;
		}
	}
	pArrayList->arrays[nArrays] = array;
	++pArrayList->nArrays;
	return pArrayList;
}
///
///
///////////////
expressions* program_expressions_create(data_size block_size)
{
	expressions* pArrayList = malloc(sizeof(expressions));
	if(pArrayList != NULL)
	{
		pArrayList->nArrays = 0;
		pArrayList->size = block_size;
		pArrayList->block_size = block_size;
		pArrayList->arrays = malloc(sizeof(expression*)*block_size);
		if(pArrayList->arrays == NULL)
		{
			free(pArrayList);
			return NULL;    
		}
	}
	return pArrayList;
}
void program_expressions_delete(expressions* pArrayList)
{
	free(pArrayList);
}
expressions* program_expressions_append_single(expression* array)
{
	data_size block_size=1;
	expressions* pArrayList = malloc(sizeof(expressions));
	if(pArrayList != NULL)
	{
		pArrayList->nArrays = 0;
		pArrayList->size = block_size;
		pArrayList->block_size = block_size;
		pArrayList->arrays = malloc(sizeof(expression*)*block_size);
		if(pArrayList->arrays == NULL)
		{
			free(pArrayList);
			return NULL;    
		}
	}
	///
	size_t nArrays = pArrayList->nArrays;
	if(nArrays >= pArrayList->size)
	{
		size_t newSize = pArrayList->size + pArrayList->block_size;
		void* newArrays = realloc(pArrayList->arrays, sizeof(expression*)*newSize); 
		if(NULL == newArrays)
		{
			return 0;
		}
		else
		{    
			pArrayList->size = newSize;
			pArrayList->arrays = (expression**)newArrays;
		}
	}
	pArrayList->arrays[nArrays] = array;
	++pArrayList->nArrays;
	return pArrayList;
}
expressions* program_expressions_append(expressions* pArrayList,expression* array)
{
	size_t nArrays = pArrayList->nArrays;
	if(nArrays >= pArrayList->size)
	{
		size_t newSize = pArrayList->size + pArrayList->block_size;
		void* newArrays = realloc(pArrayList->arrays, sizeof(expression*)*newSize); 
		if(NULL == newArrays)
		{
			return 0;
		}
		else
		{    
			pArrayList->size = newSize;
			pArrayList->arrays = (expression**)newArrays;
		}
	}
	pArrayList->arrays[nArrays] = array;
	++pArrayList->nArrays;
	return pArrayList;
}
///////////
data_bool expression_type_is_string(expression_types type)
{
	/*
	using : 
	if(expression_type_is_string($type))
	{
		//is string
	}
	*/
	return type==EXPRESSION_STRING_DOUABLE || type==EXPRESSION_STRING_SINGLE  ? TRUE : FALSE;
}
data_bool expression_type_is_ints(expression_types type)
{
	return type==EXPRESSION_INT || type==EXPRESSION_INT64  ? TRUE : FALSE;
}
data_bool expression_type_is_int64(expression_types type)
{
	return type==EXPRESSION_INT64  ? TRUE : FALSE;
}
data_bool expression_type_is_floats(expression_types type)
{
	return type==EXPRESSION_FLOAT || type==EXPRESSION_FLOAT64  ? TRUE : FALSE;
}
data_bool expression_type_is_float64(expression_types type)
{
	return type==EXPRESSION_FLOAT64  ? TRUE : FALSE;
}
data_bool expression_type_is_float(expression_types type)
{
	return type==EXPRESSION_FLOAT  ? TRUE : FALSE;
}
data_bool expression_type_is_number(expression_types type)
{
	return type==EXPRESSION_INT || type==EXPRESSION_INT64 || type==EXPRESSION_FLOAT || type==EXPRESSION_FLOAT64  ? TRUE : FALSE;
	//return type==EXPRESSION_INT64 ? TRUE : FALSE;
}
data_string operator_type(data_int type)
{
	switch(type)
	{
		///
		case TOKEN_OPERATOR_ADD:
			return "+";
		case TOKEN_OPERATOR_MINUS:
			return "-";
		case TOKEN_OPERATOR_MINUS_MANFI:
			//-(number)
			return "-";
		case TOKEN_OPERATOR_ZARB:
			return "*";
		case TOKEN_OPERATOR_TAGH:
			return "/";
		case TOKEN_OPERATOR_POW:
			return "^";
		///
		case TOKEN_OPERATOR_DOT://concat
			return ".";
		///
		case TOKEN_OPERATOR_EQUAL_ADD:
			return "+=";
		case TOKEN_OPERATOR_EQUAL_MINUS:
			return "-=";
		case TOKEN_OPERATOR_EQUAL_ZARB:
			return "*=";
		case TOKEN_OPERATOR_EQUAL_TAGH:
			return "*=";
		case TOKEN_OPERATOR_EQUAL_POW:
			return "^=";
		///
		case TOKEN_OPERATOR_BOOLING_MORE:
			return ">";
		case TOKEN_OPERATOR_BOOLING_LESS:
			return "<";
		case TOKEN_OPERATOR_BOOLING_EQUAL_MORE:
			return "=>";
		case TOKEN_OPERATOR_BOOLING_EQUAL_LESS:
			return "=<";
		///
		case TOKEN_OPERATOR_AND:
			return "&&";
		case TOKEN_OPERATOR_OR:
			return "||";
		case TOKEN_OPERATOR_NOT:
			return "!";
		///
		case TOKEN_OPERATOR_BAGHI:
			return "%";
		case TOKEN_OPERATOR_EQUAL_BAGHI:
			return "%=";
		///
		case TOKEN_OPERATOR_QUESTION:
			return "?";
		case TOKEN_OPERATOR_DONOGHTE:
			return ":";
		///
		case TOKEN_OPERATOR_ARGAND:
			return ",";
		///
		case TOKEN_OPERATOR_BRACKET_OPEN:
			return "(";
		case TOKEN_OPERATOR_BRACKET_CLOSE:
			return ")";
		///
		case TOKEN_OPERATOR_ADD_ADD:
			return "++";
		case TOKEN_OPERATOR_MINUS_MINUS:
			return "--";
		case TOKEN_OPERATOR_SHIFT_LEFT:
			return "<<";
		case TOKEN_OPERATOR_SHIFT_RIGHT:
			return ">>";
		case TOKEN_OPERATOR_NOT_EQUAL_EQUAL:
			return "!==";
		case TOKEN_OPERATOR_EQUAL:
			return "=";
		case TOKEN_OPERATOR_NOT_EQUAL:
			return "!=";
		case TOKEN_OPERATOR_EQUAL_EQUAL:
			return "==";
		case TOKEN_OPERATOR_EQUAL_EQUAL_EQUAL:
			return "===";
		///
	}
	return "unknown	";
}
data_string state_type(state_types type)
{	
	switch(type)
	{
		case STATE_MAIN:
			return "main";
		case STATE_FUNCTION:
			return "function";
	}
	return "unknown";
}
data_string runtime_expression_type(expression_types type)
{
	switch(type)
	{
		case EXPRESSION_NULL:
			return "vnull";
		case EXPRESSION_BOOL:
			return "vbool";
		case EXPRESSION_STRING_DOUABLE:
		case EXPRESSION_STRING_SINGLE:
			return "vstring";
		case EXPRESSION_INT:
			return "vint";
		case EXPRESSION_INT64:
			return "vint64";
		case EXPRESSION_FLOAT:
			return "vfloat";
		case EXPRESSION_FLOAT64:
			return "vfloat64";
		case EXPRESSION_NEEDRUNTIME:
			return "needruntime";
		/*
		case EXPRESSION_OPERATOR_DO:
			return "operator_first";
		case EXPRESSION_OPERATOR:
			return "operator";
		*/
	}
	return "unknown";
}
data_string expression_type(expression_types type)
{
	switch(type)
	{
		case EXPRESSION_NULL:
			return "null";
		case EXPRESSION_BOOL:
			return "bool";
		case EXPRESSION_STRING_DOUABLE:
			return "string_douable";
		case EXPRESSION_STRING_SINGLE:
			return "string_single";
		case EXPRESSION_INT:
			return "int";
		case EXPRESSION_INT64:
			return "int64";
		case EXPRESSION_FLOAT:
			return "float";
		case EXPRESSION_FLOAT64:
			return "float64";
		case EXPRESSION_OPERATOR_DO:
			return "operator_first";
		case EXPRESSION_OPERATOR:
			return "operator";
		case EXPRESSION_IDENT:
		case EXPRESSION_IDENT_EVAL:
			return "ident";
		case EXPRESSION_NEEDRUNTIME:
			return "needruntime";
		case EXPRESSION_BRACKET:
			return "bracket";
	}
	return "unknown";
}
data_string statement_type(statement_types type)
{
	switch(type)
	{
		case STATEMENT_PRINT:
			return "print";
		case STATEMENT_PRINTNL:
			return "printnl";
		case STATEMENT_IMPORT:
			return "import";
		case STATEMENT_FUNCTION:
			return "function";
		case STATEMENT_VARIABLE_SET:
			return "variable_set";
		case STATEMENT_UI_TITLE:
			return "title";
		case STATEMENT_UI_DESCRIPTION:
			return "description";
		case STATEMENT_UI_KEYWORDS:
			return "keywords";
		case STATEMENT_UI_AUTHOR:
			return "author";
		case STATEMENT_IF:
			return "if";
		case STATEMENT_ELSE:
			return "else";
		case STATEMENT_ELSEIF:
			return "elseif";
	}
	return "unknown";
}
expression* expression_operator2(expression* v1,expression* v2,int op1,int op2)
{
  expression* result=malloc(sizeof(expression));
  result->voperator=op1;
  result->type=EXPRESSION_OPERATOR_DO;
  data_int v1_len=0;
  data_int v2_len=0;
  if(v1 != NULL)
  {
	result->vleft=v1;
	v1_len=strlen(v1->value);
  }
  if(v2 != NULL)
  {
	result->vright=v2;
	v2_len=strlen(v2->value);
  }
  char *operator1=operator_type(op1);
  char *operator2=operator_type(op2);
  result->value=malloc((v1_len+v2_len+strlen(operator1)+strlen(operator2)+1)*sizeof(data_string));
  if(v1 != NULL && v2 != NULL)
  {
	strcpy(result->value,operator1);
	strcat(result->value," ");
	strcat(result->value,result->vleft->value);
	strcat(result->value," ");
	strcat(result->value,result->vright->value);
	strcat(result->value," ");
	strcat(result->value,operator2);
  }
  else if(v2 == NULL)
  {
	strcpy(result->value,operator1);
	strcat(result->value," ");
	strcat(result->value,result->vleft->value);
	strcat(result->value," ");
	strcat(result->value,operator2);
  }
  return result;		
}
expression* expression_operator(expression* v1,int op,expression* v2)
{
  expression* result=malloc(sizeof(expression));
  result->voperator=op;
  result->type=EXPRESSION_OPERATOR_DO;
  data_int v1_len=0;
  data_int v2_len=0;
  if(v1 != NULL)
  {
	result->vleft=v1;
	v1_len=strlen(v1->value);
  }
  if(v2 != NULL)
  {
	result->vright=v2;
	v2_len=strlen(v2->value);
  }
  char *operator=operator_type(op);
  result->value=malloc((2+v1_len+v2_len+strlen(operator)+1)*sizeof(data_string));
  strcpy(result->value,"");
  if(v1 != NULL && v2 != NULL)
  {
	strcat(result->value,result->vleft->value);
	strcat(result->value," ");
	strcat(result->value,operator);
	strcat(result->value," ");
	strcat(result->value,result->vright->value);
  }
  else if(v1 != NULL && v2 == NULL)
  {
  	if(op == TOKEN_OPERATOR_MINUS_MANFI)
  	{
		strcat(result->value,operator);
		strcat(result->value,result->vleft->value);
		result->vright=NULL;
  	}
  	else if(op == EXPRESSION_BRACKET)
  	{
  		result->type=EXPRESSION_BRACKET;
  	}
	//result->vright->type=EXPRESSION_NULL;
	//result->vright->input=malloc(2*sizeof(data_string)+1);
  }
  //printf("\n\n\n\n\n##########%s\n\n",result->value);
  /*
  else if(v2 == NULL)
  {
	//printf("bracket open\t%s\n",v1->value);
	//result->vleft->value=malloc((2+strlen(v1->value)+1)*sizeof(data_string));
	strcpy(result->value,"");
	strcat(result->value,"(");
	strcat(result->value,result->vleft->value);
	strcat(result->value,")");
	//printf("%s\n",expression_type(result->vleft->type) );
	result->type=EXPRESSION_BRACKET;
  }
  */
  return result;
}
data_size utf8len(char *s)
{
	data_size len = 0;
	for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
	return len;
}
expression* value_create_string(data_int type,data_string input,data_string buf)
{
  expression* result=malloc(sizeof(expression));
  result->type=type;
  result->input=input;
  if(type == EXPRESSION_STRING_DOUABLE || type == EXPRESSION_STRING_SINGLE)
  {
	data_int64 lenbuf=strlen(buf);
	data_int64 lenval=strlen(input);
	result->vstring=malloc((lenval+1)*sizeof(data_string));
	result->value=malloc((lenbuf+2+1)*sizeof(data_string));
	memcpy(result->vstring,input,lenval+1);
	memcpy(result->value,"\"",1+1);
	memcpy(result->value+1,buf,lenbuf+1);
	memcpy(result->value+1+lenbuf,"\"",1+1);
	result->length=utf8len(buf);
  }
  else if(type == TOKEN_IMPORT)
  {

  }
  return result;
}
expression* value_create(data_int type, data_string input)
{
  expression* result=malloc(sizeof(expression));
  result->type=type;
  result->input=input;
  result->value=input;
  if(type == EXPRESSION_INT64)
  {
	lib_inttypes=1;
	result->vint64=data_string_to_int64(input);
  }
  else if(type == EXPRESSION_INT)
  {
	result->vint=atoi(input);
  }
  else if(type == EXPRESSION_FLOAT)
  {
	result->vfloat=atof(input);
	/*
	printf("=%s\n",input);
	printf("=%f\n",result->vfloat);
	*/
  }
  else if(type == EXPRESSION_FLOAT64)
  {
	///soon
  }
  else if(type == EXPRESSION_BOOL)
  {
	//printf("=====================%s=%s\n",input,(input == "true") ? "y" : "n");
	//printf("=======================>%d%d\n",TRUE,FALSE);
	if(strcmp(input,"true") == 0)
	{
		result->vbool=TRUE;
	}
	else
	{
		result->vbool=FALSE;
	}
  }
  /*
  else if(type == EXPRESSION_STRING_DOUABLE || type == EXPRESSION_STRING_SINGLE)
  {
	result->vstring=malloc(sizeof(input)*sizeof(data_string));
	strcpy(result->vstring,input);
	result->length=strlen(result->vstring);
  }
  */
  else if(type == EXPRESSION_NULL)
  {

  }
  result->value=malloc((strlen(input)+1)*sizeof(data_string));
  memcpy(result->value,input,sizeof(input));
  return result;
}
data_string ReadFile(char *filename)
{
   char *buffer = NULL;
   data_int string_size, read_size;
   FILE *handler = fopen(filename, "r");
   if (handler)
   {
	   fseek(handler, 0, SEEK_END);
	   string_size = ftell(handler);
	   rewind(handler);
	   buffer = (data_string) malloc(sizeof(data_char) * (string_size + 1) );
	   read_size = fread(buffer, sizeof(data_char), string_size, handler);
	   buffer[string_size] = '\0';
	   if (string_size != read_size)
	   {
		   free(buffer);
		   buffer = NULL;
	   }
	   fclose(handler);
	}
	return buffer;
}
variables* list_variables;
void program_init()
{
	VM_PRINT("init");
	source 			= malloc(100000*sizeof(data_string));
	source_main 	= malloc(100000*sizeof(data_string));
	source_library 	= malloc(100000*sizeof(data_string));
	source_code 	= malloc(100000*sizeof(data_string));
	strcpy(source,"");
	strcpy(source_main,"");
	strcpy(source_library,"");
	strcpy(source_code,"");
	list_variables=program_variables_create(2);
}
void program_statement(statement* statement)
{
	state_types state;
	statement_types type;
	state=statement->state;
	type=statement->type;
	/////////////////////////////////////
	#if DEBUG_VM
	VM_PRINTF("%s @ %s",statement_type(type),state_type(state) );
	/*
	if(statement->value1 != NULL)
	{
		if(statement->value1->value == NULL)
		{
			VM_PRINTF("::::::%s = %s",(expression_type(statement->value1->type) == "operator" ? operator_type(statement->value1->voperator) : expression_type(statement->value1->type)) , statement->value1->value  );		
		}
		else
		{
		}
		//VM_PRINTF("======================================>%s",statement->value1->value);
		//VM_PRINTF("::::::%s = %s",(expression_type(statement->value1->type) == "operator" ? operator_type(statement->value1->voperator) : expression_type(statement->value1->type)) , "."  );
	}
	if(statement->value2 != NULL)
	{
		VM_PRINTF("::::::%s = %s",(expression_type(statement->value2->type) == "operator" ? operator_type(statement->value2->voperator) : expression_type(statement->value2->type)) , statement->value2->value  );
	}
	*/
	#endif
	switch(type)
	{
		case STATEMENT_FUNCTION:
		{
			program_statements(statement->stmts);
		}
		break;
		case STATEMENT_IMPORT:
		{
			SOURCE_APPEND(STATE(state), "printf(");
			SOURCE_APPEND(STATE(state), statement->value1->vstring);
			SOURCE_APPEND(STATE(state), ");\n");
		}
		break;
		case STATEMENT_PRINT:
		case STATEMENT_PRINTNL:
		{
			/*
			class_data=1;
			lib_stdio=1;
			lib_inttypes=1;
			lib_string=1;
			*/
			program_prints(state,statement->values1);
			if(type == STATEMENT_PRINTNL)
			{
				SOURCE_APPEND(STATE(state), "printf(\"\\n\");\n");
			}
		}
		break;
		case STATEMENT_VARIABLE_SET:
		{
			class_data=1;
			lib_stdio=1;
			lib_inttypes=1;
			lib_string=1;
			//lib_ctype=1;
			/*
			printf("=======>var set!\n");
			program_print(state,statement->value1);
			program_print(state,statement->value2);
			*/
			data_string varname=malloc(500000*sizeof(data_string));
			strcpy(varname,"");
			////
			strcat(varname,"variable_user_");
			strcat(varname,state_type(state));
			strcat(varname,"_");
			strcat(varname,statement->value1->value);
			////
			program_variables_append(&list_variables,varname);
			////
			////
			data_int narr=list_variables->nArrays;
			for(data_int count = narr; count >= 1; count--)
			{
				printf("==>%s\n", list_variables->arrays[0] );
			}
			//pArrayList->arrays
			SOURCE_APPEND(STATE(state), "data variable_user_");
			SOURCE_APPEND(STATE(state), state_type(state));
			SOURCE_APPEND(STATE(state), "_");
			SOURCE_APPEND(STATE(state), statement->value1->value);
			if(statement->value2->type != EXPRESSION_IDENT || statement->value2->type != EXPRESSION_IDENT_EVAL)
			{
				SOURCE_APPEND(STATE(state), "={");
				SOURCE_APPEND(STATE(state), strtoupper(runtime_expression_type(statement->value2->type)));
				SOURCE_APPEND(STATE(state), "}");
			}
			SOURCE_APPEND(STATE(state), ";\n");
			///
			if(strcmp(expression_type(statement->value2->type),"null") == 0){}else
			{
				SOURCE_APPEND(STATE(state), "variable_user_");
				SOURCE_APPEND(STATE(state), state_type(state));
				SOURCE_APPEND(STATE(state), "_");
				SOURCE_APPEND(STATE(state), statement->value1->value);
				if(statement->value2->type != EXPRESSION_IDENT || statement->value2->type != EXPRESSION_IDENT_EVAL)
				{
					SOURCE_APPEND(STATE(state), ".value");
					SOURCE_APPEND(STATE(state), ".");
					SOURCE_APPEND(STATE(state), runtime_expression_type(statement->value2->type));
				}
				SOURCE_APPEND(STATE(state), "=");
				if(statement->value2->type != EXPRESSION_IDENT || statement->value2->type != EXPRESSION_IDENT_EVAL)
				{
					SOURCE_APPEND(STATE(state), statement->value2->value);
				}
				else
				{
					SOURCE_APPEND(STATE(state), "variable_user_");
					SOURCE_APPEND(STATE(state), state_type(state));
					SOURCE_APPEND(STATE(state), "_");
					SOURCE_APPEND(STATE(state), statement->value2->value);
				}
				SOURCE_APPEND(STATE(state), ";\n");				
			}

		}
		break;
		case STATEMENT_UI_TITLE:
		{
			SOURCE_APPEND(STATE(state), "printf(\"title type : ");
			SOURCE_APPEND(STATE(state),expression_type(statement->value1->type) );
			SOURCE_APPEND(STATE(state), "\");\n" );
		}
		break;
		case STATEMENT_IF:
		{
			SOURCE_APPEND(STATE(state), "if(");
			SOURCE_APPEND(STATE(state), statement->value1->value);
			SOURCE_APPEND(STATE(state), ")\n{");
			program_statements(statement->stmts);
			SOURCE_APPEND(STATE(state), "}\n");
		}
		break;
		default:
		{

		}
		break;
	}
}
void program_statements(statements* statements)
{
	/*
	data_string data = malloc((8000)*sizeof(data_string)+1);
	sprintf(data,"%"PRId64,(int64_t) -456);
	printf("=>%s\n!!!",data);
	*/
	//printf("\n====>%"PRIu64"\n",(uint64_t) -456);
	//printf("\n====>%"PRId64"\n",(int64_t) -456);
	statement* statement;
	data_int narr=statements->nArrays;
	for(data_int count = narr; count >= 1; count--)
	{
		statement=statements->arrays[narr-count];
		program_statement(statement);
	}
}
void program_finish()
{
	if(lib_stdio)
	{
		SOURCE_APPEND(source, "#include <stdio.h>\n" );	
	}
	if(lib_stdlib)
	{
		SOURCE_APPEND(source, "#include <stdlib.h>\n" );
	}
	if(lib_inttypes)
	{
		SOURCE_APPEND(source, "#include <inttypes.h>\n" );
	}
	if(lib_string)
	{
		SOURCE_APPEND(source, "#include <string.h>\n" );
	}
	if(lib_assert)
	{
		SOURCE_APPEND(source, "#include <assert.h>\n" );
	}
	if(lib_stdint)
	{
		SOURCE_APPEND(source, "#include <stdint.h>\n" );
	}
	//for data_float64 and more...
	//SOURCE_APPEND(source, "#include <et.h>\n" );
	//SOURCE_APPEND(source, "#include \"et.h\"\n" );
	///////////////////////////////////////////////////////////
	/*
	if(class_define)
	{
		SOURCE_APPEND(source_library,ReadFile("core/class_define.etc"));
	}
	if(class_data)
	{
		SOURCE_APPEND(source_library,ReadFile("core/class_data.etc"));
	}
	if(class_expression_type)
	{
		SOURCE_APPEND(source_library,ReadFile("core/class_expression_type.etc"));
	}
	if(class_operator)
	{
		SOURCE_APPEND(source_library,ReadFile("core/class_operator.etc"));
	}
	*/
	if(func_print_float)
	{
		SOURCE_APPEND(source_library,"#define system_print_float(value)\t\t\t"\
"printf(\"%g\",value);"\
"\n");
	}
	if(func_print_float64)
	{
		SOURCE_APPEND(source_library,"#define system_print_float64(value)\t\t\t"\
"printf(\"%g\",value);"\
"\n");		
	}
	if(func_print_int)
	{
		SOURCE_APPEND(source_library,"#define system_print_int(value)\t\t\t"\
"printf(\"%d\",(int) value);"\
"\n");
	}
	if(func_print_int64)
	{
		SOURCE_APPEND(source_library,"#define system_print_int64(value)\t\t\t"\
"printf(\"%\"PRId64,(int64_t) value);"\
"\n");
	}
	if(func_print_string)
	{
		SOURCE_APPEND(source_library,"#define func_print_string(value)\t\t\t"\
"printf(\"%s\",value);"\
"\n");
	}
	if(func_print_null)
	{
		SOURCE_APPEND(source_library,"#define func_print_null()\t\t\t"\
"printf(\"null\");"\
"\n");
	}
	SOURCE_APPEND(source, source_library );
	///////////////////////////////////////////////////////////
	SOURCE_APPEND(source, source_code );
	///////////////////////////////////////////////////////////
	SOURCE_APPEND(source, "int main(int argc,char** argv)\n{\n" );
	SOURCE_APPEND(source, source_main );
	SOURCE_APPEND(source, "return 0;\n" );
	SOURCE_APPEND(source, "}\n" );
	///////////////////////////////////////////////////////////
	//printf( "\n\n%s\n\n",source );
	///////////////////////////////////////////////////////////
	FILE *output;
	output = fopen("temp.c", "w");
	if(output == NULL)
	{
		printf("I couldn't open temp.c for writing.\n");
	}
	fprintf(output,"%s",source);
	//fclose(output);
	///////////////////////////////////////////////////////////
}
statement* statement_ui_title(state_types state,expression* v)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_UI_TITLE;
	result->value1=v;
	return result;
}
statement* statement_variable_set(state_types state,expression* n,expression* v)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_VARIABLE_SET;
	//result->value=n->value;
	result->value1=n;
	result->value2=v;
	return result;
}
statement* statement_function(state_types state,expression* name,expressions* args,statements* stmts)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_FUNCTION;
	result->value1=name;
	result->values1=args;
	result->stmts=stmts;
	return result;
}
statement* statement_import(state_types state,expression* v)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_IMPORT;
	result->value1=v;
	return result;
}
statement* statement_print(state_types state,expressions* v,data_int newline)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_PRINT;
	if(newline)
	{
		result->type=STATEMENT_PRINTNL;
	}
	result->values1=v;
	return result;
}
statement* statement_if(state_types state,expression* v,statements* stmts)
{
	statement* result=malloc(sizeof(statement));
	result->state=state;
	result->type=STATEMENT_IF;
	result->stmts=stmts;
	result->value1=v;
	return result;
}
void program_prints(state_types state,expressions* v)
{
	expression* expression;
	int narr=v->nArrays;
	for(data_int count = narr; count >= 1; count--)
	{
		expression=v->arrays[narr-count];
		program_print(state,expression);
	}
}
void evaluator_operator(state_types state,expression* v)
{
	int doing=0;
	int check_number=0;
	int in_main=0;
	if(v->vleft->type == EXPRESSION_BRACKET)
	{
		v->vleft=v->vleft->vleft;
		evaluator_operator(state,v->vleft);
	}
	if(v->vright->type == EXPRESSION_BRACKET)
	{
		v->vright=v->vright->vleft;
		evaluator_operator(state,v->vright);
	}
	if(v->vleft !=NULL && v->vleft->type == EXPRESSION_OPERATOR_DO)
	{
		v->vleft->type=EXPRESSION_OPERATOR;
		evaluator_operator(state,v->vleft);
		in_main=1;
	}
	if(v->vright !=NULL && v->vright->type == EXPRESSION_OPERATOR_DO)
	{
		v->vright->type=EXPRESSION_OPERATOR;
		evaluator_operator(state,v->vright);
		in_main=1;
	}
	/*
	if(v->vleft->type == EXPRESSION_BRACKET)
	{
		v->vleft=v->vleft->vleft;
		evaluator_operator(state,v->vleft);
	}
	if(v->vright != NULL && v->vright->type == EXPRESSION_BRACKET)
	{
		v->vright=v->vright->vleft;
		evaluator_operator(state,v->vright);
	}
	*/
	//printf("))))))))))))))))))))))))))))))\n");
	//printf("\n\n!!!!!!!!!!!%s\n\n",expression_type(v->type));
	data_int tl = v->vleft->type;
	data_int tr;
	if(v->vright == NULL)
	{
		tr = EXPRESSION_NULL;
	}
	else
	{
		tr = v->vright->type;
	}
	//printf("\n\n\n!!!!!!!!!!\n");
	if(tl == EXPRESSION_IDENT || tr == EXPRESSION_IDENT)
	{
		class_operator=1;
	}
	switch(v->voperator)
	{
		case TOKEN_OPERATOR_DOT:
		{
			if(tl == EXPRESSION_IDENT || tr == EXPRESSION_IDENT || tl == EXPRESSION_IDENT_EVAL || tr == EXPRESSION_IDENT_EVAL)
			{
				printf("\n\n#Error: Not Supported!\n\n");
			}
			else
			{
				if(tl != EXPRESSION_STRING_DOUABLE)
				{
					if(tl == EXPRESSION_INT)
					{
						v->vleft->vstring=(data_string) data_convert_int_string(v->vleft->vint);
					}
					else if(tl == EXPRESSION_INT64)
					{
						v->vleft->vstring=(data_string) data_convert_int64_string(v->vleft->vint64);
					}
					else if(tl == EXPRESSION_FLOAT)
					{
						v->vleft->vstring=(data_string) data_convert_float_string(v->vleft->vfloat);
					}
					else if(tl == EXPRESSION_FLOAT64)
					{
						v->vleft->vstring=(data_string) data_convert_float64_string(v->vleft->vfloat64);
					}
					else if(tl == EXPRESSION_BOOL)
					{
						v->vleft->vstring=(data_string) data_convert_bool_string(v->vleft->vbool);
					}
					else if(tl == EXPRESSION_NULL)
					{
						v->vleft->vstring=(data_string) data_convert_null_string();
					}
					else if(tl == EXPRESSION_CHAR)
					{
						v->vleft->vstring=(data_string) data_convert_char_string(v->vleft->vchar);
					}
				}
				if(tr != EXPRESSION_STRING_DOUABLE)
				{
					if(tr == EXPRESSION_INT)
					{
						v->vright->vstring=(data_string) data_convert_int_string(v->vright->vint);
					}
					else if(tr == EXPRESSION_INT64)
					{
						v->vright->vstring=(data_string) data_convert_int64_string(v->vright->vint64);
					}
					else if(tr == EXPRESSION_FLOAT)
					{
						v->vright->vstring=(data_string) data_convert_float_string(v->vright->vfloat);
					}
					else if(tr == EXPRESSION_FLOAT64)
					{
						v->vright->vstring=(data_string) data_convert_float64_string(v->vright->vfloat64);
					}
					else if(tr == EXPRESSION_BOOL)
					{
						v->vright->vstring=(data_string) data_convert_bool_string(v->vright->vbool);
					}
					else if(tr == EXPRESSION_NULL)
					{
						v->vright->vstring=(data_string) data_convert_null_string();
					}
					else if(tr == EXPRESSION_CHAR)
					{
						v->vright->vstring=(data_string) data_convert_char_string(v->vright->vchar);
					}
				}
				v->type=EXPRESSION_STRING_DOUABLE;
				v->vstring=malloc(strlen(v->vleft->vstring)+strlen(v->vright->vstring)*sizeof(data_string)+1);//may need more realloc
				strcpy(v->vstring,v->vleft->vstring);
				strcat(v->vstring,v->vright->vstring);
				//printf("\n\n==>%s\n\n==>%s\n\n===>%s\n\n",v->vleft->vstring,v->vright->vstring,v->vstring);
				doing=1;
			}
		}
		break;
		case TOKEN_OPERATOR_BRACKET_OPEN:
		{
			printf("\n\n!!!!!!!!!!!\n\n");
		}
		break;
		case TOKEN_OPERATOR_MINUS_MANFI:
		{
			switch(v->vleft->type)
			{
				case EXPRESSION_INT:
				{
					v->vleft->vint=-v->vleft->vint;
					v->type=v->vleft->type;
					v->vint=v->vleft->vint;
				}
				break;
				case EXPRESSION_INT64:
				{
					v->vleft->vint64=-v->vleft->vint64;
					v->type=v->vleft->type;
					v->vint64=v->vleft->vint64;
				}
				break;
				case EXPRESSION_FLOAT:
				{
					v->vleft->vfloat=-v->vleft->vfloat;
					v->type=v->vleft->type;
					v->vfloat=v->vleft->vfloat;
				}
				break;
				case EXPRESSION_FLOAT64:
				{
					v->vleft->vfloat64=-v->vleft->vfloat64;
					v->type=v->vleft->type;
					v->vfloat64=v->vleft->vfloat64;
				}
				break;
				default:
				{
					exit("Error Operator -");
				}
				break;
			}
			doing=1;
		}
		break;
		case TOKEN_OPERATOR_ADD:
		{
			EVALUATOR_OPERATOR(+,"add");
		}
		break;
		case TOKEN_OPERATOR_MINUS:
		{
			EVALUATOR_OPERATOR(-,"minus");
		}
		break;
		case TOKEN_OPERATOR_ZARB:
		{
			EVALUATOR_OPERATOR(*,"zarb");
		}
		break;
		case TOKEN_OPERATOR_TAGH:
		{
			EVALUATOR_OPERATOR(/,"tagh");
		}
		break;
	}
	if(doing == 1)
	{
		if(v->type==EXPRESSION_INT)
		{
			//v->value=data_int64_to_string((data_int64) v->vint);
			data_string value = malloc((strlen(v->vleft->value)+strlen(v->vright->value))*sizeof(data_string)+1);
			sprintf(value,"%d",(data_int) v->vint);
			v->value=value;
		}
		else if(v->type==EXPRESSION_INT64)
		{
			//v->value=data_int64_to_string(v->vint64);
			//printf("\n#########\n%s\n",v->vleft->value);
			data_string value;
			if(v->vright == NULL)
			{
				value = malloc((strlen(v->vleft->value))*sizeof(data_string)+1);
			}
			else
			{
				value = malloc((strlen(v->vleft->value)+strlen(v->vright->value))*sizeof(data_string)+1);
			}
			sprintf(value,"%"PRId64,(int64_t) v->vint64);
			//printf("\n!!!!!!!!!\n%s\n",value);
			v->value=value;
		}
		else if(v->type == EXPRESSION_FLOAT)
		{
			v->value=data_float64_to_string(v->vfloat);
		}
		else if(v->type == EXPRESSION_FLOAT64)
		{
			v->value=data_float64_to_string(v->vfloat);
		}
	}
	/*
	if(tl == EXPRESSION_NULL && tr != EXPRESSION_NULL)
	{
		v->type=tr;
		v->value=v->vright->value;
		return;
	}
	else if(tl != EXPRESSION_NULL && tr == EXPRESSION_NULL)
	{
		v->type=tl;
		v->value=v->vleft->value;
		return;
	}
	else if(tl == EXPRESSION_NULL || tr == EXPRESSION_NULL)
	{
		v->type=EXPRESSION_NULL;
		return;
	}
	else if(tl == EXPRESSION_BOOL || tr == EXPRESSION_BOOL)
	{
		v->type=EXPRESSION_NULL;
		return;
	}
	*/
	/*
	else if(tl == EXPRESSION_IDENT && tr == EXPRESSION_IDENT)
	{
		class_operator=1;
		class_expression_type=1;
		class_define=1;
		v->value=malloc(55555555*sizeof(data_string));
		strcpy(v->value,"");
		strcat(v->value,"evaluator_operator (");
		if(v->vleft->type == EXPRESSION_IDENT)
		{
			strcat(v->value,"variable_user_");
			strcat(v->value,state_type(state));
			strcat(v->value,"_");
			strcat(v->value,v->vleft->value);
		}
		else
		{
			//printf("=============>%d\n", v->vleft->type);
			//strcat(v->value,"evaluator_operator_");
			//strcat(v->value,expression_type(v->vleft->type));
			//strcat(v->value,"(");
			strcat(v->value,v->vleft->value);
			//strcat(v->value,")");
		}
		strcat(v->value,",");
		strcat(v->value,data_int64_to_string(v->voperator));
		strcat(v->value,",");
		if(v->vright->type == EXPRESSION_IDENT)
		{
			strcat(v->value,"variable_user_");
			strcat(v->value,state_type(state));
			strcat(v->value,"_");
			strcat(v->value,v->vright->value);
		}
		else
		{
			strcat(v->value,"evaluator_operator_");
			strcat(v->value,expression_type(v->vright->type));
			strcat(v->value,"(");
			strcat(v->value,v->vright->value);
			strcat(v->value,")");
		}
		strcat(v->value,")");
		v->type=EXPRESSION_NEEDRUNTIME;
		check_number=1;
		doing=1;
	}
	*/
	if(doing == 0)
	{
		v->type=EXPRESSION_OPERATOR;
	}
}
void program_print(state_types state,expression* v)
{
	//printf("\n\n%s\n",expression_type(v->type));
	switch(v->type)
	{
		case EXPRESSION_IDENT:
		{
			SOURCE_APPEND(STATE(state),"system_print_data(&variable_user_");
			SOURCE_APPEND(STATE(state),state_type(state));
			SOURCE_APPEND(STATE(state),"_");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_IDENT_EVAL:
		{
			SOURCE_APPEND(STATE(state),"system_print_inline_data(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_NULL:
		{
			ADD_FUNCTION(func_print_null);
			SOURCE_APPEND(STATE(state),"func_print_null();\n");
		}
		break;
		case EXPRESSION_BRACKET:
		{
			program_print(state,v->vleft);
		}
		break;
		case EXPRESSION_BOOL:
		{
			ADD_FUNCTION(func_print_bool);
			SOURCE_APPEND(STATE(state),"printf(\"");
			SOURCE_APPEND(STATE(state),v->vbool == TRUE ? "true" : "false");
			SOURCE_APPEND(STATE(state),"\");\n");
		}
		break;
		case EXPRESSION_STRING_DOUABLE:
		case EXPRESSION_STRING_SINGLE:
		{
			ADD_FUNCTION(func_print_string);
			SOURCE_APPEND(STATE(state),"func_print_string(\"");
			SOURCE_APPEND(STATE(state),v->vstring);
			SOURCE_APPEND(STATE(state),"\");\n");
			//printf("====>%d\n",v->length);
		}
		break;
		//case 
		case EXPRESSION_INT:
		{
			ADD_FUNCTION(func_print_int);
			SOURCE_APPEND(STATE(state),"system_print_int(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_INT64:
		{
			//SOURCE_APPEND(source_library,"#include <limits.h>\n");SOURCE_APPEND(STATE(state),"printf(\"\\n*%d\\n\",INT_MAX);");
			ADD_FUNCTION(func_print_int64);
			SOURCE_APPEND(STATE(state),"system_print_int64(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_FLOAT:
		{
			ADD_FUNCTION(func_print_float);
			SOURCE_APPEND(STATE(state),"system_print_float(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_FLOAT64:
		{
			ADD_FUNCTION(func_print_float64);
			SOURCE_APPEND(STATE(state),"system_print_float64(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_OPERATOR_DO:
		{
			ADD_FUNCTION(func_print_operator_do);
			evaluator_operator(state,v);
			program_print(state,v);
		}
		break;
		case EXPRESSION_NEEDRUNTIME:
		{
			ADD_FUNCTION(func_print_inline_data);
			SOURCE_APPEND(STATE(state),"system_print_inline_data(");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
		case EXPRESSION_OPERATOR:
		{
			//ADD_FUNCTION(func_print_operator);
			SOURCE_APPEND(STATE(state),"printf(\"%g\",");
			SOURCE_APPEND(STATE(state),v->value);
			SOURCE_APPEND(STATE(state),");\n");
		}
		break;
	}
}