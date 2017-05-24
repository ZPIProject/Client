#pragma once
#include <SFML\Graphics.hpp>
#include "ConstantValues.h"
#include "ColidableObject.h"


class Player : public ColidableObject 
{
public:
	Player(sf::Color players_color, float player_size);
	~Player();
	
	void setPosition(double x, double y);
	void setPosition(sf::Vector2f position);
	void setRotation(double rot);

	void move(int directionX, int directionY);
	void move_to_mouse(int directionX, int directionY);
	void rotate(sf::Vector2f mousePosition);

	sf::Vector2f getPosition();
	sf::RectangleShape getShape();
	double getSpeed();
	
	void onCollision(ColidableObject* object);

private:
	sf::RectangleShape player_shape;
	double speed;
};

