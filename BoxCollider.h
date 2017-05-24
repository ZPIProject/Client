#pragma once
#include <SFML\Graphics.hpp>
#include "Collider.h"
#include "CircleCollider.h"

	class CircleCollider;
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(sf::Vector2f position, float width, float height);
		~BoxCollider();

		bool intersect(Collider & collider);


		void setPosition(sf::Vector2f position);

		sf::Vector2f getPosition();
		float getWidth();
		float getHeight();



	private:

		sf::Vector2f collider_position;
		float width;
		float height;

		bool point_in_line(std::vector<sf::Vector2f> points);

		bool intersect(CircleCollider& circle_collider);
		bool intersect(BoxCollider & box_collider);

	};

