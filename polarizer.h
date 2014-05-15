#ifndef POLARIZER_H
#define POLARIZER_H

#include "tool.h"

class Polarizer : public Tool
{
public:
    Polarizer();
    virtual std::string getName() const { return "Polarizer"; }
    virtual void apply(SpinCuboid &spins);

    void setPolarization(double px, double py, double pz);
    void setPolarizationSpherical(double rho, double theta, double phi);

    void setRadius(double r);
    void setPosition(int x, int y, int z) { m_x = x; m_y = y; m_z = z; }

    virtual void dumpInfo();

    static void Register();
protected:
    static bool m_registered;
    int m_x;
    int m_y;
    int m_z;
    double m_radius;

    double m_px;
    double m_py;
    double m_pz;
};

#endif // POLARIZER_H
