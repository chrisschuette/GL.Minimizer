#include "bspincuboid.h"
#include <assert.h>
#include <iostream>
#include "luainterpreter.h"
#include "exception.h"

bool BSpinCuboid::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(BSpinCuboid::Register);
void BSpinCuboid::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("SpinCuboid"))
        SpinCuboid::Register();

    if(!luaInterpreter.isClassRegistered("BSpinCuboid")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< BSpinCuboid, luabind::bases<SpinCuboid> >("BSpinCuboid")
                .def(luabind::constructor<int,int,int,int,int,int,bool,bool,bool>())
                .def(luabind::constructor<const Array&,int,int,int,bool,bool,bool>())
                .def("setBorderTo", &BSpinCuboid::setBorderTo)
                .def("unlockBorder", &BSpinCuboid::unlockBorder)
                .def("lockBorder", &BSpinCuboid::lockBorder)

        ];
        luaInterpreter.setClassRegistered("BSpinCuboid");
    }
#endif
}


BSpinCuboid::BSpinCuboid(int Nx, int Ny, int Nz, int Bx, int By, int Bz, bool Px, bool Py, bool Pz) : SpinCuboid(Nx, Ny, Nz, Px, Py, Pz), m_borderLocked(true), m_border(0) {
    m_borderThickness[0] = Bx;
    m_borderThickness[1] = By;
    m_borderThickness[2] = Bz;

    if( (Bx == 0) && (By == 0) && (Bz == 0))
        return;

    int dx = Nx + 2 * Bx;
    int dy = Ny + 2 * By;
    int dz = Nz + 2 * Bz;

    int size = dx * dy * dz * 3;
    if(size <= 0)
        return;

    m_border = gsl_vector_alloc(size);
}

BSpinCuboid::BSpinCuboid(const Array& orig, int Bx, int By, int Bz, bool Px, bool Py, bool Pz) : m_borderLocked(true), m_border(0) {
    assert(orig.getDimensions() == 4);
    assert(orig.getDimension(3) == 3);

    int Nx = orig.getDimension(0);
    int Ny = orig.getDimension(1);
    int Nz = orig.getDimension(2);

    assert(Nx >= 0);
    assert(Ny >= 0);
    assert(Nz >= 0);

    m_periodic[0] = Px;
    m_periodic[1] = Py;
    m_periodic[2] = Pz;

    m_data = 0;
    m_dimensions = 0;
    m_n = 4;
    m_dimensions = new int[4];
    m_dimensions[0] = Nx;
    m_dimensions[1] = Ny;
    m_dimensions[2] = Nz;
    m_dimensions[3] = 3;

    int innerSize = Nx * Ny * Nz * 3;
    if(innerSize > 0)
        m_data = gsl_vector_alloc(Nx * Ny * Nz * 3);
    else
        m_data = 0;

    m_borderThickness[0] = Bx;
    m_borderThickness[1] = By;
    m_borderThickness[2] = Bz;

    if( (Bx == 0) && (By == 0) && (Bz == 0))
        return;

    int dx = Nx + 2 * Bx;
    int dy = Ny + 2 * By;
    int dz = Nz + 2 * Bz;

    int size = dx * dy * dz * 3;
    if(size > 0)
        m_border = gsl_vector_alloc(size);
    else
        m_border = 0;


    int crd[4];

    for(int c = 0; c < 3; c++)
        for(int x = 0; x < Nx; x++)
            for(int y = 0; y < Ny; y++)
                for(int z = 0; z < Nz; z++) {
                    crd[0] = x;
                    crd[1] = y;
                    crd[2] = z;
                    crd[3] = c;
                    set4(x,y,z,c,orig.get(4,crd));
                }

}

