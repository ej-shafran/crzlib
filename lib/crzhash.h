#ifndef CRZHASH_H_
#define CRZHASH_H_

#include "crzdef.h"

/// Define a struct for a key-value pair retrieved from a hash table, with value of type `T`.
#define HASH_PAIR(T)            \
	struct {                \
		T value;        \
		CRZ_STRING key; \
	}

/// Define a struct for a hash table with values of type `T`.
#define HASH_TABLE(T)                \
	struct {                     \
		HASH_PAIR(T) * *ptr; \
		CRZ_SIZE size;       \
	}

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_PAIR(T)` instead.
typedef HASH_PAIR(char) Crzhash_AnyHashPair;

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_TABLE(T)` instead.
typedef struct {
	void **ptr;
	CRZ_SIZE size;
} Crzhash_AnyHashTable;

/// Get the size in bytes of each key-value pair of the hash table `self`.
#define HASH_TABLE_PAIR_SIZE(self) sizeof(**(self).ptr)

/// Zero-initialize a hash table.
///
/// Requires calling `HASH_TABLE_INIT` afterwards.
#define HASH_TABLE_NEW()               \
	{                              \
		.ptr = NULL, .size = 0 \
	}

/// Properly initialize a hash table.
///
/// Gives the table an initial size of `initial_size` pairs.
/// Note that using too small a size here can cause collisions and overflows, which will significantly decrease the performance of the hash table.
#define HASH_TABLE_INIT(selfp, initial_size)                          \
	crzhash_init((Crzhash_AnyHashTable *)(selfp), (initial_size), \
		     HASH_TABLE_PAIR_SIZE(*(selfp)))

/// Insert the value `insert_value` at the key `insert_key` for the hash table `selfp` (passed by pointer).
///
/// If the key does not exist in the table, inserts it.
/// The inserted key is a duplicate of `insert_key` (made using `CRZ_STRDUP`), so `insert_key` itself may be freed independently of the hash table.
/// The value is inserted directly.
///
/// If the key already exists in the table, overrides the existing value.
/// The value is inserted directly.
///
/// If the hash table overflows, a new hash table is created at twice the size, and all of the elements are re-inserted into it.
/// The `selfp` pointer is replaced to point at this new table.
#define HASH_TABLE_INSERT(selfp, insert_key, insert_value)                                      \
	do {                                                                                    \
		CRZ_SIZE index = crzhash_insert_index(                                          \
			(Crzhash_AnyHashTable *)(selfp), (insert_key),                          \
			HASH_TABLE_PAIR_SIZE(*(selfp)));                                        \
		if ((selfp)->ptr[index]) {                                                      \
			(selfp)->ptr[index]->value = insert_value;                              \
		} else {                                                                        \
			(selfp)->ptr[index] =                                                   \
				CRZ_MALLOC(HASH_TABLE_PAIR_SIZE(*(selfp)));                     \
			CRZ_ASSERT(                                                             \
				(selfp)->ptr[index] &&                                          \
				"Out of memory when allocating key-value pair for hash table"); \
			(selfp)->ptr[index]->key = CRZ_STRDUP(insert_key);                      \
			(selfp)->ptr[index]->value = insert_value;                              \
		}                                                                               \
	} while (0)

/// Get a pointer to the pair with key `get_key` in the hash table `self`.
///
/// If the key does not exist in the table, returns `CRZ_NULL`.
///
/// If the key exists in the table, returns a pointer to the existing pair.
#define HASH_TABLE_GET(self, get_key) \
	crzhash_get((Crzhash_AnyHashTable *)(&(self)), get_key)

/// Iterate over all indexes in the hash table `self`, naming the index variable `index`.
///
/// Note that not every index has a pair associated with it, so you must check that `self.ptr[index]` is not `CRZ_NULL` before acting on it.
#define HASH_TABLE_FOR(self, index) \
	for (CRZ_SIZE index = 0; index < (self).size; index++)

/// Print out the key-value pairs of the hash table `self`. `fmt` is the format specifier for the value type, as for `printf`.
#define HASH_TABLE_DEBUG(self, fmt)                              \
	do {                                                     \
		CRZ_DEBUG("{");                                  \
		HASH_TABLE_FOR(self, i) {                        \
			if ((self).ptr[i]) {                     \
				CRZ_DEBUG(" \"%s\" = " fmt ";",  \
					  (self).ptr[i]->key,    \
					  (self).ptr[i]->value); \
			}                                        \
		}                                                \
		CRZ_DEBUG(" }\n");                               \
	} while (0)

