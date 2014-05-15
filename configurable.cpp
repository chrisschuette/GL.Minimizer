#include "configurable.h"
#include "luainterpreter.h"

bool Configurable::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Configurable::Register);
void Configurable::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Configurable")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Configurable >("Configurable")
                .def("configure", &Configurable::configure)
        ];
        luaInterpreter.setClassRegistered("Configurable");

    }
#endif
}

Configurable::Configurable(std::string name) : Named(name)
{
}

std::string Configurable::makePath(std::string element) const {
    if(getName() != "")
        return getName() + "." + element;
    else
        return element;
}
