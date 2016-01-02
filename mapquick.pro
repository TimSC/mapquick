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

RESOURCES += mapquick.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/mapquick
INSTALLS += target

OTHER_FILES += \
    main.qml
