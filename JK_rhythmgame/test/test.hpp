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
	r << test_name << "failed. "s << "\""s << #expr ## s << "\""s << std::string(additional_msg);\
}while(false)

#define REQUIRE_TRUE(expr) do{\
	try{\
		if(!(expr)) {\
			using namespace std::string_literals;\
			FAILED_LOG(expr, name_, "is false\n"s);\
			return;\
		}\
	} catch (std::exception & e) {\
		FAILED_LOG(expr, name_, e.what());\
	}\
}while(false)

#define CHECK_TRUE(expr) do{\
	using namespace std::string_literals;\
	try{\
		if(!(expr)) FAILED_LOG(expr, name_, "is false\n"s);\
	} catch (std::exception & e) {\
		FAILED_LOG(expr, name_, e.what());\
	}\
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
