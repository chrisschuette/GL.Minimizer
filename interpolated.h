#ifndef INTERPOLATED_H
#define INTERPOLATED_H

#include "phase.h"

#include <eigen3/Eigen/Dense>

class Interpolated : public Phase
{
public:
    Interpolated(const SpinCuboid& top, const SpinCuboid& bottom);
    virtual std::string getName() const { return "Interpolated"; }
    virtual void render(SpinCuboid& array) const;
    std::string toString() const;

    //getters and setters
    int getPosition() { return m_position; }
    void setPosition(int position) { m_position = position; }
    int getWidth() { return m_width; }
    void setWidth(int width) { m_width = width; }

    static void Register();
protected:
    static bool m_registered;

    Eigen::Vector3d transition(int x, int y, int z) const;

    int m_position;
    int m_width;
    const SpinCuboid& m_top;
    const SpinCuboid& m_bottom;
};

#endif // INTERPOLATED_H
