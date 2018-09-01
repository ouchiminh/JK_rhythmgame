#pragma once
#include <list>
#include <string>
#include <iostream>
#include "log/record.hpp"
namespace jk::test {

	class test_base {
	protected:
		inline static std::list<test_base*> test_list_;
		inline static util::record<char> r;
	public:
		virtual void operator()() = 0;
		inline static void test() {
			for (auto & i : test_list_) (*i)();
		}
		~test_base() = default;
	};

}
#define FAILED_LOG(expr, test_name, additional_msg) do{\
	using namespace std::string_literals;\
	r << test_name << "failed\n"s << "\""s << #expr ## s << "\""s << additional_msg;\
}while(false)

#define REQUIRE_TRUE(expr) do{\
	if(!(expr)) {\
		using namespace std::string_literals;\
		FAILED_LOG(expr, name_, "is false\n"s);\
		return;\
	}\
}while(false)

#define CHECK_TRUE(expr) do{\
	using namespace std::string_literals;\
	if(!(expr)) FAILED_LOG(expr, name_, "is false\n"s);\
}while(false)

#define DEFINE_TEST(test_name) \
class test_name : public jk::test::test_base{\
	const std::string name_ = #test_name;\
public:\
	test_name(){test_list_.push_back(this);}\
	static void init(){\
		test_list_.push_back(new test_name);\
	}\
	void operator()() noexcept override;\
};\
namespace {\
	test_name test_name ## _instance;\
}\
inline void test_name::operator()() noexcept
