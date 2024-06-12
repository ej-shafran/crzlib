#ifndef CRZHASH_H_
#define CRZHASH_H_

#ifndef CRZ_STRING
#define CRZ_STRING char *
#endif // CRZ_STRING

#ifndef CRZ_STRING_GET
#define CRZ_STRING_GET(str, index) str[index]
#endif // CRZ_STRING_GET

#ifndef CRZ_STRING_IS_END
#define CRZ_STRING_IS_END(str, index) (CRZ_STRING_GET(str, index) == '\0')
#endif // CRZ_STRING_IS_END

#ifndef CRZ_STRING_FOR
#define CRZ_STRING_FOR(str, index) \
	for (CRZ_SIZE index = 0; !CRZ_STRING_IS_END(str, index); index++)
#endif // CRZ_STRING_FOR

#ifndef CRZ_STRING_EQ
#include <string.h>
#define CRZ_STRING_EQ(a, b) (strcmp(a, b) == 0)
#endif // CRZ_STRING_EQ

#ifndef CRZ_STRDUP
#include <string.h>
#define CRZ_STRDUP strdup
#endif // CRZ_STRDUP

#ifndef CRZ_SIZE
#include <stddef.h>
#define CRZ_SIZE size_t
#endif // CRZ_SIZE

#ifndef CRZ_MALLOC
#include <stdlib.h>
#define CRZ_MALLOC malloc
#endif // CRZ_MALLOC

#ifndef CRZ_ASSERT
#include <assert.h>
#define CRZ_ASSERT assert
#endif // CRZ_ASSERT

#ifndef CRZ_FREE
#include <stdlib.h>
#define CRZ_FREE free
#endif // CRZ_FREE

#ifndef CRZ_NULL
#include <stddef.h>
#define CRZ_NULL NULL
#endif // CRZ_NULL

#ifndef CRZ_BOOL
#include <stdbool.h>
#define CRZ_BOOL bool
#endif // CRZ_BOOL

#ifndef CRZ_TRUE
#include <stdbool.h>
#define CRZ_TRUE true
#endif // CRZ_TRUE

#ifndef CRZ_FALSE
#include <stdbool.h>
#define CRZ_FALSE false
#endif // CRZ_FALSE

#ifndef CRZ_DEBUG
#include <stdio.h>
#define CRZ_DEBUG printf
#endif // CRZ_DEBUG

#define HASH_PAIR(T)            \
	struct {                \
		T value;        \
		CRZ_STRING key; \
	}

#define HASH_TABLE(T)                \
	struct {                     \
		HASH_PAIR(T) * *ptr; \
		CRZ_SIZE size;       \
	}

typedef HASH_PAIR(char) AnyHashPair;

typedef struct {
	void **ptr;
	CRZ_SIZE size;
} AnyHashTable;

#define HASH_TABLE_PAIR_SIZE(self) sizeof(**(self).ptr)

#define HASH_TABLE_NEW()               \
	{                              \
		.ptr = NULL, .size = 0 \
	}

#define HASH_TABLE_INIT(selfp, initial_size)                          \
	crz__hash_table_init((AnyHashTable *)(selfp), (initial_size), \
			     HASH_TABLE_PAIR_SIZE(*(selfp)))

#define HASH_TABLE_INSERT(selfp, insert_key, insert_value)             \
	do {                                                           \
		CRZ_SIZE index = crz__hash_table_insert_index(         \
			(AnyHashTable *)(selfp), (insert_key),         \
			HASH_TABLE_PAIR_SIZE(*(selfp)));               \
		(selfp)->ptr[index] = CRZ_MALLOC(sizeof(AnyHashPair)); \
		(selfp)->ptr[index]->key = CRZ_STRDUP(insert_key);     \
		(selfp)->ptr[index]->value = insert_value;             \
	} while (0)

#define HASH_TABLE_GET(self, get_key) \
	crz__hash_table_get((AnyHashTable *)(&(self)), get_key)

#define HASH_TABLE_FOR(self, index) \
	for (CRZ_SIZE index = 0; index < (self).size; index++)

#define HASH_TABLE_FREE(selfp)                                  \
	do {                                                    \
		for (CRZ_SIZE i = 0; i < (selfp)->size; i++) {  \
			if ((selfp)->ptr[i]) {                  \
				CRZ_FREE((selfp)->ptr[i]->key); \
				CRZ_FREE((selfp)->ptr[i]);      \
			}                                       \
		}                                               \
		CRZ_FREE((selfp)->ptr);                         \
		(selfp)->ptr = NULL;                            \
		(selfp)->size = 0;                              \
	} while (0)

