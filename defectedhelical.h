#ifndef DEFECTEDHELICAL_H
#define DEFECTEDHELICAL_H

#include "phase.h"
#include "eigen3/Eigen/Dense"

class DefectedHelical : public Phase
{
public:
    DefectedHelical();
    //Named
    virtual std::string getName() const { return "Helical"; }

    //Printable
    virtual std::string toString() const;
    virtual void render(SpinCuboid& array) const;

    void setWidth(double w) { m_width = w; }

    static void Register();
protected:
    double sigmoid(double arg, double width, double center = 0.0) const;
    double sign(double arg) const;
    Eigen::Vector3d defectConical(double width, int x, int y, int z, Eigen::Vector3d Q) const;
    Eigen::Vector3d defectConical0(double width, int x, int y, Eigen::Vector3d Q) const;
    Eigen::Vector3d helical(int x, int y, int z, Eigen::Vector3d Q) const;
    static bool m_registered;
    double m_width;

};

#endif // DEFECTEDHELICAL_H
