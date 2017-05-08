#pragma once
#include <SFML\Graphics.hpp>

class Trap{
	
public:
	Trap(sf::Vector2f position);

	~Trap();

	sf::CircleShape getShape();
	sf::Vector2f getPosition();

	void setRadius(float f) { trap_shape.setRadius(f); }
	void setDuration(int i) { duration = i; }

private:
	sf::Color trap_color;
	sf::CircleShape trap_shape;

	int duration;
};

