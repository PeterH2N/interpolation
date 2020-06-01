#include "coordinatesystem.h"
#include <cmath>
#include <SFML/Graphics/Text.hpp>
#include <sstream>
#include <iomanip>

std::string CoordinateSystem::countingDecPlaces(double x)
{
	std::stringstream ss;

	if (fabsf(roundf(x) - x) <= 0.00001f)
		ss << std::fixed << std::setprecision(0) << x;
	else
		ss << std::fixed << std::setprecision(log2(pointPrecision)) << x;
	return ss.str();
}


CoordinateSystem::CoordinateSystem(sf::RenderWindow& _window, std::vector<double> _xinterval, std::vector<double> _yinterval, bool _viewGrid)
: window(_window),  xinterval(_xinterval), yinterval(_yinterval), viewGrid(_viewGrid)
{

	convertCenter();

	setGridPrecision();

	this->font.loadFromFile("arial.ttf");

	window.setFramerateLimit(60);

	moveToolActive = false;

}

void CoordinateSystem::changeXinterval(std::vector<double> newinterval)
{
	this->xinterval = newinterval;

	convertCenter();
}

void CoordinateSystem::changeYinterval(std::vector<double> newinterval)
{
	this->yinterval = newinterval;

	convertCenter();
}

void CoordinateSystem::draw()
{

	drawAxes();

	if (viewGrid)
	drawGrid();

	drawPointMarkers();

	for (int i = 0; i < interpolated_polynomials.size(); i++)
	{
		drawLines(graphics::LinesBetweenPoints(interpolated_polynomials[i].points, sf::Color::Red));
	}

	for (auto current : polynomials)
	{
		drawPolynomial(current);
	}

}

double CoordinateSystem::xIntervalSize()
{
	return xinterval[1] - xinterval[0];
}

double CoordinateSystem::yIntervalSize()
{
	return yinterval[1] - yinterval[0];
}

double CoordinateSystem::pixelsPerX()
{
	return window.getSize().x / xIntervalSize();
}
double CoordinateSystem::pixelsPerY()
{
	return window.getSize().y / yIntervalSize();
}

void CoordinateSystem::convertCenter()
{

	double centerxratio = -xinterval[0] / xIntervalSize();

	double centeryration = -yinterval[0] / yIntervalSize();

	double x = window.getSize().x * centerxratio;

	double y = window.getSize().y - window.getSize().y * centeryration;

	center = { x, y };
}

Point CoordinateSystem::convertCoordinates(const Point & a)
{
	//both coordinates should be the center + the coordinates * the amount of pixels to move per coordinate.
	double x_out = center.x + (a.x * pixelsPerX());
	double y_out = center.y + (-a.y * pixelsPerY());

	return { x_out, y_out };
}

Point CoordinateSystem::convertPixels(const Point & a)
{
	//reverse of the above funtion
	double x_out = (a.x - center.x) / pixelsPerX();
	double y_out = (-a.y + center.y) / pixelsPerY();

	return { x_out, y_out };
}

void CoordinateSystem::CoordinateSystem::drawAxes()
{

	//top center
	Point ytop{ center.x ,0 };
	//bottom center
	Point ybottom{ center.x, window.getSize().y };
	//left center
	Point xleft{ 0, center.y };
	//right center
	Point xright{ window.getSize().x, center.y };

	sf::VertexArray yaxis = graphics::line(ytop, ybottom, sf::Color::Black);
	sf::VertexArray xaxis = graphics::line(xleft, xright, sf::Color::Black);

	window.draw(yaxis);
	window.draw(xaxis);

}

