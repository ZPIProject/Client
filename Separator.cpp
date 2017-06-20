#include "Separator.h"
#include "ConstantValues.h"
#include <iostream>

Separator::Separator(sf::Vector2f position)
{
	separator_shape.setSize(sf::Vector2f(WINDOW_WIDTH, SEPARATOR_HEIGHT));
	separator_shape.setPosition(position);
	separator_shape.setOrigin(WINDOW_WIDTH/2, SEPARATOR_HEIGHT/2);
	separator_shape.setFillColor(sf::Color(0, 230, 230));
}

Separator::Separator()
{
	separator_shape.setSize(sf::Vector2f(WINDOW_WIDTH, SEPARATOR_HEIGHT));
	separator_shape.setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
	separator_shape.setOrigin(WINDOW_WIDTH / 2, SEPARATOR_HEIGHT / 2);
	separator_shape.setFillColor(sf::Color(0, 230, 230));
}

Separator::~Separator() {

}

sf::Vector2f Separator::getPosition()
{
	return separator_shape.getPosition();
}

sf::RectangleShape Separator::getShape()
{
	return separator_shape;
}

void Separator::setPosition(double x, double y)
{
	separator_shape.setPosition(x, y);
}
