#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H

#include "configuration.h"
#include "named.h"

#define MAKEPATH(s) makePath(s)

class Configurable: public Named
{
public:
    Configurable(std::string name);
    virtual void configure(const basic::Configuration& configuration) = 0;
    static void Register();
protected:
    std::string makePath(std::string element) const;
    static bool m_registered;
};

#endif // CONFIGURABLE_H
