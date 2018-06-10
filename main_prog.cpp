/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <stdio.h>
#ifdef USE_STDLIB_QSORT
#include <stdlib.h>
#define qsort_selected qsort
size_t g_QS_MID1, g_QS_MID2, g_QS_MID3;
#elif defined(USE_QS9E17)
#include "qs9e17.h"
#define qsort_selected qsort9e17
#elif defined(USE_QS10A5)
#include "qs10a5.h"
#define qsort_selected qsort10a5
#endif
#include <time.h>
#include <random>
#include <array>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iomanip>

constexpr char32_t seed[] = U"glibc newlib より速いクイックソート(qs9 qs10)、 世界最速をめざしてqsortの新しいアルゴリズム qs9 を開発しましたその結果、ソート対象によっては間接ソートを取り入れた方が高速なことが判りました。今回、間接ソートを取り入れた qs10 を開発しました。お願い現在、 標準qsort　qs9　qs10　を比較するベンチマークテストを行っております。";
std::mt19937 create_random_engine()
{
	std::seed_seq seq(std::begin(seed), std::end(seed));
	return std::mt19937(seq);
}

[[noreturn]] void die(const char *s)
{
	fprintf(stderr, "***** %s *****\n", s); printf("***** %s *****\n", s); exit(1);
}

typedef struct { int key; int data; } el_t;

unsigned int cmp_cnt, ass_cnt;

int strcmp0(const char *s1, const char *s2)
{
	int d;
	while ((d = (*s1++ - *s2++)) == 0) if (s1[-1] == 0) return 0;
	return d;
}

int cmp_loop, cmp_val;
int cmpfnc(const void* xp, const void* yp) {
	cmp_cnt++;
	if (cmp_loop) {
		for (int c = cmp_loop; c>0; c--) cmp_val += strcmp0("abc", "def"); 
	}
	return ((const el_t*)(xp))->key - ((const el_t*)(yp))->key;
}

int counter, arr_max, div_val, rec_siz, itarate;
char *vec, *chk;

auto& KEY(int i)
{
	return ((el_t*)(vec + i*rec_siz))->key;
}
auto& DATA(int i)
{
	return ((el_t*)(vec + i * rec_siz))->data;
}
void do_qsort(int do_qs) {
	std::mt19937 engine = create_random_engine();
	for (counter = 0; counter<itarate; counter++) {
		/*データを用意する*/
		if (div_val == 0) for (int i = 0; i < arr_max; i++) KEY(i) = 5;         /*一定*/
		if (div_val == -1) for (int i = 0; i < arr_max; i++) KEY(i) = i + 1;       /*昇順*/
		if (div_val == -2) for (int i = 0; i < arr_max; i++) KEY(i) = arr_max - i; /*降順*/
		if (div_val == 1) for (int i = 0; i < arr_max; i++) KEY(i) = std::uniform_int_distribution<>(0, 2147483647)(engine);  /*乱数*/
		if (div_val >= 2) for (int i = 0; i < arr_max; i++) KEY(i) = std::uniform_int_distribution<>(0, div_val - 1)(engine);
		if (div_val == -3) {
			for (int i = 0; i < arr_max; i++) KEY(i) = i;       /*同値キーがない乱数　入れ替えで*/

			for (int i = 0; i < arr_max; i++) {
				const int x = std::uniform_int_distribution<>(0, arr_max - 1)(engine);
				std::swap(KEY(i), KEY(x));
			}
		}

		if (rec_siz >= 8)
			for (int i = 0; i < arr_max; i++) DATA(i) = i;   /*検査のための準備*/

		if (do_qs) qsort_selected((char*)vec, arr_max, rec_siz, cmpfnc);   /*ソートの実行*/

																  /*以下でソートできたことを検査する*/
		for (int i = 1; i < arr_max; i++) {
			if (div_val >= 0 ? KEY(i - 1) > KEY(i) : KEY(i - 1) >= KEY(i)) {
				if (do_qs == 0) continue;
				puts("");
				{
					for (int h = 0; h < arr_max && h < 40; h++) std::cout << ' ' << KEY(h);
					std::cout << " arr_max error" << std::endl;
				}
				{
					for (int h = 0; h <= i && h < 40; h++) std::cout << " .";
					std::cout << "← error is here" << std::endl;
				}
				std::cout << "  counter=" << counter << "   error i=" << i << std::flush;
				die("not sorted  do_qsort(1)");
			}
			else {
				if (do_qs == 0) continue;  // do_qsort(1) do_qsort(0) の時間をできるだけ合わせるための処理
			}
		}
		if (rec_siz >= 8) {
			for (int j = 0; j < arr_max; j++) chk[j] = 0;
			for (int j = 0; j < arr_max; j++) chk[DATA(j)] = 123;
			for (int j = 0; j < arr_max; j++) if (chk[j] != 123) die("chk err");
		}
	}
}