BSpinCuboid::BSpinCuboid(const Array& orig, int Bx, int By, int Bz, bool Px, bool Py, bool Pz, double boundary1, double boundary2, double boundary3) : SpinCuboid(orig,Px,Py,Pz), m_borderLocked(true) {
    int Nx = orig.getDimension(0);
    int Ny = orig.getDimension(1);
    int Nz = orig.getDimension(2);

    m_borderThickness[0] = Bx;
    m_borderThickness[1] = By;
    m_borderThickness[2] = Bz;

    if( (Bx == 0) && (By == 0) && (Bz == 0))
        return;

    int dx = Nx + 2 * Bx;
    int dy = Ny + 2 * By;
    int dz = Nz + 2 * Bz;

    int size = dx * dy * dz * 3;
    if(size <= 0)
        return;

    m_border = gsl_vector_alloc(size);
    unlockBorder();
    setBorderTo(boundary1,boundary2,boundary3);
    lockBorder();

}

BSpinCuboid::BSpinCuboid(const BSpinCuboid& orig) : SpinCuboid(orig), m_border(0) {
    m_borderLocked = orig.m_borderLocked;
    m_borderThickness[0] = orig.m_borderThickness[0];
    m_borderThickness[1] = orig.m_borderThickness[1];
    m_borderThickness[2] = orig.m_borderThickness[2];
    if(orig.m_border) {
        m_border = gsl_vector_alloc(orig.m_border->size);
        gsl_vector_memcpy(m_border, orig.m_border);
    }
}

BSpinCuboid& BSpinCuboid::operator=(const BSpinCuboid& orig) {
    if(m_border) {
        gsl_vector_free(m_border);
        m_border = 0;
    }

    SpinCuboid::operator =(orig);

    m_borderLocked = orig.m_borderLocked;
    m_borderThickness[0] = orig.m_borderThickness[0];
    m_borderThickness[1] = orig.m_borderThickness[1];
    m_borderThickness[2] = orig.m_borderThickness[2];
    if(orig.m_border) {
        m_border = gsl_vector_alloc(orig.m_border->size);
        gsl_vector_memcpy(m_border, orig.m_border);
    }
    return *this;
}

BSpinCuboid::~BSpinCuboid() {
    if(m_border)
        gsl_vector_free(m_border);
}

bool BSpinCuboid::isBorder(int x, int y, int z) {
    if( (x < 0)
            || (x >= m_dimensions[0])
            || (y < 0)
            || (y >= m_dimensions[1])
            || (z < 0)
            || (z >= m_dimensions[2]) )
        return true;
    else
        return false;
}

double BSpinCuboid::get(int n, int* indices) const {
    assert(n == 4);
    if(m_periodic[0])
        indices[0] = mapBack(indices[0], -m_borderThickness[0], m_dimensions[0] + m_borderThickness[0] - 1);
    if(m_periodic[1])
        indices[1] = mapBack(indices[1], -m_borderThickness[1], m_dimensions[1] + m_borderThickness[1] - 1);
    if(m_periodic[2])
        indices[2] = mapBack(indices[2], -m_borderThickness[2], m_dimensions[2] + m_borderThickness[2] - 1);

    assert(indices[0] >= -m_borderThickness[0]);
    assert(indices[0] < m_dimensions[0] + m_borderThickness[0]);

    assert(indices[1] >= -m_borderThickness[1]);
    assert(indices[1] < m_dimensions[1] + m_borderThickness[1]);

    assert(indices[2] >= -m_borderThickness[2]);
    assert(indices[2] < m_dimensions[2] + m_borderThickness[2]);

    assert(indices[3] >= 0);
    assert(indices[3] < 3);
    assert(m_borderLocked);

    if( (indices[0] < 0)
            || (indices[0] >= m_dimensions[0])
            || (indices[1] < 0)
            || (indices[1] >= m_dimensions[1])
            || (indices[2] < 0)
            || (indices[2] >= m_dimensions[2]) ) {
        int index = (indices[0] + m_borderThickness[0]) + ( (indices[1] + m_borderThickness[1])  + ( (indices[2] + m_borderThickness[2]) + indices[3] * ( m_dimensions[2] + 2 * m_borderThickness[2]) )  *  ( m_dimensions[1] + 2 * m_borderThickness[1]) ) *  ( m_dimensions[0] + 2 * m_borderThickness[0]);
        return gsl_vector_get(m_border, index);
    } else {
        return SpinCuboid::get4(indices[0],indices[1],indices[2],indices[3]);
    }
}

