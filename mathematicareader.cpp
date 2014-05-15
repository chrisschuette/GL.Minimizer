#include "mathematicareader.h"
#include <fstream>
#include "luainterpreter.h"

using boost::spirit::utree;
using boost::spirit::utree_type;

bool MathematicaReader::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(MathematicaReader::Register);
void MathematicaReader::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Array"))
        Array::Register();

    if(!luaInterpreter.isClassRegistered("MathematicaReader")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< MathematicaReader >("MathematicaReader")
                .def(luabind::constructor<>())
                .def("read", &MathematicaReader::read)

        ];
        luaInterpreter.setClassRegistered("MathematicaReader");
    }
#endif
}

MathematicaReader::MathematicaReader()
{
}

Array MathematicaReader::read(std::string filename) {
    std::ifstream ifs(filename.c_str());
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    using boost::spirit::ascii::space;

    arrayData data;
    typedef std::string::const_iterator iterator_type;
    typedef magrammar<iterator_type> grammar;
    grammar ourGrammar;

    utree ut;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    bool r = phrase_parse(iter, end, ourGrammar, space, ut);
    processNode(ut.front(), data, std::vector<int>());


    std::vector<int> dimensions;
    dimensions = data.coordinates.at(0);
    for(int i = 0; i < data.coordinates.size(); i++) {
        for(int j = 0; j < dimensions.size(); j++) {
            if(dimensions.at(j) < data.coordinates.at(i).at(j) + 1)
                dimensions.at(j) = data.coordinates.at(i).at(j) + 1;
        }
    }

    Array array(dimensions);

    for(int i = 0; i < data.coordinates.size(); i++) {
        int c[dimensions.size()];
        for(int j = 0; j < dimensions.size(); j++) {
            c[j] = data.coordinates.at(i).at(j);
        }
        array.set(dimensions.size(), c, data.data.at(i));
    }

    std::cout << "read " << filename << " [ ";
    for(int j = 0; j < dimensions.size(); j++) {
        std::cout << dimensions.at(j) << " ";
    }
    std::cout << " ] " << std::endl;

    return array;
}


void MathematicaReader::processNode(boost::spirit::utree& tree, arrayData& data, std::vector<int> coordinates) {
    if(tree.which() == utree_type::list_type) {
        int coord = 0;
        for(utree::iterator it = tree.begin(); it != tree.end(); ++it) {
            std::vector<int> newCoordinates(coordinates);
            newCoordinates.push_back(coord);
            processNode(*it, data, newCoordinates);
            coord++;
        }
    } else if (tree.which() == utree_type::double_type) {
        data.coordinates.push_back(coordinates);
        data.data.push_back(tree.get<double>());
    }
}
