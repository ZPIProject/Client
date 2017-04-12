#include "SkillTreeNode.h"




SkillTreeNode::SkillTreeNode(int cos, std::string nam, std::vector<SkillTreeNode>& Nei) {
cost = cos;
Neighbours = std::vector<SkillTreeNode>();
for (SkillTreeNode N : Nei) {
	Neighbours.push_back(N);
}
Status = STATUS::NOT_AVAILABLE;
name = nam;
Description = setDescription(name);
	}

SkillTreeNode::SkillTreeNode(int cos, std::string nam, SkillTreeNode & Nei) {
	cost = cos;
	Neighbours = std::vector<SkillTreeNode>();
	Neighbours.push_back(Nei);
	Nei.addNeighbour(*this);
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	Description = setDescription(name);
}



SkillTreeNode::SkillTreeNode(int cos, std::string nam) {
	cost = cos;
	Neighbours = std::vector<SkillTreeNode>();
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	Description = setDescription(name);
}






void SkillTreeNode::addNeighbour(SkillTreeNode& Nei)
{
	bool contains = false;
	for (SkillTreeNode s : Neighbours) {
		if (s.name == Nei.name)contains = true;
	}
	if(!contains)Neighbours.push_back(Nei);
}
std::string SkillTreeNode::getName()
{
	return name;
}
bool SkillTreeNode::comparator(const SkillTreeNode & lhs, const SkillTreeNode & rhs)
{
	return lhs.name<rhs.name;
}
//TODO DATABASE CHECK IF SKILL IS BOUGHT
bool SkillTreeNode::checkBought()
{
	return false;
}
//TODO DATABASE CHECK HOW MANY SKILLPOINTS CAN CHARACTER SPEND
unsigned SkillTreeNode::checkAvailablePoints()
{
	return 0;
}
//TODO DATABASE GET SKILL DESCRIPTION 
std::string SkillTreeNode::setDescription(std::string name)
{
	//Description = SET DESCRIPTION FROM DATABASE THEN RETURN IT;
	return std::string();
}


void SkillTreeNode::setStatus()
{
	if (checkBought()) {
		Status = STATUS::SOLD;
	}
	else if (cost >= checkAvailablePoints()) {
		/*for (SkillTreeNode Node : Neighbours) {
			if (Node.Status == STATUS::SOLD)Status = STATUS::AVAILABLE;
		}*/
	}
	else Status = STATUS::NOT_AVAILABLE;
}

