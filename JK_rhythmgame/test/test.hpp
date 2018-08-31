#pragma once
#include <list>
#include <cassert>
#include <string>
#include <iostream>
#include "log/record.hpp"
namespace jk::test {
	struct test_initializer;

	class test_base {
		friend test_initializer;
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
#define LOG(str) r << str;
#define REQUIRE_TRUE(expr) do{\
	if(!(expr)) {\
		using namespace std::string_literals;\
		r << name_ << " failed.\n"s << "\""s << #expr ## s  << "\" is false\n"s;\
		return;\
	}\
}while(false)

#define CHECK_TRUE(expr) do{\
	using namespace std::string_literals;\
	if(!(expr)) r << name_ << "failed.\n"s << "\""s << #expr ## s << "\" is false\n"s;\
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
