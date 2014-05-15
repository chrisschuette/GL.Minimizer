#include "noise.h"
#include "luainterpreter.h"

bool Noise::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Noise::Register);
void Noise::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Noise")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Noise >("Noise")
                .def(luabind::constructor<>())
                .def("setSigma", &Noise::setSigma)
                .def("apply", &Noise::apply)
        ];
        luaInterpreter.setClassRegistered("Noise");

    }
#endif
}

Noise::Noise() : Named("Noise")
  ,m_sigma(1.0)
  , m_var_nor(boost::variate_generator<boost::mt19937*,
                  boost::normal_distribution<> >(&m_rng, boost::normal_distribution<>(0.0, 1.0)))
{
}

void Noise::setSigma(double s) {
    m_sigma = s;

    //update random number generator
    boost::normal_distribution<> dist(0.0, m_sigma);
    m_var_nor = boost::variate_generator<boost::mt19937*, boost::normal_distribution<> >(&m_rng, dist);
}

void Noise::apply(SpinCuboid& s) {
    for(int x = 0; x < s.getDimension(0); x++)
        for(int y = 0; y < s.getDimension(1); y++)
            for(int z = 0; z < s.getDimension(2); z++) {
                s.set4( x, y, z, 0, s.get4(x,y,z,0) + m_var_nor() );
                s.set4( x, y, z, 1, s.get4(x,y,z,1) + m_var_nor() );
                s.set4( x, y, z, 2, s.get4(x,y,z,2) + m_var_nor() );
            }
}

