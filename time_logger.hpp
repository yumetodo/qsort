#pragma once
#include <array>
#include <chrono>
#include <deque>
#include <string>
#include <future>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "std_future.hpp"
template<typename T>
struct analyzer_c {
	using value_type = T;
	std::size_t itarate;
	double average;
	double stdev;
	double se;
	double confidence_95;
	double confidence_95_min;
	double confidence_95_max;
	value_type min;
	value_type max;
	const char* qsort_func_name;
	int div_val;
	size_t arr_max;
	size_t rec_siz;
	size_t cmp_cnt;
#ifdef DEBUG
	size_t ass_cnt;
#endif
private:
	static double calc_sum(const std::deque<value_type>& logbuf) {
		return std::accumulate(logbuf.begin(), logbuf.end(), 0.0);
	}
	static double calc_average(double sum, std::size_t count) {
		return sum / count;
	}
	static double calc_stdev(const std::deque<value_type>& logbuf, double average) {
		return std::sqrt(
			std::accumulate(logbuf.begin(), logbuf.end(), 0.0, [average](double sum, value_type val) {
				return sum + std::pow(static_cast<double>(val) - average, 2);
			}) / (logbuf.size() - 1)
		);
	}
	static double calc_se(double stdev, std::size_t count) {
		return stdev / std::sqrt(count);
	}
	static double calc_confidence_95(double se) {
		return 1.959964 * se;
	}
	static std::pair<double, double> calc_confidence_interval(double average, double confidence) {
		return{ average - confidence, average + confidence };
	}
	static std::pair<value_type, value_type>  minmax(const std::deque<value_type>& logbuf) {
		const auto it_minmax = std::minmax_element(logbuf.begin(), logbuf.end());
		return{ *it_minmax.first, *it_minmax.second };
	}
public:
	analyzer_c() = default;
	analyzer_c(const analyzer_c&) = default;
	analyzer_c(analyzer_c&&) = default;
	analyzer_c& operator=(const analyzer_c&) = default;
	analyzer_c& operator=(analyzer_c&&) = default;
	analyzer_c(
		const std::deque<value_type>& logbuf, const char* qsort_func_name, int div_val, size_t arr_max, size_t rec_siz, size_t cmp_cnt
#ifdef DEBUG
		, size_t ass_cnt
#endif
	)
		: itarate(logbuf.size()), qsort_func_name(qsort_func_name), div_val(div_val), arr_max(arr_max), rec_siz(rec_siz), cmp_cnt(cmp_cnt)
#ifdef DEBUG
		, ass_cnt(ass_cnt)
#endif
	{
		auto minmax_th = std::async([&logbuf]() { return minmax(logbuf); });
		average = calc_average(calc_sum(logbuf), itarate);
		stdev = calc_stdev(logbuf, average);
		se = calc_se(stdev, itarate);
		confidence_95 = calc_confidence_95(se);
		std::tie(confidence_95_min, confidence_95_max) = calc_confidence_interval(average, confidence_95);
		std::tie(min, max) = minmax_th.get();
	}
};
template<typename T, std::size_t N>
void print_analyzed_data_array(std::ostream& os, std::array<analyzer_c<T>, N>& arr)
{
	if (arr.empty()) throw std::invalid_argument("arr doesn't contain element");
	analyzer_c<T>& arr_front = arr.front();
	if (std::any_of(arr.begin() + 1, arr.end(), [&arr_front](const analyzer_c<T>& analyzed) {
		return arr_front.div_val != analyzed.div_val || arr_front.arr_max != analyzed.arr_max || arr_front.rec_siz != analyzed.rec_siz;
	})) {
		throw std::invalid_argument("div_val, arr_max or rec_siz is not equal.");
	}
	using std::endl;
	using std::fixed;
	using std::setprecision;
	os
		<< "div_val=" << arr_front.div_val << ":arr_max=" << arr_front.arr_max << ":rec_siz=" << arr_front.rec_siz << endl
		<< endl << "|       |";
	for (analyzer_c<T>& d : arr) os << d.qsort_func_name << '|';
	os << endl << "|-------|";
	for (size_t i = 0; i < N; ++i) os << "--|";
	os << endl << "|iterate|";
	for (analyzer_c<T>& d : arr) os << d.itarate << '|';
	os << endl << "|cmp_avg|";
	for (analyzer_c<T>& d : arr) os << d.cmp_cnt / d.itarate << '|';
#ifdef DEBUG
	os << endl << "|ass_cnt|";
	for (analyzer_c<T>& d : arr) os << d.ass_cnt / d.itarate << '|';
#endif
	os << endl << "|max|";
	for (analyzer_c<T>& d : arr) os << d.max << '|';
	os << endl << "|min|";
	for (analyzer_c<T>& d : arr) os << d.min << '|';
	os << endl << "|avg|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.average << '|';
	os << endl << "|stdev|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.stdev << '|';
	os << endl << "|se|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.se << '|';
	os << endl << "|95%CI|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.confidence_95 << '|';
	os << endl << "|95%CI.max|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.confidence_95_max << '|';
	os << endl << "|95%CI.min|";
	for (analyzer_c<T>& d : arr) os << fixed << setprecision(7) << d.confidence_95_min << '|';
	os << endl;
}

class time_logger {
public:
	using logging_unit = std::chrono::nanoseconds;
	using rep = logging_unit::rep;
	using analyzer = analyzer_c<rep>;
private:
	std::deque<rep> logbuf_;
	const char* qsort_func_name_;
	int div_val_;
	size_t arr_max_;
	size_t rec_siz_;
	size_t cmp_cnt_;
#ifdef DEBUG
	size_t ass_cnt_;
#endif
public:
	time_logger() = delete;
	time_logger(const time_logger&) = delete;
	time_logger(time_logger&&) = default;
	time_logger& operator=(const time_logger&) = delete;
	time_logger& operator=(time_logger&&) = default;
	time_logger(const char* qsort_func_name, int div_val, size_t arr_max, size_t rec_siz)
		: logbuf_(), qsort_func_name_(qsort_func_name), div_val_(div_val), arr_max_(arr_max), rec_siz_(rec_siz), cmp_cnt_()
#ifdef DEBUG
		, ass_cnt_()
#endif
	{}
	void push(const logging_unit& ns) {
		this->logbuf_.emplace_back(ns.count());
	}
	template<typename rep, typename period, std::enable_if_t<!std::is_same<std::chrono::duration<rep, period>, logging_unit>::value, std::nullptr_t> = nullptr>
	void push(const std::chrono::duration<rep, period>& time) {
		this->push(std::chrono::duration_cast<logging_unit>(time));
	}
	void set_cmp_cnt(size_t cmp_cnt) noexcept { this->cmp_cnt_ = cmp_cnt; }
#ifdef DEBUG
	void set_ass_cnt(size_t ass_cnt) noexcept { this->ass_cnt_ = ass_cnt; }
#endif
	analyzer analyze() { return {
		this->logbuf_, this->qsort_func_name_, this->div_val_, this->arr_max_, this->rec_siz_, this->cmp_cnt_
#ifdef DEBUG
		, this->ass_cnt_
#endif
	}; }
	void clear(){ this->logbuf_.clear(); }
};
