#pragma once
#include "../src/log.hpp"
#include "test.hpp"

namespace jk::test {

DEFINE_TEST(log_test) {
	::ouchi::log::basic_out<char, ouchi::log::default_output<char>, std::string> logger("app2.log");
	logger.format_ << ouchi::log::msg << ouchi::log::cat;
	logger.fatal("poi");
}

}// jk::test