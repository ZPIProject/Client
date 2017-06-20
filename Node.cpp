#include "Node.h"

//if children_size <0 firstchild,secondchild = -1, if children_size = 1, secondchild = -1
Node::Node(std::string nam, int idk, int firstchild, int secondchild) {
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

Node::Node(std::string nam, int idk, int child) {
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	id = idk;
	if (child > -1) {
		children.push_back(child);
	}
}

Node::Node(std::string nam, int idk) {
	Status = STATUS::NOT_AVAILABLE;
	name = nam;
	id = idk;
}

STATUS Node::getStatus() {
	return Status;
}

void Node::setStatus(STATUS status) {
	Status = status;
}

std::string Node::getName() {
	return name;
}

int Node::getId() {
	return id;
}

std::vector<int> Node::getChildren() {
	return children;
}

int Node::getChildrenSize() {
	return children.size();
}