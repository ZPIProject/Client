#include "GameManager.h"

int main()
{
	GameManager* gm = new GameManager();
	gm->run();
	delete gm;
}