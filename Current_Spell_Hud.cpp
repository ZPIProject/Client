#include "Current_Spell_Hud.h"
#include <iostream>

Current_Spell_Hud::Current_Spell_Hud(sf::Vector2f position, int spell, int element)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Problem with loading font" << std::endl;
	}

	text.setFont(font);
	switch (spell)
	{
	case 2:
		text.setString("Aura");
		break;
	case 3:
		text.setString("Shield");
		break;
	case 7:
		text.setString("Ball");
		break;
	case 8:
		text.setString("Trap");
		break;
	default:
		break;
	}
	text.setCharacterSize(15);
	text.setStyle(sf::Text::Italic);
	text.setPosition(position);
	switch (element)
	{
	case 1:
		text.setFillColor(sf::Color::White);
		break;
	case 4:
		text.setFillColor(sf::Color(102, 51, 0));
		break;
	case 5:
		text.setFillColor(sf::Color::Red);
		break;
	case 6:
		text.setFillColor(sf::Color::Yellow);
		break;
	case 9:
		text.setFillColor(sf::Color::Blue);
		break;
	default:
		break;
	}
}

Current_Spell_Hud::Current_Spell_Hud()
{
	spell = 0;
	element = 0;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Problem with loading font" << std::endl;
	}

	text.setFont(font);
	text.setCharacterSize(15);
	text.setStyle(sf::Text::Italic);
	text.setPosition(sf::Vector2f(0, 0));
}

Current_Spell_Hud::~Current_Spell_Hud()
{
}

sf::Text Current_Spell_Hud::getText()
{
	return text;
}

void Current_Spell_Hud::setSpell(int spelll)
{
	switch (spelll)
	{
	case 2:
		text.setString("Aura");
		break;
	case 3:
		text.setString("Shield");
		break;
	case 7:
		text.setString("Ball");
		break;
	case 8:
		text.setString("Trap");
		break;
	default:
		break;
	}
	spell = spelll;
}

void Current_Spell_Hud::setElement(int elementt)
{
	switch (elementt)
	{
	case 1:
		text.setFillColor(sf::Color::White);
		break;
	case 4:
		text.setFillColor(sf::Color(102, 51, 0));
		break;
	case 5:
		text.setFillColor(sf::Color::Red);
		break;
	case 6:
		text.setFillColor(sf::Color::Yellow);
		break;
	case 9:
		text.setFillColor(sf::Color::Blue);
		break;
	default:
		break;
	}
	element = elementt;
}

void Current_Spell_Hud::setPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

int Current_Spell_Hud::getSpell()
{
	return spell;
}

int Current_Spell_Hud::getElement()
{
	return element;
}
