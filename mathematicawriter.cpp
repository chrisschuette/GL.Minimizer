#include "mathematicawriter.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "luainterpreter.h"

using namespace std;

bool MathematicaWriter::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(MathematicaWriter::Register);
void MathematicaWriter::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Array"))
        Array::Register();

    if(!luaInterpreter.isClassRegistered("MathematicaWriter")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< MathematicaWriter >("MathematicaWriter")
                .def(luabind::constructor<>())
                .def("write", &MathematicaWriter::write)
        ];
        luaInterpreter.setClassRegistered("MathematicaWriter");
    }
#endif
}


MathematicaWriter::MathematicaWriter()
{
}

void MathematicaWriter::write(std::string filename, const Array &array) {
   // std::cout << "wrote " << filename << std::endl;
        ofstream file;
        file.exceptions ( ofstream::failbit | ofstream::badbit );
        file.open(filename.c_str());
        file.precision(5);
        file.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed


        renderDimension(0, array, file, std::vector<int>());
        file << std::endl;


        std::cout << "wrote " << filename << " [ ";
        for(int d = 0; d < array.getDimensions(); d++)
            std::cout << array.getDimension(d) << " ";
        std::cout << "]" << std::endl;

}

MathematicaWriter::~MathematicaWriter() {

}

std::vector<std::string> MathematicaWriter::getPathHierarchy(std::string filename) {
    std::string temp = filename;
    std::vector<std::string> paths;

    while(true) {
    std::string::size_type begin = temp.find_last_of("//");
    if (begin == std::string::npos) {
        //paths.push_back(temp);
        return paths;
    }
    else {
        temp = temp.substr(0,begin);
        paths.insert(paths.begin(),temp);
    }
    }
    return paths;
}

int MathematicaWriter::createDirectoryStructure(std::string filename) {
    std::vector<std::string> paths = getPathHierarchy(filename);

    for(unsigned int i = 0; i < paths.size(); i++)
    {
        std::cout << paths.at(i) << std::endl;
    struct stat st;
    if(stat(paths.at(i).c_str(),&st) == 0)
    {
    //	cout << directory << " already exists" << endl;
    }
    else
    {
        struct stat st1;
        if(stat(paths.at(i).c_str(),&st1) != 0) {
        umask(0);
        //OUT(7) << "creating " << paths.at(i) << EOM;
        mkdir(paths.at(i).c_str(),S_IRWXU | S_IRWXG | S_IRWXO);
        }
        if(stat(paths.at(i).c_str(),&st1) == 0)
        {
        //	cout << "Creating " << directory  << " . . . done" << endl;
        }
        else
        {
           // ERROR("Creating " + paths.at(i)  + " . . . FAILED")
            return -1;
        }
    }
    }

//	OUT(7) << path << EOM;
    return 0;
}

void MathematicaWriter::renderDimension(int dimension, const Array &array, std::ostream &stream, std::vector<int> base) {
    stream << "{ ";
    for( int i = 0; i < array.getDimension(dimension); i++) {
        std::vector<int> coordinates = base;
        coordinates.push_back(i);
        if(dimension < (array.getDimensions() - 1) ) {
            renderDimension(dimension + 1, array, stream, coordinates);
        } else {
            stream << " " << array.get(coordinates) << " ";
        }
        if(i != array.getDimension(dimension) - 1) {
            stream << ", ";
        }
    }
    stream << "} ";
}
