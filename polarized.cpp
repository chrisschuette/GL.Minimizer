#include "polarized.h"
#include "luainterpreter.h"

bool Polarized::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Polarized::Register);
void Polarized::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Phase"))
       Phase::Register();
    if(!luaInterpreter.isClassRegistered("Polarized")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Polarized, luabind::bases<Phase>  >("Polarized")
                .def(luabind::constructor<>())
        ];
        luaInterpreter.setClassRegistered("Polarized");
    }
#endif
}

Polarized::Polarized() : Phase()
    , m_mx(0)
    , m_my(0)
    , m_mz(1)
{
}

void Polarized::render(SpinCuboid &array) const {
    int Nx = array.getDimension(0);
    int Ny = array.getDimension(1);
    int Nz = array.getDimension(2);

    for( int x = 0; x < Nx; x++ ) {
            for( int y = 0; y < Ny; y++) {
                for( int z = 0; z < Nz; z++) {
                    array.set4(x,y,z,0,m_mx);
                    array.set4(x,y,z,1,m_my);
                    array.set4(x,y,z,2,m_mz);
                }
            }
    }
}
