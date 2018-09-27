#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include <iostream>

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	namespace fs = std::filesystem;
	
	window_ = window;
	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		jk::beatmap_directory bd;
		try {
			bd.set_directory(p);				
		}catch (fs::filesystem_error & e) {
			std::cout << e.what() << std::endl;
		}
		AddButton(bd);
	}
}

void jk::map_select_renderer::AddButton(jk::beatmap_directory& bd) {
	for (const auto& b : bd) {
		// commented by ouchiminh
		// jk::ui_component����h������N���X��jk::ui_mng::create�Ő�������Ȃ���΂Ȃ�Ȃ��B
		// ����ui_mng����shared_ptr��ێ����Ă��邽�߁A�ʂŔz������K�v�͂Ȃ����A�ق��̃R�[�h�̏C�������Ȃ��ςނ悤�Ɋ��ɒ�`���Ă���ϐ��͏����Ă��Ȃ��B
		musicButtons_.push_back(components_.create<jk::button>(MakeButtonName(b)));
		mapPaths_.emplace_back(b.get_path());
	}
}

const sf::Text jk::map_select_renderer::MakeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	return name;
}

void jk::map_select_renderer::InitButtonPos() {
	sf::Vector2f pivotRe = { 0.578f, 0.216f};		//���Έʒu(�{�^��������s�{�b�g�|�C���g�Ƒz��)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		// commented by ouchiminh
		// �G���[���Ȃ������߂�musicButtons_�̃e���v���[�g������ύX�������߁A���̃R�[�h���ύX�����B
		button->set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += {0.f, 0.040f};	
	}
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	auto index = musicButtonsItr_  - musicButtons_.begin();		//���C�e���[�^���w���Ă���ꏊ�̃C���f�b�N�X���擾
	auto selectPath = mapPaths_[index];
	beatmap_directory dir(selectPath);

	return dir.GetFirstBeatmapList();
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	components_.draw(*window_);
}