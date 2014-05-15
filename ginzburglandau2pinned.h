#ifndef GinzburgLandau2Pinned_H
#define GinzburgLandau2Pinned_H

#include "functional.h"

class GinzburgLandau2Pinned : public Functional
{
public:
    GinzburgLandau2Pinned();

    virtual double f(const gsl_vector* x, SpinCuboid& array);
    virtual void df(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);
    virtual double fdf(const gsl_vector* x, SpinCuboid& array, gsl_vector* g);

    void setB(double Bx, double By, double Bz) { m_B[0] = Bx; m_B[1] = By; m_B[2] = Bz; }
    void setR0(double r0) { m_r0 = r0; }
    void setJ(double j) { m_J = j; }
    void setD(double d) { m_D = d; }
    void setPos1(int x, int y, int z) { m_pos1[0] = x;
                                       m_pos1[1] = y;
                                       m_pos1[2] = z; }
    void setPos2(int x, int y, int z) { m_pos2[0] = x;
                                       m_pos2[1] = y;
                                       m_pos2[2] = z; }
    void setWeight(double w) { m_weight = w; }
    double getPinningEnergy(const SpinCuboid& s);

    static void Register();
private:
    static bool s_registered;
    double m_r0;
    double m_B[3];
    int m_pos1[3];
    int m_pos2[3];
    int m_weight;
    double m_J;
    double m_D;
};

#endif // GinzburgLandau2Pinned_H
