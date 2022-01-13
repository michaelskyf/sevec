#include "../include/sevec.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{

	int *test_data;
	vector_t *test_vector;

/* Example usage: */
	size_t i;
	int store;
	int *data_vec;

	/* Create vector and check if it succeded */
	if(vector_create(&data_vec, 0, 0, 0))
		return -1;

	/* Push numbers 0->999 to the vector */
	for(i = 0; i < 1000; i++)
		vector_push(&data_vec, (int*)&i);

	/* Display contents of the vector */
	for(i = 0; i < vector_size(&data_vec); i++)
	{
		printf("%d ", data_vec[i]);
	}
	putc('\n', stdout);

	/* Pop elements and their contents */
	while(!vector_pop(&data_vec, &store))
	{
		printf("%d ", store);
	}
	putc('\n', stdout);

	/* Destroy the vector */
	vector_destroy(&data_vec);

/* End of example usage */

/* Tests: */

	/* Create vector and check if it succeded */
	assert (0 == vector_create(&test_data, 0, 0, 0));
	test_vector = *vector_get_memory(&test_data);

	/* Resize vector to size 1234 */
	assert (0 == vector_resize(&test_data, 1234));
	assert (1234 == test_vector->size);
	/* Shrink vector to size 5 */
	assert (0 == vector_shrink(&test_data, 5));
	assert (5 == test_vector->capacity);
	assert (5 == test_vector->size);

	/* Destroy the vector */
	vector_destroy(&test_data);
	assert (NULL == test_data);

	return 0;
}
