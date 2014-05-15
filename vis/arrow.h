#ifndef ARROW_H
#define ARROW_H

#include <GL/glut.h>

class Arrow
{
public:
    Arrow();
    Arrow(const Arrow& orig);
    Arrow& operator =(const Arrow& b);
    virtual ~Arrow();
    void setPosition(float x, float y, float z);
    void setDirectionAndLength(float x, float y, float z);
    void setDirection(float theta, float phi);
    void draw(GLUquadricObj* pObj);
    void drawSelection();
    float getX();
    float getY();
    float getZ();
    int getSelectionID();
private:
    float m_position[3];
    float m_direction[2];
    float m_color[4];
    float m_colorAmbient[4];
    float m_length;
    float m_thickness;
    float m_headRadius;
    float m_radius;
    float m_headLength;
};

#endif // ARROW_H
