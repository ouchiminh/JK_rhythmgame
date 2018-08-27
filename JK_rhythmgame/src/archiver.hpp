#pragma once
#include <filesystem>
#include "stream-traits.hpp"

namespace jk {
	class archiver {
	public:
		bool add_file(const std::filesystem::path & filepath);
		void remove_file(const std::filesystem::path & filepath);

		template<class OStream>
		void write(OStream & out);
	};
}
