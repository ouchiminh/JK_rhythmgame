#pragma once
#include <string_view>
#include "test.hpp"
#include "../src/beatmap-directory.hpp"

namespace jk::test {
	DEFINE_TEST(beatmap_dir_load_test) {
		std::string_view test_dir = "beatmap\\template\\";
		beatmap_directory bd;

		CHECK_THROW(bd.set_directory(".\\"));
		CHECK_NOTHROW(bd.set_directory(test_dir));

		for (auto i : bd) {
			LOG(i.get_path().string() + "\n");
		}
	}
}
