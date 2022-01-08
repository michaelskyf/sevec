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

vector_t *vector_create_generic(size_t capacity, size_t item_size, size_t max_size)
{
	vector_t *v = malloc(sizeof(vector_t));

	if(v)
	{
		v->data = malloc(capacity * item_size);
		if(!v->data)
		{
			free(v);
			return NULL;
		}
		v->size = 0;
		v->max_size = max_size;
		v->capacity = capacity;
		v->item_size = item_size;
	}
	return v;
}

void vector_destroy_generic(vector_t *v)
{
	free(v->data);
	free(v);
}

/* if size > capacity, realloc */
int vector_resize_generic(vector_t *v, size_t new_size)
{
	int ret = 0;
	size_t new_capacity = v->capacity;

	if(new_size > v->capacity)
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

		ret = vector_reserve_generic(v, new_capacity);
	}
	if(!ret)
		v->size++;
	return ret;
}

/* if new_capacity is less or equal old_capacity, do nothing */
int vector_reserve_generic(vector_t *v, size_t new_capacity)
{
	if(new_capacity > v->max_size && v->max_size > 0)
		return -1;

	if(new_capacity <= v->capacity)
		return 0;

	void *new_data = realloc(v->data, new_capacity * v->item_size);

	if(!new_data)
		return -1;

	v->data = new_data;
	v->capacity = new_capacity;

	return 0;
}

/* if new_capacity is greater or equal old_capacity, do nothing */
int vector_shrink_generic(vector_t *v, size_t new_capacity)
{
	if(new_capacity >= v->capacity)
		return 0;

	void *new_data = realloc(v->data, new_capacity * v->item_size);

	if(!new_data)
		return -1;

	v->data = new_data;
	v->capacity = new_capacity;

	return 0;
}

/* if const void* is NULL, do not copy data and do not zero new elements, but do resize */
void *vector_push_generic(vector_t *v, const void *e)
{
	void *ret = NULL;
	if(!vector_resize_generic(v, v->size + 1))
	{
		if(e)
			ret = memcpy((char*)v->data + (v->size-1) * v->item_size, e, v->item_size);
		else
			ret = vector_get_generic(v, v->size);
	}
	return ret;
}

/* if void* is NULL, do not copy data */
int vector_pop_generic(vector_t *v, void *e)
{
	if(v->size == 0)
		return -1;

	if(e)
		memcpy(e, (char*)v->data + v->size*v->item_size, v->item_size);

	v->size--;

	return 0;
}

void *vector_start_generic(vector_t *v)
{
	return v->data;
}

void *vector_end_generic(vector_t *v)
{
	return (char*)v->data + v->size*v->item_size;
}

void *vector_get_generic(vector_t *v, size_t index)
{
	return (char*)v->data + index*v->item_size;
}

/* Calculate offset (-1 on error despite it being size_t) */
size_t vector_index_generic(vector_t *v, const void *e)
{
	if(e >= vector_start_generic(v) && e <= vector_end_generic(v))
		return ((char*)e-(char*)v->data)/v->item_size;
	return -1;
}

/* Helper funcions for type-safe vectors */
size_t vector_size_generic(vector_t *v)
{ return v->size; }
size_t vector_capacity_generic(vector_t *v)
{ return v->capacity; }
size_t vector_max_size_generic(vector_t *v)
{ return v->max_size; }
