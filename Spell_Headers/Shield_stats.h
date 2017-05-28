#pragma once
#include "Spell.h"
#include <SFML\System\Clock.hpp>
class Shield_stats : public Spell
{
public:
	Shield_stats();
	Shield_stats(Element e, float seconds, float radius);
	~Shield_stats();
	bool has_ended();
	float get_radius() { return radius; }

private:
	float radius;
	sf::Clock duration_timer;
	float duration_time;
};

