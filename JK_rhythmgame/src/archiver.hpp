#pragma once
#include <filesystem>
#include <vector>
#include "stream-traits.hpp"
#include "arcfile.hpp"

namespace jk::archive {
	class archiver {
		std::vector<file> list_;
		
		/// <summary>discard existing data, and read archive file header / reserve list_</summary>
		void init();	
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
		void remove_file(const std::filesystem::path & filepath);

		/// <summary>
		/// if fail, throw exception
		/// </summary>
		template<class OStream>
		void write(OStream & out) const;
		void write(const std::filesystem::path & filepath) const;

		/// <summary>load designated file.</summary>
		/// <param name = "filepath">path to the file will be loaded.</param>
		/// <returns>ture</returns>
		bool load(const std::filesystem::path & filepath);

		file & get(const std::filesystem::path & filepath);
		const file & get(const std::filesystem::path & filepath) const;
	};
}
