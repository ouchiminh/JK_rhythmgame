#pragma once
#include "test.hpp"
#include "../src/color-manager.hpp"

namespace jk::test {
	DEFINE_TEST(color_mng_test) {
		CHECK_TRUE(!jk::color::color_mng::get("Data.theme_color"));
		CHECK_NOTHROW(jk::color::color_mng::set(".\\setting\\color.json"));
		auto test_color = jk::color::color_mng::get("Data.theme_color");
		CHECK_TRUE(test_color && test_color.value() == sf::Color(0xFF, 0xA0, 0, 255));
		CHECK_TRUE(!jk::color::color_mng::get("Data.chanto_error_ni_naru_color"));
	}
}
