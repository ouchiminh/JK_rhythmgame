#pragma once
#include <stdexcept>
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
		/// <summary>
		/// <para>constructor</para>
		/// load from normal file.
		/// </summary>
		template<class IStream>
		file(const std::filesystem::path & filepath, IStream & in, size_t filesize) throw(std::runtime_error);
		template<class IStream>
		file(std::filesystem::path && filepath, IStream & in, size_t filesize) throw(std::runtime_error);
		file() noexcept;			// initialize as invalid object
		~file();

		template<class IStream>
		void load_from_archive(const std::filesystem::path & filepath, IStream & in, size_t filesize);
		template<class IStream>
		void load_from_file(const std::filesystem::path & filepath, IStream & in, size_t filesize);

		bool is_avail() const noexcept;
		void * get_body() noexcept;
		void const * get_body() const noexcept;
		size_t size() const noexcept;

		template<class OStream>
		void write_header(OStream & out) throw(std::runtime_error);
		template<class OStream>
		void write_body(OStream & out) throw(std::runtime_error);

		static bool operator==(const file & f, const std::filesystem::path & p) noexcept;
		static bool operator==(const std::filesystem::path & p, const file & f) noexcept;
	};

	template<class IStream>
	inline file::file(const std::filesystem::path & filepath, IStream & in, size_t filesize) throw(std::runtime_error) {
		load_from_file(filepath, in, filesize);
	}

	template<class IStream>
	inline file::file(std::filesystem::path && filepath, IStream & in, size_t filesize) throw(std::runtime_error) {
		load_from_file(filepath, in, filesize);
	}
	template<class IStream>
	inline void file::load_from_archive(const std::filesystem::path & filepath, IStream & in, size_t filesize) {
		using is = istream_traits<IStream>;
		filepath_ = filepath;
		size_ = filesize;
		body_ = new std::int8_t[filesize];
		is::read(in, body_, filesize);
	}
	template<class IStream>
	inline void file::load_from_file(const std::filesystem::path & filepath, IStream & in, size_t filesize) {
	using is = jk::istream_traits<IStream>;
		namespace fs = std::filesystem;
		if(!fs::exists(filepath)) throw std::runtime_error("file not found.");
		try {
			body_ = new std::int8_t[filesize];
			is::read(in, body_, filesize);
		} catch (std::bad_alloc & e) {
			throw std::runtime_error("allocation failed");
		} catch (std::exception & e) { throw std::runtime_error(e.what()); }
		size_ = filesize;
		filepath_ = filepath;
	}

	template<class OStream>
	inline void file::write_header(OStream & out) throw(std::runtime_error) {
		namespace fs = std::filesystem;
		using is = istream_traits<OStream>;
		if(!is_avail()) throw(std::runtime_error("invalid data"))
		auto relative_path = filepath_.lexically_relative(fs::current_path());
		is::write(out, relative_path.c_str(), relative_path.string().size());
		is::write(out, &size_, sizeof(size));
	}

	template<class OStream>
	inline void file::write_body(OStream & out) throw(std::runtime_error) {
		using is = istream_traits<OStream>;
		if (!is_avail()) throw std::runtime_error("invalid data");
		is::write(out, body_, size_);
	}

}