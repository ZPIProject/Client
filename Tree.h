#pragma once
#include"Skill.h"
#include <SFML\Network.hpp>

class Tree {
private:
	std::vector<Skill*> skills;
	int skillpoints;

public:
	Tree();
	int AvailablePoints(sf::Packet packet);
	void setSkillPoints(int points);
	void databaseUpdate(sf::Packet recived_packet);
	void setStatus(int id, STATUS status);
};