#ifndef VIEWER_H
#define VIEWER_H

#include "named.h"
#include "spincuboid.h"
#include <iostream>

class Viewer : public Named
{
public:
    Viewer();

    virtual void updateView(const SpinCuboid& spins) { }
    virtual void updateView(const SpinCuboid& spins, std::string text) { }
    static void Register();
    virtual void lock() { }
    virtual void unlock() { }
protected:
    static bool m_registered;
};

#endif // VIEWER_H
