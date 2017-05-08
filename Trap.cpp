#include "Trap.h"
#include <iostream>

Trap::Trap(sf::Vector2f position) {
	trap_color = sf::Color::Magenta;
	trap_shape.setRadius(25.0f);
	trap_shape.setPosition(position);
	trap_shape.setFillColor(trap_color);
}

Trap::~Trap() {
	
}

sf::Vector2f Trap::getPosition()
{
	return trap_shape.getPosition();
}

sf::CircleShape Trap::getShape()
{
	return trap_shape;
}