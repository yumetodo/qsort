/*=============================================================================
  Copyright (C) 2017-2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
/**
 * @file mm88c.c
 * @brief 配列の要素のスワップ関数
 * @date 2017.11.30
 */
#include "mm88.h"
#include "die.h"
#include <stdbool.h>

/*８バイト整数(long long int)を使用して高速化  繰り返しは8回(64byte)分展開する*/
size_t g_QS_MVR = 116;
#ifdef DEBUG
static size_t ass_cnt; /*代入回数を計測しないときは、削除可能*/
void init_ass_cnt() { ass_cnt = 0; }
size_t get_ass_cnt() { return ass_cnt; }
#endif

static size_t mmkind, mmsize, high, low, high32_4;
static bool mmquick;

#define A8 ((long long int*)a)
#define B8 ((long long int*)b)
#define C8 ((long long int*)c)
#define A4 ((int*)a)
#define B4 ((int*)b)
#define C4 ((int*)c)

#define  SW8(i) {long long int v; v = A8[i]; A8[i] = B8[i]; B8[i] = v;}
#define  SW4(i) {          int v; v = A4[i]; A4[i] = B4[i]; B4[i] = v;}
#define  SW1(i) {         char v; v =  a[i];  a[i] =  b[i];  b[i] = v;}
#define  RT8(i) {long long int v; v = A8[i]; A8[i] = B8[i]; B8[i] = C8[i]; C8[i] = v;}
#define  RT4(i) {          int v; v = A4[i]; A4[i] = B4[i]; B4[i] = C4[i]; C4[i] = v;}
#define  RT1(i) {         char v; v =  a[i];  a[i] =  b[i];  b[i] =  c[i];  c[i] = v;}

#define HIGHLOW(HIGH,LOW,MOV,WS,CC) { \
   if (HIGH) { \
     char *e = a + HIGH; \
     do {MOV(0) MOV(1) MOV(2) MOV(3) MOV(4) MOV(5) MOV(6) MOV(7) CC; b += 8*WS; a += 8*WS; \
     }while (a < e); \
   } \
   switch (LOW) { \
     case 7*WS: MOV(6)  \
     case 6*WS: MOV(5)  \
     case 5*WS: MOV(4)  \
     case 4*WS: MOV(3)  \
     case 3*WS: MOV(2)  \
     case 2*WS: MOV(1)  \
     case 1*WS: MOV(0)  \
   } \
}

#define INT64_OK  (sizeof(char*)==8)

void mmswapblock( char *a, char *b, size_t size )
{
#ifdef DEBUG
 ass_cnt += (size/mmsize)*2;
#endif
 if      (mmkind == 8 && INT64_OK) HIGHLOW((size&(-64)),(size&(64-1)),SW8,8,(void)a)
 else if (mmkind == 4            ) HIGHLOW((size&(-32)),(size&(32-1)),SW4,4,(void)a)
 else   /*mmkind == 1*/            HIGHLOW((size&( -8)),(size&( 8-1)),SW1,1,(void)a)
}

void mmrot3( char *a, char *b, char *c )
{
#ifdef DEBUG
 ass_cnt += 3;
#endif
 if      (mmkind == 8 && INT64_OK) HIGHLOW(high,low,RT8,8,c+=64)
 else if (mmkind == 4            ) HIGHLOW(high,low,RT4,4,c+=32)
 else   /*mmkind == 1*/            HIGHLOW(high,low,RT1,1,c+= 8)
}

static void (*mmswapi)(char *, char *);

void mmswap(char *a, char *b) {
#ifdef DEBUG
 ass_cnt += 2;
#endif
 if (mmquick)
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
   if (INT64_OK) SW8(0)
   else          SW4(0)
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(pop)
#endif
 else            (*mmswapi)(a,b);
}
//#define  mmswap(a,b)  (*mmswapi)(a,b)  //gcc6.4.0 では上のinlineと同じコードが出る



static void mmswap1( char *a, char *b ) //if (a==b) return;  a と b は異なっていること
{
 HIGHLOW(high,low,SW1,1,(void)a)
}

#define EXD8(MOV,WS) { \
     char *e = a + high; \
     do {MOV(0) MOV(1) MOV(2) MOV(3) MOV(4) MOV(5) MOV(6) MOV(7)  b += 8*WS; a += 8*WS; \
     }while (a < e); }

