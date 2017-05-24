#include "Trap_stats.h"
#include <iostream>


Trap_stats::Trap_stats() : Spell()
{
}


Trap_stats::Trap_stats(Element e, float seconds, float radius) : Spell(e)
{
	duration_time = seconds;// sf::seconds(seconds);
	set_trap_holded_status(e);
	this->radius = radius;
	duration_timer.restart();
}

Trap_stats::~Trap_stats()
{
}

bool Trap_stats::has_ended()
{
	//std::cout << duration_timer.getElapsedTime().asSeconds() << " " << duration_time << " " << (duration_timer.getElapsedTime().asSeconds() > duration_time) << "\n";
	return duration_timer.getElapsedTime().asSeconds() > 10; //nie wiem czemu duration_time == 0 ???!!
}

void Trap_stats::set_trap_holded_status(Element e)
{
	switch (e)
	{
	case Element::FIRE:
		holded_status = Status::DAMAGE;
		break;
	case Element::WIND:
		holded_status = Status::DAMAGE_DEBUF;
		break;
	case Element::THUNDER:
		holded_status = Status::REV_DIRECTION;
		break;
	case Element::EARTH:
		holded_status = Status::SLOW;
		break;
	case Element::WATER:
		holded_status = Status::STUN;
		break;
	default:
		break;
	}
	
}