#define HASH_TABLE_DEBUG(self, fmt)                             \
	do {                                                    \
		CRZ_DEBUG("{");                                 \
		HASH_TABLE_FOR(self, i) {                       \
			if ((self).ptr[i]) {                    \
				CRZ_DEBUG(" \"%s\" = " fmt ";", \
					  self.ptr[i]->key,     \
					  self.ptr[i]->value);  \
			}                                       \
		}                                               \
		CRZ_DEBUG(" }\n");                              \
	} while (0)

void crz__hash_table_init(AnyHashTable *selfp, CRZ_SIZE initial_size,
			  CRZ_SIZE pair_size);

CRZ_SIZE crz__djb2_hash(CRZ_STRING str);

CRZ_SIZE crz__hash_table_insert_index(AnyHashTable *selfp, CRZ_STRING key,
				      CRZ_SIZE pair_size);

void crz__hash_table_clone(AnyHashTable self, AnyHashTable *other,
			   CRZ_SIZE pair_size);

void *crz__hash_table_get(AnyHashTable *selfp, CRZ_STRING key);

void crz__hash_table_init(AnyHashTable *selfp, CRZ_SIZE initial_size,
			  CRZ_SIZE pair_size)
{
	(selfp)->size = initial_size;
	(selfp)->ptr = CRZ_MALLOC(pair_size * initial_size);
	CRZ_ASSERT((selfp)->ptr && "Out of memory when allocating hash table");
	for (CRZ_SIZE i = 0; i < initial_size; i++) {
		(selfp)->ptr[i] = NULL;
	}
}

CRZ_SIZE crz__djb2_hash(CRZ_STRING str)
{
	CRZ_SIZE hash = 5381;
	CRZ_STRING_FOR(str, i) {
		hash = ((hash << 5) + hash) + CRZ_STRING_GET(str, i);
	}
	return hash;
}

CRZ_SIZE crz__hash_table_insert_index(AnyHashTable *selfp, CRZ_STRING key,
				      CRZ_SIZE pair_size)
{
	CRZ_SIZE index = crz__djb2_hash(key) % (selfp)->size;
	CRZ_BOOL wrapped = CRZ_FALSE;
	CRZ_SIZE original = index;

	while (CRZ_TRUE) {
		AnyHashPair *existing = selfp->ptr[index];

		if (existing == CRZ_NULL || CRZ_STRING_EQ(existing->key, key)) {
			return index;
		}

		index += 1;

		if (index >= selfp->size && !wrapped) {
			index = 0;
			wrapped = CRZ_TRUE;
			continue;
		}

		if (index >= original && wrapped) {
			AnyHashTable new = HASH_TABLE_NEW();
			crz__hash_table_init(&new, (selfp->size * 2),
					     pair_size);
			crz__hash_table_clone(*selfp, &new, pair_size);

			// Free pointer itself, but not the pairs
			// since they've been copied over to the new table
			CRZ_FREE((selfp)->ptr);
			(*selfp) = new;

			return crz__hash_table_insert_index(&new, key,
							    pair_size);
		}
	}
}

void crz__hash_table_clone(AnyHashTable self, AnyHashTable *other,
			   CRZ_SIZE pair_size)
{
	for (CRZ_SIZE i = 0; i < self.size; i++) {
		AnyHashPair *existing = self.ptr[i];
		if (existing) {
			CRZ_SIZE index = crz__hash_table_insert_index(
				other, existing->key, pair_size);
			other->ptr[index] = existing;
		}
	}
}

void *crz__hash_table_get(AnyHashTable *selfp, CRZ_STRING key)
{
	CRZ_SIZE index = crz__djb2_hash(key) % selfp->size;
	CRZ_BOOL wrapped = CRZ_FALSE;
	CRZ_SIZE original = index;

	while (CRZ_TRUE) {
		AnyHashPair *existing = selfp->ptr[index];

		if (existing != CRZ_NULL && CRZ_STRING_EQ(existing->key, key)) {
			return existing;
		}

		index += 1;

		if (index >= selfp->size && !wrapped) {
			index = 0;
			wrapped = CRZ_TRUE;
			continue;
		}

		if (index >= original && wrapped) {
			return CRZ_NULL;
		}
	}
}

#endif // CRZHASH_H_
