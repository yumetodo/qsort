/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "die.h"
#include "mm88.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef DEBUG
void die(const char *s) {fprintf(stderr, "++++ %s ++++\n", s); printf("++++ %s ++++ \n", s); exit(1);}
#else
void die(const char *s) { (void)s; }
#endif
