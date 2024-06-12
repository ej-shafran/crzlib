#ifndef CRZTEST_H_
#define CRZTEST_H_

#ifndef CRZ_DEBUG
#include <stdio.h>
#define CRZ_DEBUG printf
#endif // CRZ_DEBUG

/// INTERNAL: you most likely don't want to use this.
///           Try `BEFORE_EACH(cb)` instead.
static void (*crztest_before_each_cb)(void) = NULL;
/// INTERNAL: you most likely don't want to use this.
///           Try `AFTER_EACH(cb)` instead.
static void (*crztest_after_each_cb)(void) = NULL;
/// INTERNAL: you most likely don't want to use this.
///           Try `TEST_PAD()` or `TEST_LOG(...)` instead.
static int crztest_padding = 0;
/// INTERNAL: you most likely don't want to use this.
///           Try `TEST_FAIL(...)` instead.
static int crztest_failures = 0;

/// Define the `main` function for the test file.
#define TEST_MAIN(block)                                             \
	int main(void)                                               \
	{                                                            \
		do {                                                 \
			block;                                       \
		} while (0);                                         \
		CRZ_DEBUG("\n");                                     \
		if (crztest_failures <= 0) {                         \
			CRZ_DEBUG("All tests passed.\n");            \
		} else {                                             \
			CRZ_DEBUG("Failed with %d test failures.\n", \
				  crztest_failures);                 \
		}                                                    \
		return crztest_failures > 0 ? 1 : 0;                 \
	}

/// INTERNAL: you most likely don't want to use this.
///           Try `TEST_PAD()` or `TEST_LOG(...)` instead.
#define CRZTEST_PRINT_PADDING()                             \
	do {                                                \
		for (int i = 0; i < crztest_padding; i++) { \
			CRZ_DEBUG(" ");                     \
		}                                           \
	} while (0)

/// INTERNAL: you most likely don't want to use this.
///           Try `TEST_PAD()` or `TEST_LOG(...)` instead.
#define CRZTEST_INDENT()              \
	do {                          \
		crztest_padding += 2; \
	} while (0)

/// INTERNAL: you most likely don't want to use this.
///           Try `TEST_PAD()` or `TEST_LOG(...)` instead.
#define CRZTEST_DEDENT()              \
	do {                          \
		crztest_padding -= 2; \
	} while (0)

/// Temporarily indent a single log.
#define TEST_PAD()                       \
	do {                             \
		CRZTEST_INDENT();        \
		CRZTEST_PRINT_PADDING(); \
		CRZTEST_DEDENT();        \
	} while (0)

/// Define a test suite.
#define DESCRIBE(header, block)                      \
	do {                                         \
		CRZTEST_PRINT_PADDING();             \
		CRZ_DEBUG("DESCRIBE: %s\n", header); \
                                                     \
		CRZTEST_INDENT();                    \
		do {                                 \
			block;                       \
		} while (0);                         \
		CRZTEST_DEDENT();                    \
	} while (0)

/// Define a callback to run before each `TEST`.
#define BEFORE_EACH(cb)                      \
	do {                                 \
		crztest_before_each_cb = cb; \
	} while (0)

/// Define a callback to run after each `TEST`.
#define AFTER_EACH(cb)                      \
	do {                                \
		crztest_after_each_cb = cb; \
	} while (0)

/// Define a test case.
#define TEST(header, block)                                 \
	do {                                                \
		if (crztest_before_each_cb)                 \
			crztest_before_each_cb();           \
		int pretest_failures = crztest_failures;    \
		CRZTEST_PRINT_PADDING();                    \
		CRZ_DEBUG("TEST: %s\n", header);            \
		do {                                        \
			block;                              \
		} while (0);                                \
		if (crztest_failures == pretest_failures) { \
			TEST_LOG("PASS\n");                 \
		}                                           \
		if (crztest_after_each_cb)                  \
			crztest_after_each_cb();            \
	} while (0)

/// Log to stdout, using the current level of padding.
#define TEST_LOG(...)                   \
	do {                            \
		TEST_PAD();             \
		CRZ_DEBUG(__VA_ARGS__); \
	} while (0)

/// Cause the current test case to fail, with the given message (printf-like).
#define TEST_FAIL(...)                                         \
	do {                                                   \
		TEST_LOG("FAIL: %s:%d: ", __FILE__, __LINE__); \
		CRZ_DEBUG(__VA_ARGS__);                        \
		crztest_failures += 1;                         \
	} while (0)

/// Define a placeholder test that will always fail.
#define TEST_TODO(header) TEST(header, TEST_FAIL("TODO\n"))

/// If `condition` is false, fail the current test case with the given message (printf-like).
#define EXPECTF(condition, ...)                 \
	do {                                    \
		if (!(condition)) {             \
			TEST_FAIL(__VA_ARGS__); \
		}                               \
	} while (0)

/// If `condition` is false, fail the current test case with a default message.
#define EXPECT(condition) EXPECTF(condition, "Expectation failed\n")

#endif // CRZTEST_H_
