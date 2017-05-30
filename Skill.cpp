#include "Skill.h"

//if children_size <0 firstchild,secondchild = -1, if children_size = 1, secondchild = -1
Skill::Skill(std::string nam, int idk, int firstchild, int secondchild) {
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	id = idk;
	if (firstchild > -1) {
		children.push_back(firstchild);
	}
	if (secondchild > -1) {
		children.push_back(secondchild);
	}	
}

Skill::Skill(std::string nam, int idk, int child) {
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	id = idk;
	if (child > -1) {
		children.push_back(child);
	}
}

Skill::Skill(std::string nam, int idk) {
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	id = idk;
}

STATUS Skill::getStatus() {
	return Status;
}

void Skill::setStatus(STATUS status) {
	Status = status;
}

std::string Skill::getName() {
	return name;
}

int Skill::getId() {
	return id;
}

std::vector<int> Skill::getChildren() {
	return children;
}

int Skill::getChildrenSize() {
	return children.size();
}