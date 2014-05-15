/* 
 * File:   LuaInterpreter.cpp
 * Author: chris
 * 
 * Created on January 23, 2012, 1:06 PM
 */

#include "luainterpreter.h"
#include "configuration.h"
#include "utils.h"
#include "exception.h"

#include <iostream>
#include <luabind/class_info.hpp>
#include <luabind/exception_handler.hpp>
#include <stdlib.h>
#include <list>
#include <vector>

using namespace scripting;

LuaInterpreter* LuaInterpreter::m_instancePtr = 0;

LuaInterpreter& LuaInterpreter::getInstance() {
    if (m_instancePtr == NULL) {
        m_instancePtr = new LuaInterpreter;
    }
    return *m_instancePtr;
}

void LuaInterpreter::Register() {
    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    luabind::module(luaInterpreter.getL())
            [
            luabind::class_<LuaInterpreter > ("LuaInterpreter")
            .def("stackTrace", &LuaInterpreter::stackTrace)
            ];

    return;
}

void LuaInterpreter::error(std::string msg) {
    LuaInterpreter& interpreter = LuaInterpreter::getInstance();

    interpreter.stackTrace();
    lua_Debug entry;
    int depth = 1;

    lua_getstack(interpreter.getL(), depth, &entry);
	lua_getinfo(interpreter.getL(), "Sln", &entry);
    using namespace utils;

    std::string name = (entry.name ? toString(entry.name) : toString("main"));

    throw basic::Exception(msg, name.c_str(), toString(entry.short_src).c_str(), entry.currentline);
}

LuaInterpreter::LuaInterpreter() {
}

LuaInterpreter::~LuaInterpreter() {
    if(m_L)
        lua_close(m_L);
}

void LuaInterpreter::init() {

    //fire up the lua and luabind library
    m_L = luaL_newstate();
    luaL_openlibs(m_L);
    luabind::open(m_L);

    for (unsigned int i = 0; i < m_registerFunctions.size(); i++) {

        tRegisterFunction rgtFunc = m_registerFunctions.at(i);
        (*rgtFunc)();

    }

    Register();

    luabind::register_exception_handler<libconfig::SettingException > (&scripting::LuaInterpreter::translateLibconfigException);

    luabind::globals(m_L)["configuration"] = &basic::Configuration::getInstance();
    luabind::globals(m_L)["d"] = this;


    luabind::module(m_L)
            [
            luabind::class_<Arguments > ("Arguments")
            .def("at", &Arguments::at)
            .def("size", &Arguments::size)
            ];

    luabind::module(m_L)
            [
            luabind::def("error", &LuaInterpreter::error)
            ];
    luabind::module(m_L)
            [
            luabind::def("log", (void(*)(std::string)) &LuaInterpreter::log)
            ];
}

lua_State* LuaInterpreter::getL() const {
    return m_L;
}

void LuaInterpreter::exportViewer(Viewer& viewer) {
    if(m_L)
        luabind::globals(m_L)["viewer"] = &viewer;
}

void LuaInterpreter::log(std::string text) {
    std::cout << text << std::endl;
}

std::string LuaInterpreter::getCurrentLuaFunction() {
    lua_Debug entry;
    int depth = 1;
    std::string luaFunctionString;

    lua_getstack(m_L, depth, &entry);
	lua_getinfo(m_L, "Sln", &entry);
//    assert(status);
    using namespace utils;
    luaFunctionString = toString(entry.short_src) + " (" + toString(entry.currentline) + ") " + (entry.name ? toString(entry.name) : toString("main"));

    return luaFunctionString;
}

void LuaInterpreter::stackTrace() {
    lua_Debug entry;
    int depth = 0;

    //OUT(0) << "dumping stack trace:" << EOM;
    
    while (lua_getstack(m_L, depth, &entry)) {

    	lua_getinfo(m_L, "Sln", &entry);

        std::string stack;
        using namespace utils;
        stack += toString(entry.short_src) + " (" + toString(entry.currentline) + ") " + (entry.name ? toString(entry.name) : toString("main"));
        //OUT(0) << stack << EOM;
        depth++;
    }
}

void LuaInterpreter::translateLibconfigException(lua_State* L, const libconfig::SettingException& e) {
    std::string message = e.what();
    message += ": ";
    message += e.getPath();
    lua_pushstring(L, message.c_str());
}

void LuaInterpreter::execute(int argc, char** argv) {
    if (argc < 2)
        ERROR("not enough arguments");

    Arguments arguments(argc, argv);
    luabind::globals(m_L)["arguments"] = arguments;

    int status = luaL_dofile(m_L, argv[1]);
    if (status) {

        std::string msg = "Error parsing lua file: " + std::string(argv[1]);
        msg += ":\n";
        msg += lua_tostring(m_L, -1);
        ERROR(msg);
    }
    luabind::call_function<void>(m_L, "main");
}

void LuaInterpreter::execute(std::vector<std::string> args) {
    if (args.size() < 1)
        ERROR("not enough arguments");

    std::string filename = args.at(0);
    args.erase(args.begin());

    Arguments arguments(args);

    luabind::globals(m_L)["arguments"] = arguments;

    int status = luaL_dofile(m_L, filename.c_str());
    if (status) {

        std::string msg = "Error parsing lua file: " + filename;
        msg += ":\n";
        msg += lua_tostring(m_L, -1);
        ERROR(msg);
    }
    luabind::call_function<void>(m_L, "main");
}

bool LuaInterpreter::isClassRegistered(std::string className) {
   /* bool found = false;
    luabind::object obj = luabind::get_class_names(m_L);
    int counter = 0;
    for (luabind::iterator i(obj), end; i != end; ++i) {
        std::string name = luabind::object_cast<std::string>(*i);
        std::cout << counter << " " << name << std::endl;
        counter++;
        if (name == className)
            found = true;
    }
    std::cout.flush();
    return found;*/

    if(m_registeredClasses.count(className)>0)
        return true;
    return false;
}

bool LuaInterpreter::setClassRegistered(std::string name) {
    m_registeredClasses.insert(name);
    std::cout << name << " registered." << std::endl;
    return true;
}


bool LuaInterpreter::addRegisterFunction(tRegisterFunction registerFunction) {
    m_registerFunctions.push_back(registerFunction);
    return true;
}

