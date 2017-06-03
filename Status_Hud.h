#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class Status_Hud {
public:
	Status_Hud();

	~Status_Hud();
	
	sf::Text getText();
	int getElement();
	void setElement(int element);
	void setDuration(int duration);
	void setPosition(sf::Vector2f position);
	void setText(std::string text);
private:
	sf::Text text;
	sf::Font font;

	int element;
	int duration;
};