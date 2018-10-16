﻿#include "map-select-renderer.hpp"
#include "sfml-button.hpp"
#include "color-manager.hpp"
#include "button_effect.hpp"
#include <iostream>

const sf::Vector2f jk::map_select_renderer::MAINBUTTON_MARGIN = { 0.f, 0.040f };
const sf::Vector2f jk::map_select_renderer::BACKBUTTON_POS = { 0.063f, 0.097f };

void jk::map_select_renderer::init(sf::RenderWindow* window) {
	//変数初期化
	window_ = window;
	sceneflag_ = RUNNING;
	if (musicButtons_.size()) return;

	namespace fs = std::filesystem;	
	for (const auto &p : fs::directory_iterator(".\\beatmap\\"))	{
		try {
			// このvectorのデータ型はshared_ptr<>なので、もし、直接構築するとすればshared_ptrを構築しないといけない。
			beatDirectories_.push_back(std::make_shared<jk::beatmap_directory>(p.path()));		
		}catch (fs::filesystem_error & e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	f_.loadFromFile(".\\res\\fonts\\Perfograma.otf");
	makeButton();


	//イベントハンドラ登録
	{
		handlers_ << std::make_pair < sf::Event::EventType, jk::event_handler_t<> >(
			sf::Event::EventType::KeyPressed,
			[this](sf::Event const& e)->std::uint32_t {return this->on_key_down(e); }
		);
		(backTitleButton_->handlers_) << std::make_pair(
			sf::Event::EventType::MouseButtonPressed,
			[this](auto const & e, auto & s, auto & t, auto & b) { return backTitleButton_pressed(e, s, t, b); });
	}
	musicButtonsItr_ = std::begin(musicButtons_);
}

void jk::map_select_renderer::addButton(std::shared_ptr<jk::beatmap_directory> bd) {
	for (auto & b : *bd) {
		// commented by ouchiminh
		// jk::ui_componentから派生するクラスはjk::ui_mng::createで生成されなければならない。
		// 実はui_mng内でshared_ptrを保持しているため、別で配列を作る必要はないが、ほかのコードの修正が少なく済むように既に定義してある変数は消していない。
		musicButtons_.push_back(components_.create<jk::musicButton>(&b,makeButtonName(b)));
	}
	//backTitleButtonの設定
	{
		sf::Text backTitleName("Back", f_);
		backTitleName.setFillColor(jk::color::color_mng::get("Data.str_color").value_or(jk::color::str_color));
		backTitleButton_ = components_.create<jk::button>(backTitleName);
		backTitleButton_->set_position(BACKBUTTON_POS * (float)(1920/128));
	}
}

void jk::map_select_renderer::makeButton()
{
	for (auto bd : beatDirectories_) {
		addButton(bd);
	}
	initButtonPos();
}

const sf::Text jk::map_select_renderer::makeButtonName(const jk::beatmap& b) const {
	sf::Text name;
	auto sname = b.get_path().stem().generic_string();
	name.setString(sname);

	name.setFillColor(jk::color::color_mng::get("Data.str_color").value_or(jk::color::str_color));
	name.setFont(f_);

	return name;
}

void jk::map_select_renderer::initButtonPos() {
	sf::Vector2f pivotRe = { 0.578f, 0.216f};		//相対位置(ボタン左上をピボットポイントと想定)
	auto size = window_->getSize();
	for (auto button : this->musicButtons_) {
		// commented by ouchiminh
		// エラーをなくすためにmusicButtons_のテンプレート引数を変更したため、このコードも変更した。
		button->set_position({ size.x * pivotRe.x, size.y * pivotRe.y});
		pivotRe += MAINBUTTON_MARGIN;	
	}
}

std::uint32_t jk::map_select_renderer::on_key_down(const sf::Event & e)
{
	switch (e.key.code)
	{
	case sf::Keyboard::Up:
		buttonSelect_up();
		break;

	case sf::Keyboard::Down:
		buttonSelect_down();
		break;

	case sf::Keyboard::Enter:
		executeClicked();
		break;

	case sf::Keyboard::Escape:
		backTitle();
		break;
	}
	return 0;
}

std::uint32_t jk::map_select_renderer::backTitleButton_pressed(const sf::Event & e, sf::Sprite & s, sf::Text & t, jk::button & b)
{
	if (!b.get_rect().contains(sf::Vector2f{ (float)e.mouseButton.x, (float)e.mouseButton.y }) ||
		 e.mouseButton.button != sf::Mouse::Button::Left)
		return 0;

	backTitle();

	return 0;
}

void jk::map_select_renderer::buttonSelect_up()
{


	if (musicButtonsItr_ == musicButtons_.begin()) {
		return;
	}
	else {
		(*musicButtonsItr_)->setState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更
		--musicButtonsItr_;
		(*musicButtonsItr_)->setState(musicButtonState::SELECTED);				//選択先を変更
	}

}

void jk::map_select_renderer::buttonSelect_down()
{
	if (musicButtonsItr_ == musicButtons_.end() || musicButtonsItr_+1 == musicButtons_.end()) {
		return;
	}
	else {
		(*musicButtonsItr_)->setState(musicButtonState::NOT_SELECTED);			//今選択中のボタンを非選択に変更
		++musicButtonsItr_;
		(*musicButtonsItr_)->setState(musicButtonState::SELECTED);				//選択先を変更
	}
}

void jk::map_select_renderer::executeClicked()
{
	(*musicButtonsItr_)->setState(musicButtonState::CLICKED);				//今選択中のボタンをクリック状態に変更
	sceneflag_ = FINISHED;					//終了フラグを立てる
}

void jk::map_select_renderer::backTitle()
{
	sceneflag_ = FINISHED;					//終了フラグを立てる
}

std::optional<jk::beatmap> jk::map_select_renderer::get_selected() const {
	if (musicButtonsItr_ == musicButtons_.end()) return std::nullopt;
	auto bptr = (*musicButtonsItr_)->get_beatmap();
	return ((*musicButtonsItr_)->getState() == musicButtonState::CLICKED && bptr) ?
		std::optional<beatmap>(*bptr) :
		std::optional<beatmap>(std::nullopt);
}

jk::SCENEFLAG jk::map_select_renderer::operator() () {
	window_->clear();
	components_.draw(*window_);
	window_->display();
	return sceneflag_;
}

std::uint32_t jk::map_select_renderer::input(const sf::Event & e) noexcept
{
	handlers_(e);
	return components_.event_procedure(e);
}

void jk::map_select_renderer::free_resource() noexcept {
	window_ = nullptr;
	components_.get_list().clear();
	f_ = sf::Font();
	musicButtons_.clear();
	musicButtonsItr_ = std::end(musicButtons_);
	beatDirectories_.clear();
	backTitleButton_.reset();
}