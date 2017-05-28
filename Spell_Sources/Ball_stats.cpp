#include "..\Spell_Headers\Ball_stats.h"





Ball_stats::Ball_stats(Element e, float dmg, float speed, float radius) : Spell(e)
{
	damage = dmg;
	this->speed = speed;
	this->radius = radius;
}

Ball_stats::~Ball_stats()
{
}
