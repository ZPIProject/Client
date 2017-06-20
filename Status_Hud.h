#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class Status_Hud {
public:
	Status_Hud();

	~Status_Hud();
	
	sf::Text getText();

	void setPosition(sf::Vector2f position);
	void setText(std::string text);
private:
	sf::Text text;
	sf::Font font;
};