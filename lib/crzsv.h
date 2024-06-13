#ifndef CRZSV_H_
#define CRZSV_H_

#include "crzdef.h"

typedef struct {
	const char *ptr;
	CRZ_SIZE len;
} StringView;

#define SV_CONSTANT(_ptr, _len)          \
	{                                \
		.ptr = _ptr, .len = _len \
	}

#define SV_FROM_CSTR(cstr) \
	((StringView){ .ptr = cstr, .len = CRZ_STRLEN(cstr) })

#define SV_FROM_BUF(buf, buflen) ((StringView){ .ptr = buf, .len = buflen })

#define SV_CLONE(other) ((StringView){ .ptr = other.ptr, .len = other.len })

#define SV_CUT_LEFT(selfp, count)                                 \
	do {                                                      \
		CRZ_ASSERT((count) < (selfp)->len &&              \
			   "Count out of bounds of string view"); \
		(selfp)->ptr += count;                            \
		(selfp)->len -= count;                            \
	} while (0)

#define SV_CHOP_LEFT(selfp) SV_CUT_LEFT(selfp, 1)

#define SV_CUT_RIGHT(selfp, count)                                \
	do {                                                      \
		CRZ_ASSERT((count) < (selfp)->len &&              \
			   "Count out of bounds of string view"); \
		(selfp)->len -= count;                            \
	} while (0)

#define SV_CHOP_RIGHT(selfp) SV_CUT_RIGHT(selfp, 1)

#define SV_TRIM_LEFT(selfp)                                              \
	do {                                                             \
		CRZ_SIZE i = 0;                                          \
		while (i < (selfp)->len && CRZ_ISSPACE((selfp)->ptr[i])) \
			i++;                                             \
		(selfp)->ptr += i;                                       \
		(selfp)->len -= i;                                       \
	} while (0)

#define SV_TRIM_RIGHT(selfp)                         \
	do {                                         \
		CRZ_SIZE i = (selfp)->len - 1;       \
		while (CRZ_ISSPACE((selfp)->ptr[i])) \
			i--;                         \
		(selfp)->len = i + 1;                \
	} while (0)

#define SV_TRIM(selfp)                \
	do {                          \
		SV_TRIM_LEFT(selfp);  \
		SV_TRIM_RIGHT(selfp); \
	} while (0)

#define SV_CUT_LEFT_UNTIL(selfp, char)                              \
	do {                                                        \
		CRZ_SIZE i = 0;                                     \
		while (i < (selfp)->len && (selfp)->ptr[i] != char) \
			i++;                                        \
		(selfp)->ptr += i;                                  \
		(selfp)->len -= i;                                  \
	} while (0)

#define SV_CUT_RIGHT_UNTIL(selfp, char)         \
	do {                                    \
		CRZ_SIZE i = (selfp)->len - 1;  \
		while ((selfp)->ptr[i] != char) \
			i--;                    \
		(selfp)->len = i + 1;           \
	} while (0)

#define SV_EQ(a, b)                           \
	(((a).len != (b).len) ? (CRZ_FALSE) : \
				(CRZ_MEMCMP((a).ptr, (b).ptr, (a).len) == 0))

#define SV_FMT "%.*s"

#define SV_ARG(self) (int)((self).len), ((self).ptr)

#endif // CRZSV_H_
