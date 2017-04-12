#pragma once
#include <vector>
#include <string>
enum STATUS {
	AVAILABLE,
	NOT_AVAILABLE,
	SOLD
};


class SkillTreeNode {
private:
	
	STATUS Status;
	unsigned cost;
	std::vector<SkillTreeNode> Neighbours;
	std::string Description;
	std::string name;
	bool checkBought();
	unsigned checkAvailablePoints();
	
	

public:
	SkillTreeNode(int cos, std::string nam,  std::vector<SkillTreeNode>& Nei);
	SkillTreeNode(int cos, std::string nam, SkillTreeNode & Nei);

	SkillTreeNode(int cos, std::string nam);
	void setStatus();
	std::string setDescription(std::string name);

	void addNeighbour(SkillTreeNode& Nei);
	std::string getName();
	bool comparator(const SkillTreeNode &lhs, const SkillTreeNode &rhs);
	

};

	
