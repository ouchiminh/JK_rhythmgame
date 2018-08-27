#pragma once
#include <filesystem>
#include <string>
#include <cstddef>
#include "stream-traits.hpp"

namespace jk::archive {
	class file {
		void * body_;	// if invalid, nullptr
		size_t size_;	// if invalid, 0
		std::filesystem::path filepath_;
	public:
		template<class IStream>
		file(const std::filesystem::path & filepath, IStream & in, size_t size);
		template<class IStream>
		file(std::filesystem::path && filepath, IStream & in, size_t size);
		file();			// initialize as invalid object

		bool is_avail() const noexcept;
		void * get_body() noexcept;
		void const * get_body() const noexcept;
		size_t size() const noexcept;

		template<class OStream>
		void write(OStream & out);

		static bool operator==(const file & f, const std::filesystem::path & p) noexcept;
		static bool operator==(const std::filesystem::path & p, const file & f) noexcept;
	};
}