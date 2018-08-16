#pragma once
#include <type_traits>
namespace jk {
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