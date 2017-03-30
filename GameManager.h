#pragma once
#include <SFML\Graphics.hpp>


#include "Player.h"
#include "ConstantValues.h"
#include "NetworkHandler.h"
#include "Pattern_management.h"
class GameManager
{
	bool is_pattern_drawn;
	Pattern_management Pattern;
	enum Game_states{LOGING_MENU, CONNECTING_TO_SERVER, MAIN_MENU, CONNECTING_TO_GAME, GAME_IN_PROGRES, EXITING_GAME, DISCONNECT};

	bool want_to_run_with_connection_to_server;

	sf::RenderWindow main_window;
	sf::Clock frame_rate_controller;
	
	Player* local_player;
	Player* non_local_player;
	NetworkHandler* network_handler;

	Game_states current_game_state;

	bool game_is_running;

	void change_game_state(Game_states state);

	void draw();
	void event_handler();
	void logic_handler();
	void managePattern();
	void loging_menu();
	void connecting_to_server();
	void main_menu();
	void connecting_to_game();
	void game_in_progress();
	void exiting_game();
	void disconnect();


	void players_initialization();
	
	void pack_player(sf::Packet& packet_to_send);
	void unpack_player(sf::Packet& recived_packet);
	void pack_all_and_send();
	void recive_and_unpack_all();
public:
	GameManager();
	~GameManager();

	void run();
};

