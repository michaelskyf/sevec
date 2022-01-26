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

typedef struct
{
	/** Current size of the vector */
	size_t size;
	/** Maximum size that vector can have */
	size_t max_size;
	/** Current capacity of the vector */
	size_t capacity;
	/** Size of a single element */
	size_t item_size;
	/** How much vector will expand on realloc */
	float growth_rate;
	/** Pointer to data */
	unsigned char *memory;
/**
 * @brief Struct that stores vector's internals
 */
} vector_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a vector
 *
 * @param[out]			data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			item_size	Size of a single element" in vector
 * @param[in]			capacity	Initial capacity of the vector (could be set to 0)
 * @param[in]			max_size	Maximum size of the vector (if set to 0, it is unlimited)
 * @param[in]			growth_rate	Must be >= 1 (or it will get set to default)
 * @returns					0 on success, -1 on failure (more info via errno)
 */
__attribute__((warn_unused_result))
int vector_create_generic(void **data, size_t item_size, size_t capacity, size_t max_size, float growth_rate);
/**
 * @brief Destroy a vector and set pointer to vector data to NULL
 *
 * @param[in, out]		data		Pointer to \ref vector_t::data "vector data"
 */
void vector_destroy_generic(void **data);

/**
 * @brief Resize a vector
 *
 * @param[in, out]		data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			new_size	New size of the vector limited by its \ref vector_t::max_size "max_size"
 * @returns					0 on success, -1 on failure (more info via errno) \n
 *						Failure on new_size > max_size
 */
__attribute__((warn_unused_result))
int vector_resize_generic(void **data, size_t new_size);
/**
 * @brief Expand vector (reserve more memory)
 *
 * @param[in, out]		data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			new_capacity	New capacity of the vector (if new_capacity > old_capacity)
 * @returns					0 on success, -1 on failure (more info via errno) \n
 *						Success on new_capacity <= old_capacity \n
 *						Failure on new_capacity > max_size
 */
__attribute__((warn_unused_result))
int vector_reserve_generic(void **data, size_t new_capacity);
/**
 * @brief Shrink vector (use less memory)
 *
 * @param[in, out]		data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			new_capacity	New capacity of the vector (if new_capacity < old_capacity)
 * @returns					0 on success, -1 on failure (more info via errno) \n
 *						Success on new_capacity >= old_capacity
 */
int vector_shrink_generic(void **data, size_t new_capacity);

/**
 * @brief Get element at index
 *
 * @param[in]			data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			index
 * @returns					Pointer to element on success, NULL on failure
 */
__attribute__((warn_unused_result))
void *vector_get_generic(void **data, size_t index);

/**
 * @brief Push (copy) element to vector
 *
 * @param[in, out]		data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			element		Element to copy (if NULL, does not copy)
 * @returns					Pointer to copied element on success, NULL on failure (more info via errno)
 */
void *vector_push_generic(void **data, const void *element);
/**
 * @brief Push (copy) element to vector
 *
 * @param[in]			data		Pointer to \ref vector_t::data "vector data"
 * @param[in]			store		Storage for popped object (if NULL, does not copy)
 * @returns					0 on success, -1 on failure \n
 *						Fails if vector size is 0
 */
int vector_pop_generic(void **data, void *store);

/**
 * @brief Get location of pointer to vector_t
 *
 * @param[in]			data		Pointer to \ref vector_t::data "vector data"
 * @returns					Pointer to pointer to \ref vector_t
 */
__attribute__((warn_unused_result))
vector_t **vector_get_memory_generic(void **data);

/**
 * @brief Get vector size
 *
 * @param[in]			data		Pointer to \ref vector_t::data "vector data"
 * @returns					Size of the vector
 */
__attribute__((warn_unused_result))
size_t vector_size_generic(void **data);

#ifdef __cplusplus
}
#endif

/* Compare types of data and v */
#define SEVEC_ASSERT_SAME_TYPE(data, v) ((void*) (1 ? v : (__typeof__(data))0))
/* Check if data is at least of type "(type)**" */
#define SEVEC_ASSERT_DATA(data) (void**)(1 ? (void**)data : (void**)&(**data))
/* If a function returns void*, make sure to cast it to type* */
#define SEVEC_RETURN_TYPE(data) (__typeof__(*data))

#define vector_create(data, capacity, max_size, growth_rate) \
		vector_create_generic(SEVEC_ASSERT_DATA(data), sizeof(**data), capacity, max_size, growth_rate) \

#define vector_destroy(data) \
		vector_destroy_generic(SEVEC_ASSERT_DATA(data)) \


#define vector_resize(data, new_size) \
		vector_resize_generic(SEVEC_ASSERT_DATA(data), new_size)

#define vector_reserve(data, new_capacity) \
		vector_reserve_generic(SEVEC_ASSERT_DATA(data), new_capacity)

#define vector_shrink(data, new_capacity) \
		vector_shrink_generic(SEVEC_ASSERT_DATA(data), new_capacity)


#define vector_get(data, index) \
		SEVEC_RETURN_TYPE(data) \
		vector_get_generic(SEVEC_ASSERT_DATA(data), index)


#define vector_push(data, element) \
		SEVEC_RETURN_TYPE(data) \
		vector_push_generic(SEVEC_ASSERT_DATA(data), SEVEC_ASSERT_SAME_TYPE(*data, element)) \

#define vector_pop(data, store) \
		vector_pop_generic(SEVEC_ASSERT_DATA(data), SEVEC_ASSERT_SAME_TYPE(*data, store))

#define vector_get_memory(data) \
		vector_get_memory_generic(SEVEC_ASSERT_DATA(data))

#define vector_size(data) \
		vector_size_generic(SEVEC_ASSERT_DATA(data))

#endif
