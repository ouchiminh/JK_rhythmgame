#pragma once
#include "sfml-button.hpp"

namespace jk {
	class effect_button;

	template<class ...Arg>
	std::uint32_t place_holder(Arg...args) { return 0; }
	
	// effect starterとeffect procedureはいつもこの子といっしょ
	enum effect_type{};

	// effectをはじめるべきならtrueを返す。それ以外はfalse
	// エフェクトが始まったかどうか判断する。(始まっていたら必要な準備をする)
	using effect_starter = std::function<bool(const sf::Event &, sf::Sprite &, sf::Text &, jk::effect_button &)>;	
	using effect_procedure = std::function<std::uint32_t(sf::Sprite &, sf::Text &, jk::effect_button &)>;
	
	class effect_button : public button {
		
	};

}
