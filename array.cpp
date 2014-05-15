#include "array.h"
#include <assert.h>
#include <string.h>
#include <iostream>
#include "luainterpreter.h"
//#include "outputsystem.h"

bool Array::m_registered = scripting::LuaInterpreter::getInstance().addRegisterFunction(Array::Register);
void Array::Register() {
#ifndef LUADISABLED

    scripting::LuaInterpreter& luaInterpreter = scripting::LuaInterpreter::getInstance();

    if(!luaInterpreter.isClassRegistered("Array")) {
        luabind::module(luaInterpreter.getL())
        [
                luabind::class_< Array >("Array")
                .def(luabind::constructor<>())
                .def("dumpInfo", &Array::dumpInfo)
                .def("zero", &Array::zero)
                .def("getDimension", &Array::getDimension)
                .def("getDimensions", &Array::getDimensions)

        ];
        luaInterpreter.setClassRegistered("Array");

    }
#endif
}

Array::Array() : Named("Array"), m_data(0), m_dimensions(0), m_n(0) {

}

Array::Array(std::vector<int> dimensions) : Named("Array"), m_data(0), m_dimensions(0), m_n(dimensions.size()) {
    if(dimensions.size() == 0)
        return;
    int size = 1;
    m_dimensions = new int[dimensions.size()];
    for(int i = 0; i < dimensions.size(); i++) {
        size *= dimensions.at(i);
        m_dimensions[i] = dimensions.at(i);
    }
    if(size == 0)
        return;
    m_data = gsl_vector_alloc(size);
}


Array::Array(int n, int* dimensions) : Named("Array"), m_data(0), m_dimensions(0), m_n(n) {
    if(n == 0)
        return;
    int size = 1;
    m_dimensions = new int[n];
    for(int i = 0; i < n; i++) {
        size *= dimensions[i];
        m_dimensions[i] = dimensions[i];
    }
    if(size == 0)
        return;
    m_data = gsl_vector_alloc(size);
}

Array::Array(const Array& orig) : Named(orig), m_n(orig.m_n) {
    m_n = orig.m_n;
    if(m_n == 0)
        return;
    if(orig.m_dimensions) {
        m_dimensions = new int[m_n];
        memcpy(m_dimensions, orig.m_dimensions, sizeof(int) * m_n);
    } else
        return;
    if(orig.m_data) {
        m_data = gsl_vector_alloc(orig.m_data->size);
        gsl_vector_memcpy(m_data, orig.m_data);
    } else
        m_data = 0;
}


Array& Array::operator=(const Array& orig) {
    //teardown
    if(m_dimensions) {
        delete [] m_dimensions;
        m_dimensions = 0;
    }
    if(m_data) {
        gsl_vector_free(m_data);
        m_data = 0;
    }
    m_n = 0;

    m_n = orig.m_n;
    if(m_n == 0)
        return *this;
    if(orig.m_dimensions) {
        m_dimensions = new int[m_n];
        memcpy(m_dimensions, orig.m_dimensions, sizeof(int) * m_n);
    } else
        return *this;
    if(orig.m_data) {
        m_data = gsl_vector_alloc(orig.m_data->size);
        gsl_vector_memcpy(m_data, orig.m_data);
    } else
        m_data = 0;

    return *this;
}

int Array::getSize() const {
    if(m_data)
        return m_data->size;
    return 0;
}

void Array::zero() {
    if(m_data)
        gsl_vector_set_zero(m_data);
}

Array::~Array() {
    if(m_data)
        gsl_vector_free(m_data);
    if(m_dimensions)
        delete [] m_dimensions;
}

double Array::get(std::vector<int> coordinates) const {
    assert(coordinates.size() == m_n);
    int index = 0;
    int multiplier = 1;
    for(int i = 0; i < coordinates.size(); i++) {
        index += multiplier * coordinates.at(i);
        multiplier *= m_dimensions[i];
    }
    assert(index >= 0);
    assert(m_data != NULL);
    return gsl_vector_get(m_data, index);
}


double Array::get(int n, int* indices) const {
    assert(n == m_n);
    int index = 0;
    int multiplier = 1;
    for(int i = 0; i < n; i++) {
        index += multiplier * indices[i];
        multiplier *= m_dimensions[i];
    }
    assert(index >= 0);
    assert(m_data != NULL);
    return gsl_vector_get(m_data, index);
}

void Array::set(int n, int* indices, double value) {
    assert(n == m_n);
    int index = 0;
    int multiplier = 1;
    for(int i = 0; i < n; i++) {
        index += multiplier * indices[i];
        multiplier *= m_dimensions[i];
    }
    assert(index >= 0);
    assert(m_data != NULL);
    gsl_vector_set(m_data, index, value);
}

int Array::getDimension(int dimension) const {
    assert(m_dimensions != NULL);
    assert(dimension < m_n);
    return m_dimensions[dimension];
}

int Array::getDimensions() const {
    return m_n;
}

void Array::dumpInfo() const {
    std::cout << "Array info:" << std::endl;
    std::cout << m_n << " dimensions:" << std::endl;
    for(int i = 0; i < m_n; i++)
        std::cout << "dimension[" << i << "]: " << m_dimensions[i] << std::endl;
}

gsl_vector* Array::getData() {
    //out << "gsl_vector* Array::getData()" << eom;
    //out << m_data->size << eom;
    return m_data;
}

const gsl_vector* Array::getData() const {
    return m_data;
}


