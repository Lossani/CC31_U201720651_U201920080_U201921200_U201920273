QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcontacto.cpp \
    avl.cpp \
    commentmanager.cpp \
    hitagget.cpp \
    interactionmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    newpost.cpp \
    postmanager.cpp \
    postui.cpp \
    principal.cpp \
    usermanager.cpp \
    viewpost.cpp

HEADERS += \
    Arbol.h \
    ListaD.h \
    ListaT.h \
    addcontacto.h \
    avl.h \
    commentmanager.h \
    contacto.h \
    grupo.h \
    hitagget.h \
    interactionmanager.h \
    listcontroller.h \
    mainwindow.h \
    newpost.h \
    post.h \
    postmanager.h \
    postui.h \
    principal.h \
    publicacion.h \
    publication.h \
    user.h \
    usermanager.h \
    usuario.h \
    viewpost.h

FORMS += \
    addcontacto.ui \
    mainwindow.ui \
    newpost.ui \
    principal.ui \
    viewpost.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
