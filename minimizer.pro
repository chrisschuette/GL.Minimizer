#-------------------------------------------------
#
# Project created by QtCreator 2012-11-13T17:33:10
#
#-------------------------------------------------

QT       += core gui opengl
LIBS +=  -lglut -lGLU -lgsl -lgslcblas -lconfig++ -lluabind -lgtest -lboost_program_options -L/usr/lib/x86_64-linux-gnu -llua5.1
INCLUDEPATH += /usr/include/lua5.1

TARGET = minimizer
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG




SOURCES +=\
        vis/mainwindow.cpp \
   vis/main.cpp \
    array.cpp \
    array_test.cpp \
    spincuboid.cpp \
    spincuboid_test.cpp \
    bspincuboid.cpp \
    bspincuboid_test.cpp \
    bspinzylinder.cpp \
    bspinzylinder_test.cpp \
    bspincomplex.cpp \
    BSpinComplex_Test.cpp \
    mathematicareader.cpp \
    mathematicawriter.cpp \
    bspinsphere.cpp \
    luainterpreter.cpp \
    configuration.cpp \
    Exception.cpp \
    utils.cpp \
    outputsystem.cpp \
    terminator.cpp \
    message.cpp \
    simplemessage.cpp \
    printable.cpp \
    testprintable.cpp \
    output.cpp \
    soutput.cpp \
    minimizer.cpp \
    functional.cpp \
    iarray.cpp \
    ispincuboid.cpp \
    configurable.cpp \
    ginzburglandau.cpp \
    phase.cpp \
    helical.cpp \
    vis/arrow.cpp \
    vis/defect.cpp \
    vis/glwidget.cpp \
    qcustomplot.cpp \
    viewer.cpp \
    defectedhelical.cpp \
    interpolated.cpp \
    defectcounter.cpp \
    ginzburglandaupinning.cpp \
    functionalcontainer.cpp \
    pinning.cpp \
    apex.cpp \
    polarized.cpp \
    tool.cpp \
    polarizer.cpp \
    skyrmionizer.cpp \
    polarization.cpp \
    quadrupole.cpp \
    dipole.cpp \
    wave.cpp \
    skyrmionlattice.cpp \
    barefunctional.cpp \
    luathread.cpp \
    montecarlo.cpp \
    langevintest.cpp \
    langevinevolver.cpp \
    evolver.cpp \
    gaussiannoisegenerator.cpp \
    gslgaussiannoise.cpp \
    ferromagnet.cpp \
    discretization.cpp \
    ifunctional_deriv.cpp \
    ifunctional_energydiff.cpp \
    ifunctional_value.cpp \
    quantity.cpp \
    timeseries.cpp \
    ginzburglandaufixedm.cpp \
    massterm.cpp \
    position.cpp \
    constantfield.cpp

HEADERS  += vis/mainwindow.h \
    iarray.h \
    array.h \
    spincuboid.h \
    ispincuboid.h \
    bspincuboid.h \
    bspinzylinder.h \
    bspincomplex.h \
    iarraywriter.h \
    iarrayreader.h \
    mathematicareader.h \
    mathematicawriter.h \
    bspinsphere.h \
    luainterpreter.h \
    configuration.h \
    Exception.h \
    utils.h \
    outputsystem.h \
    terminator.h \
    message.h \
    simplemessage.h \
    printable.h \
    testprintable.h \
    output.h \
    soutput.h \
    minimizer.h \
    functional.h \
    configurable.h \
    ginzburglandau.h \
    phase.h \
    helical.h \
    vis/arrow.h \
    vis/defect.h \
    vis/glwidget.h \
    qcustomplot.h \
    viewer.h \
    defectedhelical.h \
    interpolated.h \
    defectcounter.h \
    ginzburglandaupinning.h \
    functionalcontainer.h \
    pinning.h \
    apex.h \
    polarized.h \
    tool.h \
    polarizer.h \
    skyrmionizer.h \
    polarization.h \
    quadrupole.h \
    dipole.h \
    wave.h \
    skyrmionlattice.h \
    barefunctional.h \
    luathread.h \
    montecarlo.h \
    langevintest.h \
    langevinevolver.h \
    evolver.h \
    gaussiannoisegenerator.h \
    gslgaussiannoise.h \
    ferromagnet.h \
    ifunctional_value.h \
    inamed.h \
    ifunctional_energydiff.h \
    ifunctional_deriv.h \
    discretization.h \
    quantity.h \
    timeseries.h \
    ginzburglandaufixedm.h \
    massterm.h \
    position.h \
    constantfield.h

FORMS    += vis/mainwindow.ui
