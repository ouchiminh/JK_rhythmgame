#pragma once
#include <filesystem>
#include <vector>
#include "stream-traits.hpp"
#include "arcfile.hpp"

namespace jk::archive {
	class archiver {
		std::vector<file> list_;

		void init();	// read archive file header
	public:
		bool add_file(const std::filesystem::path & filepath);
		void remove_file(const std::filesystem::path & filepath);

		template<class OStream>
		void write(OStream & out) const;
		void write(const std::filesystem::path & filepath) const;

		bool read(const std::filesystem::path & filepath);

		file & get(const std::filesystem::path & filepath);
		const file & get(const std::filesystem::path & filepath) const;
	};
}
