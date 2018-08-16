#pragma once
#include <type_traits>
#include <cstdint>
#include <istream>
#include "SFML/System/InputStream.hpp"
#include "meta-helper.h"


namespace jk {
	template<class T>
	class istream_traits_impl {};

	template<>
	class istream_traits_impl<sf::InputStream>{
	public:
		using stream_t = sf::InputStream;
		
		static std::int64_t read(stream_t & s, void * data, std::int64_t size) {
			return s.read(data, size);
		}
		static std::int64_t seekg(stream_t  & s, std::int64_t pos) {
			return s.seek(pos);
		}
		static std::int64_t get_size(stream_t & s) {
			return s.getSize();
		}
		static std::int64_t tellg(stream_t & s) {
			return s.tell();
		}
	};

	template<typename CharT>
	class istream_traits_impl<std::basic_istream<CharT>> {
	public:
		static_assert(sizeof(CharT) == 1, "Size of T should be 1 byte for perfect compatibility with sf::InputStream.");

		using stream_t = std::basic_istream<CharT>;

		static std::int64_t read(stream_t & s, void * data, std::int64_t size) {
			s.read((CharT*)data, size);
			return s.gcount();
		}
		static std::int64_t seekg(stream_t  & s, std::int64_t pos) {
			s.seekg(0, std::ios::beg);
			return s.tellg();
		}
		static std::int64_t get_size(stream_t & s) {
			auto org = s.tellg();
			std::int64_t size = s.seekg(0, std::ios::end).tellg();
			s.seekg(org, std::ios::beg);
			return size * sizeof(CharT);
		}
		static std::int64_t tellg(stream_t & s) {
			return s.tellg();
		}
	};

	template<class T>
	class istream_traits :
		public istream_traits_impl<typename search_base<T, sf::InputStream, std::basic_istream<char>, std::basic_istream<unsigned char>>::type>
	{ };



	template<class T>
	class ostream_traits{};

	template<typename T>
	class ostream_traits<std::basic_ostream<T>> {
	public:
		static_assert(sizeof(T) == 1, "Size of T should be 1 byte for perfect compatibility with sf::InputStream.");

		using stream_t = std::basic_ostream<T>;

		static void write(stream_t & s, void * data, std::int64_t size) {
			s.write((T*)data, size);
		}
		static std::int64_t seekp(stream_t & s, std::int64_t pos) {
			s.seekp(pos, std::ios::beg);
			return s.tellp();
		}
		static std::int64_t tellp(stream_t & s) {
			return s.tellp();
		}
	};
}
