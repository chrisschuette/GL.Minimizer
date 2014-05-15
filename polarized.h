#ifndef POLARIZED_H
#define POLARIZED_H

#include "spincuboid.h"
#include "phase.h"

class Polarized: public Phase
{
public:
    Polarized();
    virtual void render(SpinCuboid& array) const;
    void setDirection(double mx, double my, double mz) {
        m_mx=mx; m_my=my; m_mz=mz;
    }

    static void Register();
protected:
    static bool m_registered;
    double m_mx;
    double m_my;
    double m_mz;
};

#endif // POLARIZED_H
