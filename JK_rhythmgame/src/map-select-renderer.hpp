#pragma once

#include <optional>
#include <filesystem>
#include "SFML/Graphics.hpp"
#include "scene.hpp"
#include "ui-component.hpp"
#include "aes/aes-utl.hpp"
#include "beatmap-directory.hpp"
#include "musicButton.hpp"

namespace jk {

	class map_select_renderer : public renderer {
		sf::RenderWindow* window_;
		ui_mng components_;
		sf::Font f_;

		std::vector<std::shared_ptr<musicButton>> musicButtons_;				//���ʑI��{�^��
		std::vector<std::shared_ptr<musicButton>>::iterator musicButtonsItr_;	//���ʑI��{�^���R���e�i�̃C�e���[�^
		std::vector<std::shared_ptr<beatmap_directory>>  beatDirectories_;		//�ǂݍ���beatmap_directory��ێ����Ă����R���e�i
		std::shared_ptr<button> backTitleButton_;


		static const sf::Vector2f MAINBUTTON_MARGIN;		//�ȑI��{�^����margin
		static const sf::Vector2f BACKBUTTON_POS;			//�߂�{�^���̈ʒu

		jk::SCENEFLAG sceneflag_;

		event_handlers<> handlers_;
				
	public:
		void init(sf::RenderWindow* window);
		[[nodiscard]] SCENEFLAG operator() () override;
		std::uint32_t input(const sf::Event & e) noexcept override;
		void free_resource() noexcept override;

		[[nodiscard]] std::optional<jk::beatmap> get_selected() const;
	
	private:
		void addButton(std::shared_ptr<beatmap_directory> bd);
		void makeButton();
		const sf::Text makeButtonName(const jk::beatmap& b) const;
		void initButtonPos();
		std::uint32_t on_key_down(const sf::Event &e);	//�L�[�{�[�h��̓C�x���g

		//backTitleButton�N���b�N�C�x���g
		std::uint32_t backTitleButton_pressed(const sf::Event &e, sf::Sprite &s, sf::Text& t, jk::button& b);

		void buttonSelect_up();
		void buttonSelect_down();
		void executeClicked();
		void backTitle();
	};
}