#pragma once
#include <string>
#include <vector>

enum STATUS {
AVAILABLE,
NOT_AVAILABLE,
SOLD
};

class Node {
private:
	STATUS Status;

	std::vector<int> children;
	std::string name;
	int id;

public:
	Node(std::string nam, int idk, int firstchild, int secondchild);
	Node(std::string nam, int idk, int child);
	Node(std::string nam, int idk);

	void setStatus(STATUS status);

	STATUS getStatus();

	std::string getName();
	int getId();
	int getChildrenSize();
	std::vector<int> getChildren();
};
