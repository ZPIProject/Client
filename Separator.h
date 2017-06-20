#pragma once
#include <SFML\Graphics.hpp>

class Separator {
public:
	Separator(sf::Vector2f position);
	Separator();

	~Separator();
	sf::Vector2f getPosition();
	sf::RectangleShape getShape();

	void setPosition(double x, double y);
private:
	sf::RectangleShape separator_shape;
};