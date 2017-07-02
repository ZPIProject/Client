#pragma once
#include <SFML\Graphics.hpp>

#include "Player.h"
#include "Spell_Headers\Trap.h"
#include "Spell_Headers\Ball.h"
#include "Spell_Headers\Shield.h"

#include "ConstantValues.h"
#include "NetworkHandler.h"
#include "Pattern_management.h"
#include "Collider_Headers\CollisionHandler.h"

#include "Separator.h"
#include "Player_Hud.h"
#include "Status_Hud.h"
#include "Current_Spell_Hud.h"

class GameManager
{
	bool want_to_run_with_connection_to_server = true;
	bool is_pattern_drawn;
	bool exited_from_game = false;
	bool player_won = false;
	bool end_game = false;

	std::string current_username;
	Pattern_management Pattern;
	enum Game_states{LOGING_MENU, CONNECTING_TO_SERVER, MAIN_MENU, CONNECTING_TO_GAME, GAME_IN_PROGRES, EXITING_GAME, DISCONNECT};

	sf::RenderWindow* main_window;
	sf::Clock frame_rate_controller;
	
	sf::Texture background_tex;
	sf::Sprite background;

	Separator* separator;

	Player_Hud* hud;
	Current_Spell_Hud* current_spell_hud;
	Status_Hud* local_status_hud;
	Status_Hud* non_local_status_hud;
	
	Player_stats* stats;
	Player_stats* stats1;

	Player* local_player;
	Player* non_local_player;

	Shield local_shield;
	Shield non_local_shield;

	sf::Clock local_shield_timer;
	sf::Clock non_local_shield_timer; // zmieniæ na oppenent pewnie bedzie ³adniej wygl¹da³os

	std::vector<Ball> balls_vector;
	std::vector<Ball> balls_to_send;
	std::vector<Trap> trap_vector;
	std::vector<Trap> traps_to_send;

	sf::Clock ball_cooldown;
	sf::Clock trap_cooldown;
	
	void setActiveSpellData(double value);


	NetworkHandler* network_handler;
	Tree* tree;
	CollisionHandler collision_handler;

	Game_states current_game_state;

	int active_spell = 0; 
	int active_element = 0;
	float draw_precision_spell = 0;
	float draw_precision_element = 0;

	bool game_is_running;

	void change_game_state(Game_states state);

	void draw();
	void event_handler();
	void logic_handler();
	void managePattern();
	void cast_spell();
	void game_in_progress();

	void players_initialization();
	
	void pack_player(sf::Packet& packet_to_send);
	void unpack_player(sf::Packet& recived_packet);

	void pack_ball_objects(sf::Packet& packet_to_send);
	void unpack_ball_objects(sf::Packet& recived_packet);

	void pack_trap_objects(sf::Packet& packet_to_send);
	void unpack_trap_objects(sf::Packet& recived_packet);

	void pack_shield_object(sf::Packet& packet_to_send);
	void unpack_shield_object(sf::Packet& packet_to_send);

	void stats_to_send(int know, int wis, int vit);
	std::vector<int> stats_recived();

	void pack_all_and_send();
	void recive_and_unpack_all();
	void check_if_player_has_left();
public:
	GameManager(NetworkHandler* network, sf::RenderWindow& window, Tree& tree);
	~GameManager();

	bool get_player_win_status() { return player_won; }
	void reset_game_has_ended() { end_game = false; }
	void run();
};

