#include "interpolated.h"
#include "luainterpreter.h"

using namespace Eigen;

bool Interpolated::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Interpolated::Register);
void Interpolated::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Phase"))
       Phase::Register();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
       SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("Interpolated")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Interpolated, luabind::bases<Phase> >("Interpolated")
                .def(luabind::constructor<const SpinCuboid&, const SpinCuboid&>())
                .def("setPosition", &Interpolated::setPosition)
                .def("setWidth", &Interpolated::setWidth)
        ];
        luaInterpreter.setClassRegistered("Interpolated");
    }
#endif
}
using namespace Eigen;

Interpolated::Interpolated(const SpinCuboid &top, const SpinCuboid &bottom) : Phase(), m_top(top), m_bottom(bottom)
{
}

void Interpolated::render(SpinCuboid &array) const {


    int Nx = array.getDimension(0);
    int Ny = array.getDimension(1);
    int Nz = array.getDimension(2);

    int centerX = Nx/2;
    int centerY = Ny/2;
    int centerZ = Nz/2;

    Vector3d Q(m_qx,m_qy,m_qz);

    for( int x = 0; x < Nx; x++ ) {
            for( int y = 0; y < Ny; y++) {
                for( int z = 0; z < Nz; z++) {
                    Vector3d v = transition(x, y, z);
                    array.set4(x,y,z,0,v[0]);
                    array.set4(x,y,z,1,v[1]);
                    array.set4(x,y,z,2,v[2]);
                }
            }
        }
}

std::string Interpolated::toString() const {

}

Vector3d Interpolated::transition(int x, int y, int z) const {

    double x1;
    double x2;

    if(z < m_position - m_width) {
        x1 = 1.0;
        x2 = 0.0;
    } else if (z > m_position + m_width) {
        x1 = 0.0;
        x2 = 1.0;
    } else {
        double dz = z - (m_position - m_width);
        double a = 1.0 / ( 2.0 * m_width);
        x2 = dz * a;
        x1 = 1.0 - x2;
    }
    return  x1 * Eigen::Vector3d(m_bottom.get4(x,y,0,0),m_bottom.get4(x,y,0,1),m_bottom.get4(x,y,0,2)) +  x2 * Eigen::Vector3d(m_top.get4(x,y,0,0),m_top.get4(x,y,0,1),m_top.get4(x,y,0,2));
}
