#ifndef SkyrmionLattice_H
#define SkyrmionLattice_H

#include "phase.h"

class SkyrmionLattice : public Phase
{
public:
    SkyrmionLattice();

    //Named
    virtual std::string getName() const { return "SkyrmionLattice"; }

    //Printable
    virtual std::string toString() const;
    virtual void render(SpinCuboid& array) const;

    static void Register();
protected:
    static bool m_registered;
};

#endif // SkyrmionLattice_H
