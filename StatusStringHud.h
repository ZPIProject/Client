#pragma once
#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <string>

class StatusStringHud {
private:
	int id;
	std::string name;
	int duration;
	sf::Clock duration_timer;
public:
	StatusStringHud(int id, std::string name, int duration);

	int getId();
	std::string getName();
	int getDuration();

	bool has_ended();

	void setDuration(int duration);
	void restartDuration();
};