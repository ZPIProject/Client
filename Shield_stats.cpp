#include "Shield_stats.h"



Shield_stats::Shield_stats()
{
}

Shield_stats::Shield_stats(Element e, float seconds, float radius) : Spell(e)
{
	duration_time = seconds;
	this->radius = radius;
	
	
}


Shield_stats::~Shield_stats()
{
}

bool Shield_stats::has_ended()
{
	return duration_timer.getElapsedTime().asSeconds() > 20;
}
