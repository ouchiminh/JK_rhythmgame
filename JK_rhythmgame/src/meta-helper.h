#pragma once
#include <type_traits>
namespace jk {
	template<class T>
	class has_char_type {
		template<class U>
		static constexpr bool check(typename U::char_type*) { return true; }

		template<class U>
		static constexpr bool check(...) { return false; }

	public:
		static constexpr bool value = check<T>(nullptr);
	};

	template<class D, class Head, class ...Arg>
	class search_base {
	public:
		typedef typename std::conditional_t<
			std::is_base_of<Head, D>::value,
			Head,
			typename search_base<D, Arg...>::type
		> typename type;
	};

	template<class D, class Head>
	class search_base<D, Head> {
	public:
		typedef typename std::conditional_t<
			std::is_base_of<Head, D>::value,
			Head,
			void
		> typename type;
	};
}