#include "CircleCollider.h"



	CircleCollider::CircleCollider(float rad)
	{
		radius = rad;
	}


	CircleCollider::~CircleCollider()
	{
	}

	bool CircleCollider::intersect(Collider & collider)
	{
		//std::cout << "CircleCollider::intersect(Collider & collider)\n";
		if (CircleCollider* circle = dynamic_cast<CircleCollider*> (&collider))
		{
			return intersect(*circle);
		}
		if (BoxCollider* box = dynamic_cast<BoxCollider*>(&collider))
		{
			return intersect(*box);
		}
		return false;
	}

	bool CircleCollider::intersect(CircleCollider& circle_collider)
	{
		//std::cout << "CircleCollider::intersect(CircleCollider& circle_collider)\n";
		double distance = sqrt((circle_collider.getPosition().x - collider_position.x) *
			(circle_collider.getPosition().x - collider_position.x) +
			(circle_collider.getPosition().y - collider_position.y) *
			(circle_collider.getPosition().y - collider_position.y)
		);
	//	std::cout << collider_position.x << " " << collider_position.y << " " << circle_collider.getRadius() << "\n";
	//	std::cout << circle_collider.getPosition().x << " " << circle_collider.getPosition().y << " " << radius  << std::endl;
		return distance < (radius + circle_collider.getRadius());
	}
	bool CircleCollider::intersect(BoxCollider& box_collider)
	{
		sf::Vector2f left_upper_corner = box_collider.getPosition();
		sf::Vector2f right_bottom_corner = sf::Vector2f(left_upper_corner.x + box_collider.getWidth(), left_upper_corner.y + box_collider.getHeight());


	/*return circle_position.x >= (left_upper_corner.x - circle_collider.getRadius()) &&
			circle_position.x <= (right_bottom_corner.x + circle_collider.getRadius()) &&
			circle_position.y >= (left_upper_corner.y - circle_collider.getRadius()) &&
			circle_position.y <= (right_bottom_corner.y + circle_collider.getRadius());*/

		//std::cout << "Collision with box\n";
		if(radius > 10)
		std::cout << "collider_x: " << collider_position.x << " left: " << left_upper_corner.x << " right: " << right_bottom_corner.x << " radius: " << radius << "\n";
		std::cout << "collidet_y: " << collider_position.y << " up: " << left_upper_corner.y << " bottom: " << right_bottom_corner.y << "\n";
		return collider_position.x >= (left_upper_corner.x - radius) && 
			collider_position.x <= (right_bottom_corner.x + radius) &&
			collider_position.y >= (left_upper_corner.y + radius) &&
			collider_position.y <= (right_bottom_corner.y - radius);

	}

	sf::Vector2f CircleCollider::getPosition() const
	{
		return collider_position;
	}

	void CircleCollider::setPosition(sf::Vector2f position)
	{
		collider_position = position;
	}

	Collider & CircleCollider::operator=(Collider & other)
	{
		if (CircleCollider* circle = dynamic_cast<CircleCollider*>(&other))
		{
			this->collider_position = circle->collider_position;
			this->radius = circle->radius;
			return *this;
		}
		CircleCollider* c = new CircleCollider(5);
		return *c;
	}


	float CircleCollider::getRadius() const
	{
		return radius;
	}
