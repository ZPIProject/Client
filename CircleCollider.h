#pragma once
#include <SFML\Graphics.hpp>
#include <math.h>
#include <iostream>

#include "Collider.h"
#include "BoxCollider.h"

	class BoxCollider;
	class CircleCollider : public Collider
	{
	public:
		CircleCollider(float rad);
		~CircleCollider();

		bool intersect(Collider& collider);

		sf::Vector2f getPosition() const;
		float getRadius() const;

		void setPosition(sf::Vector2f position);
		Collider& operator=(Collider& other);

	private:
		sf::Vector2f collider_position;
		float radius;

		bool intersect(CircleCollider& circle_collider);
		bool intersect(BoxCollider& box_collider);
	};
