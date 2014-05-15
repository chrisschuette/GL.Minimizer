#include "named.h"

Named::Named(std::string name) : m_name(name) {

}

Named::Named(const Named& orig) : m_name(orig.m_name) {

}

std::string Named::getName() const {
    return m_name;
}

void Named::setName(std::string name) {
    m_name = name;
}

