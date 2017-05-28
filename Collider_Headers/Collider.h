#pragma once
	class Collider
	{
	public:
		Collider() {};
		virtual bool intersect(Collider&) const { return false; }
		virtual Collider& operator=(Collider &collider) = 0;

	};


