#pragma once
#include <interpolation.h>

class CoordinateSystem
{
public:
	CoordinateSystem(sf::RenderWindow& window, std::vector<double> xinterval, std::vector<double> yinterval, bool viewGrid);

	void draw();

	void drawLine(const sf::VertexArray & line);

	void drawLines(const std::vector<sf::VertexArray> & lines);

	void drawPolynomial(regression::Polynomial & pol);

	void moveTool(sf::Event & event);

	void zoomTool(sf::Event & event);

	std::vector<regression::Polynomial> polynomials;

	std::vector<interpolation::Polynomial> interpolated_polynomials;

	sf::RenderWindow& window;

	Point convertCoordinates(const Point & a);

	void displayPointName(Point a, std::string name);

private:

	bool viewGrid;
	//int that signifies how many point markers should be visible on the axes
	double pointPrecision;

	std::vector<double> xinterval;

	std::vector<double> yinterval;

	Point center;

	void convertCenter();

	Point convertPixels(const Point & a);

	sf::Font font;

	void drawAxes();

	void drawPointMarkers();

	void drawGrid();

	void changeXinterval(std::vector<double> newinterval);

	void changeYinterval(std::vector<double> newinterval);

	void setGridPrecision();

	double xIntervalSize();

	double yIntervalSize();

	double pixelsPerX();

	double pixelsPerY();

	bool moveToolActive;

	//used for move tool//
	Point currentPixels{ 0,0 };

	std::vector<double> currentXInterval{ 0,0 };
	std::vector<double> currentYInterval{ 0,0 };

	double fixedPixelsperX = 0;
	double fixedPixelsperY = 0; 

	Point fixedCenter{ 0,0 };

	Point newPixels{ 0,0 };

	Point pixelDifference{ 0,0 };

	Point difference{ 0,0 };

	std::vector<double> newXInterval{ 0,0 };
	std::vector<double> newYInterval{ 0,0 };
	//used for move tool//
	//used for zoom tool//
	double zoomFactor = 0.05;
	//used for zoom tool//

	std::string countingDecPlaces(double x);

};

