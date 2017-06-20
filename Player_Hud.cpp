#include "Player_Hud.h"
#include <sstream>
#include <vector>

Player_Hud::Player_Hud(Player_stats* local_stats, Player_stats* non_local_stats)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Problem with loading font" << std::endl;
	}

	opponentsHP.setString("HP:" + Convert((non_local_stats->get_current_health()/non_local_stats->get_MAX_health())*100) + "%");
	opponentsHP.setCharacterSize(20);
	opponentsHP.setColor(sf::Color::Black);
	opponentsHP.setStyle(sf::Text::Bold);
	opponentsHP.setPosition(sf::Vector2f(5, 5));
	opponentsHP.setFont(font);

	/*
	textHP.setString(":HP");
	textHP.setCharacterSize(20);
	textHP.setColor(sf::Color::Red);
	textHP.setStyle(sf::Text::Bold);
	textHP.setPosition(sf::Vector2f(760, 550));
	textHP.setFont(font);

	textMP.setString(":MP");
	textMP.setCharacterSize(20);
	textMP.setColor(sf::Color::Blue);
	textMP.setStyle(sf::Text::Bold);
	textMP.setPosition(sf::Vector2f(760, 575));
	textMP.setFont(font);
	*/

	HP_value.setString(Convert(local_stats->get_current_health()) + "/" + Convert(local_stats->get_MAX_health()) + ":HP");
	HP_value.setCharacterSize(20);
	HP_value.setColor(sf::Color::Black);
	HP_value.setStyle(sf::Text::Bold);
	HP_value.setPosition(sf::Vector2f(665, 550));
	HP_value.setFont(font);

	MP_value.setString(Convert(local_stats->get_current_mana()) + "/" + Convert(local_stats->get_MAX_mana()) + ":MP");
	MP_value.setCharacterSize(20);
	MP_value.setColor(sf::Color::Black);
	MP_value.setStyle(sf::Text::Bold);
	MP_value.setPosition(sf::Vector2f(665, 575));
	MP_value.setFont(font);
}

Player_Hud::~Player_Hud()
{
}

std::string Player_Hud::Convert(float number) {
	std::ostringstream buff;
	buff.precision(3);
	buff << number;
	return buff.str();
}

std::vector<sf::Text> Player_Hud::getTexts() {
	std::vector<sf::Text> result;
	result.push_back(opponentsHP);
	//result.push_back(textHP);
	//result.push_back(textMP);
	result.push_back(HP_value);
	result.push_back(MP_value);
	return result;
}

void Player_Hud::setOpponentHpText(std::string s)
{
	opponentsHP.setString(s);
}

void Player_Hud::setLocalHpText(std::string s)
{
	HP_value.setString(s);
}

void Player_Hud::setLocalMpText(std::string s)
{
	MP_value.setString(s);
}
