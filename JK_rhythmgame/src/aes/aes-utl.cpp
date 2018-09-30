#include <fstream>
#include <stdexcept>
#include "aes-utl.hpp"

enc::aes_utl::aes_utl(aes && encoder) : encoder_{encoder} {}

enc::aes_utl::aes_utl(const aes & encoder) : encoder_{encoder} {}

void enc::aes_utl::set_encoder(aes && encoder) {
	encoder_ = std::move(encoder);
}

enc::aes & enc::aes_utl::get_encoder() noexcept {
	return encoder_;
}

const enc::aes & enc::aes_utl::get_encoder() const noexcept {
	return encoder_;
}

std::vector<std::uint8_t> enc::aes_utl::decrypt(const std::string & filename) {
	std::ifstream in;
	in.open(filename, std::ios::binary | std::ios::in);
	if (!in) throw std::runtime_error("cannot open file");
	return this->decrypt(in);
}

