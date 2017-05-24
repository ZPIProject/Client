#pragma once
#include <vector>
#include <iostream>
#include "ColidableObject.h"
class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	void add_object(ColidableObject& object);
	void delete_objects();

	void show_adresses();
	void clear();
//	void check_objects_which_colide(std::vector<ColidableObject*> object_to_colide);

	void check_objects_which_colide();


private:
	std::vector<ColidableObject*> objects_to_collide;
};

