#pragma once
#include <SFML\Graphics.hpp>
#include "..\ConstantValues.h"
#include "..\Collider_Headers\ColidableObject.h"
#include "Ball_stats.h"

class Ball : public ColidableObject
{
public:

	Ball(sf::Vector2f position, Ball_stats stats); //sprawdzic wartosci domyslne w konstruktorach c++
	Ball(const Ball& other);

	~Ball();

	void move();
	void on_cast_set_direction(float players_rotation);

	void setRadius(float f) { ball_shape.setRadius(f); }
	void setSpeed(float f) { speed = f; }
	void set_owner(int i) { owner = i; }

	void onCollision(ColidableObject* object);

	sf::Vector2f getPosition();
	sf::CircleShape getShape();

	Element get_element() { return statistics.get_element(); }
	float getSinus();
	float getCosinus();
	float get_radius() { return statistics.get_radius(); }
	float get_speed() { return statistics.get_speed(); }
	int get_owner() { return owner; }

	Ball_stats getStatistics();


	
private:
	sf::CircleShape ball_shape;
	Ball_stats statistics;

	int owner; //0 for local_player 1 for non_local_player;

	float speed;
	float sinus;
	float cosinus;

	
	

	sf::Color set_fill_color(Element e);



};