void CoordinateSystem::drawPointMarkers()
{
	double pixelsX = pixelsPerX();
	double pixelsY = pixelsPerY();

	//drawing markers on the positive x-axis
	for (int i = 1; i / pointPrecision <= xinterval[1]; i++)
	{
		//making the point where the marker intersects with the axis
		Point current{ center.x + pixelsX * i / pointPrecision, center.y };
		//making the line by adding 10 pixels to each side of the point.
		Point currentTop{ current.x, current.y + 10 };
		Point currentBottom{ current.x, current.y - 10 };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, sf::Color::Black);

		//adding the number
		Point conCurrent = convertPixels(current);
		displayPointName(conCurrent, countingDecPlaces(conCurrent.x));

		window.draw(currentLine);
	}
	//drawing markers on the negative x-axis
	for (int i = 1; i / pointPrecision <= -xinterval[0]; i++)
	{
		//making the point where the marker intersects with the axis
		Point current{ center.x + pixelsX * -i / pointPrecision, center.y };
		//making the line by adding 10 pixels to each side of the point.
		Point currentTop{ current.x, current.y + 10 };
		Point currentBottom{ current.x, current.y - 10 };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, sf::Color::Black);

		//adding the number
		Point conCurrent = convertPixels(current);
		displayPointName(conCurrent, countingDecPlaces(conCurrent.x));

		window.draw(currentLine);
	}

	//drawing markers on the negative y-axis (negative in the code because the pixel are laid top-down)
	for (int i = 1; i / pointPrecision <= -yinterval[0]; i++)
	{
		//making the point where the marker intersects with the axis
		Point current{ center.x, center.y + pixelsY * i / pointPrecision };
		//making the line by adding 10 pixels to each side of the point.
		Point currentTop{ current.x + 10, current.y };
		Point currentBottom{ current.x - 10, current.y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, sf::Color::Black);

		//adding the number
		Point conCurrent = convertPixels(current);
		displayPointName(conCurrent, countingDecPlaces(conCurrent.y));

		window.draw(currentLine);
	}

	//drawing markers on the positive y-axis
	for (int i = 1; i / pointPrecision <= yinterval[1]; i++)
	{
		//making the point where the marker intersects with the axis
		Point current{ center.x, center.y + pixelsY * -i / pointPrecision };
		//making the line by adding 10 pixels to each side of the point.
		Point currentTop{ current.x + 10, current.y };
		Point currentBottom{ current.x - 10, current.y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, sf::Color::Black);

		//adding the number
		Point conCurrent = convertPixels(current);
		displayPointName(conCurrent, countingDecPlaces(conCurrent.y));


		window.draw(currentLine);
	}
}

void CoordinateSystem::drawGrid()
{
	double pixelsX = pixelsPerX();
	double pixelsY = pixelsPerY();

	sf::Color grey = sf::Color(200, 200, 200);

	//drawing gridlines on the positive x-axis
	for (int i = 1; i / pointPrecision <= xinterval[1]; i++)
	{
		//making the point where the grid intersects with the axis
		Point current{ center.x + pixelsX * i / pointPrecision, center.y };
		//making the line by extruding it to the top and bottom
		Point currentTop{ current.x, 0 };
		Point currentBottom{ current.x, window.getSize().y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, grey);

		window.draw(currentLine);
	}
	//drawing gridlines on the negative x-axis
	for (int i = 1; i / pointPrecision <= -xinterval[0]; i++)
	{
		//making the point where the grid intersects with the axis
		Point current{ center.x + pixelsX * -i / pointPrecision, center.y };
		//making the line by extruding it to the top and bottom
		Point currentTop{ current.x, 0 };
		Point currentBottom{ current.x, window.getSize().y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, grey);

		window.draw(currentLine);
	}

	//drawing gridlines on the negative y-axis (positive in the code because the pixelsarray is top-down)
	for (int i = 1; i / pointPrecision <= -yinterval[0]; i++)
	{
		//making the point where the grid intersects with the axis
		Point current{ center.x, center.y + pixelsY * i / pointPrecision };
		//making the line by extruding it to the top and bottom
		Point currentTop{ 0, current.y };
		Point currentBottom{ window.getSize().x, current.y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, grey);

		window.draw(currentLine);
	}

	//drawing gridlines on the positive y-axis
	for (int i = 1; i / pointPrecision <= yinterval[1]; i++)
	{
		//making the point where the grid intersects with the axis
		Point current{ center.x, center.y + pixelsY * -i / pointPrecision };
		//making the line by extruding it to the top and bottom
		Point currentTop{ 0, current.y };
		Point currentBottom{ window.getSize().x, current.y };
		sf::VertexArray currentLine = graphics::line(currentTop, currentBottom, grey);

		window.draw(currentLine);
	}

}

