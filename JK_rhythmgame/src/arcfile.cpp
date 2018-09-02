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

jk::archive::file::file(const file & f) : filepath_{ f.filepath_ }, size_{ f.size_ } {
	body_ = new std::int8_t[size_];
	memcpy(body_, f.body_, size_);
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
