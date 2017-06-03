#include "Status_Hud.h"
#include <iostream>

Status_Hud::Status_Hud()
{
	element = 0;
	duration = 0;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Problem with loading font" << std::endl;
	}
	text.setFont(font);
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Italic);
	text.setColor(sf::Color::Black);
}

Status_Hud::~Status_Hud()
{
}

sf::Text Status_Hud::getText()
{
	return text;
}

int Status_Hud::getElement()
{
	return element;
}

void Status_Hud::setElement(int elementt)
{
	switch (elementt)
	{
	case 1:
		text.setString("Debuff");
		break;
	case 4:
		text.setString("Stun");
		break;
	case 5:
		text.setString("Damage");
		break;
	case 6:
		text.setString("Confusion");
		break;
	case 9:
		text.setString("Slow");
		break;
	default:
		break;
	}
	element = elementt;
}

void Status_Hud::setDuration(int durationn)
{
	duration = durationn;
}

void Status_Hud::setPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

void Status_Hud::setText(std::string textt)
{
	text.setString(textt);
}
