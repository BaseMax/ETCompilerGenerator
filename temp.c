#include <stdio.h>
#include <inttypes.h>
#define system_print_int64(value)			printf("%"PRId64,(int64_t) value);
int main(int argc,char** argv)
{
system_print_int64(162);
printf("\n");
return 0;
}