void BSpinCuboid::set(int n, int* indices, const double& value) {
    assert(n == 4);
    if(m_periodic[0])
        indices[0] = mapBack(indices[0], -m_borderThickness[0], m_dimensions[0] + m_borderThickness[0] - 1);
    if(m_periodic[1])
        indices[1] = mapBack(indices[1], -m_borderThickness[1], m_dimensions[1] + m_borderThickness[1] - 1);
    if(m_periodic[2])
        indices[2] = mapBack(indices[2], -m_borderThickness[2], m_dimensions[2] + m_borderThickness[2] - 1);

    assert(indices[0] >= -m_borderThickness[0]);
    assert(indices[0] < m_dimensions[0] + m_borderThickness[0]);

    assert(indices[1] >= -m_borderThickness[1]);
    assert(indices[1] < m_dimensions[1] + m_borderThickness[1]);

    assert(indices[2] >= -m_borderThickness[2]);
    assert(indices[2] < m_dimensions[2] + m_borderThickness[2]);

    assert(indices[3] >= 0);
    assert(indices[3] < 3);

    if( (indices[0] < 0)
            || (indices[0] >= m_dimensions[0])
            || (indices[1] < 0)
            || (indices[1] >= m_dimensions[1])
            || (indices[2] < 0)
            || (indices[2] >= m_dimensions[2]) ) {
        int index = (indices[0] + m_borderThickness[0]) + ( (indices[1] + m_borderThickness[1])  + ( (indices[2] + m_borderThickness[2]) + indices[3] * ( m_dimensions[2] + 2 * m_borderThickness[2]) )  *  ( m_dimensions[1] + 2 * m_borderThickness[1]) ) *  ( m_dimensions[0] + 2 * m_borderThickness[0]);
        assert(!m_borderLocked);
        gsl_vector_set(m_border, index, value);
    } else {
        SpinCuboid::set4(indices[0],indices[1],indices[2],indices[3],value);
    }
}

void BSpinCuboid::set4(int x, int y, int z, int c, double value) {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x, -m_borderThickness[0], m_dimensions[0] + m_borderThickness[0] - 1);
    if(m_periodic[1])
        y = mapBack(y, -m_borderThickness[1], m_dimensions[1] + m_borderThickness[1] - 1);
    if(m_periodic[2])
        z = mapBack(z, -m_borderThickness[2], m_dimensions[2] + m_borderThickness[2] - 1);

    assert(x >= -m_borderThickness[0]);
    assert(x < m_dimensions[0] + m_borderThickness[0]);

    assert(y >= -m_borderThickness[1]);
    assert(y < m_dimensions[1] + m_borderThickness[1]);

    assert(z >= -m_borderThickness[2]);
    assert(z < m_dimensions[2] + m_borderThickness[2]);

    assert(c >= 0);
    assert(c < 3);

    if( (x < 0)
            || (x >= m_dimensions[0])
            || (y < 0)
            || (y >= m_dimensions[1])
            || (z < 0)
            || (z >= m_dimensions[2]) ) {
        int index = (x + m_borderThickness[0]) + ( (y + m_borderThickness[1])  + ( (z + m_borderThickness[2]) + c * ( m_dimensions[2] + 2 * m_borderThickness[2]) )  *  ( m_dimensions[1] + 2 * m_borderThickness[1]) ) *  ( m_dimensions[0] + 2 * m_borderThickness[0]);
        assert(!m_borderLocked);
        gsl_vector_set(m_border, index, value);
    } else {
        SpinCuboid::set4(x,y,z,c,value);
    }
}

