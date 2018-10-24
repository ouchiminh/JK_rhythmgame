#pragma once
#include "../src/log.hpp"
#include "test.hpp"
#include <unordered_map>

namespace jk::test {

DEFINE_TEST(log_test) {
	::ouchi::log::basic_out<char, ouchi::log::default_output<char>, std::string> logger("app2.log");
	logger.fatal(__FILE__);
	LOG_WITH_FILEINFO(logger.fatal, "poipoishipfucker!");
}

}// jk::test