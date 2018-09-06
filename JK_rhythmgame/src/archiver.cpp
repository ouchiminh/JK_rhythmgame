#include <string>
#include <algorithm>
#include "archiver.hpp"

namespace fs = std::filesystem;


/// <summary>
/// discard existing data, and read archive file header / reserve list_
/// </summary>

inline void jk::archive::archiver::init(std::istream & in) noexcept(false) {
	size_t filecnt;
	list_.clear();
	in.read((char*)&filecnt, sizeof(filecnt));
	if (filecnt == 0) throw std::out_of_range("archive file is too short");
	list_.reserve(filecnt + 1);
	for (unsigned i = 0; i < filecnt; i++) {
		std::string filepath;
		size_t filesize;
		char buf;
		do {
			in.read(&buf, sizeof(char));
			filepath.push_back(buf);
		} while (buf);
		in.read((char*)(void*)&filesize, sizeof(filesize));
		list_.emplace_back(filepath, filesize);
	}
}

bool jk::archive::archiver::add_file(const std::filesystem::path & filepath) noexcept {
	if(!fs::exists(filepath)) return false;
	try {
		list_.emplace_back(filepath);
	} catch (...) { return false; }
	return true;
}

void jk::archive::archiver::remove_file(const std::filesystem::path & filepath) noexcept {
	using namespace std;
	auto tar = std::find(begin(list_), end(list_), filepath);
	if (tar == end(list_)) return;
	list_.erase(tar);
}

void jk::archive::archiver::write(const std::filesystem::path & filepath) const {
	using namespace std;
	std::ofstream out;
	out.open(filepath, std::ios::binary);
	write(out);
}

bool jk::archive::archiver::load(const std::filesystem::path & filepath) noexcept {
	if(!fs::exists(filepath)) return false;
	std::ifstream in(filepath, std::ios::binary || std::ios::in);
	return load(in);
}

inline bool jk::archive::archiver::load(std::istream & in) noexcept {
	try {
		init(in);
		for (auto & i : list_) i.load_from_archive(in);
	} catch (std::ios::failure &) { return false; }
	catch (...) { return false; }
	return true;
}

jk::archive::file & jk::archive::archiver::get(const std::filesystem::path & filepath) {
	using namespace std;
	auto tar = find(begin(list_), end(list_), filepath);
	if (tar != end(list_)) return *tar;
	throw std::out_of_range("no such elements in this archive file");
}

const jk::archive::file & jk::archive::archiver::get(const std::filesystem::path & filepath) const {
	using namespace std;
	auto tar = find(cbegin(list_), cend(list_), filepath);
	if (tar != cend(list_)) return *tar;
	throw std::out_of_range("no such elements in this archive file");
}
