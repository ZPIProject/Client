#include "Pattern_management.h"

void Pattern_management::draw_Line(int x, int y)
{
	if (lastx >= 0 && lasty >= 0) {
		while (lastx != x || lasty != y) {
			if (lastx > x)lastx -= 1;
			else if (lastx < x)lastx += 1;
			if (lasty > y)lasty -= 1;
			else if (lasty < y)lasty += 1;
			draw_point(lastx, lasty);
		}
	}
	else {
		lastx = x;
		lasty = y;
		draw_point(x, y);
	}
}

void Pattern_management::pattern_close()
{
	lastx = lasty = -1;
	for (sf::Vector2i V: Points) {
		draw_Line(V.x, V.y);
	
	}
	Pattern.saveToFile(IMAGE_NAME+".png");
	//resize(minx,miny, maxx - minx, maxy - miny);

	Pattern = sf::Image();
	Pattern.create(Window_X, Window_Y, sf::Color::Black);
	points_on_screen.clear();
	

	lastx = lasty = -1;
}

void Pattern_management::draw_point(int x, int y)
{
	Pattern.setPixel(x, y, sf::Color::White);
	if (minx > x)minx = x;
	if (miny > y)miny = y;
	if (maxx < x)maxx = x;
	if (maxy < y)maxy = y;

	for(int i=-8;i<=8;i++)
		for (int j =-8; j<=8; j++)
			if (x + j < Window_X && x + j >= 0 && y + j < Window_Y&& y + j >= 0) {
				Pattern.setPixel(x + i, y + j, sf::Color::White);
			}
	
}

void Pattern_management::addPoint(int x, int y)
{
	if (x >= 0 && y >= 0 && x!=lastx && y!=lasty) {
		lastx = x;
		lasty = y;
		Points.push_back(sf::Vector2i(x, y));
		points_on_screen.append(sf::Vertex(sf::Vector2f(x, y)));
	}
}

void Pattern_management::pattern_open(int x, int y)
{
	Points = std::vector<sf::Vector2i>();
	Pattern = sf::Image();
	Pattern.create(Window_X, Window_Y, sf::Color::Black);
	addPoint(x, y);
}

void Pattern_management::resize(int minx, int miny, int maxx, int maxy)
//TODO RESIZE IMAGE TO FIXED SIZE
{
	
}

void Pattern_management::manage_pattern(int x, int y, bool Io)
{
	if (isOpen) {
		if (!Io) {
			pattern_close();
			isOpen = Io;
		}
		else {
			addPoint(x, y);
		}
	}
	else if (Io) {
			pattern_open(x, y);
			isOpen = Io;
		
	}
}

Pattern_management::Pattern_management(int x, int y)
{
	Window_X = x;
	Window_Y = y;
	maxx = maxy = 0;
	minx = miny = INT_MAX;
	lastx = lasty = -1;
	isOpen = false;
	Points = std::vector<sf::Vector2i>();
	points_on_screen = sf::VertexArray(sf::LineStrip);
}

Pattern_management::Pattern_management()
{

}


