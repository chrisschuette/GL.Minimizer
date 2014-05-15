#ifndef HELICAL_H
#define HELICAL_H

#include "phase.h"

class Helical : public Phase
{
public:
    Helical();

    //Named
    virtual std::string getName() const { return "Helical"; }

    //Printable
    virtual std::string toString() const;
    virtual void render(SpinCuboid& array) const;

    static void Register();
protected:
    static bool m_registered;
};

#endif // HELICAL_H
