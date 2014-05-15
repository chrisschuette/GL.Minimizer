/*
 * File:   LuaInterpreter.h
 * Author: chris
 *
 * Created on January 23, 2012, 1:06 PM
 */

#ifndef LUAINTERPRETER_H
#define	LUAINTERPRETER_H

#ifndef LUADISABLED

#include "viewer.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <vector>
#include <string>
#include <memory>
#include <set>

namespace libconfig {
    class SettingException;
}

namespace math {
    class Func;
}
//class Viewer;
namespace scripting {
/**
 * The type of function pointer expected by LuaInterpreter::addRegisterFunction(tRegisterFunction registerFunction).
 */
    typedef void(*tRegisterFunction)();

	/*!
	 * \brief Contains command-line arguments.
	 *
	 * An instance of Arguments is passed into the lua script containing command-line arguments as the global object "arguments".
	 */
    class Arguments {
    public:

        Arguments(int argc, char** argv) {
            for ( int i = 2; i < argc; i++)
                m_arguments.push_back(argv[i]);
        }

        Arguments(std::vector<std::string> args) {
            m_arguments = args;
        }

		/**
		 * returns the (i+1)th command-line argument.
		 */
        std::string at(int i) {
            return m_arguments.at(i);
        }

        void add(std::string argument) {
            m_arguments.push_back(argument);
        }

        /**
         * returns the number of command-line arguments.
         */
        int size() {
            return m_arguments.size();
        }
    private:
        std::vector<std::string> m_arguments;

    };
    /*! \brief LuaInterpreter exports C++ classes to Lua and executes Lua scripts.
     *
     *  LuaInterpreter is responsible for 2 things:
     *  1) it exports C++ classes, which implement a Register() function and have successfully informed LuaInterpreter that they do so.
     *  2) it parses and executes Lua scripts.
     */
    class LuaInterpreter /*: public basic::Named*/ {
    public:

        static void Register();
     
        static LuaInterpreter& getInstance();
        /**
         * init() calls all the previously collected Register() functions and exports some global environment variables to Lua, i.e.
         * the Configuration object (singleton).
         */
        void init();
        lua_State* getL() const;
        /**
         * returns true if a class called className is already registered. Otherwise returns false.
         */
        bool isClassRegistered(std::string className);

        bool setClassRegistered(std::string name);

        /**
         * executes the script mentioned in the first argument and passes the rest of the command-line arguments to the script.
         */
        void execute(int argc, char** argv);
        void execute(std::vector<std::string> arguments);

        /**
         * addRegisterFunction(tRegisterFunction registerFunction) expects a function pointer of type tRegisterFunction. Any class planning to expose itself to the Lua
         * environment should call this function passing a pointer to its Register() function before LuaInterpreter::init() is called.
         */
        bool addRegisterFunction(tRegisterFunction registerFunction);
        static void log(std::string text);

        void exportViewer(Viewer& viewer);

        //error handling
        void stackTrace();
        static void error(std::string msg);
        std::string getCurrentLuaFunction();
        static void translateLibconfigException(lua_State* L, const libconfig::SettingException& e);

        virtual ~LuaInterpreter();
    private:
        LuaInterpreter();
        static LuaInterpreter* m_instancePtr;
        lua_State* m_L;
        std::vector< tRegisterFunction > m_registerFunctions;
        std::vector< std::string > m_scripts;
        std::set<std::string> m_registeredClasses;
    };
}

#else

namespace scripting {
    typedef void(*tRegisterFunction)();

    class LuaInterpreter /*: public basic::Named*/ {
    public:
        static LuaInterpreter& getInstance() {
            if (m_instancePtr == 0) {
                m_instancePtr = new LuaInterpreter;
            }
            return *m_instancePtr;
        }
        bool addRegisterFunction(tRegisterFunction registerFunction) { return false; }
    private:
        static LuaInterpreter* m_instancePtr;


    };
}
#endif /* LUADISABLED */

#endif	/* LUAINTERPRETER_H */

