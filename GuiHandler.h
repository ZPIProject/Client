#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <TGUI\TGUI.hpp>
#include "NetworkHandler.h"
#include <iostream>
#include "GameManager.h"
#include "Tree.h"
#include <SFML\Audio.hpp>
class GuiHandler
{
public:
	GuiHandler(sf::RenderWindow &win);
	~GuiHandler();
	enum CurrentActiveGUI
	{
		LOGIN,
		CHARACTERSELECTION,
		MAINMENU,
		PLAYERSTATISTIC,
		TUTORIAL,
		GAME,
		ENDGAME
	};

	void set_active_gui_changed(bool b) { did_active_gui_changed = b; }
	void change_status();
	void change_active_gui(CurrentActiveGUI gui);

	void handle_event(sf::Event &event);

	void character_selection();
	void main_menu();
	void statistics();
	void login();
	void tutorial();
	void end_game(bool current_player_won);
	void game();

	int get_status() { return (int)active_gui; }
	void draw();
	void main_loop();
	void set_account_name(std::string name) { current_logged_account = name; }
	void set_current_character(std::string name) { current_picked_character = name; }
	NetworkHandler* get_network_handler() { return network_handler; }
	GameManager* get_game_manager() { return gm; }

	void buySkill(int idS, std::string character, Tree* tree);

private:
	sf::RenderWindow *window;
	CurrentActiveGUI active_gui;
	tgui::Gui *gui;
	NetworkHandler* network_handler;
	GameManager* gm;
	Tree* tree;
	sf::Music music;

	std::string current_logged_account;
	std::string current_picked_character;

	bool did_active_gui_changed;

	void on_character_select_button();
	bool check_if_lvlup();
	bool add_exp(int ammount);
	int get_character_level();
	int get_character_exp();
	void send_lvl_up_to_DB();
	sf::SoundBuffer pressed;
	sf::Sound pressed_sound;

};