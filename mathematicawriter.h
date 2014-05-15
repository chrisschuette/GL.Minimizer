#ifndef MATHEMATICAWRITER_H
#define MATHEMATICAWRITER_H

#include <vector>
#include <string>
#include "array.h"

class MathematicaWriter
{
public:
    MathematicaWriter();
    void write(std::string filename, const Array& array);
    virtual ~MathematicaWriter();
    static void Register();
protected:
    static bool m_registered;
    std::vector<std::string> getPathHierarchy(std::string filename);
    int createDirectoryStructure(std::string filename);
    void renderDimension(int dimension, const Array &array, std::ostream &stream, std::vector<int> base);
};

#endif // MATHEMATICAWRITER_H
