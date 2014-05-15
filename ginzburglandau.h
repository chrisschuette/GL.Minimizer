#ifndef GINZBURGLANDAU_H
#define GINZBURGLANDAU_H

#include "functional.h"

class GinzburgLandau : public Functional
{
public:
    GinzburgLandau();

    virtual double f(const gsl_vector* x, SpinCuboid& array);
    virtual void df(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);
    virtual double fdf(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);

    void setB(double Bx, double By, double Bz) { m_B[0] = Bx; m_B[1] = By; m_B[2] = Bz; }
    void setR0(double r0) { m_r0 = r0; }
    void setJ(double j) { m_J = j; }
    void setD(double d) { m_D = d; }
    void setEnergyDensity(bool ed) { m_ed = ed; }

    static void Register();
private:
    static bool s_registered;
    double m_r0;
    double m_B[3];
    double m_J;
    double m_D;
    bool m_ed;
};

#endif // GINZBURGLANDAU_H
