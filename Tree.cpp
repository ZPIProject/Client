#include "Tree.h"

Tree::Tree()
{
	skillpoints = 0;
	knowledge = 0;
	vitality = 0;
	wisdom = 0;

	//Fire
	skills.push_back(new Node("FB", 0, 1, 2));
	skills.push_back(new Node("FK1", 1, 3, 4));
	skills.push_back(new Node("FV1", 2, 3, 5));
	skills.push_back(new Node("FW1", 3));
	skills.push_back(new Node("FV2", 4, 6));
	skills.push_back(new Node("FK2", 5, 7));
	skills.push_back(new Node("FB2", 6, 8));
	skills.push_back(new Node("FT", 7, 9));
	skills.push_back(new Node("FW2", 8, 10));
	skills.push_back(new Node("FK3", 9, 11));
	skills.push_back(new Node("FV3", 10, 12));
	skills.push_back(new Node("FV4", 11, 13));
	skills.push_back(new Node("FK4", 12, 14));
	skills.push_back(new Node("FW3", 13, 14));
	skills.push_back(new Node("FS", 14));

	//Wind
	skills.push_back(new Node("WiB", 15, 16));
	skills.push_back(new Node("WiW1", 16, 17));
	skills.push_back(new Node("WiK1", 17, 18));
	skills.push_back(new Node("WiW2", 18, 19));
	skills.push_back(new Node("WiV1", 19, 20, 21));
	skills.push_back(new Node("WiB2", 20));
	skills.push_back(new Node("WiV2", 21, 22));
	skills.push_back(new Node("WiW3", 22, 23, 24));
	skills.push_back(new Node("WiT", 23));
	skills.push_back(new Node("WiV3", 24, 25));
	skills.push_back(new Node("WiK2", 25, 26));
	skills.push_back(new Node("WiS", 26));

	//Thunder
	skills.push_back(new Node("TB", 27, 28));
	skills.push_back(new Node("TK1", 28, 29));
	skills.push_back(new Node("TW1", 29, 30, 31));
	skills.push_back(new Node("TK2", 30, 32));
	skills.push_back(new Node("TB2", 31, 33));
	skills.push_back(new Node("TV1", 32, 34));
	skills.push_back(new Node("TW2", 33, 25));
	skills.push_back(new Node("TK3", 34, 26));
	skills.push_back(new Node("TT", 35));
	skills.push_back(new Node("TS", 36));

	//Earth
	skills.push_back(new Node("EB", 37, 38));
	skills.push_back(new Node("EW1", 38, 39, 40));
	skills.push_back(new Node("EV1", 39, 41));
	skills.push_back(new Node("EV2", 40, 42));
	skills.push_back(new Node("EK1", 41, 43));
	skills.push_back(new Node("EW2", 42, 44));
	skills.push_back(new Node("EB2", 43, 45));
	skills.push_back(new Node("ET", 44, 46));
	skills.push_back(new Node("EW3", 45, 47));
	skills.push_back(new Node("EK2", 46, 48));
	skills.push_back(new Node("EV3", 47, 49));
	skills.push_back(new Node("EK3", 48, 49));
	skills.push_back(new Node("EW4", 49, 50));
	skills.push_back(new Node("ES", 50));

	//Water
	skills.push_back(new Node("WaB", 51, 52));
	skills.push_back(new Node("WaW1", 52, 53));
	skills.push_back(new Node("WaV1", 53, 54));
	skills.push_back(new Node("WaK1", 54, 55));
	skills.push_back(new Node("WaW2", 55, 56));
	skills.push_back(new Node("WaB2", 56, 57));
	skills.push_back(new Node("WaV2", 57, 58));
	skills.push_back(new Node("WaK2", 58, 59));
	skills.push_back(new Node("WaW3", 59, 60));
	skills.push_back(new Node("WaT", 60, 61));
	skills.push_back(new Node("WaK3", 61, 62));
	skills.push_back(new Node("WaS", 62));
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
		setStatus(idskill - 1, SOLD);
		if (skillpoints > 0) {
			std::vector<int> children = skills[idskill - 1]->getChildren();
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
	std::cout << skills[id]->getName() << std::endl;
	skills[id]->setStatus(SOLD);
	skillpoints--;
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
	int result = 0;
	if (skills[1]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[5]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[9]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[12]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[17]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[25]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[28]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[34]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[46]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[48]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[41]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[54]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[58]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[61]->getStatus() == SOLD) {
		result += 5;
	}
	knowledge = result;
}

void Tree::count_vitality()
{
	int result = 0;
	if (skills[2]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[4]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[11]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[10]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[19]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[21]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[24]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[32]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[39]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[40]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[47]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[53]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[57]->getStatus() == SOLD) {
		result += 5;
	}
	vitality = result;
}

void Tree::count_wisdom()
{
	int result = 0;
	if (skills[3]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[8]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[13]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[16]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[18]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[22]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[29]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[33]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[38]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[42]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[45]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[49]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[52]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[55]->getStatus() == SOLD) {
		result += 5;
	}
	if (skills[59]->getStatus() == SOLD) {
		result += 5;
	}
	wisdom = result;
}

void Tree::count_all()
{
	count_knowledge();
	count_vitality();
	count_wisdom();
}

int Tree::getStatus(int id)
{
	return skills[id]->getStatus();
}

int Tree::getKnowledge()
{
	return knowledge;
}

int Tree::getVitality()
{
	return vitality;
}

int Tree::getWisdom()
{
	return wisdom;
}

int Tree::getSkillPoints()
{
	return skillpoints;
}

std::vector<int> Tree::getBoughtSpells()
{
	std::vector<int> result;
	if (skills[0]->getStatus() == SOLD) {
		result.push_back(0);
	}
	if (skills[6]->getStatus() == SOLD) {
		result.push_back(6);
	}
	if (skills[7]->getStatus() == SOLD) {
		result.push_back(7);
	}
	if (skills[14]->getStatus() == SOLD) {
		result.push_back(14);
	}
	if (skills[15]->getStatus() == SOLD) {
		result.push_back(15);
	}
	if (skills[20]->getStatus() == SOLD) {
		result.push_back(20);
	}
	if (skills[23]->getStatus() == SOLD) {
		result.push_back(23);
	}
	if (skills[26]->getStatus() == SOLD) {
		result.push_back(26);
	}
	if (skills[27]->getStatus() == SOLD) {
		result.push_back(27);
	}
	if (skills[31]->getStatus() == SOLD) {
		result.push_back(31);
	}
	if (skills[35]->getStatus() == SOLD) {
		result.push_back(35);
	}
	if (skills[36]->getStatus() == SOLD) {
		result.push_back(36);
	}
	if (skills[37]->getStatus() == SOLD) {
		result.push_back(37);
	}
	if (skills[43]->getStatus() == SOLD) {
		result.push_back(43);
	}
	if (skills[44]->getStatus() == SOLD) {
		result.push_back(44);
	}
	if (skills[50]->getStatus() == SOLD) {
		result.push_back(50);
	}
	if (skills[51]->getStatus() == SOLD) {
		result.push_back(51);
	}
	if (skills[56]->getStatus() == SOLD) {
		result.push_back(56);
	}
	if (skills[60]->getStatus() == SOLD) {
		result.push_back(60);
	}
	if (skills[62]->getStatus() == SOLD) {
		result.push_back(62);
	}
	return result;
}

sf::Packet Tree::buyRequest(std::string player, int idskilla) {
	sf::Packet result;
	result << 8 << player << idskilla;
	return result;
}
