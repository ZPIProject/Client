#pragma once
#include "Elements.h"
#include "Spell.h"
class Ball_stats : public Spell
{
public:
	Ball_stats() : Spell() {};
	Ball_stats(Element e, float dmg, float speed, float radius);
	~Ball_stats();
	float get_damage() { return damage; }
	float get_speed() { return speed; }
	float get_radius() { return radius; }

private:
	float damage;
	float speed;
	float radius;
};

