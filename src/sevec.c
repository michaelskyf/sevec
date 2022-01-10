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

static float growth_rate = 1.6f;

int vector_create_generic(void **data, size_t item_size, size_t capacity, size_t max_size)
{
	vector_header_t *v;
	size_t alloc_length = capacity*item_size;

	max_size = (max_size) ? (max_size) : ((size_t)-1);

	v = malloc(sizeof(vector_header_t) + ((alloc_length > max_size) ? (max_size) : (alloc_length)));
	if(v)
	{
		v->size = 0;
		v->max_size = max_size;
		v->capacity = capacity;
		v->item_size = item_size;
		*data = (char*)v+sizeof(vector_header_t);

		return 0;
	}
	return -1;
}

void vector_destroy_generic(void **data)
{
	vector_header_t *v = vector_get_struct_generic(data);
	free(v);
	*data = NULL;
}

/* REMEMBER TO GET A NEW VECTOR AFTER A CALL TO THIS FUNCTION */
int vector_resize_generic(void **data, size_t new_size)
{
	vector_header_t *v = vector_get_struct_generic(data);
	int ret = 0;
	size_t new_capacity = v->capacity;

	if(new_size > new_capacity)
	{
		/* Calculate new capacity */
		while(new_size > new_capacity)
		{
			size_t gcap = new_capacity;
			if(new_capacity == (gcap *= growth_rate))
				new_capacity++;
			else
				new_capacity = gcap;
		}
		if(!(ret = vector_reserve_generic(data, new_capacity)))
				v = vector_get_struct_generic(data);
	}
	if(!ret)
		v->size++;
	return ret;
}

/* REMEMBER TO GET A NEW VECTOR AFTER A CALL TO THIS FUNCTION */
int vector_reserve_generic(void **data, size_t new_capacity)
{
	vector_header_t *v = vector_get_struct_generic(data);
	vector_header_t *new_vector;

	if(new_capacity > v->max_size)
		return -1;
	if(new_capacity <= v->capacity)
		return 0;

	new_vector = realloc(v, sizeof(vector_header_t) + new_capacity*v->item_size);
	if(!new_vector)
		return -1;

	*data = (char*)new_vector+sizeof(vector_header_t);

	return 0;
}

/* REMEMBER TO GET A NEW VECTOR AFTER A CALL TO THIS FUNCTION */
int vector_shrink_generic(void **data, size_t new_capacity)
{
	vector_header_t *v = vector_get_struct_generic(data);
	vector_header_t *new_vector;

	if(new_capacity >= v->capacity)
		return 0;

	new_vector = realloc(v, sizeof(vector_header_t) + new_capacity*v->item_size);
	if(!new_vector)
		return -1;

	*data = (char*)new_vector+sizeof(vector_header_t);

	return 0;
}

void *vector_get_generic(void **data, size_t index)
{
	vector_header_t *v = vector_get_struct_generic(data);
	if(index > v->size)
		return NULL;
	return *(char**)data+v->item_size*index;
}

/* REMEMBER TO GET A NEW VECTOR AFTER A CALL TO THIS FUNCTION */
void *vector_push_generic(void **data, const void *element)
{
	void *ret = NULL;
	vector_header_t *v = vector_get_struct_generic(data);
	if(!vector_resize_generic(data, v->size+1))
	{
		v = vector_get_struct_generic(data);
		if(element)
			ret = memcpy(*(char**)data + (v->size-1) * v->item_size, element, v->item_size);
		else
			ret = vector_get_generic(data, v->size-1);
	}
	return ret;
}

/* REMEMBER TO GET A NEW VECTOR AFTER A CALL TO THIS FUNCTION */
int vector_pop_generic(void **data, void *store);

vector_header_t *vector_get_struct_generic(void **data)
{
	return (vector_header_t*)(*(char**)data - sizeof(vector_header_t));
}
