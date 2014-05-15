#include "tool.h"
#include "luainterpreter.h"

bool Tool::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Tool::Register);
void Tool::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("Tool")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Tool >("Tool")
                .def("apply", &Tool::apply)
                .def("dumpInfo", &Tool::dumpInfo)
        ];
        luaInterpreter.setClassRegistered("Tool");
    }
#endif
}

Tool::Tool(std::string name) : Named(name)
{
}
