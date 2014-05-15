#ifndef NOISE_H
#define NOISE_H

#include "named.h"
#include "spincuboid.h"
//random number generation
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

class Noise : public Named
{
public:
    Noise();
    static void Register();
    void setSigma(double s);
    void apply(SpinCuboid& s);
protected:
    static bool m_registered;
    // random number generation
    double m_sigma;
    boost::mt19937 m_rng;
    boost::variate_generator<boost::mt19937*, boost::normal_distribution<> > m_var_nor;
};

#endif // NOISE_H
