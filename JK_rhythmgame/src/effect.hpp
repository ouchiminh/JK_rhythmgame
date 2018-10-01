#pragma once
#include <cstdint>
#include <atomic>
#include <thread>

namespace jk {
	enum EFFECT_STATE {
		NOT_WORKING, WORKING, FINISHING
	};
	inline EFFECT_STATE & operator++(EFFECT_STATE & es) {
		int buf = es;
		buf++;
		if (buf > EFFECT_STATE::FINISHING) buf = 0;
		es = (EFFECT_STATE)buf;
		return es;
	}

	template<class ...Args>
	class basic_effect {
	protected:
		volatile std::atomic<EFFECT_STATE> es_;
		std::thread thread_;

		virtual bool judge(Args ...args) const = 0;
		virtual void proceed(Args ...args) noexcept = 0;
		virtual void finish(Args ...args) noexcept = 0;
	public:
		std::uint32_t operator()(Args ...args) {
			auto flag = judge(args...);
			if (flag && EFFECT_STATE::WORKING != es_) {
				es_ = EFFECT_STATE::NOT_WORKING;
				if (thread_.joinable()) thread_.join();
				es_ = EFFECT_STATE::WORKING;
				thread_ = std::thread([this, &args...]() { this->proceed(args...); });

			}
			else if (!flag && EFFECT_STATE::WORKING == es_) {
				es_ = EFFECT_STATE::NOT_WORKING;
				if (thread_.joinable()) thread_.join();
				es_ = EFFECT_STATE::FINISHING;
				thread_ = std::thread([this, &args...]() { this->finish(args...); });
			}
			return 0;
		}

		virtual ~basic_effect() {
			es_ = EFFECT_STATE::NOT_WORKING;
			if (thread_.joinable())thread_.detach();
		}
		basic_effect(const basic_effect<Args...> & obj) :
			es_{obj.es_.load()}
		{}
		basic_effect() = default;
	};
}
