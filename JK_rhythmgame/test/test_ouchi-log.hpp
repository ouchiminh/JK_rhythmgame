#pragma once
#include "../src/log/out.hpp"
#include "../src/log/rule.hpp"
#include "test.hpp"

namespace jk::test {

DEFINE_TEST(log_test) {
	std::ofstream f1("app2.log");
	std::ofstream f2("app3.log");
	ouchi::log::basic_out<char> logger1{ [&f1](std::string_view str) {f1 << str << std::endl; } };
	ouchi::log::basic_out<char> logger2{ [&f2](std::string_view str) {f2 << str << std::endl; } };
	ouchi::log::rule r;
	
	ouchi::log::ruled_logger<char> rl{ {r,logger1}, {r,logger2} };

	rl.trace("unko");
	rl.info("inpo");
}

}// jk::test
