/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include "qs9e17.h"
#include "qs10a6.h"
#include "qs10a6m.h"
#include "qs226ms2.h"
#include "global_variable.h"
#include "target_data.hpp"
#include "time_logger.hpp"
#include <array>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <deque>
#include <memory>
namespace {

/**
 * execute qsort
 * @param qsort_func_name qsort関数の名前
 * @param qsort_f qsort関数へのポインタ
 * @param div_val テストデータの種類
 * @param arr_max 要素の個数(要素数)
 * @param rec_siz 要素の大きさ(要素サイズ)
 * @param itarate 繰り返し回数
 */
time_logger::analyzer do_qsort(
	const char* qsort_func_name, qsort_func_t qsort_f,
	int div_val, size_t arr_max, size_t rec_siz, size_t itarate
) {
	target_data data(div_val, arr_max, rec_siz);
	time_logger logger(qsort_func_name, div_val, arr_max, rec_siz);
	for (size_t counter = 0; counter<itarate; counter++) {
		data.init();
		logger.push(data.sort(qsort_f));
		data.check_sorted();
	}
	logger.set_cmp_cnt(target_data::get_cmp_cnt());
#ifdef DEBUG
	logger.set_ass_cnt(get_ass_cnt());
#endif
	return logger.analyze();
}

size_t decide_itarate(int div_val, size_t rec_siz, size_t z)
{
	switch (div_val) {
	case -3:  return ((rec_siz < 500) ? 10000 : 2000) / 10 * z;
	case 100: return ((rec_siz < 500) ? 20000 : 2000) / 10 * z;
	case 2:   return ((rec_siz < 500) ? 40000 : 4000) / 10 * z;
	default: throw std::invalid_argument("not implemented case.");
	}
}
}
int main(int argc, char **argv) {
	try {
		switch (argc)
		{
		case 4:
			g_QS_MID3 = std::stoi(argv[7]);//size がこれ以上=のときに間接ソートを追加 qs10の既定値400
			/* fall through */
		case 3:
			g_QS_MID2 = std::stoi(argv[6]);//n がこれ以下で９点処理を行う qs9の既定値900
			/* fall through */
		case 2:
			g_QS_MID1 = std::stoi(argv[5]);//n がこれ以下で３点処理を行う qs9の既定値140
			/* fall through */
		default:
			break;
		}
		const char* const qsort_func_name[] = { "qsort", "qs9e17", "qs10a6", "qs10a6m", "qs226ms2" };
		const qsort_func_t qsort_f[] = { qsort, qsort9e17, qsort10a6, qsort10a6m, qsort226ms2 };
		static_assert(std_future::size(qsort_func_name) == std_future::size(qsort_f), "err");
		for (int div_val : { -3, 100, 2 }) for (size_t arr_max : { 10000u }) for (size_t rec_siz : { 8u, 20u, 24u, 500u, 1000u }) {
			const size_t iterate = decide_itarate(div_val, rec_siz, 10);
			std::array<time_logger::analyzer, std_future::size(qsort_f)> analyzer{};
			for (size_t i = 0; i < std_future::size(qsort_f); ++i) {
				analyzer[i] = do_qsort(qsort_func_name[i], qsort_f[i], div_val, arr_max, rec_siz, iterate);
			}
			print_analyzed_data_array(std::cout, analyzer);
		}
	}
	catch (const std::exception& er) {
		std::cerr << er.what() << std::endl;
	}
}
