#include "skyrmionlattice.h"
#include "luainterpreter.h"
#include <eigen3/Eigen/Dense>
#include <sstream>

using namespace Eigen;

bool SkyrmionLattice::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(SkyrmionLattice::Register);
void SkyrmionLattice::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Phase"))
       Phase::Register();

    if(!luaInterpreter.isClassRegistered("SkyrmionLattice")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< SkyrmionLattice, luabind::bases<Phase> >("SkyrmionLattice")
                .def(luabind::constructor<>())
        ];
        luaInterpreter.setClassRegistered("SkyrmionLattice");
    }
#endif
}

SkyrmionLattice::SkyrmionLattice() : Phase()
{
}

void SkyrmionLattice::render(SpinCuboid &array) const {
    int Nx = array.getDimension(0);
    int Ny = array.getDimension(1);
    int Nz = array.getDimension(2);

    int centerX = Nx/2;
    int centerY = Ny/2;
    int centerZ = Nz/2;



    Vector3d Q1(m_qx,m_qy,m_qz);
    Vector3d Q2 = AngleAxisd(2.0 * M_PI/ 3.0,  Vector3d::UnitZ() ) * Q1;
    Vector3d Q3 = AngleAxisd(4.0 * M_PI/ 3.0,  Vector3d::UnitZ() ) * Q1;

    for( int x = 0; x < Nx; x++ ) {
            for( int y = 0; y < Ny; y++) {
                for( int z = 0; z < Nz; z++) {
                    Vector3d r = Vector3d(x-centerX,y-centerY,z-centerZ);
                    Vector3d v1 = n( M_PI/2.0, Q1.dot(r) - M_PI );
                    Vector3d v2 = n( M_PI/2.0, Q2.dot(r) - M_PI );
                    Vector3d v3 = n( M_PI/2.0, Q3.dot(r) - M_PI );
                    Vector3d s = Vector3d(0,0,0);
                    s += AngleAxisd(atan2(Q1[1],Q1[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0,  Vector3d::UnitY() ) * v1;
                    s += AngleAxisd(atan2(Q2[1],Q2[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0,  Vector3d::UnitY() ) * v2;
                    s += AngleAxisd(atan2(Q3[1],Q3[0]),Vector3d::UnitZ()) * AngleAxisd(M_PI/2.0,  Vector3d::UnitY() ) * v3;
                    array.set4(x,y,z,0,-s[0]);
                    array.set4(x,y,z,1,-s[1]);
                    array.set4(x,y,z,2,-s[2]);
                }
            }
        }
}

std::string SkyrmionLattice::toString() const {
    std::stringstream ss;
    ss << " a = " << m_a << std::endl;
    ss << " m = (" << m_Mx << ", " << m_My << ", " << m_Mz << ")";
    return ss.str();
}

