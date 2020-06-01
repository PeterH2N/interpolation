#pragma once

#include <SFML/Graphics.hpp>
#include <point.h>


namespace graphics
{
	static sf::VertexArray line(Point a, Point b, sf::Color color)
	{
		sf::VertexArray returnLine(sf::Lines, 2);

		sf::Vertex ver1(sf::Vector2f(a.x, a.y));
		sf::Vertex ver2(sf::Vector2f(b.x, b.y));

		returnLine[0] = ver1;
		returnLine[1] = ver2;

		returnLine[0].color = color;
		returnLine[1].color = color;

		return returnLine;
	}


	static std::vector<sf::VertexArray> LinesBetweenPoints(std::vector<Point> & points, sf::Color color)
	{
		std::vector<sf::VertexArray> returnVector;

		for (size_t i = 0; i < points.size() - 1; i++)
		{

			sf::VertexArray current = line(points[i], points[i + 1], color);
			returnVector.push_back(current);
		}
		return returnVector;
	}


}
