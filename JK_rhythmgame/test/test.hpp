#pragma once
#include <list>
#include <cassert>
#include <string>
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
		~test_base() {
			for (auto & i : test_list_) delete i;
		}
	};

}
#define REQUIRE_TRUE(expr) do{\
	if(!expr) r << name_ << "failed." << std::endl << #expr << " is false" << std::endl;\
}while(false);

#define DEFINE_TEST(test_name) \
struct test_name ## _initializer;\
class test_name : public jk::test::test_base{\
	static test_name ## _initializer initializer;\
	const std::string name_ = #test_name;\
	test_name();\
public:\
	static void init(){\
		test_list_.push_back(new test_name);\
	}\
	void operator()() noexcept override;\
};\
struct test_name ## _initializer{\
	test_name ## _initializer(){\
		test_name :: init();\
	}\
};\
void test_name::operator()() noexcept
