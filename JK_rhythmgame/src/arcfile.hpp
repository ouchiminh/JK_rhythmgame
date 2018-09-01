#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <stdexcept>
#include <filesystem>
#include <string>
#include <cstddef>
#include <fstream>
#include <utility>
#include <windows.h>
#include "stream-traits.hpp"

namespace jk::archive {
	class file {
		void * body_;	// if invalid, nullptr
		size_t size_;	// if invalid, 0
		std::filesystem::path filepath_;

		void discard() noexcept;
	public:
		/// <summary>
		/// construct & load content from normal file.
		/// </summary>
		/// <param name="filepath">path to normal file NOT ARCHIVE HEADER INFO</param>
		/// <returns></returns>
		file(std::filesystem::path && filepath) noexcept(false);
		file(const std::filesystem::path & filepath);
		file(const std::string & filepath, size_t size);
		file() noexcept;			// initialize as invalid object
		~file();

		void load_from_file(const std::filesystem::path & filepath);
		/// <summary>
		/// register header info before reading file body.
		/// </summary>
		/// <param name="filepath">filepath. could be an identifier</param>
		/// <param name="filesize">filesize. this param is used when load_from_archive</param>
		void register_archived_info(const std::filesystem::path & filepath, size_t filesize) noexcept;

		/// <summary>
		/// load file body from archive file from in's position.
		/// </summary>
		/// <param name="in">source</param>
		template<class IStream>
		void load_from_archive(IStream & in) noexcept(false);

		bool is_avail() const noexcept;
		void * get_body() noexcept;
		void const * get_body() const noexcept;
		size_t size() const noexcept;

		template<class OStream>
		void write_header(OStream & out) const noexcept(false);
		template<class OStream>
		void write_body(OStream & out) const noexcept(false);

		bool operator==(const std::filesystem::path & p) const noexcept;
	};

	inline void file::load_from_file(const std::filesystem::path & filepath) {
		namespace fs = std::filesystem;
		if(!fs::exists(filepath)) throw std::runtime_error("file not found.");
		discard();
		std::ifstream in;
		filepath_ = filepath;
		in.open(filepath, std::ios::binary | std::ios::in);
		try {
			size_ = fs::file_size(filepath);
			body_ = new std::int8_t[size_+1];
			in.read(static_cast<char*>(body_), size_);
		} catch (std::bad_alloc & e) {
			throw std::runtime_error(e.what());
		} catch (std::exception & e) { throw std::runtime_error(e.what()); }
	}

	inline void file::register_archived_info(const std::filesystem::path & filepath, size_t filesize) noexcept {
		discard();
		filepath_ = filepath;
		size_ = filesize;
	}

	template<class IStream>
	inline void file::load_from_archive(IStream & in) noexcept(false) {
		using is = jk::istream_traits<IStream>;
		if (filepath_.empty() || size_ == 0) throw std::logic_error("invalid pre-condition.");
		body_ = new std::int8_t[size_];
		is::read(in, body_, size_);
	}

	template<class OStream>
	inline void file::write_header(OStream & out) const noexcept(false) {
		namespace fs = std::filesystem;
		using is = ostream_traits<OStream>;
		if (!is_avail()) throw(std::runtime_error("invalid data"));
		is::write(out, (void*)filepath_.string<char>().c_str(), filepath_.string<char>().size() + 1);
		is::write(out, (void*)&size_, sizeof(size_));
	}

	template<class OStream>
	inline void file::write_body(OStream & out) const noexcept(false) {
		using is = ostream_traits<OStream>;
		if (!is_avail()) throw std::runtime_error("invalid data");
		is::write(out, body_, size_);
	}
}
