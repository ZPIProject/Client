#include "..\Spell_Headers\Ball.h"
#include <iostream>
#include "..\Collider_Headers\CircleCollider.h"
#include "..\Player.h"

Ball::Ball(sf::Vector2f position, Ball_stats stats) : ColidableObject(new CircleCollider(stats.get_radius()))
{
	statistics = stats;
	is_active = true;
	this->speed = stats.get_speed();

	ball_shape.setRadius(stats.get_radius());
	ball_shape.setPosition(position);
	ball_shape.setFillColor(set_fill_color(stats.get_element()));
	
	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(ball_shape.getPosition());
	}

}

Ball::Ball(const Ball& other) : ColidableObject(other)
{
	statistics = other.statistics;
	is_active = other.is_active;
	ball_shape = other.ball_shape;
	speed = other.speed;
	sinus = other.sinus;
	cosinus = other.cosinus;
}


Ball::~Ball()
{
	//delete projectail_body;
}

void Ball::move()
{
	ball_shape.move(speed*sinus,speed*(-cosinus));
	if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
	{
		circle->setPosition(ball_shape.getPosition());
	}

	//std::cout << "Position in meters: " << projectail_body->GetTransform().p.x << " " << projectail_body->GetTransform().p.y << "\n";
	//std::cout << "Position in pixels: " << 
}

void Ball::on_cast_set_direction(float players_rotation)
{
	sinus = sin((players_rotation * M_PI) / 180.0f);
	cosinus = cos((players_rotation * M_PI) / 180.0f);
}

sf::Vector2f Ball::getPosition()
{
	return ball_shape.getPosition();
}

sf::CircleShape Ball::getShape()
{
	return ball_shape;
}

float Ball::getSinus()
{
	return sinus;
}

float Ball::getCosinus()
{
	return cosinus;
}

sf::Color Ball::set_fill_color(Element e)
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

void Ball::onCollision(ColidableObject* object)
{
	if (Player* player = dynamic_cast<Player*>(object));
		//is_active = false;
}
