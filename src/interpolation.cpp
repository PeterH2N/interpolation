#include <interpolation.h>



namespace interpolation
{

Polynomial::Polynomial(regression::Polynomial graph, double _numElems, std::vector<double> _interval)
	: numElems(_numElems), interval(_interval)
{

    makePoints(graph);

}

void Polynomial::makePoints(regression::Polynomial graph)
{

	double intervalsize = interval[1] - interval[0];

	double dist = intervalsize / numElems;

	//std::cout << Dx << "  " << numElems << "  " << dist << std::endl;

    for(int i = 0; i <= numElems; i++)
    {
		double x = interval[0] + dist * i;
        double y = graph(x);
		Point p{ x, y };

		//std::cout << p << "  ";

        this->points.push_back(p);
    }
}



}
