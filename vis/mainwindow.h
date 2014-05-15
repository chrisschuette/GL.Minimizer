#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "vis/glwidget.h"
#include "named.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Named
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Named
    virtual std::string getName() const { return "MainWindow"; }

    Viewer& getViewer() { return *glWidget; }

protected:
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void updateIfNeeded();
    void updateSlider(int nLayers);
    void setActiveLayer(int layer);
private:
    Ui::MainWindow *ui;
    GLWidget* glWidget;
    QTimer updateTimer;
};

#endif // MAINWINDOW_H
