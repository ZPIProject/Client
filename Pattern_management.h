#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include<vector>

#include "ConstantValues.h"

class Pattern_management {
private:
	bool isOpen;
	std::vector<sf::Vector2i> Points;
	unsigned int Window_X;
	unsigned int Window_Y;
	unsigned int minx, miny, maxx, maxy;
	int lastx, lasty;
	void draw_Line(int x, int y);
	void pattern_close();
	void draw_point(int x,int y);
	void addPoint(int x, int y);
	void pattern_open(int x,int y);
	void resize(int minx,int miny, int maxx,int maxy);
	sf::Image Pattern;
	int folder = -1;
	std::string identify;
	sf::VertexArray points_on_screen; //ALL THE POINTS THAT ARE CREATED FROM DRAWING WITH MOUSE


public:
	void manage_pattern(int x, int y, bool Io);
	Pattern_management(int x, int y);
	Pattern_management();
	void set_folder(int i) {
		folder = i;
	}
	void set_identify(std::string s) {
		identify = s;
	}
	sf::VertexArray getArray() {
		return points_on_screen;
	}
};
