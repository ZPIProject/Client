#include "ColidableObject.h"
#include <iostream>

	ColidableObject::ColidableObject(Collider * col)
	{
		collider = col;
	}
	ColidableObject::ColidableObject(const ColidableObject & other)
	{
		this->collider = other.collider;
		//std::cout << "ColidableObject konstruktor kopiuj¹cy";		
	}
	bool ColidableObject::check_collision(Collider &other_collider) const
	{
		bool contact = false;
		if (CircleCollider* circle = dynamic_cast<CircleCollider*>(collider))
		{
			contact = circle->intersect(other_collider);
			
		}
		if (BoxCollider* box = dynamic_cast<BoxCollider*>(collider))
		{
			
			contact = box->intersect(other_collider);
			
		}
		return contact;
	}
	Collider& ColidableObject::getCollider()
	{
		return *collider;
	}

	void ColidableObject::deactivate()
	{
		is_active = false;
	}

	bool ColidableObject::getActiveStatus()
	{
		return is_active;
	}

