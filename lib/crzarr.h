#ifndef CRZARR_H_
#define CRZARR_H_

#ifndef CRZ_SIZE
#include <stddef.h>
#define CRZ_SIZE size_t
#endif // CRZ_SIZE

#ifndef CRZ_SIZE_FMT
#define CRZ_SIZE_FMT "%zu"
#endif // CRZ_SIZE_FMT

#ifndef CRZ_NULL
#include <stddef.h>
#define CRZ_NULL NULL
#endif // CRZ_NULL

#ifndef CRZ_REALLOC
#include <stdlib.h>
#define CRZ_REALLOC realloc
#endif // CRZ_REALLOC

#ifndef CRZ_ASSERT
#include <assert.h>
#define CRZ_ASSERT assert
#endif // CRZ_ASSERT

#ifndef CRZ_FREE
#include <stdlib.h>
#define CRZ_FREE free
#endif // CRZ_FREE

#ifndef CRZ_MEMCPY
#include <string.h>
#define CRZ_MEMCPY memcpy
#endif // CRZ_MEMCPY

#ifndef CRZ_DEBUG
#include <stdio.h>
#define CRZ_DEBUG printf
#endif // CRZ_DEBUG

#ifndef CRZARR_MINIMUM_CAPACITY
#define CRZARR_MINIMUM_CAPACITY 8
#endif // CRZARR_MINIMUM_CAPACITY

/// Define a struct for a dynamic array with elements of type `T`.
#define ARRAY(T)              \
	struct {              \
		T *ptr;       \
		CRZ_SIZE len; \
		CRZ_SIZE cap; \
	}

/// INTERNAL: you most likely don't want to use this.
///           Try `ARRAY(T)` instead.
typedef ARRAY(void) Crzarr_AnyArray;

/// Get the size in bytes of each element of the array `self`.
/// Uses `sizeof` internally.
#define ARRAY_ELEMENT_SIZE(self) (sizeof(*(self).ptr))

/// Zero-initialize a dynamic array.
#define ARRAY_NEW()                                 \
	{                                           \
		.ptr = CRZ_NULL, .len = 0, .cap = 0 \
	}

/// Properly initialize a dynamic array.
///
/// This is not required, and does not perform any allocations.
#define ARRAY_INIT(selfp) \
	((selfp)->ptr = CRZ_NULL, (selfp)->len = 0, (selfp)->cap = 0)

/// Reserve space for `amount_elements` more elements in the dynamic array `selfp` (passed by pointer).
/// Reallocates the array to add the additional space.
#define ARRAY_GROW_BY(selfp, amount_elements)            \
	crzarr_grow_to((Crzarr_AnyArray *)(selfp),       \
		       (selfp)->cap + (amount_elements), \
		       ARRAY_ELEMENT_SIZE(*(selfp)))

/// Push the element `element` into the dynamic array `selfp` (passed by pointer).
///
/// Reallocates the array to create the necessary additional space, if needed.
#define ARRAY_PUSH(selfp, element)                                           \
	do {                                                                 \
		crzarr_grow_to((Crzarr_AnyArray *)(selfp), (selfp)->len + 1, \
			       ARRAY_ELEMENT_SIZE(*(selfp)));                \
		(selfp)->ptr[(selfp)->len++] = element;                      \
	} while (0)

/// Get the element at `index` within the dynamic array `selfp` (passed by pointer).
///
/// Asserts that the index is not out of the bounds of the array.
#define ARRAY_GET(self, index)                                    \
	(CRZ_ASSERT(index < (self).len && "Index out of bounds"), \
	 (self).ptr[index])

/// Push `count_elements` elements from `contentsp` (passed by pointer) into the dynamic array `selfp` (passed by pointer).
///
/// Reallocates the array to create the necessary additional space, if needed.
/// Copies the memory from `contentsp` using `CRZ_MEMCPY`.
#define ARRAY_PUSH_MANY(selfp, contentsp, count_elements)                    \
	do {                                                                 \
		crzarr_grow_to((Crzarr_AnyArray *)(selfp),                   \
			       (selfp)->len + (count_elements),              \
			       ARRAY_ELEMENT_SIZE(*(selfp)));                \
		CRZ_MEMCPY((selfp)->ptr + (selfp)->len, (contentsp),         \
			   (count_elements) * ARRAY_ELEMENT_SIZE(*(selfp))); \
		(selfp)->len += (count_elements);                            \
	} while (0)

/// Push the contents of the dynamic array `other` into the dynamic array `selfp` (passed by pointer).
///
/// Uses `ARRAY_PUSH_MANY` internally.
#define ARRAY_PUSH_OTHER(selfp, other) \
	ARRAY_PUSH_MANY((selfp), (other).ptr, (other).len)

