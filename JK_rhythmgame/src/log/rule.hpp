#pragma once
#include <algorithm>
#include <vector>
#include <initializer_list>
#include "format.hpp"
#include "out.hpp"

namespace ouchi::log {

struct rule {
private:
	cat_v upper_ = cat_v::fatal,
		lower_ = cat_v::info;
public:
	void set(cat_v upper, cat_v lower = cat_v::info) {
		upper_ = std::clamp(upper, cat_v::trace, cat_v::fatal);
		lower_ = std::clamp(lower, cat_v::trace, upper_);
	}
	bool is_valid(cat_v level) {
		return level <= upper_ && level >= lower_;
	}
};

template<class CharT>
class ruled_logger {
	std::vector<std::pair<rule, basic_out<CharT>>> logger_;
	
public:
	ruled_logger(std::initializer_list<std::pair<rule, basic_out<CharT>>> && il):logger_(std::move(il)) {}

	void out(cat_v c, std::basic_string_view<CharT> msg) {
		for (auto & i : logger_) {
			if (i.first.is_valid(c)) i.second.out(c, msg);
		}
	}
	void fatal(std::basic_string_view<CharT> msg) {
		out(cat_v::fatal, msg);
	}
	void error(std::basic_string_view<CharT> msg) {
		out(cat_v::error, msg);
	}
	void warn(std::basic_string_view<CharT> msg) {
		out(cat_v::warn, msg);
	}
	void info(std::basic_string_view<CharT> msg) {
		out(cat_v::info, msg);
	}
	void debug(std::basic_string_view<CharT> msg) {
		out(cat_v::debug, msg);
	}
	void trace(std::basic_string_view<CharT> msg) {
		out(cat_v::trace, msg);
	}

};

}
