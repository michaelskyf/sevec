#include "../include/sevec.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct test_data{
	int hello_world;
	float work;
	char name[20];
	char surname[20];
}test_data_t;

int main(void)
{
	int *data;
	int xd = 9;
	vector_create(&data, 0, 0);

	vector_push(&data, &xd);

	vector_destroy(&data);

	test_data_t *t;
	vector_create(&t, 0, 0);

	test_data_t dummy_data;
	vector_push(&t, &dummy_data);

	vector_destroy(&t);

	return 0;
}
