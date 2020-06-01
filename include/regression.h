#ifndef REGRESSION_H
#define REGRESSION_H

#include <graphics.h>


namespace regression
{
class Linear
{
public:

    Linear(Point a, Point b);

    double operator() (double x_in);


private:

    Point a, b;

    double returnFunc(double x_in);

};

class Polynomial
{
public:

    Polynomial(std::vector<Point> points);

    double operator() (double x_in);

    std::vector<Point> points;

private:

    double returnFunc(double x_in);

};
}
#endif // REGRESSION_H
