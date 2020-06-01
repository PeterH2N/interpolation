#define _AA 1
#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <regression.h>

using std::vector;

namespace interpolation
{


class Polynomial
{
public:

	Polynomial(regression::Polynomial graph, double numelems, std::vector<double> interval);

    vector<Point> points;

	vector<double> interval;

private:

    double numElems;

    void makePoints(regression::Polynomial graph);

};

}

#endif // INTERPOLATION_H
