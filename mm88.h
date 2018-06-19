/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef INC_MM88_H_
#define INC_MM88_H_
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern size_t g_QS_MVR;
#if defined(DEBUG) || !defined(NDEBUG)
#	ifndef DEBUG
#		define DEBUG 1
#	endif
void init_ass_cnt();
size_t get_ass_cnt();
#endif

void mmswap(char *a, char *b);
void mmrot3( char *a, char *b, char *c );
void mmswapblock( char *a, char *b, size_t size );
void mmprepare( void *base, size_t size );
void mmmove(char *a, const char *b);

#ifdef __cplusplus
}
#endif

#endif //INC_MM88_H_
