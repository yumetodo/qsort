/*=============================================================================
Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
Distributed under the Boost Software License, Version 1.0.
(See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "target_data.hpp"
#include "global_variable.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <random>
#include <deque>

namespace {
constexpr char32_t seed[] = U"glibc newlib より速いクイックソート(qs9 qs10)、 世界最速をめざしてqsortの新しいアルゴリズム qs9 を開発しましたその結果、ソート対象によっては間接ソートを取り入れた方が高速なことが判りました。今回、間接ソートを取り入れた qs10 を開発しました。お願い現在、 標準qsort　qs9　qs10　を比較するベンチマークテストを行っております。";
std::mt19937 create_random_engine()
{
	std::seed_seq seq(std::begin(seed), std::end(seed));
	return std::mt19937(seq);
}
}
int target_data::cmpfnc(const void* xp, const void* yp) noexcept {
	cmp_cnt++;
	return ((const el_t*)(xp))->key - ((const el_t*)(yp))->key;
}
size_t target_data::cmp_cnt;

size_t target_data::get_cmp_cnt() noexcept { return cmp_cnt; }

std::unique_ptr<char[]> target_data::allocate_target_data(size_t arr_max, size_t rec_siz) noexcept(false) {
	if (rec_siz < 4 || 20000 < rec_siz) throw std::invalid_argument("本プログラムでは「要素のバイトサイズは４以上&２万以下」");
	if (rec_siz % 4) throw std::invalid_argument("本プログラムでは「要素のバイトサイズは４の倍数」を仮定");
	return std::make_unique<char[]>((arr_max + 5)*rec_siz);
}

target_data::target_data(int div_val, size_t arr_max, size_t rec_siz) : vec(allocate_target_data(arr_max, rec_siz)), div_val(div_val), arr_max(arr_max), rec_siz(rec_siz)
{
	cmp_cnt = 0;
#ifdef DEBUG
	init_ass_cnt();
#endif
}

void target_data::init() {
	std::mt19937 engine = create_random_engine();
	switch (div_val) {
	case 0: for (size_t i = 0; i < arr_max; i++) key(i) = 5; break; //一定
	case -1: for (size_t i = 0; i < arr_max; i++) key(i) = int(i) + 1; break;//昇順
	case -2: for (size_t i = 0; i < arr_max; i++) key(i) = int(arr_max - i); break;//降順
	case -3://同値キーがない乱数　入れ替えで
		for (size_t i = 0; i < arr_max; i++) key(i) = int(i);
		for (size_t i = 0; i < arr_max; i++) {
			const int x = std::uniform_int_distribution<>(0, int(i))(engine);
			std::swap(key(i), key(x));
		}
		break;
	case 1: for (size_t i = 0; i < arr_max; i++) key(i) = std::uniform_int_distribution<>(0, 2147483647)(engine); break;//乱数
	default:
		if (div_val >= 2) for (size_t i = 0; i < arr_max; i++) key(i) = std::uniform_int_distribution<>(0, div_val - 1)(engine);
		break;
	}
	if (rec_siz >= 8) {
		for (size_t i = 0; i < arr_max; i++) data(i) = int(i);   /*検査のための準備*/
	}
}

target_data::duration target_data::sort(qsort_func_t qsort_f) {
	const auto t1 = clock::now();
	qsort_f(vec.get(), arr_max, rec_siz, cmpfnc);
	return clock::now() - t1;
}

void target_data::check_sorted() noexcept(false) {
	for (size_t i = 1; i < arr_max; i++) {
		if (div_val >= 0 ? key(i - 1) > key(i) : key(i - 1) >= key(i)) {
			throw not_sorted_exception(*this, i);
		}
	}
	if (rec_siz >= 8) {
		std::deque<bool> checker(arr_max);
		for (size_t i = 0; i < arr_max; ++i) checker[data(i)] = true;
		if (!std::all_of(checker.begin(), checker.end(), [](bool b) noexcept { return b; })) throw std::runtime_error("sort check failed.");
	}
}

std::string not_sorted_exception::format(const target_data & data, size_t not_sorted_pos) {
	static constexpr size_t limit_output_width = 100;
	using std::string;
	using s_pair = std::pair<string, string>;
	//.first: index, .second: elem
	std::vector<s_pair> index_elem_str_src;
	index_elem_str_src.reserve(limit_output_width / 3);
	auto not_sorted_pos_str = std::to_string(not_sorted_pos);
	const size_t elem_width_min = not_sorted_pos_str.size();
	size_t width = 0;
	for (size_t i = 0; i < not_sorted_pos; ++i, ++width) {
		auto s = std::to_string(data.key(not_sorted_pos - i));
		const auto size = std::min(elem_width_min, s.size());
		if (width + size > limit_output_width) break;
		width += size;
		index_elem_str_src.emplace_back(std::make_pair(string{}, std::move(s)));
		if (width + 1 > limit_output_width) break;
	}
	std::reverse(index_elem_str_src.begin(), index_elem_str_src.end());
	for (size_t i = 0; i < index_elem_str_src.size() - 1; ++i) {
		index_elem_str_src[i].first = std::to_string(not_sorted_pos + 1 - index_elem_str_src.size() + i);
	}
	index_elem_str_src.back().first = std::move(not_sorted_pos_str);
	auto index = std::accumulate(
		index_elem_str_src.begin(), index_elem_str_src.end(),
		[] {string s; s.reserve(limit_output_width * 3 + 20); return s; }(),
		[](string s, const s_pair& i_e) {
		if (!s.empty()) s += ' ';
		if (i_e.first.size() < i_e.second.size()) s += string(i_e.second.size() - i_e.first.size(), ' ');
		s += i_e.first;
		return s;
	}
	) + '\n';
	auto index_elem_str = std::accumulate(index_elem_str_src.begin(), index_elem_str_src.end(), std::move(index), [](string s, const s_pair& i_e) {
		if ('\n' != s.back()) s += ' ';
		if (i_e.second.size() < i_e.first.size()) s += string(i_e.first.size() - i_e.second.size(), ' ');
		s += i_e.second;
		return s;
	}) + '\n';
	const auto not_sorted_pos_width = std::max(index_elem_str_src.back().first.size(), index_elem_str_src.back().second.size());
	return std::move(index_elem_str) + string(width - not_sorted_pos_width, ' ') + '^' + string(not_sorted_pos_width - 1, '~');
}

not_sorted_exception::not_sorted_exception(const target_data & data, size_t not_sorted_pos)
	: std::runtime_error(
		"exception : not_sorted_exception:\n" + format(data, not_sorted_pos)
	)
{}
