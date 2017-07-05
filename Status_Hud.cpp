#include "Status_Hud.h"
#include <iostream>

Status_Hud::Status_Hud()
{
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Problem with loading font" << std::endl;
	}
	text.setFont(font);
	text.setCharacterSize(12);
	text.setStyle(sf::Text::Italic);
	text.setFillColor(sf::Color::Black);
}

Status_Hud::~Status_Hud()
{
}

sf::Text Status_Hud::getText()
{
	return text;
}

void Status_Hud::setPosition(sf::Vector2f position)
{
	text.setPosition(position);
}

void Status_Hud::setText(std::string textt)
{
	text.setString(textt);
}
