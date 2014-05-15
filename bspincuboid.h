#ifndef BSPINCUBOID_H
#define BSPINCUBOID_H

#include "spincuboid.h"

class BSpinComplex;
class BSpinCuboid : public SpinCuboid
{
public:
    BSpinCuboid(int Nx, int Ny, int Nz, int Bx, int By, int Bz, bool Px, bool Py, bool Pz);
    BSpinCuboid(const BSpinCuboid& orig);
    BSpinCuboid(const Array& orig, int Bx, int By, int Bz, bool Px, bool Py, bool Pz);
    BSpinCuboid(const Array& orig, int Bx, int By, int Bz, bool Px, bool Py, bool Pz, double boundary1, double boundary2, double boundary3);
    BSpinCuboid& operator=(const BSpinCuboid& orig);
    virtual Array* clone() const { return new BSpinCuboid(*this); }

    virtual ~BSpinCuboid();

    virtual double get(int n, int* indices) const;
    virtual void set(int n, int* indices, const double& value);
    virtual void set4(int x, int y, int z, int c, double value);
    virtual double get4(int x, int y, int z, int c) const;
    virtual int get4Index(int x, int y, int z, int c) const;

    virtual void setBorderTo(double c1, double c2, double c3);

    void unlockBorder();
    void lockBorder();
    bool isBorderLocked();
    static void Register();
protected:
    static bool m_registered;
    int mapBack(int coordinate, int min, int max) const;
    virtual bool isBorder(int x, int y, int z);

    bool m_borderLocked;
    int m_borderThickness[3];
    gsl_vector* m_border;
};

#endif // BSPINCUBOID_H
