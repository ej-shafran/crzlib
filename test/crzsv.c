#include "crzsv.h"
#include "crztest.h"

#define DUMP(name, sv) TEST_LOG("%s = '" SV_FMT "'\n", name, SV_ARG(sv))

#define TARGET_LEN 3
#define TARGET "abc"
static const StringView target = SV_CONSTANT(TARGET, TARGET_LEN);

static StringView sv;

TEST_MAIN({
	DESCRIBE("Trimming", {
		TEST("SV_TRIM_LEFT", {
			// Arrange
			sv = SV_FROM_CSTR("   " TARGET);

			// Act
			SV_TRIM_LEFT(&sv);

			// Assert
			EXPECT(SV_EQ(sv, target));
		});

		TEST("SV_TRIM_RIGHT", {
			// Arrange
			sv = SV_FROM_CSTR(TARGET "   ");

			// Act
			SV_TRIM_RIGHT(&sv);

			// Assert
			EXPECT(SV_EQ(sv, target));
		});

		TEST("SV_TRIM", {
			// Arrange
			sv = SV_FROM_CSTR("   " TARGET "   ");

			// Act
			SV_TRIM(&sv);

			// Assert
			EXPECT(SV_EQ(sv, target));
		});
	});

	DESCRIBE("Cutting", {
		DESCRIBE("Cutting a fixed amount", {
			TEST("SV_CHOP_LEFT", {
				// Arrange
				sv = SV_FROM_CSTR("d" TARGET);

				// Act
				SV_CHOP_LEFT(&sv);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});

			TEST("SV_CHOP_RIGHT", {
				// Arrange
				sv = SV_FROM_CSTR(TARGET "d");

				// Act
				SV_CHOP_RIGHT(&sv);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});

			TEST("SV_CUT_LEFT", {
				// Arrange
				sv = SV_FROM_CSTR("def" TARGET);

				// Act
				SV_CUT_LEFT(&sv, 3);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});

			TEST("SV_CUT_RIGHT", {
				// Arrange
				sv = SV_FROM_CSTR(TARGET "def");

				// Act
				SV_CUT_RIGHT(&sv, 3);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});
		});

		DESCRIBE("Cutting up to a delimiter", {
			TEST("SV_CUT_LEFT_UNTIL", {
				// Arrange
				sv = SV_FROM_CSTR("hello " TARGET);

				// Act
				SV_CUT_LEFT_UNTIL(&sv, TARGET[0]);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});

			TEST("SV_CUT_RIGHT_UNTIL", {
				// Arrang
				sv = SV_FROM_CSTR(TARGET " hello");

				// Act
				SV_CUT_RIGHT_UNTIL(&sv, TARGET[TARGET_LEN - 1]);

				// Assert
				EXPECT(SV_EQ(sv, target));
			});
		});
	});
})