double BSpinCuboid::get4(int x, int y, int z, int c) const {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x, -m_borderThickness[0], m_dimensions[0] + m_borderThickness[0] - 1);
    if(m_periodic[1])
        y = mapBack(y, -m_borderThickness[1], m_dimensions[1] + m_borderThickness[1] - 1);
    if(m_periodic[2])
        z = mapBack(z, -m_borderThickness[2], m_dimensions[2] + m_borderThickness[2] - 1);

    assert(x >= -m_borderThickness[0]);
    assert(x < m_dimensions[0] + m_borderThickness[0]);

    assert(y >= -m_borderThickness[1]);
    assert(y < m_dimensions[1] + m_borderThickness[1]);

    assert(z >= -m_borderThickness[2]);
    assert(z < m_dimensions[2] + m_borderThickness[2]);

    assert(c >= 0);
    assert(c < 3);
    assert(m_borderLocked);

    if( (x < 0)
            || (x >= m_dimensions[0])
            || (y < 0)
            || (y >= m_dimensions[1])
            || (z < 0)
            || (z >= m_dimensions[2]) ) {
        int index = (x + m_borderThickness[0]) + ( (y + m_borderThickness[1])  + ( (z + m_borderThickness[2]) + c * ( m_dimensions[2] + 2 * m_borderThickness[2]) )  *  ( m_dimensions[1] + 2 * m_borderThickness[1]) ) *  ( m_dimensions[0] + 2 * m_borderThickness[0]);
        return gsl_vector_get(m_border, index);
    } else {
        return SpinCuboid::get4(x,y,z,c);
    }
}

int BSpinCuboid::get4Index(int x, int y, int z, int c) const {
    assert(4 == m_n);

    if(m_periodic[0])
        x = mapBack(x, -m_borderThickness[0], m_dimensions[0] + m_borderThickness[0] - 1);
    if(m_periodic[1])
        y = mapBack(y, -m_borderThickness[1], m_dimensions[1] + m_borderThickness[1] - 1);
    if(m_periodic[2])
        z = mapBack(z, -m_borderThickness[2], m_dimensions[2] + m_borderThickness[2] - 1);

    assert(x >= -m_borderThickness[0]);
    assert(x < m_dimensions[0] + m_borderThickness[0]);

    assert(y >= -m_borderThickness[1]);
    assert(y < m_dimensions[1] + m_borderThickness[1]);

    assert(z >= -m_borderThickness[2]);
    assert(z < m_dimensions[2] + m_borderThickness[2]);

    assert(c >= 0);
    assert(c < 3);
    assert(m_borderLocked);

    if( (x < 0)
            || (x >= m_dimensions[0])
            || (y < 0)
            || (y >= m_dimensions[1])
            || (z < 0)
            || (z >= m_dimensions[2]) )
        return -1;
    return SpinCuboid::get4Index(x,y,z,c);
}


void BSpinCuboid::setBorderTo(double c1, double c2, double c3) {
    assert(!m_borderLocked);
    for(int x = -m_borderThickness[0]; x < m_dimensions[0] + m_borderThickness[0]; x++)
        for(int y = -m_borderThickness[1]; y < m_dimensions[1] + m_borderThickness[1]; y++)
            for(int z = -m_borderThickness[2]; z < m_dimensions[2] + m_borderThickness[2]; z++)
                if(isBorder(x,y,z)) {
                    set4(x,y,z,0,c1);
                    set4(x,y,z,1,c2);
                    set4(x,y,z,2,c3);
                }

}

void BSpinCuboid::unlockBorder() {
    m_borderLocked = false;
}

void BSpinCuboid::lockBorder() {
    m_borderLocked = true;
}

bool BSpinCuboid::isBorderLocked() {
    return m_borderLocked;
}

int BSpinCuboid::mapBack(int coordinate, int min, int max) const {
    while(coordinate < min)
        coordinate += (max - min + 1);
    while(coordinate > max)
        coordinate -= (max - min + 1);
    return coordinate;
}
