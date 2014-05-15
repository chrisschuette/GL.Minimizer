#include "polarizer.h"
#include "luainterpreter.h"
#include <cmath>
#include <assert.h>

bool Polarizer::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Polarizer::Register);
void Polarizer::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Tool"))
        Tool::Register();

    if(!luaInterpreter.isClassRegistered("Polarizer")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Polarizer, luabind::bases<Tool> >("Polarizer")
                .def(luabind::constructor<>())
                .def("setPosition", &Polarizer::setPosition)
                .def("setPolarization", &Polarizer::setPolarization)
                .def("setPolarizationSpherical", &Polarizer::setPolarizationSpherical)
                .def("setRadius", &Polarizer::setRadius)
        ];
        luaInterpreter.setClassRegistered("Polarizer");
    }
#endif
}

Polarizer::Polarizer() : Tool("Polarizer"), m_x(0), m_y(0), m_z(0), m_px(0.0), m_py(0.0), m_pz(1.0), m_radius(1.0)
{
}

void Polarizer::setPolarization(double px, double py, double pz) {
    m_px = px;
    m_py = py;
    m_pz = pz;
}

void Polarizer::setPolarizationSpherical(double rho, double theta, double phi) {
    assert(rho > 0);
    m_px = rho * std::sin(theta) * std::cos(phi);
    m_py = rho * std::sin(theta) * std::sin(phi);
    m_pz = rho * std::cos(theta);
}

void Polarizer::setRadius(double r) {
     assert(r>0); m_radius = r;
}

void Polarizer::dumpInfo() {
    std::cout << "position: x = " << m_x << ", y = " << m_y << ", z = " << m_z << std::endl;
    std::cout << "polarization: x = " << m_px << ", y = " << m_py << ", z = " << m_pz << std::endl;
    std::cout << "radius = " << m_radius << std::endl;
}

void Polarizer::apply(SpinCuboid &spins) {
    assert(m_z < spins.getDimension(2));

    for(int x = -m_radius; x <= m_radius; x++) {
        for(int y = -m_radius; y <= m_radius; y++) {
            int currentX = m_x + x;
            int currentY = m_y + y;

            //check if this is a valid coordinate
            if( (x * x + y * y <= m_radius * m_radius) &&
                (currentX >= 0) &&
                (currentY >= 0) &&
                (currentY < spins.getDimension(1)) &&
                (currentX < spins.getDimension(0)) ) {
                //ok it is
                float Sx = spins.get4(currentX, currentY, m_z, 0);
                float Sy = spins.get4(currentX, currentY, m_z, 1);
                float Sz = spins.get4(currentX, currentY, m_z, 2);

                double distanceToCenter = sqrt((float) (pow(currentX - m_x,2)
                                          + pow(currentY - m_y,2)));

                float strength = std::min(std::max( 1.0 - distanceToCenter / m_radius, 0.0 ), 1.0);
                spins.set4( currentX, currentY, m_z, 0, strength * m_px + (1.0 - strength) * Sx);
                spins.set4( currentX, currentY, m_z, 1, strength * m_py + (1.0 - strength) * Sy);
                spins.set4( currentX, currentY, m_z, 2, strength * m_pz + (1.0 - strength) * Sz);
            }
        }
    }
}
