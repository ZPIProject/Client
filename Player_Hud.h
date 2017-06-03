#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Player_Hud {
public:
	Player_Hud(Player_stats local_stats, Player_stats non_local_stats);

	~Player_Hud();

	std::string Convert(float number);
	std::vector<sf::Text> getTexts();
private:
	sf::Text opponentsHP;
	//sf::Text textMP;
	//sf::Text textHP;
	sf::Text HP_value;
	sf::Text MP_value;

	sf::Font font;
};