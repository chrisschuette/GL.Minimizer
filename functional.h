#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include "named.h"
#include "spincuboid.h"

class Functional: public Named
{
public:
    Functional(std::string name);
    virtual void df(const gsl_vector* x, SpinCuboid& array, gsl_vector* g) = 0;
    virtual double fdf(const gsl_vector* x, SpinCuboid& array, gsl_vector* g) = 0;
    virtual double f(const gsl_vector* x, SpinCuboid& array) = 0;
    double f(const SpinCuboid &spins);
    static void Register();
protected:
    static bool s_registered;
};

#endif // FUNCTIONAL_H
