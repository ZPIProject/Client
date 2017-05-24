#include "Shield.h"
#include <iostream>

Shield::Shield(sf::Vector2f position, Shield_stats stats) : ColidableObject(new CircleCollider(stats.get_radius())){

	statistics = stats;
	shield_shape.setRadius(statistics.get_radius());
	shield_shape.setPosition(position);
	shield_shape.setOrigin(statistics.get_radius(), statistics.get_radius());
	shield_shape.setOutlineThickness(3);
	shield_shape.setFillColor(sf::Color(0,0,0,0));
	shield_shape.setOutlineColor(set_fill_color(statistics.get_element()));
	this->speed = speed;
}
Shield::Shield() : ColidableObject(new CircleCollider(0)) {}


void Shield::setPosition(double x, double y)
{
	shield_shape.setPosition(x, y);
	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(sf::Vector2f(x, y));
	}
}

void Shield::onCollision(ColidableObject * object)
{
	//obs³uga kolizji shield'a
	//pocisk i trap tylko 
	//tak mi sie wydaje
}

Shield::~Shield() {

}

void Shield::move(sf::Vector2f position)
{	
	shield_shape.setPosition(position);
	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(position);
	}
}

sf::Vector2f Shield::getPosition()
{
	return shield_shape.getPosition();
}

sf::CircleShape Shield::getShape()
{
	return shield_shape;
}

bool Shield::has_ended()
{
	return statistics.has_ended();
}



sf::Color Shield::set_fill_color(Element e)
{
	sf::Color color = sf::Color::Black;

	switch (e)
	{
	case Element::FIRE:
		color = sf::Color::Red;
		break;
	case Element::WIND:
		color = sf::Color::Green;
		break;
	case Element::THUNDER:
		color = sf::Color::Yellow;
		break;
	case Element::EARTH:
		color = sf::Color(148, 62, 15);
		break;
	case Element::WATER:
		color = sf::Color::Blue;
		break;
	}
	return color;
}