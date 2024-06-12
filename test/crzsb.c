#include "crzdef.h"
#include "crztest.h"
#include <stdio.h>
#include "crzsb.h"

static StringBuilder sb = SB_NEW();
static StringBuilder other = SB_NEW();

void cleanup(void)
{
	SB_FREE(&sb);
	SB_FREE(&other);
}

TEST_MAIN({
	AFTER_EACH(cleanup);

	DESCRIBE("Character usage", {
		TEST("Pushing a character", {
			// Arrange + act
			SB_PUSH_CHAR(&sb, 'a');
			SB_PUSH_CHAR(&sb, 'b');
			SB_PUSH_CHAR(&sb, 'c');

			// Assert
			EXPECT(sb.len == 3);
			EXPECT(CRZ_MEMCMP(sb.ptr, "abc", sb.len) == 0);
		});

		TEST("Getting a character", {
			// Arrange
			SB_PUSH_CHAR(&sb, 'a');

			// Act
			char result = SB_GET(sb, 0);

			// Assert
			EXPECT(result == 'a');
		});
	});

	DESCRIBE("Buffer usage", {
		TEST("Pushing a buffer", {
			// Arrange + act
			SB_PUSH_BUF(&sb, ((char[]){ 'a', 'b', 'c' }), 3);

			// Assert
			EXPECT(sb.len == 3);
			EXPECT(CRZ_MEMCMP(sb.ptr, "abc", sb.len) == 0);
		});

		TEST("Printing using length and buffer", {
			// Arrange
			SB_PUSH_BUF(&sb, ((char[]){ 'a', 'b', 'c' }), 3);

			// Act
			char buf[3];
			sprintf(buf, SB_FMT, SB_ARG(sb));

			// Assert
			EXPECT(CRZ_MEMCMP(buf, "abc", 3) == 0);
		});
	});

	DESCRIBE("C string usage", {
		TEST("Pushing a C string", {
			// Arrange + act
			SB_PUSH_CSTR(&sb, "abc");

			// Assert
			EXPECT(sb.len == 3);
			EXPECT(CRZ_MEMCMP(sb.ptr, "abc", sb.len) == 0);
		});

		TEST("Using as C string", {
			// Arrange
			SB_PUSH_CSTR(&sb, "abc");
			SB_PUSH_CSTR(&sb, "def");

			// Act
			SB_TERMINATE(&sb);

			// Assert
			EXPECT(CRZ_STRING_EQ(sb.ptr, "abcdef"));
		});
	});

	DESCRIBE("Other string builders usage", {
		TEST("Pushing another string builder", {
			// Arrange
			SB_PUSH_CSTR(&sb, "abc");
			SB_PUSH_CSTR(&other, "def");
			CRZ_SIZE original = sb.len;

			// Act
			SB_PUSH_OTHER(&sb, other);

			// Assert
			EXPECT(sb.len == other.len + original);
			EXPECT(CRZ_MEMCMP(sb.ptr, "abcdef", sb.len) == 0);
		});

		DESCRIBE("Comparing string builders", {
			TEST("Comparing two equal strings", {
				// Arrange
				SB_PUSH_CSTR(&sb, "abc");
				SB_PUSH_OTHER(&other, sb);

				// Act + assert
				EXPECT(SB_EQ(sb, other));
			});

			TEST("Comparing different strings", {
				// Arrange
				SB_PUSH_CSTR(&sb, "abc");
				SB_PUSH_CSTR(&other, "def");

				// Act + assert
				EXPECT(!SB_EQ(sb, other));
			});
		});
	});
})
