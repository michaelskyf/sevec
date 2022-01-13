/*
    This file is part of sevec.

    sevec is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sevec is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sevec. If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/sevec.h"

#define GROWTH_RATE_DEFAULT 1.6f

/*
 * Note: Each function should return 0 on success and -1 on failure
 * Note: If that function returns a pointer, NULL = failure, not NULL = success
 */

/*
	+----------------------+
	|       vector_t       |
	+----------------------+
	| size_t size;         |
	| size_t max_size;     |
	| size_t capacity;     |
	| size_t item_size;    |
	| float growth_rate;   |
	| unsigned char *data; |
	+----------------------+

	+------------------+
	| allocated memory |
	+------------------+
	| vector_t* | data |
	+------------------+

 * vector_t contains a pointer to "data" inside allocated memory.
 * Function vector_get_memory_generic() return a pointer
 * to the first element of allocated memory, which is vector_t*
 */

__attribute__((warn_unused_result))
static int vector_reserve_or_shrink(vector_t *, size_t new_capacity);

int vector_create_generic(void **data, size_t item_size, size_t capacity, size_t max_size, float growth_rate)
{
	vector_t *v;
	max_size = (max_size) ? (max_size) : ((size_t)-1); /* If size == 0, set it to max value */

	v = (vector_t*)malloc(sizeof(vector_t));
	if(v)
	{
		v->size = 0;
		v->capacity = capacity;
		v->max_size = max_size;
		v->item_size = item_size;
		v->growth_rate = (growth_rate >= 1) ? (growth_rate) : (GROWTH_RATE_DEFAULT);
		v->data = NULL;

		v->data = (unsigned char*)malloc(sizeof(vector_t*) + capacity*v->item_size);
		if(v->data)
		{
			vector_t **vd;
			v->data = v->data+sizeof(vector_t*);

			*data = v->data;
			vd = vector_get_memory_generic(data);
			*vd = v;
			return 0;
		}
		free(v);
	}
	return -1;
}

void vector_destroy_generic(void **data)
{
	vector_t **v = vector_get_memory_generic(data);
	free(*v); /* Free vector */
	free(v); /* Free memory */
	*data = NULL;
}

int vector_resize_generic(void **data, size_t new_size)
{
	vector_t *v = *vector_get_memory_generic(data);
	int ret = 0;
	size_t new_capacity = v->capacity;

	if(new_size > new_capacity)
	{
		/* Calculate new capacity */
		while(new_size > new_capacity)
		{
			size_t gcap = new_capacity;
			if(new_capacity == (gcap *= v->growth_rate))
				new_capacity++;
			else
				new_capacity = gcap;
		}
		ret = vector_reserve_generic(data, new_capacity);
	}
	if(!ret)
		v->size = new_size;
	return ret;
}

static int vector_reserve_or_shrink(vector_t *v, size_t new_capacity)
{
	unsigned char *new_data;

	if(new_capacity > v->max_size)
		return -1;

	new_data = (unsigned char*)realloc(v->data-sizeof(vector_t*), sizeof(vector_t*) + new_capacity*v->item_size);
	if(!new_data)
		return -1;

	v->capacity = new_capacity;
	v->data = new_data+sizeof(vector_t*);
	if(v->size > new_capacity)
		v->size = new_capacity;

	return 0;
}

int vector_reserve_generic(void **data, size_t new_capacity)
{
	vector_t *v = *vector_get_memory_generic(data);

	if(new_capacity <= v->capacity)
		return 0;

	if(!vector_reserve_or_shrink(v, new_capacity))
	{
		*data = v->data;
		return 0;
	}
	return -1;
}

int vector_shrink_generic(void **data, size_t new_capacity)
{
	vector_t *v = *vector_get_memory_generic(data);

	if(new_capacity >= v->capacity)
		return 0;

	if(!vector_reserve_or_shrink(v, new_capacity))
	{
		*data = v->data;
		return 0;
	}
	return -1;
}

void *vector_get_generic(void **data, size_t index)
{
	vector_t *v = *vector_get_memory_generic(data);
	if(index > v->size)
		return NULL;
	return v->data+v->item_size*index;
}

void *vector_push_generic(void **data, const void *element)
{
	void *ret = NULL;
	vector_t *v = *vector_get_memory_generic(data);
	if(!vector_resize_generic(data, v->size+1))
	{
		if(element)
			ret = memcpy(v->data + (v->size-1) * v->item_size, element, v->item_size);
		else
			ret = vector_get_generic(data, v->size-1);
	}
	return ret;
}

int vector_pop_generic(void **data, void *store)
{
	vector_t *v = *vector_get_memory_generic(data);

	if(v->size == 0)
		return -1;

	if(store)
		memcpy(store, v->data + (v->size-1) * v->item_size, v->item_size);

	v->size--;

	return 0;
}

vector_t **vector_get_memory_generic(void **data)
{
	return (vector_t**)(*(char**)data - sizeof(vector_t*));
}

size_t vector_size_generic(void **data)
{
	vector_t *v = *vector_get_memory_generic(data);
	return v->size;
}