/// Free the space allocated for:
///   - the (duplicated) keys of each of the hash table's pairs
///   - the hash table's pairs
///   - the array of pairs itself
/// for the hash table `selfp` (passed by pointer), and empty-out the fields of the struct.
///
/// This **does not** free any of the values for the hash table's pairs  - if they are dynamically allocated, you must do this yourself before calling `HASH_TABLE_FREE`.
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

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_TABLE_INIT(selfp, initial_size)` instead.
void crzhash_init(Crzhash_AnyHashTable *selfp, CRZ_SIZE initial_size,
		  CRZ_SIZE pair_size);

/// INTERNAL: this is the hashing function used by the `crzhash` hash table internally.
CRZ_SIZE crzhash_djb2(CRZ_STRING str);

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_TABLE_INSERT(selfp, insert_key, insert_value)` instead.
CRZ_SIZE crzhash_insert_index(Crzhash_AnyHashTable *selfp, CRZ_STRING key,
			      CRZ_SIZE pair_size);

/// INTERNAL: this function copies one hash table's key-value pairs into another.
///           This is used when a hash table overflows, to re-insert it into a larger one.
void crzhash_clone(Crzhash_AnyHashTable self, Crzhash_AnyHashTable *other,
		   CRZ_SIZE pair_size);

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_TABLE_GET(self, key)` instead.
void *crzhash_get(Crzhash_AnyHashTable *selfp, CRZ_STRING key);

/// INTERNAL: you most likely don't want to use this.
///           Try `HASH_TABLE_INIT(selfp, initial_size)` instead.
void crzhash_init(Crzhash_AnyHashTable *selfp, CRZ_SIZE initial_size,
		  CRZ_SIZE pair_size)
{
	(selfp)->size = initial_size;
	(selfp)->ptr = CRZ_MALLOC(pair_size * initial_size);
	CRZ_ASSERT((selfp)->ptr && "Out of memory when allocating hash table");
	for (CRZ_SIZE i = 0; i < initial_size; i++)
		(selfp)->ptr[i] = CRZ_NULL;
}

CRZ_SIZE crzhash_djb2(CRZ_STRING str)
{
	CRZ_SIZE hash = 5381;
	CRZ_STRING_FOR(str, i) {
		hash = ((hash << 5) + hash) + CRZ_STRING_GET(str, i);
	}
	return hash;
}

CRZ_SIZE crzhash_insert_index(Crzhash_AnyHashTable *selfp, CRZ_STRING key,
			      CRZ_SIZE pair_size)
{
	CRZ_SIZE index = crzhash_djb2(key) % (selfp)->size;
	CRZ_BOOL wrapped = CRZ_FALSE;
	CRZ_SIZE original = index;

	while (CRZ_TRUE) {
		Crzhash_AnyHashPair *existing = selfp->ptr[index];

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
			Crzhash_AnyHashTable new = HASH_TABLE_NEW();
			crzhash_init(&new, (selfp->size * 2), pair_size);
			crzhash_clone(*selfp, &new, pair_size);

			// Free pointer itself, but not the pairs
			// since they've been copied over to the new table
			CRZ_FREE((selfp)->ptr);
			(*selfp) = new;

			return crzhash_insert_index(&new, key, pair_size);
		}
	}
}

void crzhash_clone(Crzhash_AnyHashTable self, Crzhash_AnyHashTable *other,
		   CRZ_SIZE pair_size)
{
	for (CRZ_SIZE i = 0; i < self.size; i++) {
		Crzhash_AnyHashPair *existing = self.ptr[i];
		if (existing) {
			CRZ_SIZE index = crzhash_insert_index(
				other, existing->key, pair_size);
			other->ptr[index] = existing;
		}
	}
}

void *crzhash_get(Crzhash_AnyHashTable *selfp, CRZ_STRING key)
{
	CRZ_SIZE index = crzhash_djb2(key) % selfp->size;
	CRZ_BOOL wrapped = CRZ_FALSE;
	CRZ_SIZE original = index;

	while (CRZ_TRUE) {
		Crzhash_AnyHashPair *existing = selfp->ptr[index];

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
