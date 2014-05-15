#ifndef NAMED_H
#define NAMED_H

#include <string>

class Named
{
public:
    Named(std::string name);
    Named(const Named& orig);
    std::string getName() const;
    void setName(std::string name);
protected:
    std::string m_name;
};

#endif // NAMED_H
