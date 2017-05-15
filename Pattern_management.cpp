#include "Pattern_management.h"
#include "SCSRv2.h"

double Pattern_management::initializeNetwork()
{
	try
	{

		if (!mclInitializeApplication(nullptr, 0))
		{
			std::cerr << "Could not initialize the application properly."
				<< std::endl;
			system("pause");
			return -3;
		}

		if (!mclmcrInitialize())
		{
			std::cerr << "Could not initialize MATLAB runtime properly."
				<< std::endl;
			system("pause");
			return -2;
		}

		if (!SCSRv2Initialize())
		{
			std::cerr << "Could not initialize the library properly."
				<< std::endl;
			system("pause");
			return -1;
		}
		mclmcrInitialize();

		

		
	}
	catch (mwException e)
	{
		std::cerr << e.what() << std::endl;
		e.print_stack_trace();

		return -1;
	}
}

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

double Pattern_management::pattern_close()
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
	mwArray result(1, 1, mxDOUBLE_CLASS);			//parametr z wynikiem
	std::string path("TEST.png"); //sciezka obrazka 800x600 czarno-bialego
	mwArray picturepath(path.c_str());	//parametr ze sciezka obrazka
	FinalPredict(1, result, picturepath);
	lastx = lasty = -1;

	return result;
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

double Pattern_management::manage_pattern(int x, int y, bool Io)
{
	if (isOpen) {
		if (!Io) {
			double res = pattern_close();
			isOpen = false;
			return res;

		}
		else {
			addPoint(x, y);
			return -1.0;
		}
	}
	else if (Io) {
			pattern_open(x, y);
			isOpen = Io;
			return -1.0;
		
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
	initializeNetwork();
}

Pattern_management::Pattern_management()
{
	initializeNetwork();
}


