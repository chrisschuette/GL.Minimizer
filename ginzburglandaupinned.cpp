#include "ginzburglandaupinned.h"
#include "luainterpreter.h"
#include <cmath>

#define Mx(x, y, z) array.get4(x,y,z,0)
#define My(x, y, z) array.get4(x,y,z,1)
#define Mz(x, y, z) array.get4(x,y,z,2)

bool GinzburgLandauPinned::s_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(GinzburgLandauPinned::Register);

void GinzburgLandauPinned::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Functional"))
        Functional::Register();

    if(!luaInterpreter.isClassRegistered("GinzburgLandauPinned")) {
        luabind::module(luaInterpreter.getL())
                [
                luabind::class_< GinzburgLandauPinned, luabind::bases<Functional> >("GinzburgLandauPinned")
                .def(luabind::constructor<>())
                .def("setB", &GinzburgLandauPinned::setB)
                .def("setD", &GinzburgLandauPinned::setD)
                .def("setJ", &GinzburgLandauPinned::setJ)
                .def("setR0", &GinzburgLandauPinned::setR0)
                .def("setWeight", &GinzburgLandauPinned::setWeight)
                .def("setPosition", &GinzburgLandauPinned::setPos)
                .def("getPinningEnergy", &GinzburgLandauPinned::getPinningEnergy)
                ];
        luaInterpreter.setClassRegistered("GinzburgLandauPinned");
    }
#endif
}

GinzburgLandauPinned::GinzburgLandauPinned() : Functional("GinzburgLandauPinned")
{
    m_J = 1;
    m_D = 0.5;
    m_r0 = 2;
    m_B[0] = 0;
    m_B[1] = 0;
    m_B[2] = 0;
    m_pos[0] = 0;
    m_pos[1] = 0;
    m_pos[2] = 0;
}

double GinzburgLandauPinned::f(const gsl_vector* arg, SpinCuboid& array) {
    gsl_vector* temp = array.getData();
    array.setData(const_cast< gsl_vector* >(arg));

    double F = 0.0;
    for(int x = 0; x < array.getDimension(0); x++)
        for(int y = 0; y < array.getDimension(1); y++)
            for(int z = 0; z < array.getDimension(2); z++) {
                // is this an inner voxel?
                if(array.get4Index(x,y,z,0) >= 0 ) {

                    double Mx = Mx(x,y,z);
                    double My = My(x,y,z);
                    double Mz = Mz(x,y,z);

                    double Mxx = Mx(x+1,y,z);
                    double Mxy = My(x+1,y,z);
                    double Mxz = Mz(x+1,y,z);

                    double Myx = Mx(x,y+1,z);
                    double Myy = My(x,y+1,z);
                    double Myz = Mz(x,y+1,z);

                    double Mzx = Mx(x,y,z+1);
                    double Mzy = My(x,y,z+1);
                    double Mzz = Mz(x,y,z+1);

                    double Mmxx = Mx(x-1,y,z);
                    double Mmxy = My(x-1,y,z);
                    double Mmxz = Mz(x-1,y,z);

                    double Mmyx = Mx(x,y-1,z);
                    double Mmyy = My(x,y-1,z);
                    double Mmyz = Mz(x,y-1,z);

                    double Mmzx = Mx(x,y,z-1);
                    double Mmzy = My(x,y,z-1);
                    double Mmzz = Mz(x,y,z-1);

                    double M2 = Mx * Mx + My * My + Mz * Mz;

                    F += M2*M2 + ( 3 * m_J - m_r0 ) * M2
                            - m_J/2.0 * ((Mxx+Mmxx) * Mx
                                   + (Myx+Mmyx) * Mx
                                   + (Mzx+Mmzx) * Mx
                                   + (Mxy+Mmxy) * My
                                   + (Myy+Mmyy) * My
                                   + (Mzy+Mmzy) * My
                                   + (Mxz+Mmxz) * Mz
                                   + (Myz+Mmyz) * Mz
                                   + (Mzz+Mmzz) * Mz)
                            - (m_B[0] * Mx
                               +m_B[1] * My
                               +m_B[2] * Mz)
                            -m_D * ( ( My * Mxz - Mz * Mxy ) + ( Mz * Myx - Mx * Myz ) + ( Mx * Mzy - My * Mzx ) );
                } else
                    std::cout << "bad voxel" << std::endl;
            }

    double Mx = Mx(m_pos[0],m_pos[1],m_pos[2]);
    double My = My(m_pos[0],m_pos[1],m_pos[2]);
    double Mz = Mz(m_pos[0],m_pos[1],m_pos[2]);
    double M2 = Mx * Mx + My * My + Mz * Mz;

    F += m_weight * M2;

    array.setData(temp);
    return F;
}

