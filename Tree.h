#pragma once
#include"Node.h"
#include"NetworkHandler.h"
#include <SFML\Network.hpp>

class Tree {
private:
	std::vector<Node*> skills;
	int skillpoints;
	int knowledge;
	int vitality;
	int wisdom;

public:
	Tree();
	int AvailablePoints(sf::Packet packet);
	void setSkillPoints(int points);
	void databaseUpdate(sf::Packet recived_packet);
	void setStatus(int id, STATUS status);
	void buySkill(int id);
	void count_knowledge();
	void count_vitality();
	void count_wisdom();
	void count_all();

	sf::Packet buyRequest(std::string player, int idskill);
};