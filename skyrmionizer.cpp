#include "skyrmionizer.h"
#include "luainterpreter.h"
#include <cmath>
#include <assert.h>
#include "exception.h"

bool Skyrmionizer::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Skyrmionizer::Register);
void Skyrmionizer::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Tool"))
        Tool::Register();

    if(!luaInterpreter.isClassRegistered("Skyrmionizer")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Skyrmionizer, luabind::bases<Tool> >("Skyrmionizer")
                .def(luabind::constructor<>())
                .def("setPosition", &Skyrmionizer::setPosition)
                .def("setRadius", &Skyrmionizer::setRadius)
                .def("setCharge", &Skyrmionizer::setCharge)
        ];
        luaInterpreter.setClassRegistered("Skyrmionizer");
    }
#endif
}

Skyrmionizer::Skyrmionizer() : Tool("Skyrmionizer"), m_x(0), m_y(0), m_z(0), m_radius(1.0), m_charge(1)
{
}

double Skyrmionizer::interpolate(double r, double R) {
    return -atan(5.0 / R * (r - R)) / M_PI + 0.5;
}

void Skyrmionizer::setCharge(int charge) {
    if(charge > 0)
        m_charge = 1;
    else if(charge < 0)
        m_charge = -1;
    else
        ERROR("Invalid charge.")
}

Eigen::Vector3d Skyrmionizer::skyrmion(double x, double y) {
    //out << x << " " << y << " " << std::min((x*x + y*y) / m_radius * m_radius,1.0) << eom;
    if(m_charge == 1)
        return n(M_PI * (1.0 - std::min((x*x + y*y) / (m_radius * m_radius),1.0)), atan2(y,x) + M_PI / 2.0f );
    else if(m_charge == -1)
        return n(M_PI * std::min((x*x + y*y) / (m_radius * m_radius),1.0), atan2(y,x) + M_PI / 2.0f );
}

Eigen::Vector3d Skyrmionizer::n(double theta, double phi) {
    Eigen::Vector3d v;
    v[0] = sin(theta) * cos(phi);
    v[1] = sin(theta) * sin(phi);
    v[2] = cos(theta);
    return v;
}

void Skyrmionizer::apply(SpinCuboid &spins) {
    assert(m_z < spins.getDimension(2));
    for(int x = -2*m_radius; x <= 2*m_radius; x++) {
        for(int y = -2*m_radius; y <= 2*m_radius; y++) { // is this enough?
            int currentX = m_x + x;
            int currentY = m_y + y;

            //check if this is a valid coordinate
            if( (x * x + y * y < 2*m_radius * 2*m_radius) &&
                (currentX >= 0) &&
                (currentY >= 0) &&
                (currentY < spins.getDimension(1)) &&
                (currentX < spins.getDimension(0)) ) {
                //ok it is
                float Sx = spins.get4(currentX, currentY, m_z, 0);
                float Sy = spins.get4(currentX, currentY, m_z, 1);
                float Sz = spins.get4(currentX, currentY, m_z, 2);

                //calculate theta and phi
                float length = sqrt( Sx*Sx + Sy * Sy + Sz * Sz );
                float phi = atan2(Sy,Sx);
                float theta = acos(Sz/length);
                double distanceToCenter = sqrt((float) (pow(currentX - m_x,2)
                                          + pow(currentY - m_y,2)));

                double ff = interpolate(distanceToCenter, 1.1 * m_radius);

                Eigen::Vector3d sky = skyrmion(x,y);
                Eigen::Vector3d current = Eigen::Vector3d(Sx, Sy, Sz);
                Eigen::Vector3d result = (1.0 - ff) * current + ff * sky;

                spins.set4(currentX, currentY, m_z, 0, result[0]);
                spins.set4(currentX, currentY, m_z, 1, result[1]);
                spins.set4(currentX, currentY, m_z, 2, result[2]);
            }
        }
    }
}

void Skyrmionizer::dumpInfo() {
    std::cout << "position: x = " << m_x << ", y = " << m_y << ", z = " << m_z << std::endl;
    std::cout << "radius = " << m_radius << std::endl;
}
