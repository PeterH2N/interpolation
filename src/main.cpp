#include <animation.h>

int main()
{
	vector<Point> sample_points{ {0,0}, {5,10}, {8,0}, {10,5} };

	vector<Point> sample_points2{ {11,0}, {16,10}, {19,0} };

	vector<Point> sample_points3{ {20,0}, {22,5}, {25,2}, {27,10}, {30,0} };

	regression::Polynomial polynomial_model{ sample_points };

	regression::Polynomial polynomial_model2{ sample_points2 };

	regression::Polynomial polynomial_model3{ sample_points3 };

	interpolation::Polynomial samplePolynomial(polynomial_model, 10, { 0, 10 });

	interpolation::Polynomial samplePolynomial2(polynomial_model2, 40, { 11, 19 });
	 
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Interpolation");

	CoordinateSystem mainCS(window, { -2, 32 }, { -10, 24 }, true);

	mainCS.polynomials.push_back(polynomial_model);
	mainCS.polynomials.push_back(polynomial_model2);
	mainCS.polynomials.push_back(polynomial_model3);

	//mainCS.interpolated_polynomials.push_back(samplePolynomial);

	//mainCS.interpolated_polynomials.push_back(samplePolynomial2);

	sf::CircleShape circle(25);

	circle.setFillColor(sf::Color::Blue);

	circle.setOrigin(circle.getRadius(), circle.getRadius());

	animation circleanimation(&mainCS, circle, polynomial_model, { 0, 10 }, sf::seconds(4), 12, sf::Keyboard::Q);

	animation circleanimation2(&mainCS, circle, polynomial_model2, { 11,19 }, sf::seconds(4), 24, sf::Keyboard::W);

	animation circleanimation3(&mainCS, circle, polynomial_model3, { 20,30 }, sf::seconds(4), 60, sf::Keyboard::E);

	sf::Event mainEvent;

	mainEvent.type = sf::Event::GainedFocus;

	while (mainCS.window.isOpen())
	{	
		////////////////////////////////////////////
		mainCS.window.pollEvent(mainEvent);
		mainCS.window.clear(sf::Color::White);

		//basic coordinatesystem things
		mainCS.moveTool(mainEvent);
		mainCS.zoomTool(mainEvent);
		mainCS.draw();
		//////////////////////////////////////////
		//put stuff here
			
		circleanimation.runAnimation(mainEvent);
		mainCS.displayPointName({ 4,18 }, "Press Q, 12 fps");

		circleanimation2.runAnimation(mainEvent);
		mainCS.displayPointName({ 15,12 }, "Press W, 24 fps");

		circleanimation3.runAnimation(mainEvent);
		mainCS.displayPointName({ 22,14 }, "Press E, 60 fps");

		//////////////////////////////////////////
		mainCS.window.display();

		if (mainEvent.type == sf::Event::Closed)
			window.close();
		/////////////////////////////////////////
	}
	std::cout << "return" << std::endl;
	return 0;
}