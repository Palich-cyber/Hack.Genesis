#-------------------------------------------------
#
# Project created by QtCreator 2021-04-17T07:56:28
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PromoManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        promomanager.cpp \
    dataBase/dataBaseFunction.cpp \
    QCustomPlot/qcustomplot.cpp \
    dateConvertor/convert.cpp \
    dateConvertor/matrix.cpp

HEADERS += \
        promomanager.h \
    promoManagerStructs.h \
    dataBase/dataBaseFunction.h \
    QCustomPlot/qcustomplot.h \
    dateConvertor/convert.h \
    dateConvertor/matrix.h \
    dateConvertor/pz90.h \
    promoManagerConstants.h

FORMS += \
        promomanager.ui
