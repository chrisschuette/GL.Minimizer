#ifndef SPINCUBOID_H
#define SPINCUBOID_H

#include "array.h"

class SpinCuboid:  public Array
{
public:
    SpinCuboid();
    SpinCuboid(int Nx, int Ny, int Nz, bool periodicX, bool periodicY, bool periodicZ);
    virtual ~SpinCuboid();

    //copying
    SpinCuboid(const Array& orig, bool periodicX, bool periodicY, bool periodicZ);
    SpinCuboid(const SpinCuboid& orig);
    SpinCuboid& operator=(const SpinCuboid& orig);


    virtual double get(int n, int* indices) const;
    virtual void set(int n, int* indices, double value);

    virtual void set4(int x, int y, int z, int c, double value);
    virtual double get4(int x, int y, int z, int c) const;
    virtual int get4Index(int x, int y, int z, int c) const;

    virtual Array* clone() const { return new SpinCuboid(*this); }

    // my stuff
    bool isPeriodic(int dimension) const;
    bool isXPeriodic() const { return isPeriodic(0); }
    bool isYPeriodic() const { return isPeriodic(1); }
    bool isZPeriodic() const { return isPeriodic(2); }

    void setPeriodic(int dimension, bool perdiodic);
    void setXPeriodic(bool periodic) { setPeriodic(0, periodic); }
    void setYPeriodic(bool periodic) { setPeriodic(1, periodic); }
    void setZPeriodic(bool periodic) { setPeriodic(2, periodic); }

    unsigned int getVolume();

    double getAverageSpinLength() const;
    double getVariance() const;
    void normalize();

    void setLayer(int layer, const SpinCuboid& src, int srcLayer = 0);
    void zeroLayer(int layer);

    static void Register();
protected:
    static bool m_registered;
    int mapBack(int coordinate, int extent) const;
    bool m_periodic[3];

};

#endif // SPINCUBOID_H
