/*=============================================================================
  Copyright (C) 2017-2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef QSORT_GLIBC_QSORT_H
#define QSORT_GLIBC_QSORT_H
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void qsort_glibc( void *base, size_t nel, size_t size,  int (*cmp)(const void *a, const void *b) );

#ifdef __cplusplus
}
#endif

#endif //QSORT_GLIBC_QSORT_H
