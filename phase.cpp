#include "phase.h"
#include "luainterpreter.h"

bool Phase::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Phase::Register);
void Phase::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
       SpinCuboid::Register();
    if(!luaInterpreter.isClassRegistered("Phase")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Phase >("Phase")
                .def("getA", &Phase::getA)
                .def("setA", &Phase::setA)
                .def("setM", &Phase::setM)
                .def("getNx", &Phase::getNx)
                .def("getNy", &Phase::getNy)
                .def("getNz", &Phase::getNz)
                .def("setNx", &Phase::setNx)
                .def("setNy", &Phase::setNy)
                .def("setNz", &Phase::setNz)
                .def("getQx", &Phase::getQx)
                .def("getQy", &Phase::getQy)
                .def("getQz", &Phase::getQz)
                .def("render", &Phase::render)
                .def("dumpInfo", &Phase::dumpInfo)
        ];
        luaInterpreter.setClassRegistered("Phase");
    }
#endif
}

Phase::Phase() : m_a(1), m_Mx(0), m_My(0), m_Mz(0), m_qx(0), m_qy(0), m_qz(0)
{
}

void Phase::dumpInfo() {
    std::cout << " a = " << m_a << std::endl;
    std::cout << " mX = " << m_Mx << std::endl;
    std::cout << " mY = " << m_My << std::endl;
    std::cout << " mZ = " << m_Mz << std::endl;
    std::cout << " q = (" << m_qx << ", " << m_qy << ", " << m_qz << " )" << std::endl;
}

void Phase::setA(double a) {
    m_a = a;
    //recalculate q's
    double M = sqrt( std::pow(m_Mx,2) + std::pow(m_My,2) + std::pow(m_Mz,2) );
    if(M == 0) {
        m_qx = 0;
        m_qy = 0;
        m_qz = 0;

    }
    m_qx = atan(m_a) * m_Mx / M;
    m_qy = atan(m_a) * m_My / M;
    m_qz = atan(m_a) * m_Mz / M;

}

void Phase::setM(double Mx, double My, double Mz) {
    m_Mx = Mx;
    m_My = My;
    m_Mz = Mz;
    //recalculate q's
    double M = sqrt( std::pow(m_Mx,2) + std::pow(m_My,2) + std::pow(m_Mz,2) );
    if(M == 0) {
        m_qx = 0;
        m_qy = 0;
        m_qz = 0;

    }
    m_qx = atan(m_a) * m_Mx / M;
    m_qy = atan(m_a) * m_My / M;
    m_qz = atan(m_a) * m_Mz / M;
}

double Phase::getNx()
{
    m_qx = atan(m_a) * m_Mx / sqrt( std::pow(m_Mx,2) + std::pow(m_My,2) + std::pow(m_Mz,2) );
    if(m_qx == 0)
        return 1;
    return 2.0 * M_PI * m_Mx / m_qx;
}

double Phase::getNy()
{
    m_qy = atan(m_a) * m_My / sqrt( std::pow(m_Mx,2) + std::pow(m_My,2) + std::pow(m_Mz,2) );
    if(m_qy == 0)
        return 1;
    return 2.0 * M_PI * m_My / m_qy;
}

double Phase::getNz()
{
    m_qz = atan(m_a) * m_Mz / sqrt( std::pow(m_Mx,2) + std::pow(m_My,2) + std::pow(m_Mz,2) );
    if(m_qz == 0)
        return 1;
    return 2.0 * M_PI * m_Mz / m_qz;
}

void Phase::setNx(double Nx)
{
    m_qx = 2.0 * M_PI * m_Mx / Nx;
    double Q = sqrt( std::pow(m_qx,2) + std::pow(m_qy,2) + std::pow(m_qz,2) );
    m_a = tan(Q);
}
void Phase::setNy(double Ny)
{
    m_qy = 2.0 * M_PI * m_My / Ny;
    double Q = sqrt( std::pow(m_qx,2) + std::pow(m_qy,2) + std::pow(m_qz,2) );
    m_a = tan(Q);
}

void Phase::setNz(double Nz)
{
    m_qz = 2.0 * M_PI * m_Mz / Nz;
    double Q = sqrt( std::pow(m_qx,2) + std::pow(m_qy,2) + std::pow(m_qz,2) );
    m_a = tan(Q);
}

Eigen::Vector3d Phase::n(double theta, double phi) const {
        Eigen::Vector3d v;
        v[0] = sin(theta) * cos(phi);
        v[1] = sin(theta) * sin(phi);
        v[2] = cos(theta);
        return v;
}