static void mmswap801(char *a, char *b) {SW8(0)}
static void mmswap802(char *a, char *b) {SW8(0) SW8(1)}
static void mmswap803(char *a, char *b) {SW8(0) SW8(1) SW8(2)}
static void mmswap804(char *a, char *b) {SW8(0) SW8(1) SW8(2) SW8(3)}
static void mmswap805(char *a, char *b) {SW8(0) SW8(1) SW8(2) SW8(3) SW8(4)}
static void mmswap806(char *a, char *b) {SW8(0) SW8(1) SW8(2) SW8(3) SW8(4) SW8(5)}
static void mmswap807(char *a, char *b) {SW8(0) SW8(1) SW8(2) SW8(3) SW8(4) SW8(5) SW8(6)}

static void mmswap810(char *a, char *b) {EXD8(SW8,8)}
static void mmswap811(char *a, char *b) {EXD8(SW8,8) SW8(0)}
static void mmswap812(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1)}
static void mmswap813(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1) SW8(2)}
static void mmswap814(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1) SW8(2) SW8(3)}
static void mmswap815(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1) SW8(2) SW8(3) SW8(4)}
static void mmswap816(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1) SW8(2) SW8(3) SW8(4) SW8(5)}
static void mmswap817(char *a, char *b) {EXD8(SW8,8) SW8(0) SW8(1) SW8(2) SW8(3) SW8(4) SW8(5) SW8(6)}


static void mmswap401(char *a, char *b) {SW4(0)}
static void mmswap402(char *a, char *b) {SW4(0) SW4(1)}
static void mmswap403(char *a, char *b) {SW4(0) SW4(1) SW4(2)}
static void mmswap404(char *a, char *b) {SW4(0) SW4(1) SW4(2) SW4(3)}
static void mmswap405(char *a, char *b) {SW4(0) SW4(1) SW4(2) SW4(3) SW4(4)}
static void mmswap406(char *a, char *b) {SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5)}
static void mmswap407(char *a, char *b) {SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5) SW4(6)}

static void mmswap410(char *a, char *b) {EXD8(SW4,4)}
static void mmswap411(char *a, char *b) {EXD8(SW4,4) SW4(0)}
static void mmswap412(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1)}
static void mmswap413(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1) SW4(2)}
static void mmswap414(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1) SW4(2) SW4(3)}
static void mmswap415(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1) SW4(2) SW4(3) SW4(4)}
static void mmswap416(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5)}
static void mmswap417(char *a, char *b) {EXD8(SW4,4) SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5) SW4(6)}


#define ENINT(x)  ((char*)(x) - (char*)0)

#define MMswapL(XXX,YYY,ZZZ) { char *e0; \
 if ( ((ENINT(a) ^ ENINT(b)) & 4)==0 ) { /*両方が8の倍数 または 両方が4の倍数*/ \
   if ((ENINT(a) & (8-1))==0) { \
     e0 = a + high;                                       /*両方が8の倍数*/ \
     do {SW8(0) SW8(1) SW8(2) SW8(3) b+=32; a+=32;} while (a < e0); \
     XXX \
   }else{ \
     SW4(0) a+=4; b+=4;                                  /*両方が4の倍数*/ \
     e0 = a + high32_4; \
     do {SW8(0) SW8(1) SW8(2) SW8(3) b+=32; a+=32;} while (a < e0); \
     YYY \
   } \
 }else{ \
   EXD8(SW4,4) \
   ZZZ \
 } \
}

static void mmswap4L00(char *a, char *b) MMswapL({}                         , SW8(0) SW8(1) SW8(2) SW4(6), {})
static void mmswap4L04(char *a, char *b) MMswapL(SW4(0)                     , {}                         , SW4(0))
static void mmswap4L08(char *a, char *b) MMswapL(SW8(0)                     , SW4(0)                     , SW4(0) SW4(1))
static void mmswap4L12(char *a, char *b) MMswapL(SW8(0) SW4(2)              , SW8(0)                     , SW4(0) SW4(1) SW4(2))
static void mmswap4L16(char *a, char *b) MMswapL(SW8(0) SW8(1)              , SW8(0) SW4(2)              , SW4(0) SW4(1) SW4(2) SW4(3))
static void mmswap4L20(char *a, char *b) MMswapL(SW8(0) SW8(1) SW4(4)       , SW8(0) SW8(1)              , SW4(0) SW4(1) SW4(2) SW4(3) SW4(4))
static void mmswap4L24(char *a, char *b) MMswapL(SW8(0) SW8(1) SW8(2)       , SW8(0) SW8(1) SW4(4)       , SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5))
static void mmswap4L28(char *a, char *b) MMswapL(SW8(0) SW8(1) SW8(2) SW4(6), SW8(0) SW8(1) SW8(2)       , SW4(0) SW4(1) SW4(2) SW4(3) SW4(4) SW4(5) SW4(6))




//以下、「ポインタが８バイト(sizeof(char*)==8)なら、機械語の８バイト整数あり」と仮定している。
//「ポインタが４バイトなのに、機械語の８バイト整数あり」では、十分な性能はでない。

