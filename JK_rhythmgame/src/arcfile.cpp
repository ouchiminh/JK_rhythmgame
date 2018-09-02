#include <memory.h>
#include "arcfile.hpp"

void jk::archive::file::discard() noexcept {
	size_ = 0;
	if (body_) delete[] body_;
	body_ = nullptr;
	filepath_.clear();
}

jk::archive::file::file(std::filesystem::path && filepath) noexcept(false) : body_( NULL ), size_{ 0 } { load_from_file(filepath); }

jk::archive::file::file(const std::filesystem::path & filepath) : body_( NULL ), size_{ 0 } { load_from_file(filepath); }

jk::archive::file::file(const std::string & filepath, size_t size) : body_( NULL ), size_( 0 ) { 
	register_archived_info(std::filesystem::path(filepath), size); 
}

jk::archive::file::file() noexcept : body_{ nullptr }, size_{ 0 } {}

jk::archive::file::file(file && f) : body_{ f.body_ }, size_{ f.size_ }, filepath_{ f.filepath_ } {
	f.body_ = nullptr;
	f.size_ = 0;
	f.filepath_.clear();
}

jk::archive::file::~file() {
	discard();
}

bool jk::archive::file::is_avail() const noexcept {
	return body_ != nullptr &&
		size_ != 0 &&
		filepath_.empty() ?
		false :
		true;
}

void * jk::archive::file::get_body() noexcept {
	return body_;
}

void const * jk::archive::file::get_body() const noexcept {
	return body_;
}

size_t jk::archive::file::size() const noexcept {
	return size_;
}

bool jk::archive::file::operator==(const std::filesystem::path & p) const noexcept {
	namespace fs = std::filesystem;
	const auto cur = fs::current_path();
	return p == filepath_;
}

jk::archive::file & jk::archive::file::operator=(file && f) {
	body_ = f.body_;
	size_ = f.size_;
	filepath_ = f.filepath_;

	f.body_ = nullptr;
	f.size_ = 0;
	f.filepath_.clear();
	return *this;
}
