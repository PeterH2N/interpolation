#include "animation.h"

animation::animation(CoordinateSystem * _CS, sf::CircleShape _circle, regression::Polynomial _path, std::vector<double> _interval, sf::Time _duration, int _fps, sf::Keyboard::Key _button)
	:circle(_circle), path(_path), duration(_duration.asSeconds()), fps(_fps), interval(_interval), CS(_CS), button(_button)
{
	double numFrames = fps * duration;

	points = interpolation::Polynomial(path, numFrames, interval).points;

	frameTime = duration / numFrames;

	iterations = 0;

	isRunning = false;

}

void animation::runAnimation(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == button)
	{
		iterations = 0;
		Elapsed.restart();
		isRunning = true;
	}
	if(isRunning)
	{
		circlex = CS->convertCoordinates(points[iterations]).x;
		circley = CS->convertCoordinates(points[iterations]).y;

		circle.setPosition(circlex, circley);

		CS->window.draw(circle);


		if (Elapsed.getElapsedTime().asSeconds() > frameTime * (iterations + 1))
			iterations++;

		if (iterations >= points.size())
		{
			iterations = 0;
			isRunning = false;
		}

	}
}