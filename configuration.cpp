/* 
 * File:   Configuration.cpp
 * Author: chris
 * 
 * Created on January 2, 2012, 2:32 PM
 */

#include "configuration.h"
#include "exception.h"
#include "utils.h"
#include "luainterpreter.h"

#include <iostream>
#include <sstream>

using namespace basic;

bool Configuration::s_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Configuration::Register);

void Configuration::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Configuration")) {
    luabind::module(luaInterpreter.getL())
    [
            luabind::class_< Configuration >("Configuration")
            .def("readFile", &Configuration::readFile)
            .def("addSetting", &Configuration::addSetting)
            .def("editSetting", &Configuration::editSetting)
            .def("getDouble", &Configuration::getDouble)
            .def("getBool", &Configuration::getBool)
            .def("getInteger", &Configuration::getInteger)
            .def("getString", &Configuration::getString)
            //dictionary
    ];
    luaInterpreter.setClassRegistered("Configuration");
    }
#endif
}

std::auto_ptr<Configuration> Configuration::m_instancePtr(0);

Configuration& Configuration::getInstance() {
    if(m_instancePtr.get() == 0)
    {
        m_instancePtr = std::auto_ptr<Configuration>(new Configuration);
    }
    return *m_instancePtr.get();
}

Configuration::Configuration() : Named("Configuration")
{


}

void Configuration::readFile(std::string configFile) {
    try {
    m_configuration.readFile(configFile.c_str());
    } catch (libconfig::SettingTypeException& ste) {
        ERROR(std::string("invalid type for ") + std::string(ste.getPath()));
    } catch(libconfig::SettingNotFoundException& snfe) {
        ERROR(std::string("setting not found: ") + snfe.getPath());
    } catch (libconfig::ParseException& pe) {
        ERROR(std::string("unable to parse config file ") + pe.getFile() + " at line " + utils::toString(pe.getLine()));
    } catch(libconfig::FileIOException& fioe) {
    	ERROR("Unable to read file " + configFile)
   } catch (...) {
        throw; // someone will be interested in this
    }

}

void Configuration::addSetting(std::string path, std::string name, std::string value) {
	libconfig::Setting& group = m_configuration.lookup(path);
	libconfig::Setting& setting = group.add(name, libconfig::Setting::TypeString);
	setting = value;
}

void Configuration::editSetting(std::string path, std::string name, double value) {
  std::string fullpath = "";
  if(path != "")
    fullpath = path + ".";
  fullpath += name;
  
  libconfig::Setting& setting = m_configuration.lookup(fullpath);
  setting = value;
}

double Configuration::getDouble(std::string path) {
	return m_configuration.lookup(path);
}

bool Configuration::getBool(std::string path) {
	return m_configuration.lookup(path);
}

int Configuration::getInteger(std::string path) {
	return m_configuration.lookup(path);
}

std::string Configuration::getString(std::string path){
	return m_configuration.lookup(path);
}

Configuration::~Configuration() {
}
