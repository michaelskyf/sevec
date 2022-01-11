#include "../include/sevec.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(void)
{
	size_t i;
	int *data;
	if(vector_create(&data, 0, 0))
		return -1;

	for(i = 0; i < 10000; i++)
		vector_push(&data, (int*)&i);

	for(i = 0; i < vector_size(&data); i++)
		printf("%d ", data[i]);

	putc('\n', stdout);

	vector_destroy(&data);

	return 0;
}
