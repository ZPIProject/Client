#include "Tree.h"

Tree::Tree()
{
	//Fire
	skills.push_back(new Skill("FB", 0, 1, 2));
	skills.push_back(new Skill("FInt1", 1, 3, 4));
	skills.push_back(new Skill("FVit1", 2, 3, 5));
	skills.push_back(new Skill("FWis1", 3));
	skills.push_back(new Skill("FVit2", 4, 6));
	skills.push_back(new Skill("FInt2", 5, 7));
	skills.push_back(new Skill("FB2", 6, 8));
	skills.push_back(new Skill("FT", 7, 9));
	skills.push_back(new Skill("FWis2", 8, 10));
	skills.push_back(new Skill("FInt3", 9, 10));
	skills.push_back(new Skill("FS", 10));

	//Wind
	skills.push_back(new Skill("WiB", 11, 12));
	skills.push_back(new Skill("WiWis1", 12, 13));
	skills.push_back(new Skill("WiInt1", 13, 14));
	skills.push_back(new Skill("WiWis2", 14, 15, 16));
	skills.push_back(new Skill("WiB2", 15));
	skills.push_back(new Skill("WiVit1", 16, 17));
	skills.push_back(new Skill("WiWis3", 17, 18, 19));
	skills.push_back(new Skill("WiT", 18));
	skills.push_back(new Skill("WiVit2", 19, 20));
	skills.push_back(new Skill("WiInt2", 20, 21));
	skills.push_back(new Skill("WiS", 21));

	//Thunder
	skills.push_back(new Skill("TB", 22, 23));
	skills.push_back(new Skill("TInt1", 23, 24));
	skills.push_back(new Skill("TWis1", 24, 25, 26));
	skills.push_back(new Skill("TInt2", 25, 27));
	skills.push_back(new Skill("TB2", 26, 28));
	skills.push_back(new Skill("TVit1", 27, 29));
	skills.push_back(new Skill("TWis2", 28, 30));
	skills.push_back(new Skill("TInt3", 29, 31));
	skills.push_back(new Skill("TT", 30));
	skills.push_back(new Skill("TS", 31));

	//Earth
	skills.push_back(new Skill("EB", 32, 33, 34));
	skills.push_back(new Skill("EVit1", 33, 35));
	skills.push_back(new Skill("EVit2", 34, 36));
	skills.push_back(new Skill("EInt1", 35, 37));
	skills.push_back(new Skill("EWis1", 36, 38));
	skills.push_back(new Skill("EB2", 37, 39));
	skills.push_back(new Skill("ET", 38, 40));
	skills.push_back(new Skill("EWis2", 39, 41));
	skills.push_back(new Skill("EInt2", 40, 41));
	skills.push_back(new Skill("EVit3", 41, 42));
	skills.push_back(new Skill("ES", 42));

	//Water
	skills.push_back(new Skill("WaB", 43, 44));
	skills.push_back(new Skill("WaWis1", 44, 45));
	skills.push_back(new Skill("WaVit1", 45, 46));
	skills.push_back(new Skill("WaInt1", 46, 47));
	skills.push_back(new Skill("WaB2", 47, 48));
	skills.push_back(new Skill("WaWis2", 48, 49));
	skills.push_back(new Skill("WaVit2", 49, 50));
	skills.push_back(new Skill("WaT", 50, 51));
	skills.push_back(new Skill("WaInt2", 51, 52));
	skills.push_back(new Skill("WaVit3", 52, 53));
	skills.push_back(new Skill("WaS", 53));
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
		setStatus(idskill, SOLD);
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