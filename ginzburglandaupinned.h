#ifndef GinzburgLandauPinned_H
#define GinzburgLandauPinned_H

#include "functional.h"

class GinzburgLandauPinned : public Functional
{
public:
    GinzburgLandauPinned();

    virtual double f(const gsl_vector* x, SpinCuboid& array);
    virtual void df(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);
    virtual double fdf(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);

    void setB(double Bx, double By, double Bz) { m_B[0] = Bx; m_B[1] = By; m_B[2] = Bz; }
    void setR0(double r0) { m_r0 = r0; }
    void setJ(double j) { m_J = j; }
    void setD(double d) { m_D = d; }
    void setPos(int x, int y, int z) { m_pos[0] = x;
                                       m_pos[1] = y;
                                       m_pos[2] = z; }
    void setWeight(double w) { m_weight = w; }
    double getPinningEnergy(const SpinCuboid& s);

    static void Register();
private:
    static bool s_registered;
    double m_r0;
    double m_B[3];
    int m_pos[3];
    int m_weight;
    double m_J;
    double m_D;
};

#endif // GinzburgLandauPinned_H
