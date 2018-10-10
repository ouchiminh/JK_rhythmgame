#pragma once
#include <sstream>
#include "test.hpp"
#include "../src/aes/aes-utl.hpp"
#include "../src/aes/include/key.hpp"

namespace jk::test {
	DEFINE_TEST(encrypt_decrypt_test_aes_utl) {
		enc::aes_utl encoder;
		std::stringstream original, out;
		original << "test test. this is a test of crypto class!!! i hope this will be success.";
		encoder.encrypt(original, out);
		std::vector<std::uint8_t> result;

		CHECK_NOTHROW(result = encoder.decrypt(out));
		original.seekg(0);
		
		for (auto & i : result) {
			char buf;
			original.read(&buf, 1);
			CHECK_EQUAL(buf, i);
		}
	}

	DEFINE_TEST(key_change_crypto_test_aes_utl) {
		enc::aes_utl encoder;
		enc::aes_utl decoder;
		std::stringstream original, out;
		original << "test test. this is a test of crypto class!!! i hope this will be success.";
		encoder.encrypt(original, out);
		std::vector<std::uint8_t> result;

		decoder.get_encoder().setKey(enc::makeKey("password"));

		CHECK_SPECIFIC_EXCEPTION(decoder.decrypt(out), std::runtime_error &);
	}
}
