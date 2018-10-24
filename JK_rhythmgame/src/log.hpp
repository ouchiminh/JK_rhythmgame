#pragma once
#include <time.h>
#include <string>
#include <type_traits>
#include <vector>
#include <variant>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <utility>

namespace ouchi::log{

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
	message_format<CharT> & operator<<(CharT c);

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

template<class CharT>
inline message_format<CharT>& message_format<CharT>::operator<<(CharT c) {
	format_.emplace_back(c);
	return *this;
}

#pragma endregion

#pragma region "msg_element_def"

#pragma endregion

template<class CharT>
class default_output {
	std::basic_ofstream<CharT> file_;
public:
	default_output(std::basic_string<CharT> const & filepath) { file_.open(filepath); }
	default_output() { file_.open("app.log"); }
	void operator()(std::basic_string_view<CharT> str) { file_ << str << std::endl; }
};



/// <summary>
/// this class writes log.
/// </summary>
/// <param name="Func">このクラスの()演算子に渡された文字列が出力されます。operator()(std::basic_string_view)</param>
/// <param name="Args">Funcのコンストラクタに渡す引数。</param>
template<class CharT, class Func = default_output<CharT>, class ...Args>
class basic_out {
	inline static constexpr char category_str[][32] = {
		"[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]"
	};
	Func f_;
	message_format<CharT> format_;

	void out(cat_v c, std::basic_string_view<CharT> msg);
	// ビジターのためのヘルパー型。
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

public:
	basic_out(Args ...args);
	using self_t = basic_out<CharT, Func, Args...>;
	basic_out<CharT, Func, Args...> & fatal(std::basic_string_view<CharT> msg);

	message_format<CharT> & reset_format();
};

#pragma region "basic_out def"

template<class CharT, class Func, class ...Args>
inline void basic_out<CharT, Func, Args...>::out(cat_v c, std::basic_string_view<CharT> msg) {
	std::basic_string<CharT> str;
	for (msg_element<CharT> const & i : format_) 
		std::visit(
			overloaded{
				[&str](Time) { str.append(time_to_ISO8601<CharT>()); },
				[&](Category) { std::basic_string<CharT> result; for (auto const & i : category_str[(unsigned)c]) { if (i == '\0') break; result.push_back(std::basic_ofstream<CharT>().widen(i)); } str.append(result); },
				[&msg, &str](Msg) { str.append(msg); },
				[&str](std::basic_string<CharT> const & arg) { str.append(arg); },
				[&str](CharT c) { str.push_back(c); }
			},
			i.get_content()
		);
	
	f_(str);
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>::basic_out(Args ...args) : f_(args...) {
	std::basic_ostringstream<CharT> ss;
	format_ << time << ss.widen(' ') << cat << ss.widen(' ') << msg;
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...> & basic_out<CharT, Func, Args...>::fatal(std::basic_string_view<CharT> msg) {
	out(cat_v::fatal, msg);
	return *this;
}

template<class CharT, class Func, class ...Args>
inline message_format<CharT>& basic_out<CharT, Func, Args...>::reset_format() {
	format_.clear();
	return format_;
}

#pragma endregion

#pragma region "log out utl macro"

#define LOG_WITH_FILEINFO(writer, msg)	do{\
	std::basic_stringstream<char> ss;\
	ss << msg << '@' << __FILE__ << ':' << __LINE__;\
	writer(ss.str());\
}while(false)

#pragma endregion

} // jk::log
