#pragma once
#include <string>
#include <vector>

enum STATUS {
AVAILABLE,
NOT_AVAILABLE,
SOLD
};

class Skill {
private:
	STATUS Status;

	std::vector<int> children;
	std::string name;
	int id;

public:
	Skill(std::string nam, int idk, int firstchild, int secondchild);
	Skill(std::string nam, int idk, int child);
	Skill(std::string nam, int idk);

	void setStatus(STATUS status);

	STATUS getStatus();

	std::string getName();
	int getId();
	int getChildrenSize();
	std::vector<int> getChildren();
};
