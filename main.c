#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"
#include "parser.h"
extern data_int yyparse ();
extern FILE *yyin;
/*
char * buffer = 0;
long length;
FILE * f = fopen (filename, "rb");
if(f)
{
  fseek(f, 0, SEEK_END);
  length = ftell (f);
  fseek(f, 0, SEEK_SET);
  buffer = malloc(length);
  if(buffer)
  {
    fread(buffer,1,length,f);
  }
  fclose(f);
}
if(buffer)
{
  // start to process your data / extract strings here...
}
*/
data_int main(data_int argc,data_string*argv)
{
	if(argc > 1)
	{
		if(!(yyin =fopen(argv[1],"r")))
    	{
    		fprintf(stderr,"Can not access to this file %s!\n",argv[1]);
    		exit(1);
    	}
	}
	yyparse();
	return 0;
}
