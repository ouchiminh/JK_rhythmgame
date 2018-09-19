#pragma once
#include <any>
#include <map>

namespace jk::resmng{
	template<class KeyType>
	inline std::map<KeyType, std::any> resources;

	template<typename T, typename KeyType>
	T & get(const KeyType & key) { return std::any_cast<T>(resources<KeyType>.at(key)); }
}
