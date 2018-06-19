/*=============================================================================
  Copyright (C) 2018 河村　知行 <t-kawa@crux.ocn.ne.jp>
  Copyright (C) 2018 yumetodo <yume-wikijp@live.jp>
  Distributed under the Boost Software License, Version 1.0.
  (See https://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef INC_TARGET_DATA_HPP_
#define INC_TARGET_DATA_HPP_
#include <memory>
#include <chrono>
#include <cstdint>
#include <stdexcept>

struct el_t { std::int32_t key; std::int32_t data; };
static_assert(sizeof(el_t) == 8, "err");

using comp_func_t = int(*)(const void *a, const void *b);
using qsort_func_t = void(*)(void*, size_t, size_t, comp_func_t);
using std::size_t;

class target_data {
private:
	std::unique_ptr<char[]> vec;
	int div_val;
	size_t arr_max;
	size_t rec_siz;
	static size_t cmp_cnt;
	static int cmpfnc(const void* xp, const void* yp) noexcept;
	static std::unique_ptr<char[]> allocate_target_data(size_t arr_max, size_t rec_siz) noexcept(false);
public:
	using clock = std::chrono::high_resolution_clock;
	using duration = clock::duration;
	static size_t get_cmp_cnt() noexcept;
	target_data() = delete;
	target_data(const target_data&) = delete;
	target_data(target_data&&) = default;
	target_data& operator=(const target_data&) = delete;
	target_data& operator=(target_data&&) = delete;
	target_data(int div_val, size_t arr_max, size_t rec_siz);
	auto& key(size_t i) noexcept { return reinterpret_cast<el_t*>(vec.get() + i * rec_siz)->key; }
	const auto& key(size_t i) const noexcept { return reinterpret_cast<el_t*>(vec.get() + i * rec_siz)->key; }
	auto& data(size_t i) noexcept { return reinterpret_cast<el_t*>(vec.get() + i * rec_siz)->data; }
	const auto& data(size_t i) const noexcept { return reinterpret_cast<el_t*>(vec.get() + i * rec_siz)->data; }
	void init();
	duration sort(qsort_func_t qsort_f);
	void check_sorted() noexcept(false);
};

class not_sorted_exception : public std::runtime_error {
private:
	static std::string format(const target_data& data, size_t not_sorted_pos);
public:
	not_sorted_exception(const target_data& data, size_t not_sorted_pos);
};

#endif //INC_TARGET_DATA_HPP_
