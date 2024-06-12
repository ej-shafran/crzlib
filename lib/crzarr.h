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

#ifndef CRZ_ARRAY_MINIMUM_CAPACITY
#define CRZ_ARRAY_MINIMUM_CAPACITY 8
#endif // CRZ_ARRAY_MINIMUM_CAPACITY

#define ARRAY(T)              \
	struct {              \
		T *ptr;       \
		CRZ_SIZE len; \
		CRZ_SIZE cap; \
	}

typedef ARRAY(void) AnyArray;

#define ARRAY_ELEMENT_SIZE(self) (sizeof(*(self).ptr))

#define ARRAY_NEW()                                 \
	{                                           \
		.ptr = CRZ_NULL, .len = 0, .cap = 0 \
	}

#define ARRAY_INIT(selfp) \
	((selfp)->ptr = CRZ_NULL, (selfp)->len = 0, (selfp)->cap = 0)

#define ARRAY_GROW_BY(selfp, amount)                                   \
	crz__array_grow_to((AnyArray *)(selfp), (selfp)->cap + amount, \
			   ARRAY_ELEMENT_SIZE(*(selfp)))

#define ARRAY_PUSH(selfp, element)                                        \
	do {                                                              \
		crz__array_grow_to((AnyArray *)(selfp), (selfp)->len + 1, \
				   ARRAY_ELEMENT_SIZE(*(selfp)));         \
		(selfp)->ptr[(selfp)->len++] = element;                   \
	} while (0)

#define ARRAY_GET(self, index)                                    \
	(CRZ_ASSERT(index < (self).len && "Index out of bounds"), \
	 (self).ptr[index])

#define ARRAY_PUSH_MANY(selfp, contentsp, count)                     \
	do {                                                         \
		ARRAY_GROW_BY((selfp), (count));                     \
		CRZ_MEMCPY((selfp)->ptr + (selfp)->len, (contentsp), \
			   (count) * ARRAY_ELEMENT_SIZE(*(selfp)));  \
		(selfp)->len += (count);                             \
	} while (0)

#define ARRAY_PUSH_OTHER(selfp, other) \
	ARRAY_PUSH_MANY((selfp), (other).ptr, (other).len)

#define ARRAY_SPLICE(selfp, index, remove_amount, contentsp, count)  \
	crz__array_splice((AnyArray *)(selfp), index, remove_amount, \
			  contentsp, (count), ARRAY_ELEMENT_SIZE(*(selfp)))

#define ARRAY_POP(selfp) ((selfp)->ptr[--(selfp)->len])

#define ARRAY_REMOVE(selfp, index, amount) \
	ARRAY_SPLICE(selfp, index, amount, NULL, 0)

#define ARRAY_FREE(selfp)               \
	do {                            \
		CRZ_FREE((selfp)->ptr); \
		ARRAY_INIT((selfp));    \
	} while (0)

#define ARRAY_FOR(self, name) for (CRZ_SIZE name = 0; name < (self).len; name++)

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

void crz__array_grow_to(AnyArray *selfp, CRZ_SIZE new_cap,
			CRZ_SIZE element_size)
{
	if (selfp->cap < new_cap) {
		selfp->cap *= 2;
		if (selfp->cap < CRZ_ARRAY_MINIMUM_CAPACITY)
			selfp->cap = CRZ_ARRAY_MINIMUM_CAPACITY;
		if (selfp->cap < new_cap)
			selfp->cap = new_cap;
		selfp->ptr = CRZ_REALLOC(selfp->ptr, element_size * selfp->cap);
		CRZ_ASSERT(selfp->ptr &&
			   "Out of memory when reallocating array");
	}
}

void crz__array_splice(AnyArray *selfp, CRZ_SIZE index, CRZ_SIZE remove_amount,
		       void *contentsp, CRZ_SIZE count, CRZ_SIZE element_size)
{
	CRZ_ASSERT(index + remove_amount <= selfp->len &&
		   "Index out of bounds");

	CRZ_SIZE new_length = selfp->len + count - remove_amount;
	crz__array_grow_to(selfp, new_length, element_size);

	// Cast to non-void pointer;
	// requires using `* element_size` for pointer arithmetic but allows doing pointer arithmetic...
	char *ptr = (char *)selfp->ptr;

	// Move the block of memory in one go to make room for new elements
	memmove(ptr + (index + count) * element_size,
		ptr + (index + remove_amount) * element_size,
		(selfp->len - index - remove_amount) * element_size);

	// Copy the new elements into the now-free space
	memcpy(ptr + index * element_size, contentsp, element_size * count);

	// Update the length of the array
	selfp->len = new_length;
}

#endif // CRZARR_H_
