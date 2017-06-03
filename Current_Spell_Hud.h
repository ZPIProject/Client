#pragma once
#include <SFML\Graphics.hpp>
#include <string>

class Current_Spell_Hud {
public:
	Current_Spell_Hud(sf::Vector2f position, int spell, int element);
	Current_Spell_Hud();

	~Current_Spell_Hud();

	sf::Text getText();
	void setSpell(int spell);
	void setElement(int element);
	void setPosition(sf::Vector2f position);

	int getSpell();
	int getElement();
private:
	sf::Text text;
	sf::Font font;
	int spell;
	int element;
};