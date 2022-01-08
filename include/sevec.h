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

typedef struct vector
{
	size_t size;
	size_t max_size;
	size_t capacity;
	size_t item_size;
	void *data;
}
vector_t;

/* note: return 0 on success, NULL or -1 on failure */

/* if max_size is 0, ignore it */
__attribute__((warn_unused_result))
vector_t *vector_create_generic(size_t capacity, size_t item_size, size_t max_size);
void vector_destroy_generic(vector_t *);

/* if size > capacity, realloc */
int vector_resize_generic(vector_t *, size_t new_size);
/* if new_capacity is less or equal old_capacity, do nothing */
int vector_reserve_generic(vector_t *, size_t new_capacity);
/* if new_capacity is greater or equal old_capacity, do nothing */
int vector_shrink_generic(vector_t *, size_t new_capacity);

/* if const void* is NULL, do not copy data and do not zero new elements, but do resize */
void *vector_push_generic(vector_t *, const void *);
/* if void* is NULL, do not copy data, but do resize */
int vector_pop_generic(vector_t *, void *);

__attribute__((warn_unused_result))
void *vector_start_generic(vector_t *);
__attribute__((warn_unused_result))
void *vector_end_generic(vector_t *);

__attribute__((warn_unused_result))
void *vector_get_generic(vector_t *, size_t index);
__attribute__((warn_unused_result))
size_t vector_index_generic(vector_t *, const void *);

/* Helper funcions for type-safe vectors */
__attribute__((warn_unused_result))
size_t vector_size_generic(vector_t *);
__attribute__((warn_unused_result))
size_t vector_capacity_generic(vector_t *);
__attribute__((warn_unused_result))
size_t vector_max_size_generic(vector_t *);


#define vector_create_definitions_of_type(type) \
	typedef struct vector_##type{struct vector vec;}vector_##type##_t; \
	\
	__attribute__((warn_unused_result)) \
	static inline vector_##type##_t *vector_create_##type(size_t capacity, size_t max_size) \
		{ return (vector_##type##_t*)vector_create_generic(capacity, sizeof(type), max_size); } \
	static inline void vector_destroy_##type(vector_##type##_t *v) \
		{ vector_destroy_generic((vector_t*)v); } \
	\
	static inline int vector_resize_##type(vector_##type##_t *v, size_t new_size) \
		{ return vector_resize_generic((vector_t*)v, new_size); } \
	static inline int vector_reserve_##type(vector_##type##_t *v, size_t new_capacity) \
		{ return vector_reserve_generic((vector_t*)v, new_capacity); } \
	static inline int vector_shrink_##type(vector_##type##_t *v, size_t new_capacity) \
		{ return vector_shrink_generic((vector_t*)v, new_capacity); } \
	\
	static inline type *vector_push_##type(vector_##type##_t *v, const type *e) \
		{ return (type*)vector_push_generic((vector_t*)v, (const void*)e); } \
	static inline int vector_pop_##type(vector_##type##_t *v, type *e) \
		{ return vector_pop_generic((vector_t*)v, (void*)e); } \
	\
	__attribute__((warn_unused_result)) \
	static inline type *vector_start_##type(vector_##type##_t *v) \
		{ return (type*)vector_start_generic((vector_t*)v); } \
	__attribute__((warn_unused_result)) \
	static inline type *vector_end_##type(vector_##type##_t *v) \
		{ return vector_end_generic((vector_t*)v); } \
	\
	__attribute__((warn_unused_result)) \
	static inline type *vector_get_##type(vector_##type##_t *v, size_t index) \
		{ return (type*)vector_get_generic((vector_t*)v, index); } \
	__attribute__((warn_unused_result)) \
	static inline size_t vector_index_##type(vector_##type##_t *v, const type *e) \
		{ return vector_index_generic((vector_t*)v, (const void*)e); } \
	\
	__attribute__((warn_unused_result)) \
	static inline size_t vector_size_##type(vector_##type##_t *v) \
		{ return vector_size_generic((vector_t*)v); } \
	__attribute__((warn_unused_result)) \
	static inline size_t vector_capacity_##type(vector_##type##_t *v) \
		{ return vector_capacity_generic((vector_t*)v); } \
	__attribute__((warn_unused_result)) \
	static inline size_t vector_max_size_##type(vector_##type##_t *v) \
		{ return vector_max_size_generic((vector_t*)v); }


#endif
