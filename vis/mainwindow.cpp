#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) : Named("MainWindow"),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    ui->setupUi(this);
    glWidget = new GLWidget;
    ui->mainFrame->layout()->addWidget(glWidget);

    //disable the controls for now. Maybe we will
    //completely get rid of them.
    ui->sideFrame->setVisible(false);

    //slider events
    connect(glWidget, SIGNAL(numberOfLayersChanged(int)),this,SLOT(updateSlider(int)));
    connect(ui->layerSlider, SIGNAL(valueChanged(int)),this,SLOT(setActiveLayer(int)));

    //update timer
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateIfNeeded()));
    updateTimer.start(100); //every 60 ms
    restoreState(settings.value("mainWindowState").toByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.sync();
}

/*void MainWindow::setData(ISpinCuboid *data) {
    out << "void MainWindow::setData(ISpinCuboid* data)" << eom;
    glWidget->setData(data);
}*/

void MainWindow::setActiveLayer(int layer) {
    ui->layerLabel->setText(QString::number(layer));
    glWidget->setActiveLayer(layer);
}


void MainWindow::updateSlider(int nLayers) {
   // out << "void MainWindow::numberOfLayersChanged( " << nLayers << " )" << eom;
    ui->layerSlider->setMinimum(0);
    int current = ui->layerSlider->value();
    current = std::min(current, nLayers-1);
    ui->layerSlider->setValue(current);
    ui->layerSlider->setMaximum(nLayers - 1);
}

void MainWindow::updateIfNeeded() {
    glWidget->updateIfNeeded();
}

