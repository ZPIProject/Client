#pragma once
#include <SFML\Graphics.hpp>
#include "..\Collider_Headers\ColidableObject.h"
#include "Shield_stats.h"
class Shield : public ColidableObject
{

public:
	Shield(sf::Vector2f position, Shield_stats stats);
	Shield();

	~Shield();

	void move(sf::Vector2f position);

	sf::Vector2f getPosition();
	sf::CircleShape getShape();
	Shield_stats get_statistics() { return statistics; }
	
	bool has_ended();



	void setRadius(float f) { shield_shape.setRadius(f); }
	void setPosition(double x, double y);

	void onCollision(ColidableObject* object);
private:
	sf::CircleShape shield_shape;
	Shield_stats statistics;

	double speed;

	sf::Color set_fill_color(Element e);
};