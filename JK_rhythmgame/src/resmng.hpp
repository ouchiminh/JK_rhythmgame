#pragma once
#include <any>
#include <map>

namespace jk {
	template<
		class KeyType,
		class Compare = std::less<KeyType>,
		class Allocator = std::allocator<std::pair<const KeyType, std::any>>
	>
	class resmng {
		inline static std::map<KeyType, std::any, Compare, Allocator> resources_;

	public:
		template<class T, class ...Args>
		static T & emplace(const KeyType & key, Args ...args);
		
		template<class T>
		static T & at(const KeyType & key);
		template<class T>
		static const T & at(const KeyType & key) const;

		template<class T>
		T & operator[](const KeyType & key);
		template<class T>
		const T & operator[](const KeyType & key) const;

		template<class T>
		static void insert_or_assign(const KeyType & key, const T & obj);

		static void erase(const KeyType & key);
	};

	template<class KeyType, class Compare, class Allocator>
	template<class T, class ...Args>
	inline static T & resmng<KeyType, Compare, Allocator>::emplace(const KeyType & key, Args ...args) {
		resources_.emplace(,std::piecewise_construct, 
			std::forward_as_tuple(key),
			T(args...));
		return std::any_cast<T>(resources_.at(key));
	}
	template<class KeyType, class Compare, class Allocator>
	template<class T>
	inline static T & resmng<KeyType, Compare, Allocator>::at(const KeyType & key) {
		return std::any_cast<T>(resources_.at(key));
	}
	template<class KeyType, class Compare, class Allocator>
	template<class T>
	inline static const T & resmng<KeyType, Compare, Allocator>::at(const KeyType & key) const {
		return std::any_cast<T>(resources_.at(key));
	}
	template<class KeyType, class Compare, class Allocator>
	template<class T>
	inline T & resmng<KeyType, Compare, Allocator>::operator[](const KeyType & key) {
		return std::any_cast<T>(resources_[key]);
	}
	template<class KeyType, class Compare, class Allocator>
	template<class T>
	inline const T & resmng<KeyType, Compare, Allocator>::operator[](const KeyType & key) const {
		return std::any_cast<T>(resources_[key]);
	}
	template<class KeyType, class Compare, class Allocator>
	template<class T>
	inline static void resmng<KeyType, Compare, Allocator>::insert_or_assign(const KeyType & key, const T & obj) {
		resources_.insert_or_assign(key, obj);
	}
	template<class KeyType, class Compare, class Allocator>
	inline static void resmng<KeyType, Compare, Allocator>::erase(const KeyType & key) {
		resources_.erase(key);
	}
}