void CoordinateSystem::drawLine(const sf::VertexArray & line)
{
	Point a, b;
	//convert vertex to the correct coordinates
	a = convertCoordinates({ line[0].position.x, line[0].position.y });
	b = convertCoordinates({ line[1].position.x, line[1].position.y });

	if ((a.x < 0 && b.x < 0) || (a.x > window.getSize().x && b.x > window.getSize().x) || (a.y < 0 && b.y < 0) || (a.y > window.getSize().y && b.y > window.getSize().y))
		return;

	sf::VertexArray convLine = graphics::line(a, b, line[1].color);

	window.draw(convLine);
}
void CoordinateSystem::drawLines(const std::vector<sf::VertexArray> & lines)
{
	for (auto current : lines)
	{
		drawLine(current);
	}
}


// at bit unstable if you zoom in too far, crashes if too far to the left
void CoordinateSystem::drawPolynomial(regression::Polynomial & pol)
{
	int pieceIndex = 0;

	std::vector<Point> points;
	points.reserve(window.getSize().x + 1);

	double dist = xIntervalSize() / window.getSize().x;
	for (double i = 0; i * dist <= xIntervalSize(); i++)
	{
		double x = xinterval[0] + dist * i;
		double y = pol(x);

			Point p{ x, y };

			points.push_back(p);

	}



	std::vector<sf::VertexArray> lines = graphics::LinesBetweenPoints(points, sf::Color::Red);

	drawLines(lines);

}

void CoordinateSystem::moveTool(sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (!moveToolActive)
		{
			moveToolActive = true;

			currentPixels = { (double)sf::Mouse::getPosition(window).x, (double)sf::Mouse::getPosition(window).y };

			currentXInterval = xinterval;
			currentYInterval = yinterval;

			fixedPixelsperX = pixelsPerX();
			fixedPixelsperY = pixelsPerY();

			fixedCenter = center;
		}
	}
		
	if (moveToolActive)
	{
		if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		{
			moveToolActive = false;
			return;
		}
		newPixels = { (double)sf::Mouse::getPosition(window).x, (double)sf::Mouse::getPosition(window).y };

		pixelDifference = newPixels - currentPixels;

		difference = { pixelDifference.x / fixedPixelsperX, pixelDifference.y / fixedPixelsperY };

		newXInterval = { currentXInterval[0] - difference.x, currentXInterval[1] - difference.x };
		newYInterval = { currentYInterval[0] + difference.y, currentYInterval[1] + difference.y };

		changeXinterval(newXInterval);
		changeYinterval(newYInterval);

	}

}

void CoordinateSystem::zoomTool(sf::Event & event)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		//std::cout << "wheel movement: " << event.mouseWheel.delta << std::endl;

		changeXinterval({ xinterval[0] - (xIntervalSize() * zoomFactor * event.mouseWheel.delta), xinterval[1] + (xIntervalSize() * zoomFactor * event.mouseWheel.delta)});
		changeYinterval({ yinterval[0] - (yIntervalSize() * zoomFactor * event.mouseWheel.delta), yinterval[1] + (yIntervalSize() * zoomFactor * event.mouseWheel.delta)});

		//not working yet - trying to make the mouse position the center of the zoom
		//double pointsX = ((double)event.mouseWheel.x - (double)window.getSize().x / 2) / pixelsPerX();
		//double pointsY = (-(double)event.mouseWheel.y + (double)window.getSize().y / 2) / pixelsPerY();
		//
		//changeXinterval({ xinterval[0] + pointsX, xinterval[1] + pointsX });
		//changeYinterval({ yinterval[0] + pointsY, yinterval[1] + pointsY });

		setGridPrecision();

	}
}



void CoordinateSystem::setGridPrecision()
{

	for (int i = -100; i < 100; i++)
	{
		//if intervalsize is some double of 10, or half, quarter and so on, of 10, we increase or decrease the gridprecision
		if (xIntervalSize() >= 10.0 * pow(2,i) )
		{
			//std::cout << i << " " << xIntervalSize() << std::endl;
			pointPrecision = pow(2,-i);
		}
	}

}

//function that displays a text next to any point
void CoordinateSystem::displayPointName(Point a, std::string name)
{
	int characterSize = 10;

	sf::Text pointName;
	pointName.setFont(font);
	pointName.setFillColor(sf::Color::Black);
	pointName.setCharacterSize(characterSize);
	pointName.setString(name);
	pointName.setOrigin(0, characterSize);

	Point conPoint = convertCoordinates(a);

	int pointNameX = conPoint.x + 5;
	int pointNameY = conPoint.y - 5;

	pointName.setPosition(pointNameX, pointNameY);

	window.draw(pointName);

}


