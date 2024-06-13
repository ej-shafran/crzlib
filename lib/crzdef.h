#ifndef CRZDEF_H_
#define CRZDEF_H_

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

#ifndef CRZ_STRING
#define CRZ_STRING char *
#endif // CRZ_STRING

#ifndef CRZ_STRING_GET
#define CRZ_STRING_GET(str, index) (str)[index]
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

#ifndef CRZ_STRLEN
#include <string.h>
#define CRZ_STRLEN strlen
#endif // CRZ_STRLEN

#ifndef CRZ_MALLOC
#include <stdlib.h>
#define CRZ_MALLOC malloc
#endif // CRZ_MALLOC

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

#ifndef CRZ_MEMCMP
#include <string.h>
#define CRZ_MEMCMP memcmp
#endif // CRZ_MEMCMP

#ifndef CRZ_SPRINTF
#include <stdio.h>
#define CRZ_SPRINTF sprintf
#endif // CRZ_SPRINTF

#ifndef CRZ_ISSPACE
#include <ctype.h>
#define CRZ_ISSPACE isspace
#endif // CRZ_ISSPACE

#endif // CRZDEF_H_
