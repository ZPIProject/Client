#pragma once
#include <SFML\Graphics.hpp>
#include "..\Collider_Headers\ColidableObject.h"
#include "Trap_stats.h"
class Trap : public ColidableObject
{
	
public:
	Trap(sf::Vector2i position, Trap_stats stats);
	Trap(const Trap& other);
	~Trap();

	sf::CircleShape getShape();
	sf::Vector2f getPosition();

	bool has_ended();

	void setRadius(float f) { trap_shape.setRadius(f); }
	void onCollision(ColidableObject* object);
	Trap_stats getTrapStats();
private:
	Trap_stats statistics;
	sf::CircleShape trap_shape;
	
	sf::Color set_fill_color(Element e);
};

