/*=============================================================================
  Copyright (C) 2017-2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef INC_QS10a5_H_
#define INC_QS10a5_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
extern size_t g_QS_MID1, g_QS_MID2, g_QS_MID3;

void qsort10a5( void *base, size_t nel, size_t size,  int (*cmp)(const void *a, const void *b) );

#ifdef __cplusplus
}
#endif

#endif //INC_QS10a5_H_
