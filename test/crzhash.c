#include "crzhash.h"
#include "crztest.h"

#define DUMP(name, ht)                       \
	do {                                 \
		TEST_LOG("%s = ", name);     \
		HASH_TABLE_DEBUG(ht, "%zu"); \
	} while (0)

static HASH_TABLE(size_t) ht = HASH_TABLE_NEW();

void cleanup(void)
{
	HASH_TABLE_FREE(&ht);
}

TEST_MAIN({
	AFTER_EACH(cleanup);

	DESCRIBE("HASH_TABLE_INSERT", {
		TEST("Inserting a key-value pair", {
			// Arrange
			HASH_TABLE_INIT(&ht, 2);

			// Act
			HASH_TABLE_INSERT(&ht, "a", 1);

			// Assert
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "a");
			EXPECT(result != CRZ_NULL && result->value == 1);
		});

		TEST("Properly inserting with collision", {
			// Invariant
			EXPECT(crzhash_djb2("ab") % 2 ==
			       crzhash_djb2("ba") % 2);

			// Arrange
			HASH_TABLE_INIT(&ht, 2);

			// Act
			HASH_TABLE_INSERT(&ht, "ab", 1);
			HASH_TABLE_INSERT(&ht, "ba", 2);

			// Assert
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "ab");
			EXPECT(result != CRZ_NULL && result->value == 1);
		});

		TEST("Properly inserting with overflow", {
			// Arrange
			HASH_TABLE_INIT(&ht, 2);
			CRZ_SIZE original = ht.size;

			// Act
			HASH_TABLE_INSERT(&ht, "a", 1);
			HASH_TABLE_INSERT(&ht, "b", 2);
			HASH_TABLE_INSERT(&ht, "c", 3);

			// Assert
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "a");
			EXPECT(result != CRZ_NULL && result->value == 1);
			EXPECT(ht.size == original * 2);
		});

		TEST("Overriding existing value", {
			// Arrange
			HASH_TABLE_INIT(&ht, 2);
			HASH_TABLE_INSERT(&ht, "a", 1);

			// Act
			HASH_TABLE_INSERT(&ht, "a", 2);

			// Assert
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "a");
			EXPECT(result != CRZ_NULL && result->value == 2);
		});
	});

	DESCRIBE("HASH_TABLE_GET", {
		TEST("Returning value", {
			// Arrange
			HASH_TABLE_INIT(&ht, 2);
			HASH_TABLE_INSERT(&ht, "a", 1);

			// Act
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "a");

			// Assert
			EXPECT(result != CRZ_NULL && result->value == 1);
		});

		TEST("Returning CRZ_NULL when nonexistent", {
			// Arrange
			HASH_TABLE_INIT(&ht, 2);

			// Act
			HASH_PAIR(size_t) *result = HASH_TABLE_GET(ht, "a");

			// Assert
			EXPECT(result == CRZ_NULL);
		});
	});
})
