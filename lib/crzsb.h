#ifndef CRZSB_H_
#define CRZSB_H_

#include "crzarr.h"
#include "crzdef.h"

typedef ARRAY(char) StringBuilder;

#define SB_NEW() ARRAY_NEW()

#define SB_INIT(selfp) ARRAY_INIT(selfp)

#define SB_GROW_BY(selfp, amount_chars) ARRAY_GROW_BY(selfp, amount_chars)

#define SB_PUSH_CHAR(selfp, char) ARRAY_PUSH(selfp, char)

#define SB_GET(self, index) ARRAY_GET(self, index)

#define SB_PUSH_BUF(selfp, buf, count_chars) \
	ARRAY_PUSH_MANY(selfp, buf, count_chars)

#define SB_PUSH_CSTR(selfp, cstr) SB_PUSH_BUF(selfp, cstr, CRZ_STRLEN(cstr))

#define SB_PUSH_OTHER(selfp, other) ARRAY_PUSH_OTHER(selfp, other)

#define SB_SPLICE_BUF(selfp, index, remove_amount_chars, buf, count_chars) \
	ARRAY_SPLICE(selfp, index, remove_amount_chars, buf, count_chars)

#define SB_SPLICE_CSTR(selfp, index, remove_amount_chars, cstr) \
	SB_SPLICE_BUF(selfp, index, remove_amount_chars, cstr, CRZ_STRLEN(cstr))

#define SB_REMOVE(selfp, index, amount_chars) \
	ARRAY_REMOVE(selfp, index, amount_chars)

#define SB_INSERT_BUF(selfp, index, buf, count_chars) \
	ARRAY_INSERT(selfp, index, buf, count_chars)

#define SB_INSERT_CSTR(selfp, index, cstr) \
	SB_INSERT_BUF(selfp, index, cstr, CRZ_STRLEN(cstr))

#define SB_TERMINATE(selfp) SB_PUSH_CHAR(selfp, '\0')

#define SB_FMT "%.*s"

#define SB_ARG(self) (int)((self).len), (self).ptr

#define SB_EQ(a, b)                           \
	(((a).len != (b).len) ? (CRZ_FALSE) : \
				(CRZ_MEMCMP((a).ptr, (b).ptr, (a).len) == 0))

#define SB_FREE(selfp) ARRAY_FREE(selfp)

#endif // CRZSB_H_
