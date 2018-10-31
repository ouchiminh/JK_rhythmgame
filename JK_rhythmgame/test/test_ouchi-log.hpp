#pragma once
#include "../src/log/out.hpp"
#include "../src/log/rule.hpp"
#include "test.hpp"

namespace jk::test {
#if 0
DEFINE_TEST(log_test) {
	std::ofstream f1(".\\log\\app2.log");
	std::ofstream f2(".\\log\\app3.log");
	ouchi::log::basic_out<char> logger1;
	ouchi::log::basic_out<char> logger2{ [&f2](std::string_view str) {f2 << str << std::endl; } };
	ouchi::log::rule r;
	
	ouchi::log::ruled_logger<char> rl{ {r,logger1}, {r,logger2} };

	rl.trace("unko");
	rl.info("inpo");
}

DEFINE_TEST(out_func_test) {
	ouchi::log::out_func<char> of(".\\log\\out_func_test.log");
	ouchi::log::basic_out<char> logger(of);
	ouchi::log::rule r;
	ouchi::log::ruled_logger<char> rl{ {r, logger} };

	rl.info("test success??");
	rl.fatal("fatal success??");
}
#endif

DEFINE_TEST(read_setting_from_json_test) {
	ouchi::log::ruled_logger<char> logger(".\\test\\log_setting.json", "setting");
	logger.info("success");
	logger.fatal("success");
}

}// jk::test
