#ifndef CRZTEST_H_
#define CRZTEST_H_

#include <stdio.h>

static void (*crz__before_each_cb)(void) = NULL;
static void (*crz__after_each_cb)(void) = NULL;
static int padding = 0;
static int failures = 0;

#define TEST_MAIN(block)                                                     \
	int main(void)                                                       \
	{                                                                    \
		do {                                                         \
			block;                                               \
		} while (0);                                                 \
		printf("\n");                                                \
		if (failures <= 0) {                                         \
			printf("All tests passed.\n");                       \
		} else {                                                     \
			printf("Failed with %d test failures.\n", failures); \
		}                                                            \
		return failures > 0 ? 1 : 0;                                 \
	}

#define CRZ_PRINT_PADDING()                         \
	do {                                        \
		for (int i = 0; i < padding; i++) { \
			printf(" ");                \
		}                                   \
	} while (0)

#define CRZ_TEST_INDENT() padding += 2;

#define CRZ_TEST_DEDENT() padding -= 2;

#define TEST_PAD()                   \
	do {                         \
		CRZ_TEST_INDENT();   \
		CRZ_PRINT_PADDING(); \
		CRZ_TEST_DEDENT();   \
	} while (0)

#define DESCRIBE(header, block)                   \
	do {                                      \
		CRZ_PRINT_PADDING();              \
		printf("DESCRIBE: %s\n", header); \
                                                  \
		CRZ_TEST_INDENT();                \
		do {                              \
			block;                    \
		} while (0);                      \
		CRZ_TEST_DEDENT();                \
	} while (0)

#define BEFORE_EACH(cb)                   \
	do {                              \
		crz__before_each_cb = cb; \
	} while (0)

#define AFTER_EACH(cb)                   \
	do {                             \
		crz__after_each_cb = cb; \
	} while (0)

#define TEST(header, block)                         \
	do {                                        \
		if (crz__before_each_cb)            \
			crz__before_each_cb();      \
		int pretest_failures = failures;    \
		CRZ_PRINT_PADDING();                \
		printf("TEST: %s\n", header);       \
		do {                                \
			block;                      \
		} while (0);                        \
		if (failures == pretest_failures) { \
			TEST_LOG("PASS\n");         \
		}                                   \
		if (crz__after_each_cb)             \
			crz__after_each_cb();       \
	} while (0)

#define TEST_LOG(...)                \
	do {                         \
		TEST_PAD();          \
		printf(__VA_ARGS__); \
	} while (0)

#define TEST_FAIL(...)                                         \
	do {                                                   \
		TEST_LOG("FAIL: %s:%d: ", __FILE__, __LINE__); \
		printf(__VA_ARGS__);                           \
		failures += 1;                                 \
	} while (0)

#define TEST_TODO(header) TEST(header, TEST_FAIL("TODO\n"))

#define EXPECTF(condition, ...)                 \
	do {                                    \
		if (!(condition)) {             \
			TEST_FAIL(__VA_ARGS__); \
		}                               \
	} while (0)

#define EXPECT(condition) EXPECTF(condition, "Expectation failed\n")

#endif // CRZTEST_H_
