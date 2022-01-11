#include "../include/sevec.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(void)
{
	size_t i;
	int store;
	int *data;
	if(vector_create(&data, 0, 0, 0))
		return -1;

	for(i = 0; i < 1000; i++)
		vector_push(&data, (int*)&i);

	for(i = 0; i < vector_size(&data); i++)
		printf("%d ", data[i]);
	putc('\n', stdout);

	while(!vector_pop(&data, &store))
	{
		printf("%d ", store);
	}
	putc('\n', stdout);

	printf("%d\n", vector_resize(&data, 123456));

	vector_destroy(&data);

	return 0;
}