/// Remove `remove_amount_elements` elements from `index` of dynamic array `selfp` (passed by pointer).
/// Then, insert `count_elements` elements from `contentsp` (passed by pointer) into `index` of `selfp`.
///
/// This moves the other elements of `selfp` as needed, and reallocates the array to create the necessary additional space, if needed.
/// Copies the memory from `contentsp` using `CRZ_MEMCPY`.
#define ARRAY_SPLICE(selfp, index, remove_amount_elements, contentsp,      \
		     count_elements)                                       \
	crzarr_splice((Crzarr_AnyArray *)(selfp), index,                   \
		      remove_amount_elements, contentsp, (count_elements), \
		      ARRAY_ELEMENT_SIZE(*(selfp)))

/// Remove and return the last element of the dynamic array `selfp` (passed by pointer).
#define ARRAY_POP(selfp) ((selfp)->ptr[--(selfp)->len])

/// Remove `amount_elements` elements from `index` of dynamic array `selfp` (passed by pointer).
///
/// This moves the other elements of `selfp` as needed.
#define ARRAY_REMOVE(selfp, index, amount_elements) \
	ARRAY_SPLICE(selfp, index, amount_elements, NULL, 0)

/// Insert `count_elements` elements from `contentsp` (passed by pointer) into `index` of dynamic array `selfp` (passed by pointer).
///
/// This moves the other elements of `selfp` as needed, and reallocates the array to create the necessary additional space, if needed.
/// Copies the memory from `contentsp` using `CRZ_MEMCPY`.
#define ARRAY_INSERT(selfp, index, contentsp, count_elements) \
	ARRAY_SPLICE(selfp, index, 0, contentsp, count_elements)

/// Iterate over the indexes of the dynamic array `self`, naming the index variable `index`.
#define ARRAY_FOR(self, index) \
	for (CRZ_SIZE index = 0; index < (self).len; index++)

/// Print out the elements of dynamic array `self`. `fmt` is the format specifier for the element type, as for `printf`.
#define ARRAY_DEBUG(self, fmt)                                           \
	do {                                                             \
		CRZ_DEBUG("[");                                          \
		for (CRZ_SIZE crz__index = 0; crz__index < (self).len;   \
		     crz__index++) {                                     \
			CRZ_DEBUG((fmt), ARRAY_GET((self), crz__index)); \
			if (crz__index + 1 != (self).len) {              \
				CRZ_DEBUG(", ");                         \
			}                                                \
		}                                                        \
		CRZ_DEBUG("]\n");                                        \
	} while (0)

/// Free the space allocated for the elements of the dynamic array `selfp` (passed by pointer), and empty-out the fields of the struct.
///
/// This **does not** free any of the elements of `selfp` - if they are dynamically allocated, you must do this yourself before calling `ARRAY_FREE`.
#define ARRAY_FREE(selfp)               \
	do {                            \
		CRZ_FREE((selfp)->ptr); \
		ARRAY_INIT((selfp));    \
	} while (0)

/// INTERNAL: you most likely don't want to use this.
///           Try `ARRAY_GROW_BY(selfp, amount_elements)` instead.
void crzarr_grow_to(Crzarr_AnyArray *selfp, CRZ_SIZE new_cap,
		    CRZ_SIZE element_size);

/// INTERNAL: you most likely don't want to use this.
///           Try `ARRAY_SPLICE(selfp, index, remove_amount_elements, contentsp, count_elements)` instead.
void crzarr_splice(Crzarr_AnyArray *selfp, CRZ_SIZE index,
		   CRZ_SIZE remove_amount_elements, void *contentsp,
		   CRZ_SIZE count_elements, CRZ_SIZE element_size);

void crzarr_grow_to(Crzarr_AnyArray *selfp, CRZ_SIZE new_cap,
		    CRZ_SIZE element_size)
{
	if (selfp->cap < new_cap) {
		selfp->cap *= 2;
		if (selfp->cap < CRZARR_MINIMUM_CAPACITY)
			selfp->cap = CRZARR_MINIMUM_CAPACITY;
		if (selfp->cap < new_cap)
			selfp->cap = new_cap;
		selfp->ptr = CRZ_REALLOC(selfp->ptr, element_size * selfp->cap);
		CRZ_ASSERT(selfp->ptr &&
			   "Out of memory when reallocating array");
	}
}

void crzarr_splice(Crzarr_AnyArray *selfp, CRZ_SIZE index,
		   CRZ_SIZE remove_amount_elements, void *contentsp,
		   CRZ_SIZE count_elements, CRZ_SIZE element_size)
{
	CRZ_ASSERT(index + remove_amount_elements <= selfp->len &&
		   "Index out of bounds");

	CRZ_SIZE new_length =
		selfp->len + count_elements - remove_amount_elements;
	crzarr_grow_to(selfp, new_length, element_size);

	// Cast to non-void pointer;
	// requires using `* element_size` for pointer arithmetic but allows doing pointer arithmetic...
	char *ptr = (char *)selfp->ptr;

	// Move the block of memory in one go to make room for new elements
	memmove(ptr + (index + count_elements) * element_size,
		ptr + (index + remove_amount_elements) * element_size,
		(selfp->len - index - remove_amount_elements) * element_size);

	// Copy the new elements into the now-free space
	memcpy(ptr + index * element_size, contentsp,
	       element_size * count_elements);

	selfp->len = new_length;
}

#endif // CRZARR_H_
