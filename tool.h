#ifndef TOOL_H
#define TOOL_H

#include "named.h"
#include "spincuboid.h"

class Tool : public Named
{
public:
    Tool(std::string name);
    virtual void apply(SpinCuboid& spins) = 0;
    virtual void dumpInfo() = 0;
    static void Register();
protected:
    static bool m_registered;
};

#endif // TOOL_H
