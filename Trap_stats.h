#pragma once
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include "Spell.h"
#include "Status.h"
class Trap_stats : public Spell
{
public:
	Trap_stats(Element e, float seconds ,float radius);
	Trap_stats();
	~Trap_stats();
	
	bool has_ended();
	
	float get_radius() { return radius; }
	Status get_status() { return holded_status; }
private:
	float radius;
	
	sf::Clock duration_timer;
	float duration_time = 10;
	
	Status holded_status;
	
	
	void set_trap_holded_status(Element e);
};

