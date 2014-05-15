#include "spincuboid.h"

#include <iostream>
#include <assert.h>
#include <cmath>
#include "luainterpreter.h"
#include "utils.h"


bool SpinCuboid::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(SpinCuboid::Register);
void SpinCuboid::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Array"))
        Array::Register();

    if(!luaInterpreter.isClassRegistered("SpinCuboid")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< SpinCuboid, luabind::bases< Array > >("SpinCuboid")
                .def(luabind::constructor<>())
                .def(luabind::constructor<int,int,int,bool,bool,bool>())
                .def(luabind::constructor<const Array&, bool, bool, bool>())
                .def("getAverageSpinLength", &SpinCuboid::getAverageSpinLength)
                .def("getVariance", &SpinCuboid::getVariance)
                .def("set4", &SpinCuboid::set4)
                .def("get4", &SpinCuboid::get4)
                .def("setLayer", &SpinCuboid::setLayer)
                .def("normalize", &SpinCuboid::normalize)
                .def("zeroLayer", &SpinCuboid::zeroLayer)

        ];
        luaInterpreter.setClassRegistered("SpinCuboid");
    }
#endif
}


SpinCuboid::SpinCuboid() : Array()
{
    //default is periodic
    m_periodic[0] = true;
    m_periodic[1] = true;
    m_periodic[2] = true;

    m_n = 4;
    m_dimensions = new int[4];
    m_dimensions[0] = 0;
    m_dimensions[1] = 0;
    m_dimensions[2] = 0;
    m_dimensions[3] = 3;
}

SpinCuboid::SpinCuboid(int Nx, int Ny, int Nz, bool periodicX, bool periodicY, bool periodicZ) {
    m_periodic[0] = periodicX;
    m_periodic[1] = periodicY;
    m_periodic[2] = periodicZ;

    m_data = 0;
    m_dimensions = 0;
    m_n = 4;
    m_dimensions = new int[4];
    m_dimensions[0] = Nx;
    m_dimensions[1] = Ny;
    m_dimensions[2] = Nz;
    m_dimensions[3] = 3;

    int size = Nx * Ny * Nz * 3;
    if(size <= 0)
        return;
    m_data = gsl_vector_alloc(Nx * Ny * Nz * 3);
}

SpinCuboid::~SpinCuboid() {
}

SpinCuboid::SpinCuboid(const Array& orig, bool periodicX, bool periodicY, bool periodicZ) : Array(orig) {
    setName("SpinCuboid");
    m_periodic[0] = periodicX;
    m_periodic[1] = periodicY;
    m_periodic[2] = periodicZ;

    if((m_n != 4) || (m_dimensions[3] != 3)) {
        std::cerr << "Incompatible array!" << std::endl;
        if(m_dimensions)
            delete [] m_dimensions;
        m_n = 0;
    }
}

SpinCuboid::SpinCuboid(const SpinCuboid& orig) : Array(orig) {
    m_periodic[0] = orig.m_periodic[0];
    m_periodic[1] = orig.m_periodic[1];
    m_periodic[2] = orig.m_periodic[2];
}

SpinCuboid& SpinCuboid::operator=(const SpinCuboid& orig) {
    Array::operator =(orig);
    m_periodic[0] = orig.m_periodic[0];
    m_periodic[1] = orig.m_periodic[1];
    m_periodic[2] = orig.m_periodic[2];
    return *this;
}

double SpinCuboid::get(int n, int* indices) const
{
    assert(n == 4);
    for(int i = 0; i < 3; i++) {
        if(m_periodic[i])
            indices[i] = mapBack(indices[i], m_dimensions[i]);
    }
    return Array::get(n,indices);
}

void SpinCuboid::set(int n, int* indices, double value)
{
    assert(n == 4);
    for(int i = 0; i < 3; i++) {
        if(m_periodic[i])
            indices[i] = mapBack(indices[i], m_dimensions[i]);
    }
    Array::set(n, indices, value);
}

void SpinCuboid::set4(int x, int y, int z, int c, double value) {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x,m_dimensions[0]);
    if(m_periodic[1])
        y = mapBack(y,m_dimensions[1]);
    if(m_periodic[2])
        z = mapBack(z,m_dimensions[2]);

    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);
    assert(c >= 0);
    assert(x < m_dimensions[0]);
    assert(y < m_dimensions[1]);
    assert(z < m_dimensions[2]);
    assert(c < 3);

    int index = x + ( y  + (z + c * m_dimensions[2] )  * m_dimensions[1] ) * m_dimensions[0];
    assert(index >= 0);
    assert(m_data != NULL);
    gsl_vector_set(m_data, index, value);
}

double SpinCuboid::get4(int x, int y, int z, int c) const {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x,m_dimensions[0]);
    if(m_periodic[1])
        y = mapBack(y,m_dimensions[1]);
    if(m_periodic[2])
        z = mapBack(z,m_dimensions[2]);

    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);
    assert(c >= 0);
    assert(x < m_dimensions[0]);
    assert(y < m_dimensions[1]);
    assert(z < m_dimensions[2]);
    assert(c < 3);

    int index = x + ( y  + (z + c * m_dimensions[2] )  * m_dimensions[1] ) * m_dimensions[0];
    assert(index >= 0);
    assert(m_data != NULL);
    return gsl_vector_get(m_data, index);
}

