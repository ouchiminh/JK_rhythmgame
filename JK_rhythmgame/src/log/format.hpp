#pragma once
#include <time.h>
#include <string>
#include <type_traits>
#include <vector>
#include <variant>
#include <iomanip>
#include <fstream>


namespace ouchi::log {

enum class cat_v { fatal, error, warn, info, debug, trace };

inline namespace msg_elem_types {
struct Msg {}msg;

struct Category{}cat;

struct Time{}time;
}// msg_elem_types

template<typename CHAR_T>
inline std::basic_string<CHAR_T> time_to_ISO8601(time_t gmt) {
	using namespace std;
	basic_stringstream<CHAR_T> ss;
	tm t;
	localtime_s(&t, &gmt);
	ss << t.tm_year + 1900 << '-' << setw(2) << setfill((CHAR_T)'0') << t.tm_mon + 1 << '-' << setw(2) << setfill((CHAR_T)'0') << t.tm_mday
		<< 'T' << setw(2) << setfill((CHAR_T)'0') << t.tm_hour << ':' << setw(2) << setfill((CHAR_T)'0') << t.tm_min << ':' << setw(2) << setfill((CHAR_T)'0') << t.tm_sec;
	return ss.str();
}

template<typename CharT>
inline std::basic_string<CharT> time_to_ISO8601() {
	return time_to_ISO8601<CharT>(::time(NULL));
}

template<class CharT>
class msg_element {
public:
	typedef std::variant<std::basic_string<CharT>, CharT, Msg, Category, Time> var_t;
	msg_element(std::basic_string<CharT> const & str) : content_(str) {}
	msg_element(std::basic_string<CharT> && str) : content_(str) {}
	msg_element(CharT c) : content_(c) {}
	msg_element(Msg & m) : content_(m) {}
	msg_element(Category & c) : content_(c) {}
	msg_element(Time & t) : content_(t) {}

	var_t get_content() const noexcept {
		return content_;
	}
private:
	var_t content_;
};

template<class CharT>
class message_format {
	std::vector<msg_element<CharT>> format_;

public:
	message_format<CharT> & operator<<(msg_element<CharT> elm);
	//message_format<CharT> & operator<<(CharT c);

	void clear() noexcept {
		format_.clear();
	}

	using iterator = typename std::vector<msg_element<CharT>>::iterator;
	iterator begin() {
		return format_.begin();
	}
	iterator end() {
		return format_.end();
	}
};

#pragma region "message_format_def"
template<class CharT>
inline message_format<CharT> & message_format<CharT>::operator<<(msg_element<CharT> elm) {
	format_.emplace_back(elm);
	return *this;
}

//template<class CharT>
//inline message_format<CharT>& message_format<CharT>::operator<<(CharT c) {
//	format_.emplace_back(c);
//	return *this;
//}

#pragma endregion

}
