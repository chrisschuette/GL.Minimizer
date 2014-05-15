#include "glwidget.h"
#include <assert.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <QWheelEvent>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    , m_data(0)
    , m_dx(0.0)
    , m_dy(0.0)
    , m_angle(0.0)
    , m_z(-10)
    , m_repaintNeeded(false)
    , m_revizNeeded(false)
    , m_activeLayer(0)
{
    //expanding widget
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
}

GLWidget::~GLWidget()
{
    if(m_data)
        delete m_data;
}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400,400);
}


void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 100.0, 100.0, 0.0, 0.0 };
    GLfloat light_position2[] = { -100.0, -100.0, 0.0, 0.0 };
    GLfloat light_position1[] = { -100.0, 100.0, 0.0, 0.0 };
    GLfloat light_position3[] = { 100.0, -100.0, 0.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position1);

    glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT3, GL_POSITION, light_position3);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glClearColor(	1.0, 1.0, 1.0 , 1.0);
}

void GLWidget::gl_draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(m_data == 0)
        return;
    GLUquadricObj *pObj;
    pObj = gluNewQuadric();
    glTranslatef(0.0,0.0,m_z);
    glRotatef(m_angle,1.0,0.0,0.0);
    glTranslatef(-(float) m_data->getDimension(0)/2.0 + m_dx,-(float) m_data->getDimension(1)/2.0 + m_dy,0.0);

    //draw spins
    for(unsigned int i = 0; i < m_arrows.size(); i++)
        m_arrows.at(i).draw(pObj);

    gluDeleteQuadric(pObj);
}


void GLWidget::paintGL() {
    gl_draw();
    if(m_text.size() > 0) {
        glColor4f(0.0, 0.0, 0.0, 1.0);
        renderText(size().width()-120, 22, QString(m_text),  QFont("Helvetica",20));
    }

}

void GLWidget::resizeGL(int width, int height) {
    m_width = width;
    m_height = height;
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum (-(double) width / (double) height, (double) width / (double) height, -1, 1, 3, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::updateView(const SpinCuboid& spins) {
    setData(static_cast<SpinCuboid*>(spins.clone()));
}

void GLWidget::updateView(const SpinCuboid& spins, std::string text) {
    setData(static_cast<SpinCuboid*>(spins.clone()));
    m_viewMutex.lock();
    m_text = QString(text.c_str());
    m_viewMutex.unlock();

}

void GLWidget::setData(SpinCuboid *data) {
    m_viewMutex.lock();

    bool setZ = false;

    if(m_data)
        delete m_data;
    else
        setZ = true;

    m_data = data;
    if(m_data) {
        int N = std::max(m_data->getDimension(0), m_data->getDimension(1));
        if(setZ)
            m_z = -(float) N/2.0;
        int nLayers = m_data->getDimension(2);
        emit numberOfLayersChanged(nLayers);
        m_revizNeeded = true;
        m_repaintNeeded = true;
    }
    m_viewMutex.unlock();
}

void GLWidget::visualizeLayer(int index) {
 //   out << "void GLWidget::visualizeLayer(int index)" << eom;
    assert(m_data);
    assert(index < m_data->getDimension(2));
    m_activeLayer = index;
    m_arrows.clear();
    m_viewMutex.lock();
     for(int i = 0; i < m_data->getDimension(0); i++) {
              for(int j = 0; j < m_data->getDimension(1); j++) {
                  Arrow arrow;
                  arrow.setPosition(i,j,0.0);
                  arrow.setDirectionAndLength(m_data->get4(i,j,m_activeLayer,0),m_data->get4(i,j,m_activeLayer,1),m_data->get4(i,j,m_activeLayer,2));
                  m_arrows.push_back(arrow);
              }
         }
     m_viewMutex.unlock();
    // out << m_arrows.size() << " arrows created." << eom;

}

void GLWidget::updateIfNeeded() {
    if(m_revizNeeded) {
        visualizeLayer(m_activeLayer);
        m_revizNeeded = false;
    }
    if(m_repaintNeeded) {
        update();
        m_repaintNeeded = false;
    }
}

void GLWidget::setActiveLayer(int number) {
//    out << "void GLWidget::setActiveLayer(int number)" << eom;
    assert(m_data);
    assert(number < m_data->getDimension(2));
    m_activeLayer = number;
    m_revizNeeded = true;
    m_repaintNeeded = true;
}

void GLWidget::wheelEvent ( QWheelEvent * event ) {
    if (event->orientation() == Qt::Vertical) {
        if( event->modifiers() == Qt::ControlModifier) {
            m_z += event->delta() * 0.02;
            if(m_z > -2.0)
                m_z = -2.0;
        }
        else if(event->modifiers() == Qt::AltModifier) {
            m_angle += event->delta() * 0.1;
        }
        else if(event->modifiers() == Qt::ShiftModifier) {
            m_angle += event->delta() * 0.1;
        }
        else
            m_dy -= event->delta() * 0.0005 * abs(m_z);
       repaint();
    } else {
           m_dx += event->delta() * 0.0005 * abs(m_z);
       repaint();
    }

}
