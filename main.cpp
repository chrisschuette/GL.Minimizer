
#include "array.h"
#include "spincuboid.h"
#include "luainterpreter.h"
#include "luathread.h"
#include "exception.h"

#include "vis/mainwindow.h"

#include <GL/glut.h>
#include <boost/program_options.hpp>
#include <QtGui/QApplication>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{   
    glutInit(&argc,argv);

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "show help message")
                ("viewer", "open viewer window")
                ("luaArguments", po::value<std::vector<std::string> >(), "lua arguments");
        po::positional_options_description p;
        p.add("luaArguments",-1);
        po::variables_map vm;
        po::store(po::command_line_parser(argc,argv).options(desc).positional(p).run(),vm);
        po::notify(vm);
        if(vm.count("help")) {
            std::cout << "usage: " << argv[0] << " options script arguments" << std::endl;
            std::cout << desc << std::endl;
            return 0;
        }

        std::vector<std::string> arguments;
        if(vm.count("luaArguments") > 0)
            arguments = vm["luaArguments"].as< std::vector< std::string > >();
        else {
            std::cout << "usage: " << argv[0] << " options script arguments" << std::endl;
            std::cout << desc << std::endl;
            return 0;
        }

        scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

        if(vm.count("viewer")) {
            // viewer
            QApplication app(argc,argv);
            MainWindow w;
            w.show();
            Viewer& viewer = w.getViewer();

            LuaThread luathread(luaInterpreter, viewer);
            luathread.execute(arguments);
            app.exec();

        } else {
            // no viewer
            Viewer viewer;
            luaInterpreter.init();
            luaInterpreter.exportViewer(viewer);
            luaInterpreter.execute(arguments);
        }

        delete &luaInterpreter;
    } catch (luabind::error& e) { //repackage any errors
        std::string error = lua_tostring(e.state(), -1);
        std::cerr << error << std::endl;
    } catch (basic::Exception& e) {
        std::cerr << "error: " << e.getErrorMsg() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "std::error: " << e.what() << std::endl;
    }

    return 0;
}
