#include "GameManager.h"
#include "GuiHandler.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SignCaster", sf::Style::Close);
	GuiHandler gui(window);
	gui.main_loop();
	//GameManager* gm = new GameManager();
	//gm->run();
	
}