#pragma once
#include <vector>
#include <cstdint>
#include "include/aes.hpp"
#include "../stream-traits.hpp"
#include "include/SHA256.hpp"

namespace enc {
	class aes_utl {
		aes encoder_;
	public:
		aes_utl() = default;
		aes_utl(aes && encoder);
		aes_utl(const aes & encoder);

		void set_encoder(aes && encoder);
		aes & get_encoder() noexcept;
		const aes & get_encoder() const noexcept;

		[[nodiscard]] std::vector<std::uint8_t> decrypt(const std::string & filename);
		template<class IStream>
		[[nodiscard]] std::vector<std::uint8_t> decrypt(IStream & in);
		template<class IStream, class OStream>
		void decrypt(IStream & in, OStream & out);
		
		template<class IStream, class OStream>
		void encrypt(IStream & in, OStream & out);
	};
	
	template<class IStream>
	inline std::vector<std::uint8_t> aes_utl::decrypt(IStream & in) {
		using in_traits = jk::istream_traits<IStream>;
		auto size = in_traits::get_size(in);
		void * data = new uint8_t[size];
		SHA256 hash;
		block<32> newb, orgb;

		in_traits::read(in, data, size);
		auto result = encoder_.decrypt(data, size);
		delete[] data;
		orgb = result.data() + (result.size() - 32);
		hash.Push(result.data(), (u_32)result.size() - 32);
		hash.Final((unsigned char*)(void*)newb.bytes);

		for (int i = 0; i < 32; i++) result.pop_back();
		
		if (orgb == newb) return std::forward<std::vector<std::uint8_t>>(result);
		throw std::runtime_error("hash incorrenct. data may be corrupted.");
	}

	template<class IStream, class OStream>
	inline void aes_utl::decrypt(IStream & in, OStream & out) {
		std::vector<std::uint8_t> result = decrypt(in);
		jk::ostream_traits<OStream>::write(out, result.data(), result.size() * sizeof(decltype(result)::value_type));
	}

	template<class IStream, class OStream>
	inline void aes_utl::encrypt(IStream & in, OStream & out) {
		using in_traits = jk::istream_traits<IStream>;
		SHA256 hash;
		void * data;
		std::size_t size = in_traits::get_size(in);
		data = new std::uint8_t[size + 32];

		in_traits::read(in, data, size);
		hash.Push(static_cast<unsigned char*>(data), static_cast<u_32>(size));
		hash.Final(static_cast<unsigned char*>(data) + size);
		auto result = encoder_.encrypt(data, size + 32);

		delete[] data;
		jk::ostream_traits<OStream>::write(out, result.data(), result.size());
	}
}
