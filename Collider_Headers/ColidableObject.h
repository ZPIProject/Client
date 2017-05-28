#pragma once
#include "Collider.h"
#include "CircleCollider.h"
#include "BoxCollider.h"

class ColidableObject
{
	public:
		ColidableObject(Collider *col);
		ColidableObject(const ColidableObject& other);


		virtual bool check_collision(Collider&) const;
		Collider& getCollider();
		virtual void onCollision(ColidableObject* object) = 0;
		void deactivate();
		bool getActiveStatus();
	protected:
		Collider* collider;
		bool is_active;
};

