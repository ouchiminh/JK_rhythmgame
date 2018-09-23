#include <fstream>
#include "aes/aes-utl.hpp"
#include "aes/include/key.hpp"
#include "boost/interprocess/sync/file_lock.hpp"
#include "boost/tokenizer.hpp"
#include "boost/exception/all.hpp"
#include "beatmap.hpp"

namespace {
	constexpr char password[]{ "sonouchi kangaeru" };
}

void jk::beatmap::fill_data(std::string & line) {
	boost::tokenizer tk(line);
	auto beg = tk.begin();
	try {
		float timing = std::stof(*beg++);
		unsigned lane = std::stoul(*beg);
		if (lane > MAX_LANE_CNT) throw;
		for (unsigned i = lane + 1 - static_cast<unsigned>(notes_.size()); i > 0; i--)
			notes_.emplace_back();
		notes_.at(lane).emplace_back(timing, lane, music_);
	} catch(...){}
}

jk::beatmap::beatmap(std::filesystem::path const & path, std::shared_ptr<sf::Music> m) { set(path, m); }

jk::beatmap::beatmap(std::filesystem::path && path, std::shared_ptr<sf::Music> m) { set(path, m); }

void jk::beatmap::set(std::filesystem::path const & path, std::shared_ptr<sf::Music> m) {
	music_ = m;
	map_location_ = path;
}

void jk::beatmap::set(std::filesystem::path && path, std::shared_ptr<sf::Music> m) { set(path, m); }

void jk::beatmap::load(std::exception_ptr & ep) noexcept {
	if (!std::filesystem::exists(map_location_)) {
		try {
			throw std::filesystem::filesystem_error(std::string("no such file:" + map_location_.generic_string()),
				std::make_error_code(std::errc::no_such_file_or_directory));
		} catch (...) { ep = std::current_exception(); }
	}
	std::ifstream ifs(map_location_);
	std::stringstream data;
	std::string line;
	enc::aes_utl encoder(enc::aes(enc::makeKey(password)));
	boost::interprocess::file_lock fl(map_location_.generic_string().c_str());

	jk::beatmap::free();
	try {
		fl.lock();
		encoder.decrypt(ifs, data);
	} catch (...) { 
		jk::beatmap::free();
		ep = std::current_exception();
	}

	data.seekg(0, std::ios::beg);
	while (!data.eof()) {
		std::getline(data, line);
		fill_data(line);
	}
	fl.unlock();
}

void jk::beatmap::free() noexcept {
	notes_.clear();
}

unsigned jk::beatmap::get_lane_cnt() const noexcept {
	return static_cast<unsigned>(notes_.size());
}

std::weak_ptr<sf::Music> jk::beatmap::get_music() const noexcept {
	return music_;
}

std::filesystem::path const & jk::beatmap::get_path() const noexcept {
	return map_location_;
}

const std::vector<std::deque<jk::note>>& jk::beatmap::get_notes() const noexcept {
	return notes_;
}

bool jk::beatmap::operator!() const noexcept {
	return notes_.size() ? false : true;
}
