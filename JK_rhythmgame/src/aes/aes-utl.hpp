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
		void set_encoder(aes && encoder);
		aes & get_encoder() noexcept;
		const aes & get_encoder() const noexcept;

		std::vector<std::uint8_t> decrypt(const std::string & filename);
		template<class IStream>
		std::vector<std::uint8_t> decrypt(IStream & in);
		
		template<class IStream, class OStream>
		void encrypt(IStream & in, OStream & out);
	};
	
	template<class IStream>
	inline std::vector<std::uint8_t> aes_utl::decrypt(IStream & in) {
		using in_traits = jk::istream_traits<IStream>;
		auto size = in_traits::get_size(in);
		void * data = new uint8_t[size];

		in_traits::read(in, data, size);
		auto result = encoder_.decrypt(data, size);

		delete[] data;
		return std::forward<std::vector<std::uint8_t>>(result);
	}

	template<class IStream, class OStream>
	inline void aes_utl::encrypt(IStream & in, OStream & out) {
		using in_traits = jk::istream_traits<IStream>;
		SHA256 hash;
		void * data;
		std::size_t size = in_traits::get_size(in);
		data = new std::uint8_t[size + 32];

		in_traits::read(in, data, size);
		hash.Push(data, size);
		hash.Final(data + size);
		auto result = encoder_.encrypt(data, size + 32);

		delete[] data;
		jk::ostream_traits<OStream>::write(out, result.data(), result.size());
	}
}
