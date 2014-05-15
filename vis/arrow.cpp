#include "arrow.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

Arrow::Arrow()
{
    //initialize to origin
    m_position[0] = 0.0;
    m_position[1] = 0.0;
    m_position[2] = 0.0;

    //pointing in z-direction
    setDirection(0.0,0.0);

    // copy length and thickness
    m_thickness = 0.1;

    // arrow dimensions
    //float fAxisRadius = 0.05f;
    //float fAxisHeight = 1.0f;
    //float fArrowRadius = 0.12f;
    //float fArrowHeight = 0.2f;

    m_length = 1.0f;
    m_radius = 0.05f;
    m_headRadius = 0.12f;
    m_headLength = 0.2f;
}

Arrow::Arrow(const Arrow& orig) {
    //copy position
    m_position[0] = orig.m_position[0];
    m_position[1] = orig.m_position[1];
    m_position[2] = orig.m_position[2];

    //copy direction
    setDirection(orig.m_direction[0], orig.m_direction[1]);

    // copy length and thickness et cetera
    m_thickness = orig.m_thickness;
    m_length = orig.m_length;
    m_radius = orig.m_radius;
    m_headRadius = orig.m_headRadius;
    m_headLength = orig.m_headLength;
}

Arrow& Arrow::operator =(const Arrow& orig) {
    //copy position
    m_position[0] = orig.m_position[0];
    m_position[1] = orig.m_position[1];
    m_position[2] = orig.m_position[2];

    //copy direction
    setDirection(orig.m_direction[0], orig.m_direction[1]);

    // copy length and thickness
    m_thickness = orig.m_thickness;
    m_length = orig.m_length;

    // copy length and thickness et cetera
    m_thickness = orig.m_thickness;
    m_length = orig.m_length;
    m_radius = orig.m_radius;
    m_headRadius = orig.m_headRadius;
    m_headLength = orig.m_headLength;

    return *this;
}

float Arrow::getX() {
    return m_position[0];
}

float Arrow::getY() {
    return m_position[1];
}

float Arrow::getZ() {
    return m_position[2];
}

void Arrow::setDirectionAndLength(float x, float y, float z) {
    m_length = sqrt( x*x + y * y + z * z );
    float phi = atan2(y,x);
    float theta = acos(z/m_length);
    setDirection(theta / (2.0 * M_PI) * 360.0,phi / (2.0 * M_PI) * 360.0);
}

int Arrow::getSelectionID() {
    return m_position[1] * 10000 + m_position[0];
}

void Arrow::setPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
}

void Arrow::setDirection(float theta, float phi) {
    m_direction[0] = theta;
    m_direction[1] = phi;
    if(theta < 90.0) {
        m_color[2] = 1.0 - theta/90.0;
        m_color[1] = theta / 90.0 + 0.1; //pure blue = no shading
        m_color[0] = 0.0;
    } else {
        theta -= 90.0;
        m_color[2] = 0.0;
        m_color[1] = 1.0 - theta/90.0;
        m_color[0] = theta/90.0;
    }

    //ambient color is darker than specular & diffuse color
    m_colorAmbient[0] = 0.4 * m_color[0];
    m_colorAmbient[1] = 0.4 * m_color[1];
    m_colorAmbient[2] = 0.4 * m_color[2];
    m_color[3] = 1.0;
    m_colorAmbient[3] = 1.0;
}

void Arrow::drawSelection() {
    glPushMatrix();
    glTranslatef(m_position[0], m_position[1], m_position[2]);
    //draw square
    glBegin(GL_QUADS); // Start drawing a quad primitive
    glVertex3f(-0.5f, -0.5f, 0.0f); // The bottom left corner
    glVertex3f(0.5f, -0.5f, 0.0f); // The top left corner
    glVertex3f(0.5f, 0.5f, 0.0f); // The top right corner
    glVertex3f(-0.5f, 0.5f, 0.0f); // The bottom right corner
    glEnd();
    glPopMatrix();

}


void Arrow::draw(GLUquadricObj* pObj) {
    int res = 5;
    //save the state of the matrix stack
    glPushMatrix();
        //move into position
        glTranslatef(m_position[0], m_position[1], m_position[2]);
        //rotate phi (polar angle)
        glRotatef(m_direction[1], 0.0f, 0.0f, 1.0f);
        //rotate theta (azimuthal angle)
        glRotatef(m_direction[0], 0.0f, 1.0f, 0.0f);

        gluQuadricDrawStyle(pObj, GLU_FILL);
        gluQuadricNormals(pObj, GLU_SMOOTH);
        gluQuadricOrientation(pObj, GLU_OUTSIDE);
        gluQuadricTexture(pObj, GLU_FALSE);

        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_color);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_color);
        glMaterialfv(GL_FRONT, GL_AMBIENT, m_colorAmbient);
        glTranslatef(0.0f, 0.0f, -m_length/2.0);

        gluCylinder(pObj, m_radius, m_radius, m_length, res, 1);
        glPushMatrix();
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(180.0f/((double) res), 0.0f, 0.0f, 1.0f);
            gluDisk(pObj, 0.0, m_radius, res, 1);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, m_length);
            gluCylinder(pObj, m_headRadius, 0.0f, m_headLength, res, 1);
            glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(180.0f/((double) res), 0.0f, 0.0f, 1.0f);
            gluDisk(pObj, m_radius, m_headRadius, res, 1);
        glPopMatrix();
    glPopMatrix();

}

Arrow::~Arrow() {
}
