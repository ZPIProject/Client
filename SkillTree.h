#pragma once
#include <vector>
#include"SkillTreeNode.h"


class SkillTree {
private:
	std::vector<SkillTreeNode> Nodes;

public :

	SkillTree();
	void populateAll();
	void addAll();
};