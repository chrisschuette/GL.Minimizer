#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMutex>

#include <vector>
#include "arrow.h"
#include "../spincuboid.h"
#include "viewer.h"

class GLWidget : public QGLWidget, public Viewer
{
    Q_OBJECT
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    // Named
    std::string getName() const { return "GLWidget"; }

    //Viewer
    virtual void updateView(const SpinCuboid& spins);
    virtual void updateView(const SpinCuboid& spins, std::string text = std::string());

    void setData(SpinCuboid* data);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void updateIfNeeded();

    virtual void lock() { m_viewMutex.lock(); }
    virtual void unlock() { m_viewMutex.unlock(); }

signals:
    void numberOfLayersChanged(int number);
public slots:
    void setActiveLayer(int number);
protected:
    //mouse controls
    void wheelEvent ( QWheelEvent * event );

    //update cycle
    bool m_repaintNeeded;
    bool m_revizNeeded;

    //drawing routines
    void gl_draw();
    void visualizeLayer(int index);

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    int m_width;
    int m_height;
    int m_activeLayer;

    //data
    SpinCuboid* m_data;
    QMutex m_viewMutex;

    //stuff to draw
    std::vector<Arrow> m_arrows;

    // camera position
    float m_dx;
    float m_dy;
    int m_z;
    float m_angle;

    QString m_text;

};

#endif // GLWIDGET_H