double GinzburgLandauPinned::getPinningEnergy(const SpinCuboid& s) {
    double Mx = s.get4(m_pos[0],m_pos[1],m_pos[2],0);
    double My = s.get4(m_pos[0],m_pos[1],m_pos[2],1);
    double Mz = s.get4(m_pos[0],m_pos[1],m_pos[2],2);
    double M2 = Mx * Mx + My * My + Mz * Mz;
    return m_weight * M2;
}


void GinzburgLandauPinned::df(const gsl_vector* arg, SpinCuboid& array, gsl_vector* g) {
    gsl_vector* temp = array.getData();
    array.setData(const_cast< gsl_vector* >(arg));

    for(int x = 0; x < array.getDimension(0); x++)
        for(int y = 0; y < array.getDimension(1); y++)
            for(int z = 0; z < array.getDimension(2); z++) {
                if(array.get4Index(x,y,z,0) >= 0 ) {

                    double Mx = Mx(x,y,z);
                    double My = My(x,y,z);
                    double Mz = Mz(x,y,z);

                    double Mxx = Mx(x+1,y,z);
                    double Mxy = My(x+1,y,z);
                    double Mxz = Mz(x+1,y,z);

                    double Myx = Mx(x,y+1,z);
                    double Myy = My(x,y+1,z);
                    double Myz = Mz(x,y+1,z);

                    double Mzx = Mx(x,y,z+1);
                    double Mzy = My(x,y,z+1);
                    double Mzz = Mz(x,y,z+1);

                    double Mmxx = Mx(x-1,y,z);
                    double Mmxy = My(x-1,y,z);
                    double Mmxz = Mz(x-1,y,z);

                    double Mmyx = Mx(x,y-1,z);
                    double Mmyy = My(x,y-1,z);
                    double Mmyz = Mz(x,y-1,z);

                    double Mmzx = Mx(x,y,z-1);
                    double Mmzy = My(x,y,z-1);
                    double Mmzz = Mz(x,y,z-1);

                    double M2 = Mx * Mx + My * My + Mz * Mz;

                    gsl_vector_set(g, array.get4Index(x,y,z,0), 4.0 * Mx * M2 + (6 * m_J - 2 * m_r0) * Mx - m_B[0]
                                   - m_J * (Mxx + Myx + Mmyx + Mmxx + Mzx + Mmzx)
                                   - m_D * (Mmyz - Myz  + Mzy - Mmzy)
                                   );
                    gsl_vector_set(g, array.get4Index(x,y,z,1), 4.0 * My * M2 + (6 * m_J - 2 * m_r0) * My - m_B[1]
                                   - m_J * (Mxy + Myy + Mmyy + Mmxy + Mzy + Mmzy)
                                   - m_D * (Mxz - Mmxz + Mmzx - Mzx)
                                   );
                    gsl_vector_set(g, array.get4Index(x,y,z,2), 4.0 * Mz * M2 + (6 * m_J - 2 * m_r0) * Mz - m_B[2]
                                   - m_J * (Mxz + Myz + Mmyz + Mmxz + Mzz + Mmzz)
                                   - m_D * (Mmxy - Mxy + Myx - Mmyx)
                                   );

                }
            }

    double Mx = Mx(m_pos[0],m_pos[1],m_pos[2]);
    double My = My(m_pos[0],m_pos[1],m_pos[2]);
    double Mz = Mz(m_pos[0],m_pos[1],m_pos[2]);

    gsl_vector_set(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],0), gsl_vector_get(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],0)) + 2.0 * m_weight * Mx);
    gsl_vector_set(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],1), gsl_vector_get(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],1)) + 2.0 * m_weight * My);
    gsl_vector_set(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],2), gsl_vector_get(g, array.get4Index(m_pos[0],m_pos[1],m_pos[2],2)) + 2.0 * m_weight * Mz);

    array.setData(temp);
}

double GinzburgLandauPinned::fdf(const gsl_vector* x, SpinCuboid& array, gsl_vector* g) {
    df(x, array, g);
    return f(x,array);
}