void mmprepare( void *base, size_t size )
{
#ifdef DEBUG
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
 if (sizeof(          int) != 4) die("sizeof(          int) != 4"); //cygwin64はsizeof(long int)==8
 if (sizeof(long long int) != 8) die("sizeof(long long int) != 8");
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(pop)
#endif
 if (size <= 0) die("mmsize <= 0");
#endif
 if      ( (ENINT(base) & (8-1)) == 0  &&  (size & (8-1)) == 0 && INT64_OK) mmkind = 8;
 else if ( (ENINT(base) & (4-1)) == 0  &&  (size & (4-1)) == 0            ) mmkind = 4;
 else                                                                       mmkind = 1;
 mmsize = size;
 mmquick = false;
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(push)
#pragma warning(disable: 4127)
#endif
 if      ( INT64_OK ) {if ( mmkind == 8  &&  size == 8 ) mmquick = true;}
 else                 {if ( mmkind == 4  &&  size == 4 ) mmquick = true;}
#if defined(_MSC_VER) && !defined(__c2__)
#pragma warning(pop)
#endif

 if      (mmkind==8 && INT64_OK) {
   high = (size & (~((size_t)(0x3F))));
   low  = (size & (64-1));
   if (high==0)
     switch (low) {
       case 1*8: mmswapi=mmswap801; break;
       case 2*8: mmswapi=mmswap802; break;
       case 3*8: mmswapi=mmswap803; break;
       case 4*8: mmswapi=mmswap804; break;
       case 5*8: mmswapi=mmswap805; break;
       case 6*8: mmswapi=mmswap806; break;
       case 7*8: mmswapi=mmswap807; break;
     }
   else
     switch (low) {
       case 0*8: mmswapi=mmswap810; break;
       case 1*8: mmswapi=mmswap811; break;
       case 2*8: mmswapi=mmswap812; break;
       case 3*8: mmswapi=mmswap813; break;
       case 4*8: mmswapi=mmswap814; break;
       case 5*8: mmswapi=mmswap815; break;
       case 6*8: mmswapi=mmswap816; break;
       case 7*8: mmswapi=mmswap817; break;
     }
 }
 else if (mmkind==4) {
   high = (size & (-32));
   low  = (size & (32-1));
   if (size<=g_QS_MVR || !INT64_OK) {
     if (high==0)
       switch (low) {
         case 1*4: mmswapi=mmswap401; break;
         case 2*4: mmswapi=mmswap402; break;
         case 3*4: mmswapi=mmswap403; break;
         case 4*4: mmswapi=mmswap404; break;
         case 5*4: mmswapi=mmswap405; break;
         case 6*4: mmswapi=mmswap406; break;
         case 7*4: mmswapi=mmswap407; break;
       }
     else
       switch (low) {
         case 0*4: mmswapi=mmswap410; break;
         case 1*4: mmswapi=mmswap411; break;
         case 2*4: mmswapi=mmswap412; break;
         case 3*4: mmswapi=mmswap413; break;
         case 4*4: mmswapi=mmswap414; break;
         case 5*4: mmswapi=mmswap415; break;
         case 6*4: mmswapi=mmswap416; break;
         case 7*4: mmswapi=mmswap417; break;
       }
   }else{
     high32_4 = ((size-4) & (-32));
       switch (low) {
         case 0*4: mmswapi=mmswap4L00; break;
         case 1*4: mmswapi=mmswap4L04; break;
         case 2*4: mmswapi=mmswap4L08; break;
         case 3*4: mmswapi=mmswap4L12; break;
         case 4*4: mmswapi=mmswap4L16; break;
         case 5*4: mmswapi=mmswap4L20; break;
         case 6*4: mmswapi=mmswap4L24; break;
         case 7*4: mmswapi=mmswap4L28; break;
       }
   }
 }
 else {
   high = (size & ( -8));
   low  = (size & ( 8-1));
   mmswapi=mmswap1;
 }
}

//mmmoveは間接ソートの３箇所でのみ使用。memcpy関数が遅いときに使用する。
#define  MV8(i) {A8[i] = B8[i];}
#define  MV4(i) {A4[i] = B4[i];}
#define  MV1(i) { a[i] =  b[i];}
void mmmove(char *a, const char *b)
{
#ifdef DEBUG
	ass_cnt += 1;
#endif
	if (mmkind == 8 && INT64_OK) HIGHLOW(high, low, MV8, 8, (void)a)
	else if (mmkind == 4) HIGHLOW(high, low, MV4, 4, (void)a)
	else   /*mmkind == 1*/            HIGHLOW(high, low, MV1, 1, (void)a)
}