int main(int argc, char **argv) {
	clock_t clk_start, clk_end, clk_end2;
	cmp_cnt = ass_cnt = 0;

	if (argc != 9) die("Usage: main.exe div_val arr_max rec_siz itarate MID1 MID2 MID3 cmp");

	div_val = atoi(argv[1]);       /*テストデータの種類を指定する random()%div_val等*/
	arr_max = atoi(argv[2]);       /*要素の個数(要素数)*/
	rec_siz = atoi(argv[3]);       /*要素の大きさ(要素サイズ)*/
	itarate = atoi(argv[4]);       /*繰り返し回数*/
	g_QS_MID1 = atoi(argv[5]);     /* n がこれ以下で３点処理を行う qs9の既定値140*/
	g_QS_MID2 = atoi(argv[6]);     /* n がこれ以下で９点処理を行う qs9の既定値900*/
	g_QS_MID3 = atoi(argv[7]);     /* size がこれ以上=のときに間接ソートを追加 qs10の既定値400*/
	cmp_loop = atoi(argv[8]);     /*比較関数の重たさを調整する*/

	std::cerr
		<< std::endl << std::setw(8) << argv[0] + 2
		<< " d=" << div_val << " e=" << arr_max << " s=" << rec_siz << ' ' << arr_max*rec_siz / 1000000 << "M R" << itarate
		<< (sizeof(char*) == 8 ? 'M' : 'm') << '=' << g_QS_MID1 << ':' << g_QS_MID2 << ':' << g_QS_MID3
		<< ':' << cmp_loop << ": " << std::flush;

	std::cout
		<< std::setw(9) << argv[0] + 2
		<< " d=" << div_val << " e=" << arr_max << " s=" << rec_siz << ' ' << arr_max*rec_siz / 1000000 << " R" << itarate
		<< (sizeof(char*) == 8 ? 'M' : 'm')
		<< std::setw(3) << std::setfill('0') << g_QS_MID1 << ':'
		<< std::setw(3) << std::setfill('0') << g_QS_MID2 << ':'
		<< std::setw(3) << std::setfill('0') << g_QS_MID3 << ':'
		<< cmp_loop << ':' << std::flush;

	if (rec_siz < 4 || 20000 < rec_siz) die("本プログラムでは「要素のバイトサイズは４以上&２万以下」");
	if (rec_siz % 4) die("本プログラムでは「要素のバイトサイズは４の倍数」を仮定");
	if ((vec = (char*)malloc((arr_max + 5)*rec_siz)) == NULL) die("vec NULL");
	if ((chk = (char*)malloc(arr_max)) == NULL) die("chk NULL");

	clk_start = clock();
	do_qsort(1);
	clk_end = clock();
	do_qsort(0);
	clk_end2 = clock();

	{
		unsigned int cmp_av = cmp_cnt / itarate;
		unsigned int ass_av = ass_cnt / itarate;
		double sum_av = (double)cmp_av + ass_av;
		double etime = (double)((clk_end - clk_start) - (clk_end2 - clk_end)) / CLOCKS_PER_SEC;
		fprintf(stderr, "   %4.0f ", etime / itarate * 100000);
		printf(" c=%-6u %10u a=%-6u %10u i=%1.0f T=%1.2f %4.0f \n", cmp_av, cmp_cnt, ass_av, ass_cnt, sum_av, etime, etime / itarate * 100000);
	}

	fflush(stdout);
	return 0;
}
