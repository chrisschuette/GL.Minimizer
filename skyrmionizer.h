#ifndef SKYRMIONIZER_H
#define SKYRMIONIZER_H

#include "tool.h"
#include <eigen3/Eigen/Dense>

class Skyrmionizer : public Tool
{
public:
    Skyrmionizer();
    virtual std::string getName() const { return "Skyrmionizer"; }
    virtual void apply(SpinCuboid &spins);
    virtual void dumpInfo();
    void setPosition(int x, int y, int z) { m_x = x; m_y = y; m_z = z; }
    void setRadius(double r) { m_radius = r; }
    void setCharge(int charge);
    static void Register();
protected:
    double interpolate(double r, double R);
    Eigen::Vector3d skyrmion(double x, double y);
    Eigen::Vector3d n(double theta, double phi);

    static bool m_registered;

    int m_x;
    int m_y;
    int m_z;
    double m_radius;
    int m_charge;
};

#endif // SKYRMIONIZER_H
