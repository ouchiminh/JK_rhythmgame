#include "arcfile.hpp"

jk::archive::file::file(std::filesystem::path && filepath) noexcept(false) { load_from_file(filepath); }

jk::archive::file::file() noexcept : body_{ nullptr }, size_{ 0 } {}

jk::archive::file::~file() {
	if (body_) delete[] body_;
}

bool jk::archive::file::is_avail() const noexcept {
	return body_ == nullptr ||
		size_ == 0 ||
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

bool jk::archive::file::operator==(const file & f, const std::filesystem::path & p) noexcept {
	namespace fs = std::filesystem;
	const auto cur = fs::current_path();
	return f.filepath_.lexically_relative(cur) == p.lexically_relative(cur);
}

bool jk::archive::file::operator==(const std::filesystem::path & p, const file & f) noexcept {
	namespace fs = std::filesystem;
	const auto cur = fs::current_path();
	return f.filepath_.lexically_relative(cur) == p.lexically_relative(cur);
}
