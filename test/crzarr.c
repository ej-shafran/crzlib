#include "crztest.h"
#include "crzarr.h"

#define DUMP(name, array)                 \
	do {                              \
		TEST_LOG("%s = ", name);  \
		ARRAY_DEBUG(array, "%d"); \
	} while (0)

#define SIZEOF_CONTENTSP 3
static int contentsp[SIZEOF_CONTENTSP] = { 1, 2, 3 };

static ARRAY(int) a = ARRAY_NEW();
static ARRAY(int) b = ARRAY_NEW();

void cleanup(void)
{
	ARRAY_FREE(&a);
	ARRAY_FREE(&b);
}

TEST_MAIN({
	AFTER_EACH(cleanup);

	DESCRIBE("ARRAY_PUSH", {
		TEST("Pushing a single element", {
			// Arrange
			int element = 1;

			// Act
			ARRAY_PUSH(&a, element);

			// Assert
			EXPECT(ARRAY_GET(a, 0) == element);
		});
	});

	DESCRIBE("ARRAY_POP", {
		TEST("Removes and returns last element", {
			// Arrange
			int element = 1;
			ARRAY_PUSH(&a, element);

			// Act
			int result = ARRAY_POP(&a);

			EXPECT(result == element);
			EXPECT(a.len == 0);
		});
	});

	DESCRIBE("ARRAY_PUSH_MANY", {
		TEST("Pushing the entirety of `contentsp`", {
			// Arrange + act
			ARRAY_PUSH_MANY(&a, contentsp, SIZEOF_CONTENTSP);

			// Assert
			EXPECT(a.len == SIZEOF_CONTENTSP);
			ARRAY_FOR(a, i) {
				EXPECT(ARRAY_GET(a, i) == contentsp[i]);
			}
		});

		TEST("Pushing only part of `contentsp`", {
			// Arrange + act
			ARRAY_PUSH_MANY(&a, contentsp, SIZEOF_CONTENTSP - 1);

			// Assert
			EXPECT(a.len == SIZEOF_CONTENTSP - 1);
			ARRAY_FOR(a, i) {
				EXPECT(ARRAY_GET(a, i) == contentsp[i]);
			}
		});

		TEST("Pushing nothing", {
			// Arrange + act
			ARRAY_PUSH_MANY(&a, contentsp, 0);

			// Assert
			EXPECT(a.len == 0);
		});
	});

	DESCRIBE("ARRAY_PUSH_OTHER", {
		TEST("Pushing to an empty array", {
			// Arrange
			ARRAY_PUSH_MANY(&b, ((int[]){ 1, 2, 3 }), 3);

			// Act
			ARRAY_PUSH_OTHER(&a, b);

			// Assert
			EXPECT(a.len == b.len);
			ARRAY_FOR(a, i) {
				EXPECT(ARRAY_GET(a, i) == ARRAY_GET(b, i));
			}
		});

		TEST("Pushing to a prefilled array", {
			// Arrange
			ARRAY_PUSH_MANY(&a, ((int[]){ 4, 5, 6 }), 3);
			ARRAY_PUSH_MANY(&b, ((int[]){ 1, 2, 3 }), 3);
			int original_len = a.len;

			// Act
			ARRAY_PUSH_OTHER(&a, b);

			// Assert
			EXPECT(a.len == b.len + original_len);
			ARRAY_FOR(b, i) {
				EXPECT(ARRAY_GET(a, i + original_len) ==
				       ARRAY_GET(b, i));
			}
		});
	});

	DESCRIBE("ARRAY_SPLICE", {
		TEST("Splicing as no-op", {
			// Arrange + act
			ARRAY_SPLICE(&a, 0, 0, NULL, 0);

			// Assert
			EXPECT(a.len == 0);
		});

		TEST("Splicing as adding", {
			// Arrange + act
			ARRAY_SPLICE(&a, 0, 0, contentsp, SIZEOF_CONTENTSP);

			// Assert
			EXPECT(a.len == SIZEOF_CONTENTSP);
			ARRAY_FOR(a, i) {
				EXPECT(ARRAY_GET(a, i) == contentsp[i]);
			}
		});

		TEST("Splicing as removing", {
			// Arrange
			ARRAY_PUSH_MANY(&a, contentsp, SIZEOF_CONTENTSP);

			// Act
			ARRAY_SPLICE(&a, 0, SIZEOF_CONTENTSP, NULL, 0);

			// Assert
			EXPECT(a.len == 0);
		});

		TEST("Splicing as adding and removing", {
			// Arrange
			ARRAY_PUSH_MANY(&a, ((int[]){ 1, 2, 3 }), 3);

			// Act
			ARRAY_SPLICE(&a, 1, 1, ((int[]){ 1, 2, 3 }), 3);

			// Assert
			int *target = ((int[]){ 1, 1, 2, 3, 3 });
			EXPECT(a.len == 5);
			ARRAY_FOR(a, i) {
				EXPECT(ARRAY_GET(a, i) == target[i]);
			}
		});
	});
})
