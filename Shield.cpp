#include "Shield.h"
#include "ConstantValues.h"
#include <iostream>

Shield::Shield(Player player) {
	shield_color = sf::Color::Blue;
	shield_shape.setRadius(SHIELD_RADIUS); //do ConstantValues.h const float SHIELD_RADIUS = sqrt(pow(PLAYER_SIZE, 2.0) + pow(PLAYER_SIZE, 2.0)) / 2.0;
	shield_shape.setPosition(player.getPosition() - sf::Vector2f(SHIELD_RADIUS, SHIELD_RADIUS));
	shield_shape.setOutlineThickness(15);
	shield_shape.setFillColor(sf::Color::Black);
	shield_shape.setOutlineColor(shield_color);
	speed = player.getSpeed();
}

void Shield::setPosition(double x, double y)
{
	shield_shape.setPosition(x, y);
}

Shield::~Shield() {

}

void Shield::move(int directionX, int directionY)
{
	shield_shape.move(speed*(directionX), speed*(directionY));
}

sf::Vector2f Shield::getPosition()
{
	return shield_shape.getPosition();
}

sf::CircleShape Shield::getShape()
{
	return shield_shape;
}