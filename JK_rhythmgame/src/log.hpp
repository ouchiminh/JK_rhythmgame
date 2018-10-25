#pragma once
#include <time.h>
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <sstream>
#include <utility>
#include "log/format.hpp"

namespace ouchi::log{

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
	// visitorのためのヘルパー型。
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

public:
	basic_out(Args ...args);
	using self_t = basic_out<CharT, Func, Args...>;
	basic_out<CharT, Func, Args...> & fatal(std::basic_string_view<CharT> msg);
	basic_out<CharT, Func, Args...> & error(std::basic_string_view<CharT> msg);
	basic_out<CharT, Func, Args...> & warn(std::basic_string_view<CharT> msg);
	basic_out<CharT, Func, Args...> & info(std::basic_string_view<CharT> msg);
	basic_out<CharT, Func, Args...> & debug(std::basic_string_view<CharT> msg);
	basic_out<CharT, Func, Args...> & trace(std::basic_string_view<CharT> msg);


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
inline basic_out<CharT, Func, Args...>& basic_out<CharT, Func, Args...>::error(std::basic_string_view<CharT> msg) {
	out(cat_v::error, msg);
	return *this;
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>& basic_out<CharT, Func, Args...>::warn(std::basic_string_view<CharT> msg) {
	out(cat_v::warn, msg);
	return *this;
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>& basic_out<CharT, Func, Args...>::info(std::basic_string_view<CharT> msg) {
	out(cat_v::info, msg);
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>& basic_out<CharT, Func, Args...>::debug(std::basic_string_view<CharT> msg) {
	out(cat_v::debug, msg);
	return *this;
}

template<class CharT, class Func, class ...Args>
inline basic_out<CharT, Func, Args...>& basic_out<CharT, Func, Args...>::trace(std::basic_string_view<CharT> msg) {
	out(cat_v::trace, msg);
	return *this;
}

template<class CharT, class Func, class ...Args>
inline message_format<CharT>& basic_out<CharT, Func, Args...>::reset_format() {
	format_.clear();
	return format_;
}

#pragma endregion

#pragma region "log out utl macro"

#define LOG_WITH_FILEINFO(logger, msg)	do{\
	std::basic_stringstream<char> ss;\
	ss << msg << '@' << __FILE__ << ':' << __LINE__;\
	logger(ss.str());\
}while(false)

#pragma endregion

} // jk::log
