#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Shield {

public:
	Shield(Player player);

	~Shield();

	void move(int directionX, int directionY);

	sf::Vector2f getPosition();
	sf::CircleShape getShape();
	
	void setRadius(float f) { shield_shape.setRadius(f); }
	void setDuration(int i) { duration = i; }

	void setPosition(double x, double y);
private:
	sf::Color shield_color;
	sf::CircleShape shield_shape;

	int duration;
	double speed;
};