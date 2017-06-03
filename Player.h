#pragma once
#include <SFML\Graphics.hpp>
#include "ConstantValues.h"
#include "Collider_Headers\ColidableObject.h"
#include "Player_stats.h"


class Player : public ColidableObject 
{
public:
	Player(sf::Color players_color, float player_size,Player_stats stts);
	~Player();
	
	void setPosition(double x, double y);
	void setPosition(sf::Vector2f position);
	void setRotation(double rot);

	void move(int directionX, int directionY);
	void move_to_mouse(int directionX, int directionY);
	void rotate(sf::Vector2f mousePosition);

	sf::Vector2f getPosition();
	sf::CircleShape getShape();
	
	void onCollision(ColidableObject* object);

	Player_stats getStats();

	std::vector<std::string> getCurrent_status();

private:
	sf::CircleShape player_shape;
	Player_stats stats;
	std::vector<std::string> current_status;
};

