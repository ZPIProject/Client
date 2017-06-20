#include "StatusStringHud.h"

StatusStringHud::StatusStringHud(int idk, std::string nam, int durationn)
{
	id = idk;
	name = nam;
	duration = durationn;
	duration_timer.restart();
}

int StatusStringHud::getId()
{
	return id;
}

std::string StatusStringHud::getName()
{
	return name;
}

int StatusStringHud::getDuration()
{
	return duration;
}

bool StatusStringHud::has_ended()
{
	return duration_timer.getElapsedTime().asSeconds() > duration;
}

void StatusStringHud::setDuration(int durationn)
{
	duration = durationn;
}

void StatusStringHud::restartDuration()
{
	duration_timer.restart();
}
