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
#ifndef SEVEC_H
#define SEVEC_H
#include <sys/types.h>

typedef struct vector_header
{
	size_t size;
	size_t max_size;
	size_t capacity;
	size_t item_size;
}
vector_header_t;

int vector_create_generic(void **data, size_t item_size, size_t capacity, size_t max_size); /* if max_size is 0, set it to -1 */
void vector_destroy_generic(void **data);

int vector_resize_generic(void **data, size_t new_size);
int vector_reserve_generic(void **data, size_t new_capacity);
int vector_shrink_generic(void **data, size_t new_capacity);

__attribute__((warn_unused_result))
void *vector_get_generic(void **data, size_t index);

void *vector_push_generic(void **data, const void *);
int vector_pop_generic(void **data, void *);

__attribute__((warn_unused_result))
vector_header_t *vector_get_struct_generic(void **data);

/* Compare types of a and b */
#define SEVEC_ASSERT_SAME_TYPE(type, v) ((void*) (1 ? v : (__typeof__(type))0))
/* Check if x is at least of type "(type)**" */
#define SEVEC_ASSERT_DATA(data) (void**)(1 ? (void**)data : (void**)&(**data))


#define vector_create(data, capacity, max_size) \
		vector_create_generic(SEVEC_ASSERT_DATA(data), sizeof(**data), capacity, max_size) \

#define vector_destroy(data) \
		vector_destroy_generic(SEVEC_ASSERT_DATA(data)) \


#define vector_resize(data, new_size) \
		vector_resize_generic(SEVEC_ASSERT_DATA(data), new_size)

#define vector_reserve(data, new_capacity) \
		vector_reserve_generic(SEVEC_ASSERT_DATA(data), new_capacity)

#define vector_shrink(data, new_capacity) \
		vector_shrink_generic(SEVEC_ASSERT_DATA(data), new_capacity)


#define vector_get(data, index) \
		vector_get_generic(SEVEC_ASSERT_DATA(data), index)


#define vector_push(data, element) \
		vector_push_generic(SEVEC_ASSERT_DATA(data), SEVEC_ASSERT_SAME_TYPE(*data, element)) \

#define vector_pop(data, store) \
		vector_pop_generic(SEVEC_ASSERT_DATA(data), SEVEC_ASSERT_SAME_TYPE(*data, element))

#endif
