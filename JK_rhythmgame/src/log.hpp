#pragma once
#include <time.h>
#include <string>
#include <type_traits>
#include <vector>
#include <variant>
#include <iomanip>
#include <fstream>
#include <utility>

namespace ouchi::log{

enum class category { fatal, error, warn, info, debug, trace };
enum class msg_elem_type { msg, category, time, user_defined /* = concrete string. */ };

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
inline std::basic_string<CharT> time() {
	return time_to_ISO8601(time(NULL));
}

template<class CharT>
class msg_element {
public:
	using var_t = std::variant<std::basic_string<CharT>, msg_elem_type>;
	msg_element(std::basic_string<CharT> const & str);
	msg_element(std::basic_string<CharT> && str);
	explicit msg_element(msg_elem_type elm);

	var_t get_content() const noexcept;
private:
	var_t content_;
};

template<class CharT>
class message_format {
	std::vector<msg_element<CharT>> format_;

public:
	explicit message_format<CharT> & operator<<(msg_element<CharT> elm);
	message_format<CharT> & operator<<(std::basic_string_view<CharT> str);

	void clear() noexcept;

	using iterator = std::vector<msg_element<CharT>>::iterator;
	iterator begin();
	iterator end();
};

#pragma region "message_format_def"
template<class CharT>
inline message_format<CharT> & message_format<CharT>::operator<<(msg_element<CharT> elm) {
	format_.emplace_back(elm);
	return *this;
}

template<class CharT>
inline message_format<CharT>& message_format<CharT>::operator<<(std::basic_string_view<CharT> str) {
	format_.emplace_back(str);
	return *this;
}

template<class CharT>
inline void message_format<CharT>::clear() noexcept {
	format_.clear();
}
template<class CharT>
inline message_format<CharT>::iterator message_format<CharT>::begin() {
	return format_.begin();
}
template<class CharT>
inline message_format<CharT>::iterator message_format<CharT>::end() {
	return format_.end();
}
#pragma endregion

#pragma region "msg_element_def"

template<class CharT>
inline msg_element<CharT>::msg_element(std::basic_string<CharT> const & str) : content_(str){}

template<class CharT>
inline msg_element<CharT>::msg_element(std::basic_string<CharT>&& str) : content_(str) {}

template<class CharT>
inline msg_element<CharT>::msg_element(msg_elem_type elm) : content_(elm) {}

template<class CharT>
inline msg_element<CharT>::var_t msg_element<CharT>::get_content() const noexcept {
	return content_;
}


#pragma endregion

template<class CharT>
class default_output {
	inline static std::basic_ofstream<CharT> file("app.log");
public:
	void operator()(std::basic_string_view<CharT> str) { file << str << std::endl; }
};

/// <summary>
/// this class writes log.
/// </summary>
/// <param name="Func">このクラスの()演算子に渡された文字列が出力されます。</param>
/// <param name="Args">Funcのコンストラクタに渡す引数。</param>
template<class CharT, class Func = default_output<CharT>, class ...Args>
class basic_out {
	Func f_;
public:
	basic_out(Args ...args);
	
	void fatal(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));
	void error(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));
	void warn(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));
	void info(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));
	void debug(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));
	void trace(std::basic_string_view<CharT> msg) noexcept(noexcept(std::declval<Func>()()));

	message_format<CharT> format_;
};

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>::basic_out(Args ...args) : f_(args...) {}

} // jk::log
