/*=============================================================================
  Copyright (C) 2017-2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef INC_GLOBAL_VARIABLE_H_
#define INC_GLOBAL_VARIABLE_H_
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

extern size_t g_QS_MID1, g_QS_MID2, g_QS_MID3;

extern size_t g_QS_MVR;

#if defined(DEBUG) || !defined(NDEBUG)
#	ifndef DEBUG
#		define DEBUG 1
#	endif
void init_ass_cnt();
void inc_ass_cnt(size_t d);
size_t get_ass_cnt();
#endif

#ifdef __cplusplus
}
#endif
#endif //INC_GLOBAL_VARIABLE_H_
