#include "../include/sevec.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

vector_create_definitions_of_type(int)
vector_create_definitions_of_type(size_t)
vector_create_definitions_of_type(float)

int main(void)
{
	vector_int_t *vi = vector_create_int(0, 0);
	for(size_t i = 0; i < 10000; i++)
	{
		vector_push_int(vi, NULL);
	}
	for(size_t i = 0; i < 10000; i++)
	{

	}
	vector_destroy_int(vi);

	return 0;
}
