#pragma once
#include <list>
#include <string>
#include <iostream>
#include "log/record.hpp"
namespace jk::test {

	class test_base {
	protected:
		const std::string name_;
		inline static std::list<test_base*> test_list_;
		inline static util::record<char> r;
		test_base(std::string && name) : name_{name} {}
	public:
		virtual void operator()() = 0;
		inline static void test() {
			using namespace std::string_literals;
			for (auto & i : test_list_) {
				try { (*i)(); }
				catch (std::exception & e) {
					r << util::FatalLog<char>(i->name_ + "detected unhandled exception! : "s + std::string(e.what()));
				} catch (...) {
					r << util::FatalLog<char>(i->name_ + " failed. unhandled exception!");
				}
			}
		}
		~test_base() = default;
	};

}
#define LOG(str)	do{\
	auto outstr = name_ + "'s log: " + std::string(str);\
	r << outstr;\
	OutputDebugStringA(outstr.c_str());\
}while(false)

#define FAILED_LOG(expr, test_name, additional_msg) do{\
	using namespace std::string_literals;\
	auto outstr = test_name + " failed. "s + "\""s + #expr ## s + "\" "s + std::string(additional_msg);\
	r << outstr;\
	OutputDebugStringA(outstr.c_str());\
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
		return;\
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

#define CHECK_NOTHROW(expr) do{\
	using namespace std::string_literals;\
	try{\
		expr;\
	} catch (std::exception & e) { FAILED_LOG(expr, name_, e.what()); }\
	catch(const std::string & str){ FAILED_LOG(expr, name_, str); }\
	catch(...){FAILED_LOG(expr, name_, "something happened");}\
} while(false)

#define CHECK_THROW(expr) do{\
	try{\
		expr;\
		FAILED_LOG(expr, name_, "exception are not thrown.");\
	} catch(...){}\
} while(false)

#define REQUIRE_EQUAL(expr, value) REQUIRE_TRUE((expr) == (value))
#define CHECK_EQUAL(expr, value) CHECK_TRUE((expr) == (value))

#define DEFINE_TEST(test_name) \
class test_name : public jk::test::test_base{\
public:\
	test_name() : test_base(#test_name) {test_list_.push_back(this);}\
	void operator()() override;\
};\
namespace {\
	test_name test_name ## _instance;\
}\
inline void test_name::operator()()
