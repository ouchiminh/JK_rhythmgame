#include "notes.hpp"

jk::note::note(float sec, unsigned lane, std::shared_ptr<sf::Music> m) {}

std::optional<float> jk::note::hit() {
	return std::optional<float>(std::nullopt);
}
