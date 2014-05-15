#ifndef PHASE_H
#define PHASE_H

#include <cmath>
#include "spincuboid.h"
#include <eigen3/Eigen/Dense>

class Phase
{
public:
    Phase();

    //Named
    virtual std::string getName() const { return "Phase"; }
    virtual void render(SpinCuboid& array) const = 0;

    void setA(double a);
    double getA() { return m_a; }
    void setM(double Mx, double My, double Mz);
    void setNx(double Nx);
    void setNy(double Ny);
    void setNz(double Nz);
    double getNx();
    double getNy();
    double getNz();
    double getQx() { return m_qx; }
    double getQy() { return m_qy; }
    double getQz() { return m_qz; }
    void dumpInfo();
    static void Register();
protected:
    Eigen::Vector3d n(double theta, double phi) const;
    static bool m_registered;
    double m_a;
    double m_Mx;
    double m_My;
    double m_Mz;
    double m_qx;
    double m_qy;
    double m_qz;
};

#endif // PHASE_H
