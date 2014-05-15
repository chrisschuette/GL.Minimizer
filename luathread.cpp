#include "luathread.h"

LuaThread::LuaThread(scripting::LuaInterpreter& luaInterpreter, Viewer& viewer, QObject *parent) :
    QThread(parent),
  m_interpreter(luaInterpreter),
    m_viewer(viewer)
{
}

void LuaThread::run() {
    try {
    m_interpreter.init();
    m_interpreter.exportViewer(m_viewer);
    m_interpreter.execute(m_arguments);
    } catch (luabind::error& e) { //repackage any errors
        m_interpreter.stackTrace();
        std::string error = lua_tostring(e.state(), -1);
        std::cerr << error << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    }

void LuaThread::execute(std::vector<std::string> args) {
    setArguments(args);
    start();
}
