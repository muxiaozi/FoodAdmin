#-------------------------------------------------
#
# Project created by QtCreator 2018-03-03T12:24:45
#
#-------------------------------------------------

QT       += core gui printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FoodAdmin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    foodmodel.cpp \
    purchasemodel.cpp \
    purchasedialog.cpp \
    addfooddialog.cpp \
    shipmentmodel.cpp \
    shipmentdialog.cpp \
    adduserdialog.cpp \
    addadmindialog.cpp \
    addsupplierdialog.cpp \
    usermodel.cpp \
    mytablemodel.cpp \
    adminmodel.cpp \
    suppliermodel.cpp \
    userdetaildialog.cpp \
    authorizedialog.cpp

HEADERS  += mainwindow.h \
    database.h \
    bean.h \
    foodmodel.h \
    purchasemodel.h \
    purchasedialog.h \
    addfooddialog.h \
    shipmentmodel.h \
    shipmentdialog.h \
    adduserdialog.h \
    addadmindialog.h \
    addsupplierdialog.h \
    usermodel.h \
    mytablemodel.h \
    adminmodel.h \
    suppliermodel.h \
    userdetaildialog.h \
    authorizedialog.h

FORMS    += mainwindow.ui \
    purchasedialog.ui \
    addfooddialog.ui \
    shipmentdialog.ui \
    adduserdialog.ui \
    addadmindialog.ui \
    addsupplierdialog.ui \
    userdetaildialog.ui \
    authorizedialog.ui

RESOURCES += \
    resource.qrc

RC_ICONS = icon.ico
