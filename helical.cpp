#include "helical.h"
#include "luainterpreter.h"
#include <eigen3/Eigen/Dense>
#include <sstream>

using namespace Eigen;

bool Helical::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Helical::Register);
void Helical::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Phase"))
       Phase::Register();

    if(!luaInterpreter.isClassRegistered("Helical")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Helical, luabind::bases<Phase> >("Helical")
                .def(luabind::constructor<>())
        ];
        luaInterpreter.setClassRegistered("Helical");
}
#endif
}

Helical::Helical() : Phase()
{
}

void Helical::render(SpinCuboid &array) const {
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
                    Vector3d r = Vector3d(x-centerX,y-centerY,z-centerZ);
                    Vector3d v = n( M_PI/2.0, Q.dot(r) - M_PI );
                    Vector3d s = AngleAxisd(atan2(Q[1],Q[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0,  Vector3d::UnitY() ) * v;
                    array.set4(x,y,z,0,s[0]);
                    array.set4(x,y,z,1,s[1]);
                    array.set4(x,y,z,2,s[2]);
                }
            }
        }
}

std::string Helical::toString() const {
    std::stringstream ss;
    ss << " a = " << m_a << std::endl;
    ss << " m = (" << m_Mx << ", " << m_My << ", " << m_Mz << ")";
    return ss.str();
}

