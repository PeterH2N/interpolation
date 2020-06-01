#include "regression.h"

namespace regression
{
Linear::Linear(Point a, Point b)
{
    if(!(a.x < b.x)) throw std::runtime_error("X-values should be in ascending order.");
    this->a = a;
    this->b = b;
}

double Linear::returnFunc(double x_in)
{
    return (a.y * ((x_in - b.x) / (a.x - b.x)) + b.y * ((x_in - a.x) / (b.x - a.x)));
}

double Linear::operator() (double x_in)
{
    return returnFunc(x_in);
}

Polynomial::Polynomial(std::vector<Point> points)
{
	if (!std::is_sorted(points.begin(), points.end())) throw std::runtime_error("X-values should be in ascending order.");

    this->points = points;
}

double Polynomial::returnFunc(double x_in)
{
    double returnValue = 0;

    for (size_t i = 0; i < points.size(); i++)
    {
        //start lambda that makes a Lagrangepolynomium for the point we are currently at.
        auto L = [&](double x_in){

            double num = 1; double den = 1;

            for (size_t j = 0; j < points.size(); j++)
            {
                //calculates the numerator and denominator according to the Lagrangepolynomial method.
                //Hvert led inluderes bortset fra det punkt som polynomiet laves til.

                if (i != j)
                {
                num = num * (x_in - points[j].x);

                den = den * (points[i].x - points[j].x);
                }
            }
            return (num / den);
        };
        //end of lambda

        returnValue += points[i].y * L(x_in);
    }

    return returnValue;
}

double Polynomial::operator()(double x_in)
{
    return returnFunc(x_in);
}

}
