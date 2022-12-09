QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AddBook.cpp \
    ClientWindow.cpp \
    CreateOrder.cpp \
    DeleteAccout.cpp \
    DirectorWindow.cpp \
    EditCredWindow.cpp \
    Login.cpp \
    OperatorWindow.cpp \
    Order.cpp \
    Registration.cpp \
    User.cpp \
    changestatus.cpp \
    main.cpp

HEADERS += \
    AddBook.h \
    ClientWindow.h \
    CreateOrder.h \
    DeleteAccout.h \
    DirectorWindow.h \
    EditCredWindow.h \
    Login.h \
    OperatorWindow.h \
    Order.h \
    Registration.h \
    User.h \
    changestatus.h

FORMS += \
    AddBook.ui \
    ClientWindow.ui \
    CreateOrder.ui \
    DeleteAccout.ui \
    DirectorWindow.ui \
    EditCredWindow.ui \
    Login.ui \
    OperatorWindow.ui \
    Order.ui \
    Registration.ui \
    changestatus.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