int SpinCuboid::get4Index(int x, int y, int z, int c) const {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x,m_dimensions[0]);
    if(m_periodic[1])
        y = mapBack(y,m_dimensions[1]);
    if(m_periodic[2])
        z = mapBack(z,m_dimensions[2]);

    assert(x >= 0);
    assert(y >= 0);
    assert(z >= 0);
    assert(c >= 0);
    assert(x < m_dimensions[0]);
    assert(y < m_dimensions[1]);
    assert(z < m_dimensions[2]);
    assert(c < 3);

    int index = x + ( y  + (z + c * m_dimensions[2] )  * m_dimensions[1] ) * m_dimensions[0];
    assert(index >= 0);
    return index;
}

void SpinCuboid::zeroLayer(int layer) {
    for(int x = 0; x < getDimension(0); x++)
        for(int y = 0; y < getDimension(1); y++) {
            set4(x,y,layer,0,0);
            set4(x,y,layer,1,0);
            set4(x,y,layer,2,0);
        }
}

void SpinCuboid::setLayer(int layer, const SpinCuboid &src, int srcLayer) {
   // std::cout << "SpinCuboid::setLayer(" + utils::toString(layer) + ", const SpinCuboid &src, " + utils::toString(srcLayer) + ")" << std::endl;
    assert(getDimension(0) == src.getDimension(0));
    assert(getDimension(1) == src.getDimension(1));
    for(int x = 0; x < getDimension(0); x++)
        for(int y = 0; y < getDimension(1); y++) {
            set4(x,y,layer,0,src.get4(x,y,srcLayer,0));
            set4(x,y,layer,1,src.get4(x,y,srcLayer,1));
            set4(x,y,layer,2,src.get4(x,y,srcLayer,2));
        }
}


bool SpinCuboid::isPeriodic(int dimension) const {
    assert(dimension >= 0);
    assert(dimension < 3);
    return m_periodic[dimension];
}

unsigned int SpinCuboid::getVolume() {
    if(m_data)
        return m_data->size / 3;
    else
        return 0;
}

void SpinCuboid::normalize() {
    for(int x = 0; x < m_dimensions[0]; x++) {
        for(int y = 0; y < m_dimensions[1]; y++) {
            for(int z = 0; z < m_dimensions[2]; z++) {
                double l = std::sqrt(std::pow(get4(x,y,z,0),2)
                                     + std::pow(get4(x,y,z,1),2)
                                     + std::pow(get4(x,y,z,2),2));
                if(l == 0) {
                    set4(x,y,z,0,0);
                    set4(x,y,z,1,0);
                    set4(x,y,z,2,1);
                } else {
                    set4(x,y,z,0,get4(x,y,z,0)/l);
                    set4(x,y,z,1,get4(x,y,z,1)/l);
                    set4(x,y,z,2,get4(x,y,z,2)/l);
                }
            }
        }
    }
}

double SpinCuboid::getVariance() const {
    double avgLength = 0;
    for(int x = 0; x < m_dimensions[0]; x++) {
        for(int y = 0; y < m_dimensions[1]; y++) {
            for(int z = 0; z < m_dimensions[2]; z++) {
                avgLength += std::sqrt(std::pow(get4(x,y,z,0),2)
                        + std::pow(get4(x,y,z,1),2)
                        + std::pow(get4(x,y,z,2),2));
            }
        }
    }
    avgLength /= (double) (m_dimensions[0] * m_dimensions[1] * m_dimensions[2]);
    double variance = 0;
    for(int x = 0; x < m_dimensions[0]; x++) {
        for(int y = 0; y < m_dimensions[1]; y++) {
            for(int z = 0; z < m_dimensions[2]; z++) {
                double length = std::sqrt(std::pow(get4(x,y,z,0),2)
                        + std::pow(get4(x,y,z,1),2)
                        + std::pow(get4(x,y,z,2),2));
                variance += std::pow(avgLength - length,2);
            }
        }
    }
    variance /= (double) (m_dimensions[0] * m_dimensions[1] * m_dimensions[2]);
    return variance;
}

double SpinCuboid::getAverageSpinLength() const {
    double avgLength = 0;
    for(int x = 0; x < m_dimensions[0]; x++) {
        for(int y = 0; y < m_dimensions[1]; y++) {
            for(int z = 0; z < m_dimensions[2]; z++) {
                avgLength += std::sqrt(std::pow(get4(x,y,z,0),2)
                        + std::pow(get4(x,y,z,1),2)
                        + std::pow(get4(x,y,z,2),2));
            }
        }
    }
    avgLength /= (double) (m_dimensions[0] * m_dimensions[1] * m_dimensions[2]);
    return avgLength;
}



int SpinCuboid::mapBack(int coordinate, int extent) const {
    while(coordinate < 0)
        coordinate += extent;
    while(coordinate >= extent)
        coordinate -= extent;
    return coordinate;
}
