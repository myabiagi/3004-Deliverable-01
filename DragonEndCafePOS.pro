QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/model $$PWD/controller $$PWD/view

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    view/loginView.cpp \
    view/staffWorkspace.cpp \
    view/customerWorkspace.cpp \
    view/menuView.cpp \
    view/checkoutView.cpp \
    view/dashboardView.cpp \
    view/kitchenWorkspace.cpp \
    view/kitchenQueueView.cpp \
    view/orderDetailsView.cpp

SOURCES += $$files($$PWD/model/*.cpp)
SOURCES += $$files($$PWD/controller/*.cpp)

HEADERS += \
    mainwindow.h \
    view/viewFormatting.h \
    view/loginView.h \
    view/staffWorkspace.h \
    view/customerWorkspace.h \
    view/menuView.h \
    view/checkoutView.h \
    view/dashboardView.h \
    view/kitchenWorkspace.h \
    view/kitchenQueueView.h \
    view/orderDetailsView.h

HEADERS += $$files($$PWD/model/*.h)
HEADERS += $$files($$PWD/controller/*.h)

FORMS += \
    mainwindow.ui \
    view/loginView.ui \
    view/staffWorkspace.ui \
    view/customerWorkspace.ui \
    view/menuView.ui \
    view/checkoutView.ui \
    view/dashboardView.ui \
    view/kitchenWorkspace.ui \
    view/kitchenQueueView.ui \
    view/orderDetailsView.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
