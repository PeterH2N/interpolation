#pragma once
#include <coordinatesystem.h>
class animation
{
public:
	animation(CoordinateSystem * CS, sf::CircleShape _circle, regression::Polynomial _path, std::vector<double> _interval, sf::Time _duration, int _fps, sf::Keyboard::Key _button);

	void runAnimation(sf::Event event);
	
private:

	sf::Keyboard::Key button;

	bool isRunning;

	sf::Clock Elapsed;

	CoordinateSystem * CS;

	sf::CircleShape circle;

	double duration;

	double frameTime;

	int fps;

	int iterations;	

	regression::Polynomial path;

	std::vector<Point> points;

	vector<double> interval;


	double circlex = 0;
	double circley = 0;

};

