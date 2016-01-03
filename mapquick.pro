QT += qml quick
QT += opengl

DEFINES += __QNXNTO__=1

HEADERS += fboinsgrenderer.h \
    logorenderer.h \
    polygonrenderer.h \
    linerenderer.h \
    glues/source/libtess/dict.h \
    glues/source/libtess/dict-list.h \
    glues/source/libtess/geom.h \
    glues/source/libtess/memalloc.h \
    glues/source/libtess/mesh.h \
    glues/source/libtess/normal.h \
    glues/source/libtess/priorityq.h \
    glues/source/libtess/priorityq-heap.h \
    glues/source/libtess/priorityq-sort.h \
    glues/source/libtess/render.h \
    glues/source/libtess/sweep.h \
    glues/source/libtess/tess.h \
    glues/source/libtess/tessmono.h
SOURCES += fboinsgrenderer.cpp main.cpp \
    logorenderer.cpp \
    polygonrenderer.cpp \
    linerenderer.cpp \
    glues/source/libtess/dict.c \
    glues/source/libtess/geom.c \
    glues/source/libtess/memalloc.c \
    glues/source/libtess/mesh.c \
    glues/source/libtess/normal.c \
    glues/source/libtess/priorityq.c \
    glues/source/libtess/render.c \
    glues/source/libtess/sweep.c \
    glues/source/libtess/tess.c \
    glues/source/libtess/tessmono.c

INCLUDEPATH += glues/source

RESOURCES += mapquick.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quick/scenegraph/mapquick
INSTALLS += target

OTHER_FILES += \
    main.qml
