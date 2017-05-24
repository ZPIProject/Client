#include "CollisionHandler.h"



CollisionHandler::CollisionHandler()
{
}


CollisionHandler::~CollisionHandler()
{
}

void CollisionHandler::add_object(ColidableObject& object)
{
	objects_to_collide.push_back(&object);
}

void CollisionHandler::clear()
{
	/*for (auto it = objects_to_collide.begin(); it != objects_to_collide.end(); it++)
		delete *it;*/
	objects_to_collide.clear();
}
void CollisionHandler::delete_objects()
{
	objects_to_collide.clear();
}

void CollisionHandler::show_adresses()
{
	for (int i = 0; i < objects_to_collide.size(); i++)
	{
		std::cout << "object_to_colide colider address: " << &objects_to_collide[i]->getCollider() << "\n";
	}
}

void CollisionHandler::check_objects_which_colide()//ewentualnie handle_collisions()
{
	
//	std::vector<std::pair<ColidableObject*, ColidableObject*> > objects_which_colide;
	//std::cout << objects_to_collide.size() << "\n";
	for (int i = 0; i < objects_to_collide.size() ; i++)
	{
	//	std::cout << "i: " << i << " " << &objects_to_collide[i]->getCollider() << std::endl;
		for (int j = i + 1; j < objects_to_collide.size(); j++)
		{
			
		//	std::cout << "j: " << j << " " << &objects_to_collide[j]->getCollider() << std::endl;
			if (objects_to_collide[i]->check_collision(objects_to_collide[j]->getCollider()))
			{
		
				objects_to_collide[i]->onCollision(objects_to_collide[j]);
				objects_to_collide[j]->onCollision(objects_to_collide[i]);
			}
		}
	}
}

