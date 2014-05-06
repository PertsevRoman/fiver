#-------------------------------------------------
#
# Project created by QtCreator 2014-04-10T21:31:50
#
#-------------------------------------------------

QT += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fiver
TEMPLATE = app

SOURCES += \
    YandexImageConnector.cpp \
    WebDocument.cpp \
    TriangleNumber.cpp \
    TriangleMathCollector.cpp \
    MainWindow.cpp \
    FileDocument.cpp \
    DocumentsAnalyser.cpp \
    Document.cpp \
    DataModel.cpp \
    main.cpp \
    optionsdialog.cpp \
    aboutdialog.cpp

HEADERS  += \
    YandexImageConnector.h \
    WebDocument.h \
    TriangleNumber.h \
    TriangleMathCollector.h \
    MainWindow.h \
    FileDocument.h \
    DocumentsAnalyser.h \
    Document.h \
    DataModel.h \
    codes.h \
    optionsdialog.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui \
    aboutdialog.ui

CONFIG += c++11

OTHER_FILES += \
    fiver.pro.user

unix:!macx: LIBS += -L$$PWD/../../../../opt/boost/lib/ -lboost_system -lboost_thread -lboost_regex

INCLUDEPATH += $$PWD/../../../../opt/boost/include
DEPENDPATH += $$PWD/../../../../opt/boost/include


unix:!macx: LIBS += -L$$PWD/../../../../opt/htmlcxx/lib/ -lhtmlcxx -lcss_parser -lcss_parser_pp

INCLUDEPATH += $$PWD/../../../../opt/htmlcxx/include
DEPENDPATH += $$PWD/../../../../opt/htmlcxx/include

unix:!macx: LIBS += -L$$PWD/../../../../opt/gsl/lib/ -lgsl -lgslcblas

INCLUDEPATH += $$PWD/../../../../opt/gsl/include
DEPENDPATH += $$PWD/../../../../opt/gsl/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../opt/gsl/lib/libgsl.a
unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../opt/gsl/lib/libgslcblas.a
