#pragma once
#include <filesystem>
#include <vector>
#include <fstream>
#include "stream-traits.hpp"
#include "arcfile.hpp"

namespace jk::archive {
	class archiver {
		std::vector<file> list_;
		
		/// <summary>
		/// discard existing data, and read archive file header / reserve list_
		/// </summary>
		void init(std::istream & in) noexcept(false);
	public:
		/// <summary>
		/// add file to archive list.
		/// </summary>
		/// <param name="filepath">target</param>
		/// <returns>
		/// <para>false : if file does not exist, cannot be read, or failed for other reasons.</para>
		/// <para>true : if nothing happened(succeeded)</para>
		/// </returns>
		bool add_file(const std::filesystem::path & filepath) noexcept;
		void remove_file(const std::filesystem::path & filepath) noexcept;

		/// <summary>
		/// if fail, throw exception
		/// </summary>
		template<class OStream>
		void write(OStream & out) const;
		void write(const std::filesystem::path & filepath) const;

		/// <summary>load designated archive file.</summary>
		/// <param name = "filepath">path to the file will be loaded.</param>
		/// <returns>true:succeed</returns>
		bool load(const std::filesystem::path & filepath) noexcept;
		bool load(std::istream & in) noexcept;

		file & get(const std::filesystem::path & filepath);
		const file & get(const std::filesystem::path & filepath) const;
	};
	template<class OStream>
	inline void archiver::write(OStream & out) const {
		using os = jk::ostream_traits<OStream>;

		for (auto & i : list_) i.write_header(out);
		for (auto & i : list_) i.write_body(out);
	}
}
