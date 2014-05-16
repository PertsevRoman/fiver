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
    aboutdialog.cpp \
    insertarticledialog.cpp \
    changemarkdialog.cpp \
    appenduridialog.cpp \
    spinboxdelegate.cpp \
    progressbardelegate.cpp

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
    aboutdialog.h \
    insertarticledialog.h \
    changemarkdialog.h \
    appenduridialog.h \
    spinboxdelegate.h \
    documenterrors.h \
    progressbardelegate.h

FORMS    += mainwindow.ui \
    optionsdialog.ui \
    aboutdialog.ui \
    insertarticledialog.ui \
    changemarkdialog.ui \
    appenduridialog.ui

CONFIG += c++11

OTHER_FILES += \
    fiver.pro.user

unix:!macx {
    LIBS += -L$$PWD/../../../../opt/boost/lib/ -lboost_system -lboost_thread -lboost_regex

    INCLUDEPATH += $$PWD/../../../../opt/boost/include
    DEPENDPATH += $$PWD/../../../../opt/boost/include

    LIBS += -L$$PWD/../../../../opt/htmlcxx/lib/ -lhtmlcxx -lcss_parser -lcss_parser_pp

    INCLUDEPATH += $$PWD/../../../../opt/htmlcxx/include
    DEPENDPATH += $$PWD/../../../../opt/htmlcxx/include

    LIBS += -L$$PWD/../../../../opt/o2scl/lib/ -lo2scl -lo2scl_part -lo2scl_eos

    INCLUDEPATH += $$PWD/../../../../opt/o2scl/include
    DEPENDPATH += $$PWD/../../../../opt/o2scl/include

    LIBS += -L$$PWD/../../../../opt/gsl/lib/ -lgsl -lgslcblas

    INCLUDEPATH += $$PWD/../../../../opt/gsl/include
    DEPENDPATH += $$PWD/../../../../opt/gsl/include
} else:win32 {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../Programming/boost/lib/ -lboost_system-mgw48-1_55 -lboost_system-mgw48-mt-1_55 -lboost_thread-mgw48-mt-1_55 -lboost_regex-mgw48-1_55
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../Programming/boost/lib/ -lboost_system-mgw48-d-1_55 -lboost_system-mgw48-mt-d-1_55 -lboost_thread-mgw48-mt-d-1_55 -lboost_regex-mgw48-d-1_55

    INCLUDEPATH += $$PWD/../../../../../Programming/boost/include/boost-1_55
    DEPENDPATH += $$PWD/../../../../../Programming/boost/include/boost-1_55

    LIBS += -L$$PWD/../../../../../Programming/htmlcxx/lib/ -lhtmlcxx -lcss_parser -lcss_parser_pp

    INCLUDEPATH += $$PWD/../../../../../Programming/htmlcxx/include
    DEPENDPATH += $$PWD/../../../../../Programming/htmlcxx/include

    LIBS += -L$$PWD/../../../../../Programming/gsl/lib/ -lgsl -lgslcblas

    INCLUDEPATH += $$PWD/../../../../../Programming/gsl/include
    DEPENDPATH += $$PWD/../../../../../Programming/gsl/include

    LIBS += -L$$PWD/../../../../../Programming/o2scl/lib/ -lo2scl -lo2scl_eos -lo2scl_part

    INCLUDEPATH += $$PWD/../../../../../Programming/o2scl/include
    DEPENDPATH += $$PWD/../../../../../Programming/o2scl/include

    LIBS += -L$$PWD/../../../../../Programming/RussianGrammaticalDictionary/lib/ -lsolarix_grammar_engine

    INCLUDEPATH += $$PWD/../../../../../Programming/RussianGrammaticalDictionary/include
    DEPENDPATH += $$PWD/../../../../../Programming/RussianGrammaticalDictionary/include
}
