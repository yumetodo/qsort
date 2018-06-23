/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "global_variable.h"
#include <stddef.h>

size_t g_QS_MID1=140, g_QS_MID2=900, g_QS_MID3=400;  // 関数の外で設定することも可 g_QS_MID3=500も可

/*８バイト整数(long long int)を使用して高速化  繰り返しは8回(64byte)分展開する*/
size_t g_QS_MVR = 116;

#ifdef DEBUG

static size_t ass_cnt; /*代入回数を計測しないときは、削除可能*/
void init_ass_cnt() { ass_cnt = 0; }
void inc_ass_cnt(size_t d) { ass_cnt += d; }
size_t get_ass_cnt() { return ass_cnt; }

#endif
