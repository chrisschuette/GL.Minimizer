#include "viewer.h"

#include "luainterpreter.h"

bool Viewer::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Viewer::Register);
void Viewer::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();


    if(!luaInterpreter.isClassRegistered("Viewer")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Viewer >("Viewer")
                .def("updateView", (void(Viewer::*)(const SpinCuboid&)) &Viewer::updateView)
                .def("updateView", (void(Viewer::*)(const SpinCuboid&, std::string)) &Viewer::updateView)
        ];
        luaInterpreter.setClassRegistered("Viewer");
    }
#endif
}

Viewer::Viewer() : Named("Viewer")
{
}
