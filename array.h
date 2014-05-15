#ifndef ARRAY_H
#define ARRAY_H

#include "named.h"
#include <gsl/gsl_vector.h>
#include <vector>

class Array : public Named
{
public:
    Array();
    Array(std::vector<int> dimensions);
    Array(int n, int *dimensions);
    Array(const Array& orig);
    Array& operator=(const Array& orig);
    virtual ~Array();

    double get(std::vector<int> coordinates) const;
    virtual double get(int n, int* indices) const;
    virtual void set(int n, int* indices, double value);
    virtual int getDimension(int dimension) const;
    virtual int getDimensions() const;
    void dumpInfo() const;
    virtual gsl_vector* getData();
    virtual const gsl_vector* getData() const;
    virtual void setData(gsl_vector* data) { m_data = data; }
    virtual int getSize() const;


    void zero();
    static void Register();
protected:
    static bool m_registered;
    gsl_vector* m_data;
    int* m_dimensions;
    int m_n;
};

#endif // ARRAY_H
