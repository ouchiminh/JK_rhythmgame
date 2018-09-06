#include <memory.h>
#include "arcfile.hpp"

void jk::archive::file::discard() noexcept {
	size_ = 0;
	if (body_) delete[] body_;
	body_ = nullptr;
	pos_ = 0;
	filepath_.clear();
}

jk::archive::file::file(std::filesystem::path && filepath) noexcept(false) : body_(NULL), size_{ 0 }, pos_{ 0 } { load_from_file(filepath); }

jk::archive::file::file(const std::filesystem::path & filepath) : body_(NULL), size_{ 0 }, pos_{ 0 } { load_from_file(filepath); }

jk::archive::file::file(const std::string & filepath, size_t size, size_t pos) : body_( NULL ), size_( 0 ), pos_(pos)
{ 
	register_archived_info(std::filesystem::path(filepath), size, pos); 
}

jk::archive::file::file() noexcept : body_{ nullptr }, size_{ 0 }, pos_{ 0 } { }

jk::archive::file::file(file && f) : body_{ f.body_ }, size_{ f.size_ }, filepath_{ f.filepath_ }, pos_{ f.pos_ } {
	f.body_ = nullptr;
	f.size_ = 0;
	f.filepath_.clear();
	f.pos_ = 0;
}

jk::archive::file::~file() {
	discard();
}

bool jk::archive::file::is_valid() const noexcept {
	return body_ != nullptr &&
		size_ != 0 &&
		filepath_.empty() ?
		false :
		true;
}

const std::filesystem::path jk::archive::file::get_filepath() const noexcept {
	return filepath_;
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
	pos_ = f.pos_;
	filepath_ = f.filepath_;

	f.body_ = nullptr;
	f.size_ = 0;
	f.pos_ = 0;
	f.filepath_.clear();
	return *this;
}
