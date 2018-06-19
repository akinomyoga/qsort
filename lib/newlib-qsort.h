#ifndef QSORT_LIB_NEWLIB_QSORT_H
#define QSORT_LIB_NEWLIB_QSORT_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void
qsort_newlib (void *a,
	size_t n,
	size_t es,
	int	(*cmp)(const void *, const void *));

#ifdef __cplusplus
}
#endif

#endif
