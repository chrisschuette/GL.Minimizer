#ifndef DEFECTCOUNTER_H
#define DEFECTCOUNTER_H

#include "spincuboid.h"
#include "named.h"
#ifndef LUADISABLED
#include <luabind/luabind.hpp>
#endif
#include <list>
#include <utility>

class DefectCounter : public Named
{
public:
    DefectCounter();
    virtual ~DefectCounter();
    static void Register();

#ifndef LUADISABLED
    luabind::object getDefects(const SpinCuboid& spins);
#endif

    SpinCuboid* getFluxField(const SpinCuboid& spins);

    double saoAXY(const SpinCuboid& spins, int x, int y, int z);
    double saoAYZ(const SpinCuboid& spins, int x, int y, int z);
    double saoAZX(const SpinCuboid& spins, int x, int y, int z);
    double saoCube(const SpinCuboid& spins, int x, int y, int z);
protected:
    double sa(const SpinCuboid& spins, int * base, int coord1, int d1, int coord2, int d2);
    double saoA(const SpinCuboid& spins, int * base, int coord1, int coord2);
    double saoCube(const SpinCuboid& spins, int * base);

    static bool m_registered;
    double charge(int i, int j, int k, const SpinCuboid& spins);
    double angle(int a1, int a2, int a3, int b1, int b2, int b3, int c1, int c2, int c3, const SpinCuboid& spins);
    double scalarproduct(int a1, int a2, int a3, int b1, int b2, int b3, const SpinCuboid& spins);
};

#endif // DEFECTCOUNTER_H
