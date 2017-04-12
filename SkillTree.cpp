#include "SkillTree.h"

SkillTree::SkillTree()
{
	Nodes = std::vector<SkillTreeNode>();
	populateAll();
}


void SkillTree::populateAll()
{
	for (SkillTreeNode Node : Nodes) {
		Node.setStatus();
		Node.setDescription(Node.getName());
		
	}
}

void SkillTree::addAll()
{
}
