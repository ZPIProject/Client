#include "Tree.h"

Tree::Tree()
{
	skillpoints = 0;
	knowledge = 0;
	vitality = 0;
	wisdom = 0;

	//Fire
	skills.push_back(new Node("FB", 0, 1, 2));
	skills.push_back(new Node("FInt1", 1, 3, 4));
	skills.push_back(new Node("FVit1", 2, 3, 5));
	skills.push_back(new Node("FWis1", 3));
	skills.push_back(new Node("FVit2", 4, 6));
	skills.push_back(new Node("FInt2", 5, 7));
	skills.push_back(new Node("FB2", 6, 8));
	skills.push_back(new Node("FT", 7, 9));
	skills.push_back(new Node("FWis2", 8, 10));
	skills.push_back(new Node("FInt3", 9, 10));
	skills.push_back(new Node("FS", 10));

	//Wind
	skills.push_back(new Node("WiB", 11, 12));
	skills.push_back(new Node("WiWis1", 12, 13));
	skills.push_back(new Node("WiInt1", 13, 14));
	skills.push_back(new Node("WiWis2", 14, 15, 16));
	skills.push_back(new Node("WiB2", 15));
	skills.push_back(new Node("WiVit1", 16, 17));
	skills.push_back(new Node("WiWis3", 17, 18, 19));
	skills.push_back(new Node("WiT", 18));
	skills.push_back(new Node("WiVit2", 19, 20));
	skills.push_back(new Node("WiInt2", 20, 21));
	skills.push_back(new Node("WiS", 21));

	//Thunder
	skills.push_back(new Node("TB", 22, 23));
	skills.push_back(new Node("TInt1", 23, 24));
	skills.push_back(new Node("TWis1", 24, 25, 26));
	skills.push_back(new Node("TInt2", 25, 27));
	skills.push_back(new Node("TB2", 26, 28));
	skills.push_back(new Node("TVit1", 27, 29));
	skills.push_back(new Node("TWis2", 28, 30));
	skills.push_back(new Node("TInt3", 29, 31));
	skills.push_back(new Node("TT", 30));
	skills.push_back(new Node("TS", 31));

	//Earth
	skills.push_back(new Node("EB", 32, 33, 34));
	skills.push_back(new Node("EVit1", 33, 35));
	skills.push_back(new Node("EVit2", 34, 36));
	skills.push_back(new Node("EInt1", 35, 37));
	skills.push_back(new Node("EWis1", 36, 38));
	skills.push_back(new Node("EB2", 37, 39));
	skills.push_back(new Node("ET", 38, 40));
	skills.push_back(new Node("EWis2", 39, 41));
	skills.push_back(new Node("EInt2", 40, 41));
	skills.push_back(new Node("EVit3", 41, 42));
	skills.push_back(new Node("ES", 42));

	//Water
	skills.push_back(new Node("WaB", 43, 44));
	skills.push_back(new Node("WaWis1", 44, 45));
	skills.push_back(new Node("WaVit1", 45, 46));
	skills.push_back(new Node("WaInt1", 46, 47));
	skills.push_back(new Node("WaB2", 47, 48));
	skills.push_back(new Node("WaWis2", 48, 49));
	skills.push_back(new Node("WaVit2", 49, 50));
	skills.push_back(new Node("WaT", 50, 51));
	skills.push_back(new Node("WaInt2", 51, 52));
	skills.push_back(new Node("WaVit3", 52, 53));
	skills.push_back(new Node("WaS", 53));
}

int Tree::AvailablePoints(sf::Packet recived_packet) {
	int points;
	recived_packet >> points;
	setSkillPoints(points);
	return skillpoints;
}

void Tree::databaseUpdate(sf::Packet recived_packet) {
	int counter;
	std::vector<int> change_to_avalible;
	recived_packet >> counter;
	for (int i = 0; i < counter; i++) {
		int idskill;
		recived_packet >> idskill;
		/*
		std::string skillid;
		recived_packet >> skillid;
		int idskill = std::stoi(skillid);
		*/
		setStatus(idskill -1, SOLD);
		if (skillpoints > 0) {
			std::vector<int> children = skills[idskill]->getChildren();
			for (int i : children) {
				change_to_avalible.push_back(i);
			}
		}
	}
	for (int i : change_to_avalible) {
		if (skills[i]->getStatus() == NOT_AVAILABLE) {
			setStatus(i, AVAILABLE);
		}
	}
}

void Tree::setSkillPoints(int points) {
	skillpoints = points;
}

void Tree::setStatus(int id, STATUS status) {
	skills[id]->setStatus(status);
}

void Tree::buySkill(int id) {
	skills[id]->setStatus(SOLD);
	std::vector<int> children = skills[id]->getChildren();
	if (skillpoints > 0) {
		for (int i : children) {
			if (skills[i]->getStatus() == NOT_AVAILABLE) {
				setStatus(i, AVAILABLE);
			}
		}
	}
}

void Tree::count_knowledge()
{
	if (skills[1]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[5]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[9]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[13]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[20]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[23]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[25]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[29]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[35]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[40]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[46]->getStatus() == SOLD) {
		knowledge += 5;
	}
	if (skills[51]->getStatus() == SOLD) {
		knowledge += 5;
	}
}

void Tree::count_vitality()
{
	if (skills[2]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[4]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[16]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[19]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[27]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[33]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[34]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[41]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[45]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[49]->getStatus() == SOLD) {
		vitality += 5;
	}
	if (skills[52]->getStatus() == SOLD) {
		vitality += 5;
	}
}

void Tree::count_wisdom()
{
	if (skills[3]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[8]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[12]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[14]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[17]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[24]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[28]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[36]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[39]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[44]->getStatus() == SOLD) {
		wisdom += 5;
	}
	if (skills[48]->getStatus() == SOLD) {
		wisdom += 5;
	}
}

void Tree::count_all()
{
	count_knowledge();
	count_vitality();
	count_wisdom();
}

sf::Packet Tree::buyRequest(std::string player, int idskilla) {
	sf::Packet result;
	result << 8 << player << idskilla;
	return result;
}

