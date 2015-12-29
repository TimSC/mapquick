QT += qml quick

HEADERS += fboinsgrenderer.h \
    logorenderer.h \
    polygonrenderer.h \
    linerenderer.h
SOURCES += fboinsgrenderer.cpp main.cpp \
    logorenderer.cpp \
    polygonrenderer.cpp \
    linerenderer.cpp

INCLUDEPATH += ../shared

RESOURCES += textureinsgnode.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/textureinsgnode
INSTALLS += target

OTHER_FILES += \
    main.qml
