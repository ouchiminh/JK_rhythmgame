﻿#include <fstream>
#include "aes/aes-utl.hpp"
#include "aes/include/key.hpp"
#include "boost/interprocess/sync/file_lock.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"
#include "boost/exception/all.hpp"
#include "beatmap.hpp"

namespace {
	constexpr char password[] = u8"sonouchi_kangaeru";
}

void jk::beatmap::fill_data(std::string & line) {
	std::string delim(", ");
	std::list<std::string> strlist;
	boost::split(strlist, line, boost::is_any_of(delim));
	auto beg = std::begin(strlist);
	try {
		float timing = std::stof(*beg++);
		unsigned lane = std::stoul(*beg);
		if (lane > MAX_LANE_CNT) throw;
		const long long cur_lane_cnt = notes_.size();
		for (int i = lane; i >= cur_lane_cnt; i--)
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
		} catch (...) { 
			ep = std::current_exception();
			return;
		}
	}
	std::ifstream ifs(map_location_, std::ios::binary | std::ios::in);
	std::stringstream data;
	std::string line;
	enc::aes_utl encoder{ enc::aes{enc::makeKey(std::string(password))} };
	boost::interprocess::file_lock fl(map_location_.generic_string().c_str());

	jk::beatmap::free();

	try {
		fl.lock_sharable();
		encoder.decrypt(ifs, data);
	} catch (boost::interprocess::interprocess_exception &) {
		ep = std::current_exception();
		return;
	} catch (...) { 
		ep = std::current_exception();
		fl.unlock_sharable();
		return;
	}

	data.seekg(0, std::ios::beg);
	while (!data.eof()) {
		std::getline(data, line);
		fill_data(line);
	}
	fl.unlock_sharable();
	notes_.shrink_to_fit();
	for (unsigned i = 0; i < notes_.size(); i++) {
		notes_.at(i).shrink_to_fit();
		notes_itr_.push_back(notes_.at(i).begin());
	}
}

std::exception_ptr jk::beatmap::load() noexcept {
	std::exception_ptr ep;
	load(ep);
	return ep;
}

void jk::beatmap::free() noexcept {
	notes_itr_.clear();
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

std::vector<std::deque<jk::note>>& jk::beatmap::get_notes() noexcept {
	return notes_;
}

const std::deque<jk::note>& jk::beatmap::get_notes(unsigned lane) const {
	return notes_.at(lane);
}

std::deque<jk::note>& jk::beatmap::get_notes(unsigned lane) {
	return notes_.at(lane);
}

jk::note & jk::beatmap::get_current_note(unsigned lane) {
	return *notes_itr_.at(lane);
}

auto jk::beatmap::get_current_note_itr(unsigned lane) noexcept -> std::deque<jk::note>::iterator {
	return notes_itr_[lane];
}

auto jk::beatmap::get_current_note_itr(unsigned lane) const noexcept -> const std::deque<note>::iterator {
	return notes_itr_[lane];
}

auto jk::beatmap::end(unsigned lane) -> std::deque<note>::iterator {
	return notes_.at(lane).end();
}

auto jk::beatmap::cend(unsigned lane) const -> std::deque<note>::const_iterator {
	return notes_.at(lane).cend();
}

void jk::beatmap::forward_note(unsigned lane) {
	if(notes_itr_.at(lane) != notes_.at(lane).end())
		notes_itr_.at(lane)++;
}

bool jk::beatmap::operator!() const noexcept {
	return notes_.size() ? false : true;
}
