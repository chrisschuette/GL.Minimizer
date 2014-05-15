#include "functional.h"
#include "luainterpreter.h"

bool Functional::s_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Functional::Register);
void Functional::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("Functional")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Functional >("Functional")
        ];
        luaInterpreter.setClassRegistered("Functional");
    }
#endif
}

Functional::Functional(std::string name) : Named(name)
{
}

double Functional::f(const SpinCuboid& spins) {
    return f(spins.getData(), const_cast<SpinCuboid&>(spins));
}
