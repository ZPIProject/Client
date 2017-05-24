#include "BoxCollider.h"



BoxCollider::BoxCollider(sf::Vector2f position, float width, float height) : Collider()
{
	collider_position = sf::Vector2f(position.x - width/2, position.y - height/2);
	this->width = width;
	this->height = height;
}


BoxCollider::~BoxCollider()
{
}

bool BoxCollider::intersect(Collider & other_collider)
{
	
	bool collision = false;
	if (BoxCollider* col = dynamic_cast<BoxCollider*>(&other_collider))
	{
		collision = intersect(*col);
	}
	if (CircleCollider* col = dynamic_cast<CircleCollider*>(&other_collider))
	{
		collision = intersect(*col);
	}
	return collision;
}

sf::Vector2f BoxCollider::getPosition()
{
	return collider_position;
}

float BoxCollider::getWidth()
{
	return width;
}

float BoxCollider::getHeight()
{
	return height;
}

void BoxCollider::setPosition(sf::Vector2f position)
{
	collider_position = sf::Vector2f(position.x-width/2, position.y - height/2);
}

bool BoxCollider::point_in_line(std::vector<sf::Vector2f> points)
{
	float det = points[0].x*points[1].y + points[1].x * points[2].y + points[2].x * points[0].y -
		points[2].x * points[1].y - points[0].x * points[2].y + points[1].x * points[0].y;

	return det != 0 ? false : ((std::min(points[0].x, points[1].x) <= points[2].x) && (std::max(points[0].x, points[1].x) >= points[2].x) &&
		(std::min(points[0].y, points[1].y) <= points[2].y) && (std::max(points[0].y, points[1].y) >= points[2].y)
		);
}

bool BoxCollider::intersect(CircleCollider & circle_collider)
{
	std::cout << "intersect with collider";

	sf::Vector2f circle_position = circle_collider.getPosition();
	sf::Vector2f left_upper_corner = collider_position;
	sf::Vector2f right_bottom_corner = sf::Vector2f(collider_position.x + width, collider_position.y + height);

	return circle_position.x >= (left_upper_corner.x - circle_collider.getRadius()) && circle_position.x <= (right_bottom_corner.x + circle_collider.getRadius()) &&
		circle_position.y >= (left_upper_corner.y - circle_collider.getRadius()) && circle_position.y <= (right_bottom_corner.y + circle_collider.getRadius());
	

}

bool BoxCollider::intersect(BoxCollider & box_collider)
{
	float other_width = box_collider.getWidth();
	float other_height = box_collider.getHeight();
	sf::Vector2f other_position = box_collider.getPosition();

	return	collider_position.x < other_position.x + other_width &&
			collider_position.x + width > other_position.x &&
			collider_position.y < other_position.y + other_height &&
			collider_position.y + height > other_position.y;
}
